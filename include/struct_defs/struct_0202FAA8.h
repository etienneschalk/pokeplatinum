#ifndef POKEPLATINUM_STRUCT_0202FAA8_H
#define POKEPLATINUM_STRUCT_0202FAA8_H

#include "struct_defs/trainer.h"

typedef struct {
    u32 battleType;
    int resultMask;
    int trainerIDs[4];
    Trainer trainer[4];
    int background;
    int terrain;
    int mapLabelTextID;
    int mapHeaderID;
    int timeOfDay;
    int mapEvolutionMethod;
    int visitedContestHall;
    int metBebe;
    int caughtBattlerIdx;
    int fieldWeather;
    int leveledUpMonsMask;
    u32 systemVersion[4];
    u32 battleStatusMask;
    int countSafariBalls;
    u32 rulesetMask;
    u32 seed;
    int unk_134[4];
    u16 networkID;
    u16 dummy18B;
    int totalTurnsElapsed;
    u8 unk_14C[4];
} BattleRecordingDataFieldBattleDTO;

#endif // POKEPLATINUM_STRUCT_0202FAA8_H
