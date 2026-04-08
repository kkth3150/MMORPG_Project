#pragma once
#include "ItemData.h"

class CPlayer;

class CItemData_UseItem : public CItemData
{
public:
    virtual ~CItemData_UseItem() = default;
    virtual void Use_Item(CPlayer* pPlayer) PURE;
};