#pragma once
#include "define.h"
#include <TCHAR.h>

#define INVEN_LEFT      0
#define ITEM_NOEXIST    -1
#define INVEN_FULL      -1
#define INVEN_SIZE      20

enum ITEM_TYPE
{
    ITEM_USE,
    ITEM_EQUIPMENT,
    ITEM_END
};

enum POTION_TYPE
{
    POTION_HP,
    POTION_MP,
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
    SLOT_END
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