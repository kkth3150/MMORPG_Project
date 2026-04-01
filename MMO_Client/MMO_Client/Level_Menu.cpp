#include "pch.h"
#include "Level_Menu.h"
#include "Img_Manager.h"
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

void CLevel_Menu::Render(ID2D1RenderTarget* pRT)
{


}

void CLevel_Menu::Release(void)
{

}
