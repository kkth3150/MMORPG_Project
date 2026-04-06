#include "pch.h"
#include "ItemData_Potion.h"
#include "Player.h"
#include "Object_Manager.h"

void CItemData_Potion::Set_PotionType(POTION_TYPE eType)
{
    m_ePotionType = eType;

    const POTION_DATA& data = s_PotionTable[eType];
    lstrcpy(m_szName, data.szName);
    lstrcpy(m_szIconKey, data.szIconKey);
    m_iAmount = data.iAmount;
    m_eType = ITEM_USE;
}

void CItemData_Potion::Use_Item()
{

    CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject_Manager::Get_Instance()->Get_Player());

    switch (m_ePotionType)
    {
    case POTION_HP:
        pPlayer->Add_Hp(10);
        break;
    case POTION_MP:
        break;
    default:
        break;
    }
}
