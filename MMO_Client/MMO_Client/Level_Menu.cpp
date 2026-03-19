#include "pch.h"
#include "Level_Menu.h"
#include "Bmp_Manager.h"
#include "UI_Manager.h"
#include "UI.h"
#include "UI_Button.h"


CLevel_Menu::CLevel_Menu()
{
}

CLevel_Menu::~CLevel_Menu()
{
	Release();
}

void CLevel_Menu::Initialize()
{
	CBmp_Manager::Get_Instance()->Insert_Bmp(L"../Resource/background/MainBackground(1280X720).bmp", L"BACKGROUND");
	CBmp_Manager::Get_Instance()->Insert_Bmp(L"../Resource/Button/Login(1536X512).bmp", L"LOGIN_BUTTON");
	CBmp_Manager::Get_Instance()->Insert_Bmp(L"../Resource/Button/Exit(1536X512).bmp", L"EXIT_BUTTON");

	CUI* LOGIN_Button = new CUI_Button;
	dynamic_cast<CUI_Button*>(LOGIN_Button)->SetButtonID(BUTTON_LOGIN);
	LOGIN_Button->Initialize();
	CUI_Manager::Get_Instance()->Add_UI(UI_BUTTON,LOGIN_Button);


}

int CLevel_Menu::Update(float dt)
{
	CUI_Manager::Get_Instance()->Update(dt);

	return OBJ_NOEVENT;
}

void CLevel_Menu::Late_Update(float dt)
{
	CUI_Manager::Get_Instance()->Late_Update(dt);
}

void CLevel_Menu::Render(HDC hDC)
{
	HDC	hMemDC = CBmp_Manager::Get_Instance()->Find_Img(L"BACKGROUND");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
	CUI_Manager::Get_Instance()->Render(hDC);

}

void CLevel_Menu::Release(void)
{

}
