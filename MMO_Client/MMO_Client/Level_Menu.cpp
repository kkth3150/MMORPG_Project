#include "pch.h"
#include "Level_Menu.h"
#include "Bmp_Manager.h"

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
}

int CLevel_Menu::Update(float dt)
{
	return 0;
}

void CLevel_Menu::Late_Update(float dt)
{
}

void CLevel_Menu::Render(HDC hDC)
{
	HDC	hMemDC = CBmp_Manager::Get_Instance()->Find_Img(L"BACKGROUND");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CLevel_Menu::Release(void)
{

}
