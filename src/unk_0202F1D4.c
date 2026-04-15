#include "unk_0202F1D4.h"

#include <nitro.h>
#include <string.h>

#include "constants/battle.h"
#include "constants/species.h"

#include "struct_defs/struct_0202F264.h"
#include "struct_defs/struct_0202F298.h"
#include "struct_defs/struct_0202F298_sub1.h"
#include "struct_defs/struct_0202F41C.h"
#include "struct_defs/struct_0202FAA8.h"
#include "struct_defs/struct_0202FD30.h"
#include "struct_defs/struct_02030A80.h"
#include "struct_defs/struct_02078B40.h"

#include "savedata/save_table.h"

#include "battle_regulation.h"
#include "enums.h"
#include "field_battle_data_transfer.h"
#include "game_options.h"
#include "heap.h"
#include "math_util.h"
#include "party.h"
#include "pokedex.h"
#include "pokemon.h"
#include "save_player.h"
#include "savedata.h"
#include "sound_chatot.h"
#include "system.h"
#include "trainer_info.h"

BattleRecording *gBattleRecording = NULL;

static void Copy_Party_to_BattleRecordingParty(const Party *param0, BattleRecordingParty *param1);
static void Copy_BattleRecordingParty_to_Party(BattleRecordingParty *param0, Party *param1);
static BOOL sub_0202F75C(SaveData *saveData, BattleRecording *battleRecording);
static BOOL sub_0202F794(SaveData *saveData, const BattleRecording *battleRecording);
static void sub_0202F860(void *param0, u32 param1, u32 param2);
static void sub_0202F510(SaveData *saveData, UnkStruct_0202F41C *param1, const BattleRecordingData *param2, int param3, int param4);

int BattleRecording_SaveSize(void)
{
    GF_ASSERT(sizeof(BattleRecording) < 0x1000 * 2);

    return sizeof(BattleRecording);
}

void BattleRecording_Init(BattleRecording *battleRecording)
{
    MI_CpuClear32(battleRecording, sizeof(BattleRecording));
    battleRecording->unk_00 = 0xFFFFFFFF;
}

void BattleRecording_New(SaveData *saveData, enum HeapID heapID, int *resultCode)
{
    if (gBattleRecording != NULL) {
        Heap_Free(gBattleRecording);
        gBattleRecording = NULL;
    }

    gBattleRecording = SaveData_GetBattleRecording(saveData, heapID, resultCode, 0);
    BattleRecording_Init(gBattleRecording);
}

void BattleRecording_Free(void)
{
    GF_ASSERT(gBattleRecording);

    Heap_Free(gBattleRecording);
    gBattleRecording = NULL;
}

BOOL BattleRecording_Exists(void)
{
    return gBattleRecording != NULL;
}

BattleRecording *BattleRecording_Get(void)
{
    GF_ASSERT(gBattleRecording);
    return gBattleRecording;
}

void *sub_0202F27C(void)
{
    u8 *v0;

    GF_ASSERT(gBattleRecording);

    v0 = (u8 *)gBattleRecording;
    return &v0[sizeof(u32)];
}

BOOL sub_0202F298(SaveData *saveData, int heapID, int *resultCode, FieldBattleDTO *fieldBattleDTO, int recNum)
{
    BattleRecordingData *battleRecordingData;
    UnkStruct_0202F41C *v1;

    if (gBattleRecording) {
        Heap_Free(gBattleRecording);
        gBattleRecording = NULL;
    }

    gBattleRecording = SaveData_GetBattleRecording(saveData, heapID, resultCode, recNum);

    if (*resultCode != 1) {
        *resultCode = 3;
        return 1;
    }

    battleRecordingData = &gBattleRecording->battleRecordingData;
    v1 = &gBattleRecording->unk_84;

    sub_0202F860(battleRecordingData, sizeof(BattleRecordingData) - (sizeof(BattleRecordingData_sub1)), battleRecordingData->unk_1BEC.unk_00 + ((battleRecordingData->unk_1BEC.unk_00 ^ 0xffff) << 16));

    if (sub_0202F75C(saveData, gBattleRecording) == 1) {
        *resultCode = 0;
        return 1;
    }

    if (sub_0202F794(saveData, gBattleRecording) == 0) {
        *resultCode = 2;
        return 1;
    }

    if (fieldBattleDTO) {
        sub_0202FAFC(fieldBattleDTO, saveData);
    }

    *resultCode = 1;
    return 1;
}

