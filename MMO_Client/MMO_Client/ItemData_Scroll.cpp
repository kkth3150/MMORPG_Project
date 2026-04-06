#include "pch.h"
#include "ItemData_Scroll.h"

static const SCROLL_DATA s_ScrollTable[SCROLL_END] =
{
    { L"마을 귀환 주문서", L"ICON_SCROLL_TOWN"     },
    { L"감정 주문서",      L"ICON_SCROLL_IDENTIFY" },
};

void CItemData_Scroll::Set_ScrollType(SCROLL_TYPE eType)
{
    m_eScrollType = eType;

    const SCROLL_DATA& data = s_ScrollTable[eType];
    lstrcpy(m_szName, data.szName);
    lstrcpy(m_szIconKey, data.szIconKey);
    m_eType = ITEM_USE;
}

void CItemData_Scroll::Use_Item()
{
    switch (m_eScrollType)
    {
    case SCROLL_TOWN:
        break;
    case SCROLL_IDENTIFY:
        break;
    default:
        break;
    }
}