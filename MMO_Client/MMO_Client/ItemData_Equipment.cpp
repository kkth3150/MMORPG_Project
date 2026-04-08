#include "pch.h"
#include "ItemData_Equipment.h"

static const EQUIP_DATA s_EquipTable[EQUIP_TYPE_END] =
{
    // ¿Ã∏ß               æ∆¿Ãƒ‹ ¿ÃπÃ¡ˆ              iAtk  iDef  eSlot
    { L"«—º’∞À",        L"ICON_EQUIP_SWORD",      10,   0,  SLOT_WEAPON },
    { L"»∞",            L"ICON_EQUIP_BOW",          8,   0,  SLOT_WEAPON },
    { L"∞°¡◊ ∞©ø ",     L"ICON_EQUIP_LEATHER",      0,   5,  SLOT_ARMOR  },
    { L"∆«±› ∞©ø ",     L"ICON_EQUIP_PLATE",         0,  15,  SLOT_ARMOR  },
};

void CItemData_Equipment::Set_EquipType(EQUIPMENT_TYPE eType)
{
    m_eEquipType = eType;

    const EQUIP_DATA& data = s_EquipTable[eType];
    lstrcpy(m_szName, data.szName);
    lstrcpy(m_szIconKey, data.szIconKey);
    m_iAtkBonus = data.iAtkBonus;
    m_iDefBonus = data.iDefBonus;
    m_eSlot = data.eSlot;
    m_eType = ITEM_EQUIPMENT;
}