BOOL sub_0202F330(SaveData *saveData, int param1, int *param2, int param3)
{
    BattleRecordingData *battleRecordingData;
    UnkStruct_0202F41C *v1;
    BattleRecording *battleRecording = SaveData_GetBattleRecording(saveData, param1, param2, param3);

    if (*param2 != 1) {
        *param2 = 3;
        Heap_Free(battleRecording);
        return 0;
    }

    battleRecordingData = &battleRecording->battleRecordingData;
    v1 = &battleRecording->unk_84;

    sub_0202F860(battleRecordingData, sizeof(BattleRecordingData) - (sizeof(BattleRecordingData_sub1)), battleRecordingData->unk_1BEC.unk_00 + ((battleRecordingData->unk_1BEC.unk_00 ^ 0xffff) << 16));

    if (sub_0202F75C(saveData, battleRecording) == 1) {
        *param2 = 0;
        Heap_Free(battleRecording);
        return 0;
    }

    if (sub_0202F794(saveData, battleRecording) == 0) {
        *param2 = 2;
        Heap_Free(battleRecording);
        return 0;
    }

    *param2 = 1;
    Heap_Free(battleRecording);
    return 1;
}

int sub_0202F3AC(SaveData *saveData, BattleRecording *param1, int param2, u16 *param3)
{
    int v0;

    switch (*param3) {
    case 0:
        ResetLock(RESET_LOCK_0x8);
        InitHeapCanary(11);

        v0 = SaveData_SaveBattleRecording(saveData, param1, param2);

        if (v0 == 2) {
            SaveData_SaveStateInit(saveData, 2);
            (*param3)++;
            return 0;
        }

        ResetUnlock(RESET_LOCK_0x8);
        return v0;
    case 1:
        v0 = SaveData_SaveStateMain(saveData);

        if ((v0 == 2) || (v0 == 3)) {
            (*param3) = 0;
            FreeHeapCanary();
            ResetUnlock(RESET_LOCK_0x8);
        }

        return v0;
    }

    return 0;
}

int sub_0202F41C(SaveData *saveData, int param1, int param2, int param3, u16 *param4, u16 *param5)
{
    UnkStruct_0202F41C *v0;
    BattleRecordingData *v1;
    int v2;

    switch (*param4) {
    case 0:

        if (gBattleRecording == NULL) {
            return 3;
        }

        v0 = &gBattleRecording->unk_84;
        v1 = &gBattleRecording->battleRecordingData;

        sub_0202F510(saveData, v0, v1, param1, param2);

        v0->unk_48 = 0xe281;
        v0->unk_60.unk_00 = SaveData_CalculateChecksum(saveData, v0, sizeof(UnkStruct_0202F41C) - (sizeof(BattleRecordingData_sub1)) - (sizeof(u64)));
        v1->unk_1BEA = 0xe281;
        v1->unk_1BEC.unk_00 = SaveData_CalculateChecksum(saveData, v1, sizeof(BattleRecordingData) - (sizeof(BattleRecordingData_sub1)));

        sub_0202F858(v1, sizeof(BattleRecordingData) - (sizeof(BattleRecordingData_sub1)), v1->unk_1BEC.unk_00 + ((v1->unk_1BEC.unk_00 ^ 0xffff) << 16));

        *param5 = 0;
        (*param4)++;
        break;
    case 1:
        v2 = sub_0202F3AC(saveData, gBattleRecording, param3, param5);
        return v2;
    }

    return 0;
}

