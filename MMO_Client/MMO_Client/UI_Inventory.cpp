#include "pch.h"
#include "UI_Inventory.h"
#include "Inventory.h"
#include "Equipment.h"
#include "Player.h"
#include "ItemData.h"
#include "ItemData_UseItem.h"
#include "ItemData_Equipment.h"
#include "Img_Manager.h"
#include "Input_Manager.h"

// 장비 슬롯 레이아웃 (패널 내 상대좌표+  렌더 사이즈)
static const EQUIP_SLOT_INFO s_SlotLayout[SLOT_END] =
{
        {  14.f,  84.f, 28.f, 84.f },   // SLOT_WEAPON  - 무기    28x84
    {  70.f, 28.f, 56.f, 84.f },    // SLOT_ARMOR   - 방어구  56x84
    {  10.f, 10.f,56.f,84.f },      //SLOT_SHIELD   - 쉴드    56x84
    {2.f,2.f,56.f,56.f},            //SLOT_HELMAT   - 헬멧    56X56
    {2.f,2.f,INVEN_SLOT_SIZE,INVEN_SLOT_SIZE},            //SLOT_PENDANT - INVEN_SLOT_SIZE
    {2.f,2.f,INVEN_SLOT_SIZE,INVEN_SLOT_SIZE}             //SLOT_RING   - INVEN_SLOT_SIZE
};

void CUI_Inventory::Initialize()
{
    CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/UI/Inventory.png", L"INVENTORY_FRAME");

    Set_Pos(700.f, 310.f);   // 패널 중심
    Set_Size(PANEL_W, PANEL_H);
    Update_Rect();
}

int CUI_Inventory::Update(float dt)
{
    if (CInput_Manager::Get_Instance()->Key_Down('I'))
        m_bVisible = !m_bVisible;

    if (!m_bVisible) return OBJ_NOEVENT;

    POINT tMouse = CInput_Manager::Get_Instance()->Get_MousePos();

    if (CInput_Manager::Get_Instance()->Key_Down(VK_RBUTTON))
    {
        int iSlot = Get_InvenSlotAt(tMouse);
        if (iSlot != -1)
        {
            On_InvenRClick(iSlot);
            return OBJ_NOEVENT;
        }

        EQUIP_SLOT eSlot = Get_EquipSlotAt(tMouse);
        if (eSlot != SLOT_END)
        {
            On_EquipRClick(eSlot);
            return OBJ_NOEVENT;
        }
    }

    return OBJ_NOEVENT;
}

void CUI_Inventory::Late_Update(float dt) {}

void CUI_Inventory::Render(ID2D1RenderTarget* pRT)
{
    if (!m_bVisible) return;

    Render_Background(pRT);
    Render_EquipSlots(pRT);
    Render_InvenSlots(pRT);
    Render_Gold(pRT);
}

void CUI_Inventory::Release() {}

// ===================== 렌더 =====================

void CUI_Inventory::Render_Background(ID2D1RenderTarget* pRT)
{
    float fL = m_tRect.left;
    float fT = m_tRect.top;

}

void CUI_Inventory::Render_EquipSlots(ID2D1RenderTarget* pRT)
{
    float fPanelL = m_tRect.left;
    float fPanelT = m_tRect.top;

    ID2D1SolidColorBrush* pBrush = nullptr;

    for (int i = 0; i < SLOT_END; ++i)
    {
        const EQUIP_SLOT_INFO& info = s_SlotLayout[i];

        float fX = fPanelL + EQUIP_OFF_X + info.fX;
        float fY = fPanelT + EQUIP_OFF_Y + info.fY;

        // 장착 아이템 -장비창 사이즈 그대로
        CItemData_Equipment* pEquip = m_pEquip->Get_Equipped((EQUIP_SLOT)i);
        if (pEquip)
            Render_Item(pRT, pEquip, fX, fY, info.fW, info.fH);
    }
}

