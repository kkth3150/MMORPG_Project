#pragma once
#include "define.h"
#include <TCHAR.h>

#define INVEN_LEFT      0
#define ITEM_NOEXIST    -1
#define INVEN_FULL      -1
#define INVEN_SIZE      40
#define INVEN_RATE       1.3f
#define INVEN_SLOT_SIZE  28.f * INVEN_RATE
#define STACK_FULL      99

enum ITEM_TYPE
{
    ITEM_USE,
    ITEM_EQUIPMENT,
    ITEM_ETC,
    ITEM_END
};

enum POTION_TYPE
{
    POTION_HP_M,
    POTION_HP_L,
    POTION_MP_M,
    POTION_MP_L,
    POTION_INVINCIBLE,
    POTION_ATK,
    POTION_END
};

enum SCROLL_TYPE
{
    SCROLL_TOWN,       // 마을 귀환
    SCROLL_IDENTIFY,   // 감정 주문서
    SCROLL_END
};

enum EQUIP_SLOT
{
    SLOT_WEAPON,
    SLOT_ARMOR,
    SLOT_SHIELD,
    SLOT_HELMET,
    SLOT_PENDANT,
    SLOT_RING,
    SLOT_END
};



struct EQUIP_SLOT_INFO
{
    float fX;
    float fY;
    float fW;  
    float fH;
};

enum EQUIPMENT_TYPE
{
    // Sword
    EQUIP_SWORD_0, EQUIP_SWORD_1, EQUIP_SWORD_2,
    EQUIP_SWORD_3, EQUIP_SWORD_4, EQUIP_SWORD_5, EQUIP_SWORD_6,
    // Armor
    EQUIP_ARMOR_0, EQUIP_ARMOR_1, EQUIP_ARMOR_2, EQUIP_ARMOR_3,
    EQUIP_ARMOR_4, EQUIP_ARMOR_5, EQUIP_ARMOR_6, EQUIP_ARMOR_7,
    // Helmet
    EQUIP_HELMET_0, EQUIP_HELMET_1, EQUIP_HELMET_2, EQUIP_HELMET_3,
    EQUIP_HELMET_4, EQUIP_HELMET_5, EQUIP_HELMET_6,
    // Shield
    EQUIP_SHIELD_0, EQUIP_SHIELD_1, EQUIP_SHIELD_2, EQUIP_SHIELD_3,
    EQUIP_SHIELD_4, EQUIP_SHIELD_5, EQUIP_SHIELD_6,
    // Ring
    EQUIP_RING_0, EQUIP_RING_1, EQUIP_RING_2, EQUIP_RING_3, EQUIP_RING_4,
    // Pendant
    EQUIP_PENDANT_0, EQUIP_PENDANT_1, EQUIP_PENDANT_2,
    EQUIP_PENDANT_3, EQUIP_PENDANT_4,

    EQUIP_TYPE_END
};

enum ETC_TYPE
{
    ETC_BLOODSTONE,
    ETC_BLUEGEM,
    ETC_PRIZMGEM,
    ETC_REDGEM,
    ETC_BRAIN,
    ETC_CLAW,
    ETC_EAR_0,
    ETC_EAR_1,
    ETC_EAR_2,
    ETC_TOOTH,
    ETC_END
};



// 포션 종류별 데이터 테이블
struct POTION_DATA
{
    const TCHAR* szName;
    const TCHAR* szIconKey;
    int             iAmount;    // 회복량
};

// 주문서 종류별 데이터 테이블
struct SCROLL_DATA
{
    const TCHAR* szName;
    const TCHAR* szIconKey;
};

// 장비 종류별 데이터 테이블
struct EQUIP_DATA
{
    const TCHAR* szName;
    const TCHAR* szIconKey;
    int             iAtkBonus;
    int             iDefBonus;
    EQUIP_SLOT      eSlot;
    float           fInvenCX;
    float           fInvenCY;
    float           fEquipCX;
    float           fEquipCY;
};

struct ETC_DATA
{
    const TCHAR* szName;
    const TCHAR* szIconKey;
};