void sub_0202F4C0(int param0, int *param1, int *param2)
{
    switch (param0) {
    case (UnkEnum_0202F510_17):
    case (UnkEnum_0202F510_20):
    case (UnkEnum_0202F510_23):
    case (UnkEnum_0202F510_26):
    case (UnkEnum_0202F510_29):
    case (UnkEnum_0202F510_32):
    case (UnkEnum_0202F510_14):
        *param1 = 4;
        *param2 = 6 / 2;
        break;
    default:
        *param1 = 4 / 2;
        *param2 = 6;
        break;
    }
}

static void sub_0202F510(SaveData *saveData, UnkStruct_0202F41C *param1, const BattleRecordingData *battleRecordingData, int param3, int param4)
{
    int v0, v1, v2, v3, v4, v5, v6;
    const BattleRecordingPokemon *mon;
    const u8 v8[2][4] = {
        { 0, 2, 3, 1 },
        { 3, 1, 0, 2 },
    };
    const u8 v9[4] = { 0, 2, 1, 3 };

    MI_CpuClear8(param1, sizeof(UnkStruct_0202F41C));
    sub_0202F4C0(param3, &v2, &v3);

    v4 = 0;

    if (battleRecordingData->brdFieldBattleDTO.battleType & 0x4) {
        if (battleRecordingData->brdFieldBattleDTO.battleType & 0x80) {
            v6 = battleRecordingData->brdFieldBattleDTO.networkID * 2;
        } else {
            v6 = battleRecordingData->brdFieldBattleDTO.networkID;
        }
    } else {
        v6 = 0;
    }

    for (v0 = 0; v0 < v2; v0++) {
        if ((battleRecordingData->brdFieldBattleDTO.battleType & 0x8) && ((battleRecordingData->brdFieldBattleDTO.battleType & 0x80) == 0)) {
            for (v5 = 0; v5 < v2; v5++) {
                if (battleRecordingData->brdFieldBattleDTO.unk_134[v5] == v8[battleRecordingData->brdFieldBattleDTO.unk_134[v6] & 1][v0]) {
                    break;
                }
            }
        } else if ((battleRecordingData->brdFieldBattleDTO.battleType & 0x8) && (battleRecordingData->brdFieldBattleDTO.battleType & 0x80)) {
            v5 = v9[v0];
        } else {
            v5 = v0;

            if (v6 & 1) {
                v5 ^= 1;
            }
        }

        for (v1 = 0; v1 < v3; v1++) {
            mon = &(battleRecordingData->parties[v5].pokemon[v1]);

            if ((mon->isEgg == 0) && (mon->checksumFailed == 0)) {
                param1->unk_00[v4] = mon->species;
                param1->unk_18[v4] = mon->form;
            }

            v4++;
        }
    }

    switch (param3) {
    case (UnkEnum_0202F510_01):
    case (UnkEnum_0202F510_08):
        param1->battleRegulation = *(BattleRegulation_GetByIndex(saveData, 0));
        break;
    case (UnkEnum_0202F510_02):
    case (UnkEnum_0202F510_09):
        param1->battleRegulation = *(BattleRegulation_GetByIndex(saveData, 1));
        break;
    case (UnkEnum_0202F510_03):
    case (UnkEnum_0202F510_10):
        param1->battleRegulation = *(BattleRegulation_GetByIndex(saveData, 2));
        break;
    case (UnkEnum_0202F510_04):
    case (UnkEnum_0202F510_11):
        param1->battleRegulation = *(BattleRegulation_GetByIndex(saveData, 3));
        break;
    case (UnkEnum_0202F510_05):
    case (UnkEnum_0202F510_12):
        param1->battleRegulation = *(BattleRegulation_GetByIndex(saveData, 4));
        break;
    case (UnkEnum_0202F510_06):
    case (UnkEnum_0202F510_13):
        param1->battleRegulation = *(BattleRegulation_GetByIndex(saveData, 5));
        break;
    case (UnkEnum_0202F510_00):
    case (UnkEnum_0202F510_07):
    default:
        param1->battleRegulation = *(BattleRegulation_GetDefault());
        break;
    }

    param1->unk_24 = param4;
    param1->unk_26 = param3;
}