void CUI_Inventory::Render_InvenSlots(ID2D1RenderTarget* pRT)
{
    float fPanelL = m_tRect.left;
    float fPanelT = m_tRect.top;

    ID2D1SolidColorBrush* pBrush = nullptr;

    for (int i = 0; i < INVEN_SIZE; ++i)
    {
        int iCol = i % INVEN_COLS;
        int iRow = i / INVEN_COLS;

        float fX = fPanelL + INVEN_OFF_X + iCol * (INVEN_SLOT_SIZE + SLOT_GAP);
        float fY = fPanelT + INVEN_OFF_Y + iRow * (INVEN_SLOT_SIZE + SLOT_GAP);


        // 아이템-항상 인벤 슬롯 사이즈(28x28)로 압축
        CItemData* pItem = m_pInven->Get_Item(i);
        if (!pItem) continue;

        Render_Item(pRT, pItem, fX, fY, INVEN_SLOT_SIZE, INVEN_SLOT_SIZE);

        // 스택 수량
        if (m_pInven->Get_StackCount(i) > 1)
        {
            TCHAR szCount[8];
            swprintf_s(szCount, 8, L"%d", m_pInven->Get_StackCount(i));

            pRT->CreateSolidColorBrush(D2D1::ColorF(1.f, 1.f, 1.f), &pBrush);
            pRT->DrawText(szCount, lstrlen(szCount),
                CImg_Manager::Get_Instance()->Get_DebugFont(),
                D2D1::RectF(fX, fY + 16.f,
                    fX + INVEN_SLOT_SIZE, fY + INVEN_SLOT_SIZE),
                pBrush);
            pBrush->Release();
        }
    }
}

void CUI_Inventory::Render_Gold(ID2D1RenderTarget* pRT)
{
    TCHAR szGold[32];
    swprintf_s(szGold, 32, L"Gold: %d", m_pInven->Get_Gold());

    ID2D1SolidColorBrush* pBrush = nullptr;
    pRT->CreateSolidColorBrush(D2D1::ColorF(1.f, 0.85f, 0.f), &pBrush);

    float fL = m_tRect.left;
    float fB = m_tRect.top + PANEL_H;

    pRT->DrawText(szGold, lstrlen(szGold),
        CImg_Manager::Get_Instance()->Get_DebugFont(),
        D2D1::RectF(fL + 10.f, fB - 25.f, fL + PANEL_W, fB - 5.f),
        pBrush);
    pBrush->Release();
}

void CUI_Inventory::Render_Item(ID2D1RenderTarget* pRT, CItemData* pItem,
    float fX, float fY, float fW, float fH)
{
    ID2D1Bitmap* pBitmap =
        CImg_Manager::Get_Instance()->Find_Png(pItem->Get_IconKey());
    if (!pBitmap) return;

    pRT->DrawBitmap(pBitmap,
        D2D1::RectF(fX, fY, fX + fW, fY + fH),
        1.0f,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}

// ===================== 입력 처리 =====================

void CUI_Inventory::On_InvenRClick(int iSlot)
{
    CItemData* pItem = m_pInven->Get_Item(iSlot);
    if (!pItem) return;

    if (pItem->Get_Type() == ITEM_USE)
        m_pPlayer->Use_Item(iSlot);
    else if (pItem->Get_Type() == ITEM_EQUIPMENT)
        m_pPlayer->Equip_Item(iSlot);
}

void CUI_Inventory::On_EquipRClick(EQUIP_SLOT eSlot)
{
    // 인벤토리에 빈 슬롯 있을 때만 해제
    CItemData_Equipment* pEquip = m_pEquip->Get_Equipped(eSlot);
    if (!pEquip) return;

    m_pPlayer->UnEquip_Item(eSlot);
}

// ===================== Hit Test =====================

int CUI_Inventory::Get_InvenSlotAt(POINT tMouse)
{
    float fPanelL = m_tRect.left;
    float fPanelT = m_tRect.top;

    for (int i = 0; i < INVEN_SIZE; ++i)
    {
        int iCol = i % INVEN_COLS;
        int iRow = i / INVEN_COLS;

        float fX = fPanelL + INVEN_OFF_X + iCol * (INVEN_SLOT_SIZE + SLOT_GAP);
        float fY = fPanelT + INVEN_OFF_Y + iRow * (INVEN_SLOT_SIZE + SLOT_GAP);

        RECT tSlot = {
            (LONG)fX, (LONG)fY,
            (LONG)(fX + INVEN_SLOT_SIZE), (LONG)(fY + INVEN_SLOT_SIZE)
        };

        if (PtInRect(&tSlot, tMouse))
            return i;
    }
    return -1;
}

EQUIP_SLOT CUI_Inventory::Get_EquipSlotAt(POINT tMouse)
{
    float fPanelL = m_tRect.left;
    float fPanelT = m_tRect.top;

    for (int i = 0; i < SLOT_END; ++i)
    {
        const EQUIP_SLOT_INFO& info = s_SlotLayout[i];

        float fX = fPanelL + EQUIP_OFF_X + info.fX;
        float fY = fPanelT + EQUIP_OFF_Y + info.fY;

        RECT tSlot = {
            (LONG)fX, (LONG)fY,
            (LONG)(fX + info.fW), (LONG)(fY + info.fH)
        };

        if (PtInRect(&tSlot, tMouse))
            return (EQUIP_SLOT)i;
    }
    return SLOT_END;
}