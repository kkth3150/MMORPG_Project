#pragma once
#include "define.h"

class CInput_Manager
{
private:
    CInput_Manager();
    ~CInput_Manager();

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

public:
    bool        Key_Pressing(int _iKey);
    bool        Key_Down(int _iKey);
    bool        Key_Up(int _iKey);

    bool        Mouse_Pressing(MOUSE_BUTTON _eButton);
    bool        Mouse_Down(MOUSE_BUTTON _eButton);
    bool        Mouse_Up(MOUSE_BUTTON _eButton);


    void        Update();
    
    void        Set_InputMode(INPUT_MODE eMode) { m_eInputMode = eMode; }
    void        Set_CursorMode(CURSOR_MODE eMode) { m_eCursorMode = eMode; }
    
    bool        Is_GameMode() const { return m_eInputMode == INPUT_MODE_GAME; }

    INPUT_MODE  Get_InputMode() const { return m_eInputMode; }
    POINT       Get_MousePos() { return m_tMousePos; }
    CURSOR_MODE Get_CursorMode() { return m_eCursorMode; }
    void        Render_Cursor(ID2D1RenderTarget* pRT);
    bool        Mouse_Down_Snap(MOUSE_BUTTON _eButton) { return m_bMouseDown[_eButton]; }


private:
    static CInput_Manager* m_pInstance;

    static const int CURSOR_SPRITE_W = 67;
    static const int CURSOR_SPRITE_H = 85;

    bool            m_bKeyState[VK_MAX];
    bool            m_bMouseState[MBUTTON_END]; // 0: L, 1: R, 2: M
    bool            m_bMouseDown[MBUTTON_END] = {};


    POINT           m_tMousePos = {};  // Ãß°¡
    CURSOR_MODE     m_eCursorMode = CURSOR_NORMAL;

    struct HOTSPOT { int iX; int iY; };
    HOTSPOT m_tHotSpot[CURSOR_END] =
    {
        { 28, 42 },
        {  28, 42 }, 
        { 28, 42 },  
        { 28, 42 },
        {  28, 42 },
        { 28, 42 },
        { 28, 42 }
    };

    INPUT_MODE  m_eInputMode = INPUT_MODE_GAME;
};