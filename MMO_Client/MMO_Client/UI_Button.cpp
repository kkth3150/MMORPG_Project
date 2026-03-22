#include "pch.h"
#include "UI_Button.h"
#include "Img_Manager.h"
#include "Input_Manager.h"

CUI_Button::CUI_Button() : m_iDrawID(0), m_bEffect(false)
{
}

CUI_Button::~CUI_Button()
{
}

void CUI_Button::Initialize()
{
	switch (m_eButtonID) {
	case BUTTON_LOGIN:
		m_pFrameKey = L"LOGIN_BUTTON";
		m_bEffect = true;
		m_tInfo.fCX = 768.f;
		m_tInfo.fCY = 512.f;
		m_tInfo.fX = 400.f;
		m_tInfo.fY = 400.f;

		break;

	case BUTTON_EXIT:
		break;

	default:
		break;
	}
}

int CUI_Button::Update(float dt)
{

	m_bClick = false;
		 
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		m_bMouseOn = true;
		m_iDrawID = 1;

		if (CInput_Manager::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			 m_bClick= true;
			 return UI_EVENT;
		}
	}
	else {

		m_bMouseOn = false;
		m_iDrawID = 0;
	}


	__super::Update_Rect();
	return UI_NOEVENT;
}

void CUI_Button::Late_Update(float dt)
{
}

void CUI_Button::Render(HDC hDC)
{
	HDC	hMemDC = CImg_Manager::Get_Instance()->Find_Bmp(m_pFrameKey);

	GdiTransparentBlt(
		hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		m_iDrawID * (int)m_tInfo.fCX,
		0,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 255, 255));
}

void CUI_Button::Release(void)
{
	m_bClick = false;
}

void CUI_Button::Process_Event()
{
	
}
