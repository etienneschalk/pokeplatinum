#ifndef POKEPLATINUM_STRUCT_0202FD30_H
#define POKEPLATINUM_STRUCT_0202FD30_H

#include "constants/pokemon.h"

#include "struct_defs/struct_02078B40.h"

typedef struct {
    u16 capacity;
    u16 currentCount;
    BattleRecordingPokemon pokemon[MAX_PARTY_SIZE];
} BattleRecordingParty;

#endif // POKEPLATINUM_STRUCT_0202FD30_H
