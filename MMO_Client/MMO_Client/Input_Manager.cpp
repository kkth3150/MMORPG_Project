#include "pch.h"
#include "Input_Manager.h"

CInput_Manager* CInput_Manager::m_pInstance = nullptr;

CInput_Manager::CInput_Manager()
{
    ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
    ZeroMemory(m_bMouseState, sizeof(m_bMouseState));
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