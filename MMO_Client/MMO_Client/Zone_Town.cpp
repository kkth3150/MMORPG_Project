#include "pch.h"
#include "Zone_Town.h"
#include "Img_Manager.h"

void CZone_Town::Build()
{
    // 마을은 같은 타일셋이지만 키를 분리해서 관리
    // 추후 마을 전용 타일로 교체 시 이미지 경로만 바꾸면 됨
    CImg_Manager* pImg = CImg_Manager::Get_Instance();
    pImg->Insert_Png(L"../Resource/Tile/Tile_Grass.png", L"TOWN_GRASS");
    pImg->Insert_Png(L"../Resource/Tile/Tile_Block.png", L"TOWN_BLOCK");
    pImg->Insert_Png(L"../Resource/Tile/Tile_Border_LT.png", L"TOWN_BORDER_LT");
    pImg->Insert_Png(L"../Resource/Tile/Tile_Border_RT.png", L"TOWN_BORDER_RT");
    pImg->Insert_Png(L"../Resource/Tile/Tile_Border_RB.png", L"TOWN_BORDER_RB");
    pImg->Insert_Png(L"../Resource/Tile/Tile_Border_LB.png", L"TOWN_BORDER_LB");
    pImg->Insert_Png(L"../Resource/Tile/Tile_Border_T.png", L"TOWN_BORDER_T");
    pImg->Insert_Png(L"../Resource/Tile/Tile_Border_R.png", L"TOWN_BORDER_R");
    pImg->Insert_Png(L"../Resource/Tile/Tile_Border_B.png", L"TOWN_BORDER_B");
    pImg->Insert_Png(L"../Resource/Tile/Tile_Border_L.png", L"TOWN_BORDER_L");
    pImg->Insert_Png(L"../Resource/Tile/Tile_Outside.png", L"TOWN_OUTSIDE");

    static const int BLOCK_MAP[30][20] =
    {
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,1,1,0,0,0,0,0,0, 0,0,0,0,0,0,1,1,0,0 },
        { 0,0,1,1,0,0,0,0,0,0, 0,0,0,0,0,0,1,1,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,1,1,0,0,0,0,0,0, 0,0,0,0,0,0,1,1,0,0 },
        { 0,0,1,1,0,0,0,0,0,0, 0,0,0,0,0,0,1,1,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,1,1,0,0,0,0,0,0, 0,0,0,0,0,0,1,1,0,0 },
        { 0,0,1,1,0,0,0,0,0,0, 0,0,0,0,0,0,1,1,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,1,1,0,0,0,0,0,0, 0,0,0,0,0,0,1,1,0,0 },
        { 0,0,1,1,0,0,0,0,0,0, 0,0,0,0,0,0,1,1,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
    };

    Build_TileGrid(20, 30, &BLOCK_MAP[0][0]);

    Apply_ImgKeys({
        { TILE_GRASS,     L"TOWN_GRASS"     },
        { TILE_BLOCK,     L"TOWN_BLOCK"     },
        { TILE_BORDER_LT, L"TOWN_BORDER_LT" },
        { TILE_BORDER_RT, L"TOWN_BORDER_RT" },
        { TILE_BORDER_RB, L"TOWN_BORDER_RB" },
        { TILE_BORDER_LB, L"TOWN_BORDER_LB" },
        { TILE_BORDER_T,  L"TOWN_BORDER_T"  },
        { TILE_BORDER_R,  L"TOWN_BORDER_R"  },
        { TILE_BORDER_B,  L"TOWN_BORDER_B"  },
        { TILE_BORDER_L,  L"TOWN_BORDER_L"  },
        { TILE_OUTSIDE,   L"TOWN_OUTSIDE"   },
        });

}

void CZone_Town::Spawn_Objects()
{
}

void CZone_Town::Clear_Objects()
{
}
