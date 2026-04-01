#include "pch.h"
#include "Map_Manager.h"
#include "Img_Manager.h"

CMap_Manager* CMap_Manager::m_pInstance = nullptr;

void CMap_Manager::Create_Map(int iTileCountX, int iTileCountZ)
{
    Release(); // 기존 맵 초기화

    m_iTileCountX = iTileCountX;
    m_iTileCountZ = iTileCountZ;

    // 타일 이미지 로드
    CImg_Manager::Get_Instance()->Insert_Png(
        L"../Resource/Tile/Tile_Normal.png", L"TILE_NORMAL");
    CImg_Manager::Get_Instance()->Insert_Png(
        L"../Resource/Tile/Tile_Block.png", L"TILE_BLOCK");

    // 로드 확인
    if (!CImg_Manager::Get_Instance()->Find_Png(L"TILE_NORMAL"))
        MessageBox(g_hWnd, L"타일 이미지 로드 실패", L"오류", MB_OK);

    m_vecTile.reserve(iTileCountX * iTileCountZ);

    for (int z = 0; z < iTileCountZ; ++z)
    {
        for (int x = 0; x < iTileCountX; ++x)
        {
            CTile* pTile = new CTile;
            pTile->Set_TileIndex(x, z);
            pTile->Set_TileType(TILE_NORMAL);
            m_vecTile.push_back(pTile);
        }
    }
}

bool CMap_Manager::Load_Map(const TCHAR* pFilePath)
{
    Release();

    // 파일 열기
    FILE* pFile = nullptr;
    _tfopen_s(&pFile, pFilePath, L"rb");
    if (!pFile) return false;

    // 맵 크기 읽기
    fread(&m_iTileCountX, sizeof(int), 1, pFile);
    fread(&m_iTileCountZ, sizeof(int), 1, pFile);

    // 타일 이미지 로드
    CImg_Manager::Get_Instance()->Insert_Png(
        L"../Resource/Tile/Tile_Normal.png", L"TILE_NORMAL");
    CImg_Manager::Get_Instance()->Insert_Png(
        L"../Resource/Tile/Tile_Block.png", L"TILE_BLOCK");

    m_vecTile.reserve(m_iTileCountX * m_iTileCountZ);

    // 타일 데이터 읽기
    for (int z = 0; z < m_iTileCountZ; ++z)
    {
        for (int x = 0; x < m_iTileCountX; ++x)
        {
            TILE_TYPE eType = TILE_NORMAL;
            fread(&eType, sizeof(TILE_TYPE), 1, pFile);

            CTile* pTile = new CTile;
            pTile->Set_TileIndex(x, z);
            pTile->Set_TileType(eType);
            m_vecTile.push_back(pTile);
        }
    }

    fclose(pFile);
    return true;
}

bool CMap_Manager::Save_Map(const TCHAR* pFilePath)
{
    FILE* pFile = nullptr;
    _tfopen_s(&pFile, pFilePath, L"wb");
    if (!pFile) return false;

    // 맵 크기 저장
    fwrite(&m_iTileCountX, sizeof(int), 1, pFile);
    fwrite(&m_iTileCountZ, sizeof(int), 1, pFile);

    // 타일 데이터 저장
    for (auto& pTile : m_vecTile)
    {
        TILE_TYPE eType = pTile->Get_TileType();
        fwrite(&eType, sizeof(TILE_TYPE), 1, pFile);
    }

    fclose(pFile);
    return true;
}

void CMap_Manager::Render(ID2D1RenderTarget* pRT)
{
    for (auto& pTile : m_vecTile)
        pTile->Render(pRT);
}

bool CMap_Manager::Is_Movable(int iTileX, int iTileZ)
{
    if (iTileX < 0 || iTileX >= m_iTileCountX) return false;
    if (iTileZ < 0 || iTileZ >= m_iTileCountZ) return false;

    int iIdx = iTileZ * m_iTileCountX + iTileX;
    return m_vecTile[iIdx]->Get_TileType() == TILE_NORMAL;
}

TILE_TYPE CMap_Manager::Get_TileType(int iTileX, int iTileZ)
{
    int iIdx = iTileZ * m_iTileCountX + iTileX;
    return m_vecTile[iIdx]->Get_TileType();
}

void CMap_Manager::Set_TileType(int iTileX, int iTileZ, TILE_TYPE eType)
{
    int iIdx = iTileZ * m_iTileCountX + iTileX;
    m_vecTile[iIdx]->Set_TileType(eType);
}

void CMap_Manager::Release()
{
    for (auto& pTile : m_vecTile)
    {
        delete pTile;
        pTile = nullptr;
    }
    m_vecTile.clear();
    m_iTileCountX = 0;
    m_iTileCountZ = 0;
}