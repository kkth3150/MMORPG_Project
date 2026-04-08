#pragma once
#include "ItemData.h"

enum EQUIPMENT_TYPE
{
    EQUIP_SWORD,
    EQUIP_BOW,
    EQUIP_LEATHER_ARMOR,
    EQUIP_PLATE_ARMOR,
    EQUIP_TYPE_END
};

class CItemData_Equipment : public CItemData
{
public:
    CItemData_Equipment() = default;
    virtual ~CItemData_Equipment() = default;

    void            Set_EquipType(EQUIPMENT_TYPE eType);
    EQUIPMENT_TYPE  Get_EquipType()  const { return m_eEquipType; }
    EQUIP_SLOT      Get_EquipSlot()  const { return m_eSlot; }
    int             Get_AtkBonus()   const { return m_iAtkBonus; }
    int             Get_DefBonus()   const { return m_iDefBonus; }

private:
    EQUIPMENT_TYPE  m_eEquipType = EQUIP_TYPE_END;
    EQUIP_SLOT      m_eSlot = SLOT_END;
    int             m_iAtkBonus = 0;
    int             m_iDefBonus = 0;
};