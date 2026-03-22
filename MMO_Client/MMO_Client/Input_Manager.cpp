#include "pch.h"
#include "Input_Manager.h"
#include "Img_Manager.h"

CInput_Manager* CInput_Manager::m_pInstance = nullptr;

CInput_Manager::CInput_Manager()
{
    ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
    ZeroMemory(m_bMouseState, sizeof(m_bMouseState));
    CImg_Manager::Get_Instance()->Insert_Bmp(L"../Resource/Cursor/Cursor.bmp", L"CURSOR");
    
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

void CInput_Manager::Render_Cursor(HDC hDC)
{

    HDC BmpDC = CImg_Manager::Get_Instance()->Find_Bmp(L"CURSOR");
    if (!BmpDC) return;

    // 현재 커서 모드에 따라 스프라이트 X위치 결정
    // 스프라이트 순서: 0=NORMAL, 1=ATTACK, 2=TALK
    int iSpriteIdx = (int)m_eCursorMode;

    // 핫스팟 - 마우스 실제 클릭 기준점
    int iHotX = m_tHotSpot[m_eCursorMode].iX;
    int iHotY = m_tHotSpot[m_eCursorMode].iY;

    // 핫스팟 기준으로 렌더링 위치 보정
    int iRenderX = m_tMousePos.x - iHotX;
    int iRenderY = m_tMousePos.y - iHotY;

    GdiTransparentBlt(hDC,
        iRenderX,
        iRenderY,
        CURSOR_SPRITE_W,
        CURSOR_SPRITE_H,
        BmpDC,
        CURSOR_SPRITE_W * iSpriteIdx,  // 스프라이트 X 위치
        0,
        CURSOR_SPRITE_W,
        CURSOR_SPRITE_H,
        RGB(255, 255, 255));  // 배경색 (실제 배경색으로 변경)


}