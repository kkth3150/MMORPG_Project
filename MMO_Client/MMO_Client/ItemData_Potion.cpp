#include "pch.h"
#include "ItemData_Potion.h"
#include "Player.h"
#include "Object_Manager.h"

static const POTION_DATA s_PotionTable[POTION_END] =
{
    { L"HP 포션",  L"ICON_POTION_HP",  50  },   // POTION_HP
    { L"MP 포션",  L"ICON_POTION_MP",  30  },   // POTION_MP
};


void CItemData_Potion::Set_PotionType(POTION_TYPE eType)
{
    m_ePotionType = eType;

    const POTION_DATA& data = s_PotionTable[eType];
    lstrcpy(m_szName, data.szName);
    lstrcpy(m_szIconKey, data.szIconKey);
    m_iAmount = data.iAmount;
    m_eType = ITEM_USE;
}

void CItemData_Potion::Use_Item(CPlayer* pPlayer)
{
    switch (m_ePotionType)
    {
    case POTION_HP:
        pPlayer->Add_Hp(m_iAmount);
        break;
    case POTION_MP:
        break;
    default:
        break;
    }
    
}


