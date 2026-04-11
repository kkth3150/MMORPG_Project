#pragma once
#include "define.h"

class CItemData;

struct DRAG_INFO
{
    bool        bDragging = false;
    CItemData*  pItem = nullptr;  // 드래그 중인 아이템 (참조만, 소유X)
    int         iFromSlot = -1;       // 인벤 슬롯 번호
    POINT       tStartPos = {};
};

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
    bool        Mouse_Down_Snap(MOUSE_BUTTON _eButton);


    void        Update();
    
    void        Set_InputMode(INPUT_MODE eMode) { m_eInputMode = eMode; }
    void        Set_CursorMode(CURSOR_MODE eMode) { m_eCursorMode = eMode; }
    
    bool        Is_GameMode() const { return m_eInputMode == INPUT_MODE_GAME; }

    INPUT_MODE  Get_InputMode() const { return m_eInputMode; }
    POINT       Get_MousePos() { return m_tMousePos; }
    CURSOR_MODE Get_CursorMode() { return m_eCursorMode; }
    void        Render_Cursor(ID2D1RenderTarget* pRT);

public:
    void        Start_Drag(CItemData* pItem, int iSlot)
    {
        m_tDrag.bDragging = true;
        m_tDrag.pItem = pItem;
        m_tDrag.iFromSlot = iSlot;
        m_tDrag.tStartPos = m_tMousePos;
    }
    void        End_Drag() { m_tDrag = {}; }
    DRAG_INFO& Get_Drag() { return m_tDrag; }
    bool        Is_Dragging() { return m_tDrag.bDragging; }

private:
    static CInput_Manager* m_pInstance;

    static const int CURSOR_SPRITE_W = 67;
    static const int CURSOR_SPRITE_H = 85;

    bool            m_bKeyState[VK_MAX];
    bool            m_bMouseState[MBUTTON_END]; // 0: L, 1: R, 2: M
    bool m_bMouseDown[MBUTTON_END] = {};
    bool m_bMouseUp[MBUTTON_END] = {};   // 추가

    POINT           m_tMousePos = {};  // 추가
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

    DRAG_INFO   m_tDrag;

    INPUT_MODE  m_eInputMode = INPUT_MODE_GAME;
    
};