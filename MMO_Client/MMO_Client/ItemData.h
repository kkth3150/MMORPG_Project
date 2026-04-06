#pragma once
#include "Item_Define.h"

class CItemData
{
public:
    virtual ~CItemData() = default;

    const TCHAR* Get_Name()    const { return m_szName; }
    const TCHAR* Get_IconKey() const { return m_szIconKey; }
    ITEM_TYPE       Get_Type()    const { return m_eType; }

protected:
    TCHAR       m_szName[64] = {};
    TCHAR       m_szIconKey[64] = {};
    ITEM_TYPE   m_eType = ITEM_END;
};

