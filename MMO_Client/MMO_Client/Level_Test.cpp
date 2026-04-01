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
		L"../Resource/background/TestMap.png", L"BACKGROUND");
	CMap_Manager::Get_Instance()->Create_Map(30, 30);

	Ready_Player();
	Ready_NPC();

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

void CLevel_Test::Render(ID2D1RenderTarget* pRT)
{

	//ID2D1Bitmap* pBg = CImg_Manager::Get_Instance()->Find_Png(L"BACKGROUND");
	//if (pBg)
	//{
	//	pRT->DrawBitmap(pBg, D2D1::RectF(0.f, 0.f, (float)WINCX, (float)WINCY));
	//}
	CMap_Manager::Get_Instance()->Render(pRT);
	CObject_Manager::Get_Instance()->Render(pRT);
	CInput_Manager::Get_Instance()->Render_Cursor(pRT);

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

	// Ä«¸Þ¶ó Å¸°Ù ¼³Á¤
	CCamera::Get_Instance()->Set_Target(pPlayer);
}

void CLevel_Test::Ready_NPC() {

	CImg_Manager::Get_Instance()->Insert_Png(
		L"../Resource/NPC/Traders/Trader0_Idle.png", L"TRADER0_IDLE");
	CImg_Manager::Get_Instance()->Insert_Png(
		L"../Resource/NPC/Traders/Trader0_Talk.png", L"TRADER0_TALK");
	CImg_Manager::Get_Instance()->Insert_Png(
		L"../Resource/NPC/Traders/Trader1_Idle.png", L"TRADER1_IDLE");
	CImg_Manager::Get_Instance()->Insert_Png(
		L"../Resource/NPC/Traders/Trader1_Talk.png", L"TRADER1_TALK");

	CNPC_Shop* pNpcShop0 = new CNPC_Shop;
	pNpcShop0->Set_ShopType(SHOP_TRADER_0);
	pNpcShop0->Set_WorldPos(15.f,15.f);
	pNpcShop0->Initialize();
	CObject_Manager::Get_Instance()->Add_Object(OBJ_NPC, pNpcShop0);


}