static BOOL sub_0202F75C(SaveData *saveData, BattleRecording *battleRecording)
{
    BattleRecordingData *v0 = &battleRecording->battleRecordingData;
    UnkStruct_0202F41C *v1 = &battleRecording->unk_84;

    if (SaveData_MiscSaveBlock_InitFlag(saveData) == 0) {
        return 1;
    }

    if ((v0->unk_1BEA != 0xe281) || (v1->unk_48 != 0xe281)) {
        return 1;
    }

    return 0;
}

static BOOL sub_0202F794(SaveData *saveData, const BattleRecording *battleRecording)
{
    const BattleRecordingData *v0 = &battleRecording->battleRecordingData;
    const UnkStruct_0202F41C *v1 = &battleRecording->unk_84;
    u16 v2;

    if ((v0->unk_1BEA != 0xe281) || (v1->unk_48 != 0xe281)) {
        return 0;
    }

    v2 = SaveData_CalculateChecksum(saveData, v1, sizeof(UnkStruct_0202F41C) - (sizeof(BattleRecordingData_sub1)) - (sizeof(u64)));

    if (v2 != v1->unk_60.unk_00) {
        return 0;
    }

    v2 = SaveData_CalculateChecksum(saveData, v0, sizeof(BattleRecordingData) - (sizeof(BattleRecordingData_sub1)));

    if (v2 != v0->unk_1BEC.unk_00) {
        return 0;
    }

    {
        int v3, v4, v5;
        const BattleRecordingPokemon *v6;

        for (v3 = 0; v3 < 4; v3++) {
            for (v4 = 0; v4 < 6; v4++) {
                v6 = &(v0->parties[v3].pokemon[v4]);

                if (v6->species > MAX_SPECIES) {
                    return 0;
                }

                if (v6->heldItem > 467) {
                    return 0;
                }

                for (v5 = 0; v5 < 4; v5++) {
                    if (v6->moves[v5] > 467) {
                        return 0;
                    }
                }
            }
        }
    }

    return 1;
}

void sub_0202F858(void *param0, u32 param1, u32 param2)
{
    EncodeData(param0, param1, param2);
}

static void sub_0202F860(void *param0, u32 param1, u32 param2)
{
    DecodeData(param0, param1, param2);
}

void sub_0202F868(int param0, int param1, u8 param2)
{
    if (gBattleRecording == NULL) {
        return;
    }

    gBattleRecording->battleRecordingData.unk_150.unk_00[param0][param1] = param2;
}

u8 sub_0202F884(int param0, int param1)
{
    GF_ASSERT(gBattleRecording != NULL);
    return gBattleRecording->battleRecordingData.unk_150.unk_00[param0][param1];
}

