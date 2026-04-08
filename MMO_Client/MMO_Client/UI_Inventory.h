#pragma once
#include "UI.h"
#include "Item_Define.h"

class CInventory;
class CEquipment;
class CPlayer;
class CItemData;
class CItemData_Equipment;

class CUI_Inventory : public CUI
{
public:
    CUI_Inventory() = default;
    virtual ~CUI_Inventory() = default;

public:
    virtual void    Initialize()                    override;
    virtual int     Update(float dt)                override;
    virtual void    Late_Update(float dt)           override;
    virtual void    Render(ID2D1RenderTarget* pRT)  override;
    virtual void    Release()                       override;
    virtual void    Process_Event()                 override {}

public:
    void    Set_References(CPlayer* pPlayer, CInventory* pInven, CEquipment* pEquip)
    {
        m_pPlayer = pPlayer;
        m_pInven = pInven;
        m_pEquip = pEquip;
    }

private:
    // 렌더
    void    Render_Background(ID2D1RenderTarget* pRT);
    void    Render_InvenSlots(ID2D1RenderTarget* pRT);
    void    Render_EquipSlots(ID2D1RenderTarget* pRT);
    void    Render_Gold(ID2D1RenderTarget* pRT);
    void    Render_Item(ID2D1RenderTarget* pRT, CItemData* pItem,
        float fX, float fY, float fW, float fH);

    // 입력
    void    On_InvenRClick(int iSlot);
    void    On_EquipRClick(EQUIP_SLOT eSlot);

    // 슬롯 hit test
    int         Get_InvenSlotAt(POINT tMouse);
    EQUIP_SLOT  Get_EquipSlotAt(POINT tMouse);

private:
    CPlayer* m_pPlayer = nullptr;
    CInventory* m_pInven = nullptr;
    CEquipment* m_pEquip = nullptr;

    bool        m_bVisible = false;

    // 레이아웃 상수
    static constexpr float PANEL_W = 200.f;
    static constexpr float PANEL_H = 420.f;
    static constexpr float INVEN_OFF_X = 10.f;
    static constexpr float INVEN_OFF_Y = 190.f;  // 장비창 아래
    static constexpr float EQUIP_OFF_X = 10.f;
    static constexpr float EQUIP_OFF_Y = 10.f;
    static constexpr int   INVEN_COLS = 10;
    static constexpr float SLOT_GAP = 2.f;
};