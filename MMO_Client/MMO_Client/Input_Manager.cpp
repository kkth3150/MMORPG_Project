#include "pch.h"
#include "Input_Manager.h"
#include "Img_Manager.h"

CInput_Manager* CInput_Manager::m_pInstance = nullptr;

CInput_Manager::CInput_Manager()
{
    ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
    ZeroMemory(m_bMouseState, sizeof(m_bMouseState));
    CImg_Manager::Get_Instance()->Insert_Png(
        L"../Resource/Cursor/Cursor.png", L"CURSOR");
    
}

CInput_Manager::~CInput_Manager()
{
}

bool CInput_Manager::Key_Pressing(int _iKey)
{
    return (GetAsyncKeyState(_iKey) & 0x8000) != 0;
}

bool CInput_Manager::Key_Down(int _iKey)
{
    if (GetAsyncKeyState(_iKey) & 0x8000)
    {
        if (!m_bKeyState[_iKey])
        {
            m_bKeyState[_iKey] = true;
            return true;
        }
    }
    else
    {
        m_bKeyState[_iKey] = false;
    }

    return false;
}

bool CInput_Manager::Key_Up(int _iKey)
{
    if (!(GetAsyncKeyState(_iKey) & 0x8000))
    {
        if (m_bKeyState[_iKey])
        {
            m_bKeyState[_iKey] = false;
            return true;
        }
    }
    else
    {
        m_bKeyState[_iKey] = true;
    }

    return false;
}
bool CInput_Manager::Mouse_Pressing(MOUSE_BUTTON _eButton)
{
    int vk = (_eButton == MBUTTON_L) ? VK_LBUTTON :
        (_eButton == MBUTTON_R) ? VK_RBUTTON : VK_MBUTTON;

    return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

bool CInput_Manager::Mouse_Down(MOUSE_BUTTON _eButton)
{
    int vk = (_eButton == MBUTTON_L) ? VK_LBUTTON :
        (_eButton == MBUTTON_R) ? VK_RBUTTON : VK_MBUTTON;

    if (GetAsyncKeyState(vk) & 0x8000)
    {
        if (!m_bMouseState[_eButton])
        {
            m_bMouseState[_eButton] = true;
            return true;
        }
    }
    else
    {
        m_bMouseState[_eButton] = false;
    }

    return false;
}

bool CInput_Manager::Mouse_Up(MOUSE_BUTTON _eButton)
{
    int vk = (_eButton ==MBUTTON_L) ? VK_LBUTTON :
        (_eButton == MBUTTON_R) ? VK_RBUTTON : VK_MBUTTON;

    if (!(GetAsyncKeyState(vk) & 0x8000))
    {
        if (m_bMouseState[_eButton])
        {
            m_bMouseState[_eButton] = false;
            return true;
        }
    }
    else
    {
        m_bMouseState[_eButton] = true;
    }

    return false;
}



void CInput_Manager::Update()
{
    // 매 프레임 마우스 커서 위치 갱신
    GetCursorPos(&m_tMousePos);
    ScreenToClient(g_hWnd, &m_tMousePos); // 윈도우 클라이언트 기준으로 변환
}

void CInput_Manager::Render_Cursor(ID2D1RenderTarget* pRT)
{

    ID2D1Bitmap* pBitmap = CImg_Manager::Get_Instance()->Find_Png(L"CURSOR");
    if (!pBitmap) return;

    // 현재 커서 모드에 따라 스프라이트 X위치 결정
    int iSpriteIdx = (int)m_eCursorMode;

    // 핫스팟 보정
    int iHotX = m_tHotSpot[m_eCursorMode].iX;
    int iHotY = m_tHotSpot[m_eCursorMode].iY;

    float fRenderX = (float)(m_tMousePos.x - iHotX);
    float fRenderY = (float)(m_tMousePos.y - iHotY);

    // 목적지 Rect
    D2D1_RECT_F destRect = D2D1::RectF(
        fRenderX,
        fRenderY,
        fRenderX + CURSOR_SPRITE_W,
        fRenderY + CURSOR_SPRITE_H
    );

    // 소스 Rect - 스프라이트 시트에서 현재 모드 잘라내기
    float fSrcX = (float)(CURSOR_SPRITE_W * iSpriteIdx);
    D2D1_RECT_F srcRect = D2D1::RectF(
        fSrcX,
        0.f,
        fSrcX + CURSOR_SPRITE_W,
        (float)CURSOR_SPRITE_H
    );

    pRT->DrawBitmap(
        pBitmap,
        destRect,
        1.0f,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        srcRect
    );


}