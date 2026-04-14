#ifndef POKEPLATINUM_STRUCT_OV61_0222BED8_SUB2_SUB3_SUB1_H
#define POKEPLATINUM_STRUCT_OV61_0222BED8_SUB2_SUB3_SUB1_H

typedef struct {
    u32 personality;
    u16 partyDecrypted : 1;
    u16 boxDecrypted : 1;
    u16 checksumFailed : 1;
    u16 : 13;
    u16 species;
    u16 heldItem;
    u32 otID;
    u32 exp;
    u8 friendship;
    u8 ability;
    u8 hpEV;
    u8 atkEV;
    u8 defEV;
    u8 speedEV;
    u8 spAtkEV;
    u8 spDefEV;
    u16 moves[4];
    u8 moveCurrentPPs[4];
    u8 movePPUps[4];
    u32 hpIV : 5;
    u32 atkIV : 5;
    u32 defIV : 5;
    u32 speedIV : 5;
    u32 spAtkIV : 5;
    u32 spDefIV : 5;
    u32 isEgg : 1;
    u32 hasNickname : 1;
    u8 fatefulEncounter : 1;
    u8 gender : 2;
    u8 form : 5;
    u16 nickname[11];
    u16 otName[8];
    u8 pokeball;
    u8 originLanguage;
    u32 status;
    u8 level;
    u8 ballCapsuleID;
    u16 hp;
    u16 maxHP;
    u16 attack;
    u16 defense;
    u16 speed;
    u16 spAtk;
    u16 spDef;
} UnkStruct_ov61_0222BED8_sub2_sub3_sub1;

#endif // POKEPLATINUM_STRUCT_OV61_0222BED8_SUB2_SUB3_SUB1_H
