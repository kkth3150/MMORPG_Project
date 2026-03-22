#include "pch.h"
#include "Level_Test.h"
#include "Img_Manager.h"
#include "Object_Manager.h"
#include "Player.h"
#include "Map_Manager.h"
#include "Camera.h"

CLevel_Test::CLevel_Test()
{
}

CLevel_Test::~CLevel_Test()
{
	Release();
}

void CLevel_Test::Initialize()
{
	CImg_Manager::Get_Instance()->Insert_Bmp(L"../Resource/background/TestMap.bmp", L"BACKGROUND");
	CMap_Manager::Get_Instance()->Create_Map(30, 30);

	Ready_Player();
	

}

int CLevel_Test::Update(float dt)
{
	CCamera::Get_Instance()->Update(dt);
	CObject_Manager::Get_Instance()->Update(dt);
	return OBJ_NOEVENT;
}

void CLevel_Test::Late_Update(float dt)
{
	CObject_Manager::Get_Instance()->Late_Update(dt);
}

void CLevel_Test::Render(HDC hDC)
{
	HDC	hMemDC = CImg_Manager::Get_Instance()->Find_Bmp(L"BACKGROUND");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
	CMap_Manager::Get_Instance()->Render(hDC);
	CObject_Manager::Get_Instance()->Render(hDC);

}

void CLevel_Test::Release(void)
{
}

void CLevel_Test::Ready_Player()
{
#pragma region Sprites :

	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/idle/idle_B.png", L"PLAYER_IDLE_B");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/idle/idle_LB.png", L"PLAYER_IDLE_LB");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/idle/idle_L.png", L"PLAYER_IDLE_L");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/idle/idle_LT.png", L"PLAYER_IDLE_LT");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/idle/idle_T.png", L"PLAYER_IDLE_T");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/idle/idle_RT.png", L"PLAYER_IDLE_RT");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/idle/idle_R.png", L"PLAYER_IDLE_R");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/idle/idle_RB.png", L"PLAYER_IDLE_RB");

#pragma endregion PLAYER IDLE

#pragma region Sprites :

	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_B.png", L"PLAYER_WALK_B");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_LB.png", L"PLAYER_WALK_LB");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_L.png", L"PLAYER_WALK_L");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_LT.png", L"PLAYER_WALK_LT");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_T.png", L"PLAYER_WALK_T");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_RT.png", L"PLAYER_WALK_RT");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_R.png", L"PLAYER_WALK_R");
	CImg_Manager::Get_Instance()->Insert_Png(L"../Resource/Character/Player/walk/walk_RB.png", L"PLAYER_WALK_RB");

#pragma endregion PLAYER WALK





	CPlayer* pPlayer = new CPlayer;
	pPlayer->Set_WorldPos(15.f, 15.f); // 30x30 ¸Ê Áß¾Ó
	pPlayer->Initialize();

	CObject_Manager::Get_Instance()->Add_Object(OBJ_PLAYER, pPlayer);

	// Ä«¸Ş¶ó Å¸°Ù ¼³Á¤
	CCamera::Get_Instance()->Set_Target(pPlayer);
}
