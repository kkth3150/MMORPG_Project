#pragma once
#pragma once
#include "ItemData_UseItem.h"


class CItemData_Scroll : public CItemData_UseItem
{
public:
    void            Set_ScrollType(SCROLL_TYPE eType);
    SCROLL_TYPE     Get_ScrollType() const { return m_eScrollType; }
    virtual void    Use_Item(CPlayer* pPlayer) override;

private:
    SCROLL_TYPE     m_eScrollType = SCROLL_END;
};