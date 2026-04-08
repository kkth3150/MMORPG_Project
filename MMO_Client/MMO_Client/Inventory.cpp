#include "pch.h"
#include "Inventory.h"
#include "ItemData.h"

CInventory::~CInventory()
{
    for (int i = 0; i < INVEN_SIZE; ++i)
    {
        if (m_aSlot[i])
        {
            delete m_aSlot[i];
            m_aSlot[i] = nullptr;
        }
    }
}

bool CInventory::Add_Item(CItemData* pItem)
{
    if (pItem->Get_Type() == ITEM_EQUIPMENT)
    {
        int iSlot = Find_EmptySlot();
        if (iSlot == INVEN_FULL) return false;

        m_aSlot[iSlot] = pItem;
        m_iStackCount[iSlot] = 1;
        return true;
    }

    int iSameSlot = Find_SameItem(pItem);
    if (iSameSlot != ITEM_NOEXIST)
    {
        ++m_iStackCount[iSameSlot];
        delete pItem;
        return true;
    }

    int iEmptySlot = Find_EmptySlot();
    if (iEmptySlot == INVEN_FULL) return false;

    m_aSlot[iEmptySlot] = pItem;
    m_iStackCount[iEmptySlot] = 1;
    return true;
}

CItemData* CInventory::Remove_Item(int iSlot)
{
    if (iSlot < 0 || iSlot >= INVEN_SIZE) return nullptr;
    if (!m_aSlot[iSlot]) return nullptr;

    if (m_iStackCount[iSlot] > 1)
    {
        --m_iStackCount[iSlot];
        return nullptr;
    }

    CItemData* pItem = m_aSlot[iSlot];
    m_aSlot[iSlot] = nullptr;
    m_iStackCount[iSlot] = 0;
    return pItem;
}

CItemData* CInventory::Get_Item(int iSlot) const
{
    if (iSlot < 0 || iSlot >= INVEN_SIZE) return nullptr;
    return m_aSlot[iSlot];
}

bool CInventory::Is_Empty(int iSlot) const
{
    return m_aSlot[iSlot] == nullptr;
}

int CInventory::Find_SameItem(CItemData* pItem)
{
    for (int i = 0; i < INVEN_SIZE; ++i)
    {
        if (!m_aSlot[i]) continue;
        if (m_aSlot[i]->Get_Type() == pItem->Get_Type() &&
            lstrcmp(m_aSlot[i]->Get_Name(), pItem->Get_Name()) == 0)
            return i;
    }
    return ITEM_NOEXIST;
}

int CInventory::Find_EmptySlot()
{
    for (int i = 0; i < INVEN_SIZE; ++i)
        if (!m_aSlot[i]) return i;
    return INVEN_FULL;
}

void CInventory::Add_Gold(int iAmount)
{
    m_iGold += iAmount;
}

bool CInventory::Spend_Gold(int iAmount)
{
    if (m_iGold < iAmount) return false;
    m_iGold -= iAmount;
    return true;
}