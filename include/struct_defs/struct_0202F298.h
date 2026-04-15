#ifndef POKEPLATINUM_STRUCT_0202F298_H
#define POKEPLATINUM_STRUCT_0202F298_H

#include "struct_defs/struct_0202F298_sub1.h"
#include "struct_defs/struct_0202FAA8.h"
#include "struct_defs/struct_0202FAA8_sub1.h"
#include "struct_defs/struct_0202FD30.h"

#include "game_options.h"
#include "trainer_info.h"

typedef struct BattleRecordingData_t {
    BattleRecordingDataFieldBattleDTO brdFieldBattleDTO;
    BattleRecordingDataFieldBattleDTO_sub1 unk_150;
    BattleRecordingParty parties[4];
    TrainerInfo trainerInfo[4];
    Options options;
    u16 unk_1BEA;
    BattleRecordingData_sub1 unk_1BEC;
} BattleRecordingData;

#endif // POKEPLATINUM_STRUCT_0202F298_H
