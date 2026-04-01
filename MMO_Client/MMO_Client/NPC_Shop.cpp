#include "pch.h"
#include "NPC_Shop.h"
#include "Img_Manager.h"

CNPC_Shop::CNPC_Shop() {}
CNPC_Shop::~CNPC_Shop() { Release(); }

void CNPC_Shop::Initialize()
{
    __super::Initialize();

    Set_NpcName(L"상인");
    m_eType = NPC_SHOP;

    m_tIsoInfo.fCX = 160.f;
    m_tIsoInfo.fCY = 128.f;
    m_tIsoInfo.fHeight = 30.f;
    m_tIsoInfo.fWorldX = 10.f;
    m_tIsoInfo.fWorldZ = 10.f;

    // 플레이어 범위 체크용 콜리더
    Set_Collider(1.5f, 1.5f);

    // 마우스 클릭용 직교 박스
    Set_MouseCollider(40.f, 10.f, 80.f, 100.f);

    Set_Frame(5, 120);

    CImg_Manager::Get_Instance()->Insert_Png(
        L"../Resource/NPC/Shop/shop_idle.png", L"NPC_SHOP_IDLE");
}

int CNPC_Shop::Update(float dt)
{
    __super::Update(dt);  // 클릭 감지, 말풍선 타이머
    __super::Move_Frame();        // 애니메이션
    return OBJ_NOEVENT;
}

void CNPC_Shop::Late_Update(float dt)
{
    __super::Late_Update(dt);  // Update_Rect, Update_MouseRect
}

void CNPC_Shop::Render(ID2D1RenderTarget* pRT)
{
    ID2D1Bitmap* pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"NPC_SHOP_IDLE");
    Render_Sprite(pRT, pBitmap);  // 스프라이트
    __super::Render(pRT);         // 이름표, 인디케이터, 디버그
}

void CNPC_Shop::On_Click()
{
    // 말풍선
    m_bShowBubble = true;
    m_fBubbleTimer = 3.f;
    lstrcpy(m_szBubbleText, L"어서오세요!");

    // 추후: CUI_Manager::Get_Instance()->Open_Shop();
}

void CNPC_Shop::On_Interact()
{
    // 추후: 상점 UI 표시
}

void CNPC_Shop::Release() {}