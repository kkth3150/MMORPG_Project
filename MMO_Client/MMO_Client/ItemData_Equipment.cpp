#include "pch.h"
#include "ItemData_Equipment.h"

static const EQUIP_DATA s_EquipTable[EQUIP_TYPE_END] =
{
    // ÀÌ¸§           ¾ÆÀÌÄÜÅ°        iAtk  iDef  eSlot
    { L"°Ë_0",      L"Sword_0",      12,   0,  SLOT_WEAPON },
    { L"°Ë_1",      L"Sword_1",      15,   0,  SLOT_WEAPON },
    { L"°Ë_2",      L"Sword_2",      18,   0,  SLOT_WEAPON },
    { L"°Ë_3",      L"Sword_3",      22,   0,  SLOT_WEAPON },
    { L"°Ë_4",      L"Sword_4",      27,   0,  SLOT_WEAPON },
    { L"°Ë_5",      L"Sword_5",      33,   0,  SLOT_WEAPON },
    { L"°Ë_6",      L"Sword_6",      40,   0,  SLOT_WEAPON },

    { L"°©¿Ê_0",    L"Armor_0",       0,   5,  SLOT_ARMOR  },
    { L"°©¿Ê_1",    L"Armor_1",       0,   8,  SLOT_ARMOR  },
    { L"°©¿Ê_2",    L"Armor_2",       0,  11,  SLOT_ARMOR  },
    { L"°©¿Ê_3",    L"Armor_3",       0,  14,  SLOT_ARMOR  },
    { L"°©¿Ê_4",    L"Armor_4",       0,  18,  SLOT_ARMOR  },
    { L"°©¿Ê_5",    L"Armor_5",       0,  23,  SLOT_ARMOR  },
    { L"°©¿Ê_6",    L"Armor_6",       0,  28,  SLOT_ARMOR  },
    { L"°©¿Ê_7",    L"Armor_7",       0,  34,  SLOT_ARMOR  },

    { L"Åõ±¸_0",    L"Helmet_0",      0,   3,  SLOT_HELMET },
    { L"Åõ±¸_1",    L"Helmet_1",      0,   5,  SLOT_HELMET },
    { L"Åõ±¸_2",    L"Helmet_2",      0,   7,  SLOT_HELMET },
    { L"Åõ±¸_3",    L"Helmet_3",      0,  10,  SLOT_HELMET },
    { L"Åõ±¸_4",    L"Helmet_4",      0,  13,  SLOT_HELMET },
    { L"Åõ±¸_5",    L"Helmet_5",      0,  17,  SLOT_HELMET },
    { L"Åõ±¸_6",    L"Helmet_6",      0,  22,  SLOT_HELMET },

    { L"¹æÆÐ_0",    L"shield_0",      0,   4,  SLOT_SHIELD },
    { L"¹æÆÐ_1",    L"shield_1",      0,   7,  SLOT_SHIELD },
    { L"¹æÆÐ_2",    L"shield_2",      0,  10,  SLOT_SHIELD },
    { L"¹æÆÐ_3",    L"shield_3",      0,  14,  SLOT_SHIELD },
    { L"¹æÆÐ_4",    L"shield_4",      0,  19,  SLOT_SHIELD },
    { L"¹æÆÐ_5",    L"shield_5",      0,  25,  SLOT_SHIELD },
    { L"¹æÆÐ_6",    L"shield_6",      0,  32,  SLOT_SHIELD },

    { L"¹ÝÁö_0",    L"Ring_0",        2,   2,  SLOT_RING   },
    { L"¹ÝÁö_1",    L"Ring_1",        4,   4,  SLOT_RING   },
    { L"¹ÝÁö_2",    L"Ring_2",        6,   6,  SLOT_RING   },
    { L"¹ÝÁö_3",    L"Ring_3",        9,   9,  SLOT_RING   },
    { L"¹ÝÁö_4",    L"Ring_4",       13,  13,  SLOT_RING   },

    { L"¸ñ°ÉÀÌ_0",  L"Pendant_0",     3,   0,  SLOT_PENDANT },
    { L"¸ñ°ÉÀÌ_1",  L"Pendant_1",     5,   0,  SLOT_PENDANT },
    { L"¸ñ°ÉÀÌ_2",  L"Pendant_2",     8,   0,  SLOT_PENDANT },
    { L"¸ñ°ÉÀÌ_3",  L"Pendant_3",     12,  0,  SLOT_PENDANT },
    { L"¸ñ°ÉÀÌ_4",  L"Pendant_4",     17,  0,  SLOT_PENDANT },
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