void sub_0202F8AC(FieldBattleDTO *fieldBattleDTO)
{
    int t; // Trainer counter for iteration
    BattleRecordingData *battleRecordingData;
    BattleRecordingDataFieldBattleDTO *brdFieldBattleDTO;

    if (gBattleRecording == NULL) {
        return;
    }

    battleRecordingData = &gBattleRecording->battleRecordingData;
    brdFieldBattleDTO = &battleRecordingData->brdFieldBattleDTO;

    brdFieldBattleDTO->battleType = fieldBattleDTO->battleType;
    brdFieldBattleDTO->resultMask = fieldBattleDTO->resultMask;
    brdFieldBattleDTO->background = fieldBattleDTO->background;
    brdFieldBattleDTO->terrain = fieldBattleDTO->terrain;
    brdFieldBattleDTO->mapLabelTextID = fieldBattleDTO->mapLabelTextID;
    brdFieldBattleDTO->mapHeaderID = fieldBattleDTO->mapHeaderID;
    brdFieldBattleDTO->timeOfDay = fieldBattleDTO->timeOfDay;
    brdFieldBattleDTO->mapEvolutionMethod = fieldBattleDTO->mapEvolutionMethod;
    brdFieldBattleDTO->visitedContestHall = fieldBattleDTO->visitedContestHall;
    brdFieldBattleDTO->metBebe = fieldBattleDTO->metBebe;
    brdFieldBattleDTO->caughtBattlerIdx = fieldBattleDTO->caughtBattlerIdx;
    brdFieldBattleDTO->fieldWeather = fieldBattleDTO->fieldWeather;
    brdFieldBattleDTO->leveledUpMonsMask = fieldBattleDTO->leveledUpMonsMask;
    brdFieldBattleDTO->battleStatusMask = fieldBattleDTO->battleStatusMask;
    brdFieldBattleDTO->countSafariBalls = fieldBattleDTO->countSafariBalls;
    brdFieldBattleDTO->rulesetMask = fieldBattleDTO->rulesetMask;
    brdFieldBattleDTO->seed = fieldBattleDTO->seed;
    brdFieldBattleDTO->networkID = fieldBattleDTO->networkID;
    brdFieldBattleDTO->dummy18B = fieldBattleDTO->dummy18B;
    brdFieldBattleDTO->totalTurnsElapsed = fieldBattleDTO->totalTurnsElapsed;

    // Copy Trainers
    for (t = 0; t < 4; t++) {
        brdFieldBattleDTO->trainerIDs[t] = fieldBattleDTO->trainerIDs[t];
        brdFieldBattleDTO->trainer[t] = fieldBattleDTO->trainer[t];

        if (fieldBattleDTO->systemVersion[t] == 0) {
            brdFieldBattleDTO->systemVersion[t] = 0x140;
        } else {
            brdFieldBattleDTO->systemVersion[t] = fieldBattleDTO->systemVersion[t];
        }

        brdFieldBattleDTO->unk_134[t] = fieldBattleDTO->unk_178[t];
        brdFieldBattleDTO->unk_14C[t] = fieldBattleDTO->unk_194[t];
    }

    // Copy Trainers' Parties
    for (t = 0; t < 4; t++) {
        Copy_Party_to_BattleRecordingParty(fieldBattleDTO->parties[t], &battleRecordingData->parties[t]);
        TrainerInfo_Copy(fieldBattleDTO->trainerInfo[t], &battleRecordingData->trainerInfo[t]);

        brdFieldBattleDTO->unk_14C[t] = Sound_GetChatterActivationParameter(fieldBattleDTO->chatotCries[t]);
    }

    Options_Copy(fieldBattleDTO->options, &battleRecordingData->options);
}

void sub_0202FAA8(int param0, u32 param1)
{
    BattleRecordingData *battleRecordingData;
    BattleRecordingDataFieldBattleDTO *brdFieldBattleDTO;

    if (gBattleRecording == NULL) {
        return;
    }

    battleRecordingData = &gBattleRecording->battleRecordingData;
    brdFieldBattleDTO = &battleRecordingData->brdFieldBattleDTO;

    brdFieldBattleDTO->systemVersion[param0] = param1;
}

BOOL sub_0202FAC0(void)
{
    int v0;
    BattleRecordingData *battleRecordingData;
    BattleRecordingDataFieldBattleDTO *brdFieldBattleDTO;

    if (gBattleRecording == NULL) {
        return 1;
    }

    battleRecordingData = &gBattleRecording->battleRecordingData;
    brdFieldBattleDTO = &battleRecordingData->brdFieldBattleDTO;

    for (v0 = 0; v0 < 4; v0++) {
        if (brdFieldBattleDTO->systemVersion[v0] > 0x140) {
            return 0;
        }
    }

    return 1;
}

