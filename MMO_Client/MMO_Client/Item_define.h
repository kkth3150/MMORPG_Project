#pragma once
#include "define.h"
#include <TCHAR.h>

#define INVEN_LEFT      0
#define ITEM_NOEXIST    -1
#define INVEN_FULL      -1
#define INVEN_SIZE      40
#define INVEN_SLOT_SIZE  28.f


enum ITEM_TYPE
{
    ITEM_USE,
    ITEM_EQUIPMENT,
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
    SLOT_HELMAT,
    SLOT_PENDANT,
    SLOT_RING,
    SLOT_END
};


struct EQUIP_SLOT_INFO
{
    float fX;       // 장비창 내 위치
    float fY;
    float fW;       // 렌더 사이즈
    float fH;
};


// 장비창 슬롯 정보 테이블
static const EQUIP_SLOT_INFO s_EquipSlotInfo[SLOT_END] =
{
    // fX    fY     fW    fH
    {  14.f,  84.f, 28.f, 84.f },   // SLOT_WEAPON  - 무기    28x84
    {  70.f, 28.f, 56.f, 84.f },    // SLOT_ARMOR   - 방어구  56x84
    {  10.f, 10.f,56.f,84.f },      //SLOT_SHIELD   - 쉴드    56x84
    {2.f,2.f,56.f,56.f},            //SLOT_HELMAT   - 헬멧    56X56
    {2.f,2.f,INVEN_SLOT_SIZE,INVEN_SLOT_SIZE},            //SLOT_PENDANT - INVEN_SLOT_SIZE
    {2.f,2.f,INVEN_SLOT_SIZE,INVEN_SLOT_SIZE}             //SLOT_RING   - INVEN_SLOT_SIZE


    // 필요시 헬멧, 방패 추가
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