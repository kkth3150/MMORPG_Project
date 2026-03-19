#pragma once
#include "define.h"

class CInput_Manager
{
private:
    CInput_Manager();
    ~CInput_Manager();

public:
    bool        Key_Pressing(int _iKey);
    bool        Key_Down(int _iKey);
    bool        Key_Up(int _iKey);

    bool        Mouse_Pressing(MOUSE_BUTTON _eButton);
    bool        Mouse_Down(MOUSE_BUTTON _eButton);
    bool        Mouse_Up(MOUSE_BUTTON _eButton);

public:
    static CInput_Manager* Get_Instance()
    {
        if (!m_pInstance)
        {
            m_pInstance = new CInput_Manager;
        }
        return m_pInstance;
    }

    static void Destroy_Instance()
    {
        if (m_pInstance)
        {
            delete m_pInstance;
            m_pInstance = nullptr;
        }
    }

private:
    static CInput_Manager* m_pInstance;

    bool m_bKeyState[VK_MAX];
    bool m_bMouseState[MBUTTON_END]; // 0: L, 1: R, 2: M
};