void sub_0202FAFC(FieldBattleDTO *dto, SaveData *saveData)
{
    int i;
    BattleRecordingData *battleRecordingData = &gBattleRecording->battleRecordingData;

    dto->battleType = battleRecordingData->brdFieldBattleDTO.battleType;
    dto->background = battleRecordingData->brdFieldBattleDTO.background;
    dto->terrain = battleRecordingData->brdFieldBattleDTO.terrain;
    dto->mapLabelTextID = battleRecordingData->brdFieldBattleDTO.mapLabelTextID;
    dto->mapHeaderID = battleRecordingData->brdFieldBattleDTO.mapHeaderID;
    dto->timeOfDay = battleRecordingData->brdFieldBattleDTO.timeOfDay;
    dto->mapEvolutionMethod = battleRecordingData->brdFieldBattleDTO.mapEvolutionMethod;
    dto->visitedContestHall = battleRecordingData->brdFieldBattleDTO.visitedContestHall;
    dto->metBebe = battleRecordingData->brdFieldBattleDTO.metBebe;
    dto->caughtBattlerIdx = battleRecordingData->brdFieldBattleDTO.caughtBattlerIdx;
    dto->fieldWeather = battleRecordingData->brdFieldBattleDTO.fieldWeather;
    dto->battleStatusMask = battleRecordingData->brdFieldBattleDTO.battleStatusMask | BATTLE_STATUS_RECORDING;
    dto->countSafariBalls = battleRecordingData->brdFieldBattleDTO.countSafariBalls;
    dto->rulesetMask = battleRecordingData->brdFieldBattleDTO.rulesetMask;
    dto->seed = battleRecordingData->brdFieldBattleDTO.seed;
    dto->networkID = battleRecordingData->brdFieldBattleDTO.networkID;
    dto->resultMask = BATTLE_IN_PROGRESS;
    dto->leveledUpMonsMask = 0;

    Pokedex_Copy(SaveData_GetPokedex(saveData), dto->pokedex);

    for (i = 0; i < 4; i++) {
        dto->trainerIDs[i] = battleRecordingData->brdFieldBattleDTO.trainerIDs[i];
        dto->trainer[i] = battleRecordingData->brdFieldBattleDTO.trainer[i];
        dto->systemVersion[i] = battleRecordingData->brdFieldBattleDTO.systemVersion[i];
        dto->unk_178[i] = battleRecordingData->brdFieldBattleDTO.unk_134[i];

        Copy_BattleRecordingParty_to_Party(&battleRecordingData->parties[i], dto->parties[i]);
        TrainerInfo_Copy(&battleRecordingData->trainerInfo[i], dto->trainerInfo[i]);

        dto->unk_194[i] = battleRecordingData->brdFieldBattleDTO.unk_14C[i];
    }

    Options_Copy(SaveData_GetOptions(saveData), dto->options);
    dto->options->frame = battleRecordingData->options.frame;

    if (dto->options->frame >= 20) {
        dto->options->frame = 0;
    }
}

static void Copy_Party_to_BattleRecordingParty(const Party *sourceParty, BattleRecordingParty *targetParty)
{
    int slotIndex;
    Pokemon *pokemon;

    MI_CpuClear8(targetParty, sizeof(BattleRecordingParty));

    targetParty->capacity = Party_GetCapacity(sourceParty);
    targetParty->currentCount = Party_GetCurrentCount(sourceParty);

    for (slotIndex = 0; slotIndex < targetParty->currentCount; slotIndex++) {
        pokemon = Party_GetPokemonBySlotIndex(sourceParty, slotIndex);
        Copy_Pokemon_to_BattleRecordingPokemon(pokemon, &targetParty->pokemon[slotIndex]);
    }
}

static void Copy_BattleRecordingParty_to_Party(BattleRecordingParty *sourceParty, Party *targetParty)
{
    int slotIndex;
    Pokemon *pokemon;
    u8 v2 = 0;

    pokemon = Pokemon_New(HEAP_ID_FIELD2);

    Party_InitWithCapacity(targetParty, sourceParty->capacity);

    for (slotIndex = 0; slotIndex < sourceParty->currentCount; slotIndex++) {
        Copy_BattleRecordingPokemon_to_Pokemon(&sourceParty->pokemon[slotIndex], pokemon);
        Pokemon_SetValue(pokemon, MON_DATA_BALL_CAPSULE_ID, &v2);
        Party_AddPokemon(targetParty, pokemon);
    }

    Heap_Free(pokemon);
}

