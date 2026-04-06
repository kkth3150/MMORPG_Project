#pragma once
#include "ItemData_UseItem.h"

static const POTION_DATA s_PotionTable[POTION_END] =
{
    { L"HP 포션",  L"ICON_POTION_HP",  50  },   // POTION_HP
    { L"MP 포션",  L"ICON_POTION_MP",  30  },   // POTION_MP
};


class CItemData_Potion : public CItemData_UseItem
{
public:
    CItemData_Potion() = default;
    virtual ~CItemData_Potion() = default;

    void            Set_PotionType(POTION_TYPE eType);
    POTION_TYPE     Get_PotionType() const { return m_ePotionType; }
    virtual void    Use_Item() override;

private:

    POTION_TYPE     m_ePotionType = POTION_END;
    int             m_iAmount = 0;
};