#include "pch.h"
#include "Level_Login.h"
#include "UI_Manager.h"
#include "Img_Manager.h"
#include "UI_LoginBox.h"

CLevel_Login::CLevel_Login()
{
}

CLevel_Login::~CLevel_Login()
{
	Release();
}

void CLevel_Login::Initialize()
{

	CImg_Manager::Get_Instance()->Insert_Png(
		L"../Resource/background/Login.png", L"LOGIN_BACK");

	CUI_LoginBox* pBox = new CUI_LoginBox;
	pBox->Initialize();
	CUI_Manager::Get_Instance()->Add_UI(UI_BOX, pBox);
}

int CLevel_Login::Update(float dt)
{
	CUI_Manager::Get_Instance()->Update(dt);
	return 0;
}

void CLevel_Login::Late_Update(float dt)
{
}

void CLevel_Login::Render(ID2D1RenderTarget* pRT)
{
	ID2D1Bitmap* pBG = CImg_Manager::Get_Instance()->Find_Png(L"LOGIN_BACK");
	if (pBG)
	{
		pRT->DrawBitmap(
			pBG,
			D2D1::RectF(0.f, 0.f, WINCX, WINCY)
		);
	}
	CUI_Manager::Get_Instance()->Render(pRT);


}

void CLevel_Login::Release(void)
{
	CUI_Manager::Get_Instance()->DeleteID(UI_BOX);
}