UnkStruct_0202F41C *sub_0202FD88(enum HeapID heapID)
{
    UnkStruct_0202F41C *v0;

    GF_ASSERT(gBattleRecording != NULL);

    v0 = Heap_Alloc(heapID, sizeof(UnkStruct_0202F41C));
    MI_CpuCopy32(&gBattleRecording->unk_84, v0, sizeof(UnkStruct_0202F41C));

    return v0;
}

UnkStruct_02030A80 *sub_0202FDB8(enum HeapID heapID)
{
    UnkStruct_02030A80 *v0;

    GF_ASSERT(gBattleRecording != NULL);

    v0 = Heap_Alloc(heapID, sizeof(UnkStruct_02030A80));
    MI_CpuCopy32(&gBattleRecording->unk_04, v0, sizeof(UnkStruct_02030A80));

    return v0;
}

UnkStruct_02030A80 *sub_0202FDE8(void)
{
    GF_ASSERT(gBattleRecording != NULL);
    return &gBattleRecording->unk_04;
}

UnkStruct_0202F41C *sub_0202FE04(void)
{
    GF_ASSERT(gBattleRecording != NULL);
    return &gBattleRecording->unk_84;
}

void sub_0202FE20(UnkStruct_02030A80 *param0, UnkStruct_0202F41C *param1, BattleRecordingData *param2, FieldBattleDTO *param3, SaveData *saveData)
{
    GF_ASSERT(gBattleRecording != NULL);

    MI_CpuCopy8(param1, &gBattleRecording->unk_84, sizeof(UnkStruct_0202F41C));
    MI_CpuCopy8(param2, &gBattleRecording->battleRecordingData, sizeof(BattleRecordingData));
    MI_CpuCopy8(param0, &gBattleRecording->unk_04, sizeof(UnkStruct_02030A80));

    sub_0202F860(&gBattleRecording->battleRecordingData, sizeof(BattleRecordingData) - (sizeof(BattleRecordingData_sub1)), gBattleRecording->battleRecordingData.unk_1BEC.unk_00 + ((gBattleRecording->battleRecordingData.unk_1BEC.unk_00 ^ 0xffff) << 16));

    if (param3 != NULL) {
        sub_0202FAFC(param3, saveData);
    }
}

u64 sub_0202FE98(UnkStruct_0202F41C *param0, int param1, int param2)
{
    GF_ASSERT((sizeof(u64)) <= sizeof(u64));

    switch (param1) {
    case 0:
        GF_ASSERT(param2 < 12);

        if (param0->unk_00[param2] > NATIONAL_DEX_COUNT) {
            return 0;
        }

        return param0->unk_00[param2];
    case 1:
        GF_ASSERT(param2 < 12);
        return param0->unk_18[param2];
    case 2:
        if (param0->unk_24 > 9999) {
            return 9999;
        }

        return param0->unk_24;
    case 3:
        if (param0->unk_26 >= (UnkEnum_0202F510_32 + 1)) {
            return UnkEnum_0202F510_00;
        }

        return param0->unk_26;
    case 4:
        return param0->unk_58;
    case 5:
        return param0->unk_27;
    }

    GF_ASSERT(0);
    return 0;
}

UnkStruct_0202F41C *sub_0202FF2C(enum HeapID heapID)
{
    UnkStruct_0202F41C *v0 = Heap_Alloc(heapID, sizeof(UnkStruct_0202F41C));
    MI_CpuClear8(v0, sizeof(UnkStruct_0202F41C));

    return v0;
}

void sub_0202FF44(UnkStruct_0202F41C *param0)
{
    Heap_Free(param0);
}
