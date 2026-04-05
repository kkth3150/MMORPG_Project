#include "pch.h"
#include "Level_Test.h"
#include "Img_Manager.h"
#include "Object_Manager.h"
#include "Player.h"
#include "NPC_Shop.h"
#include "Map_Manager.h"
#include "Camera.h"
#include "Input_Manager.h"

CLevel_Test::CLevel_Test()
{
}

CLevel_Test::~CLevel_Test()
{
	Release();
}

void CLevel_Test::Initialize()
{
	ShowCursor(FALSE);

	CImg_Manager::Get_Instance()->Insert_Png(
		L"../Resource/background/Background1.png", L"BACKGROUND");

	CMap_Manager::Get_Instance()->Initialize();
	Ready_Player();

}

int CLevel_Test::Update(float dt)
{
	CMap_Manager::Get_Instance()->Update();
	CCamera::Get_Instance()->Update(dt);
	CObject_Manager::Get_Instance()->Update(dt);
	return OBJ_NOEVENT;
}

void CLevel_Test::Late_Update(float dt)
{
	CObject_Manager::Get_Instance()->Late_Update(dt);
}

void CLevel_Test::Render(ID2D1RenderTarget* pRT)
{



	if (CMap_Manager::Get_Instance()->Is_Loading())
	{
		Render_Loading(pRT);
		return;
	}

	ID2D1Bitmap* pBG = CImg_Manager::Get_Instance()->Find_Png(L"BACKGROUND");
	if (pBG)
	{
		pRT->DrawBitmap(
			pBG,
			D2D1::RectF(0.f, 0.f, WINCX, WINCY)
		);
	}

	CMap_Manager::Get_Instance()->Render(pRT);
	CObject_Manager::Get_Instance()->Render(pRT);
	CInput_Manager::Get_Instance()->Render_Cursor(pRT);

}

void CLevel_Test::Render_Loading(ID2D1RenderTarget* pRT)
{
	pRT->Clear(D2D1::ColorF(0.f, 0.f, 0.f));

	float fProgress = CMap_Manager::Get_Instance()->Get_LoadProgress();
	float barW = 400.f, barH = 20.f;
	float barX = (WINCX - barW) / 2.f;
	float barY = WINCY / 2.f;

	ID2D1SolidColorBrush* pBrush = nullptr;

	// ¹è°æ ¹Ù
	pRT->CreateSolidColorBrush(D2D1::ColorF(0.3f, 0.3f, 0.3f), &pBrush);
	pRT->FillRectangle(D2D1::RectF(barX, barY, barX + barW, barY + barH), pBrush);
	pBrush->Release();

	// ÁøÇà ¹Ù
	pRT->CreateSolidColorBrush(D2D1::ColorF(0.2f, 0.8f, 0.2f), &pBrush);
	pRT->FillRectangle(D2D1::RectF(barX, barY, barX + barW * fProgress, barY + barH), pBrush);
	pBrush->Release();
}

void CLevel_Test::Release(void)
{
}

void CLevel_Test::Ready_Player()
{

	CPlayer* pPlayer = new CPlayer;
	pPlayer->Set_WorldPos(15.f, 15.f); // 30x30 ¸Ê Áß¾Ó
	pPlayer->Initialize();

	CObject_Manager::Get_Instance()->Add_Object(OBJ_PLAYER, pPlayer);

	// Ä«¸Ş¶ó Å¸°Ù ¼³Á¤
	CCamera::Get_Instance()->Set_Target(pPlayer);
}


