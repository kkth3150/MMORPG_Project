#include "pch.h"
#include "ItemData_Potion.h"
#include "Player.h"
#include "Object_Manager.h"

static const POTION_DATA s_PotionTable[POTION_END] =
{
    { L"HP 器记(家)",  L"Potion_mHP",  30  },   
    { L"HP 器记(措)",  L"Potion_lHP",  50  }, 
    { L"MP 器记(家)",  L"Potion_mMp",  30  },
    { L"MP 器记(措)",  L"Potion_lMp",  50  },
    { L"傍拜仿 器记",   L"Potion_Atk",  10  },
    { L"公利 器记",     L"Potion_Invincible",  0  },

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
    case POTION_HP_M:
        pPlayer->Add_Hp(m_iAmount);
        break;
    case POTION_HP_L:
        pPlayer->Add_Hp(m_iAmount);
        break;
    case POTION_MP_M:
        pPlayer->Add_Mp(m_iAmount);
        break;
    case POTION_MP_L:
        pPlayer->Add_Mp(m_iAmount);
        break;
    case POTION_INVINCIBLE:
        break;
    case POTION_ATK:
        break;

    default:
        break;
    }
    
}


