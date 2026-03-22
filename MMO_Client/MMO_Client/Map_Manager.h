#pragma once
#include "Tile.h"

class CMap_Manager
{
public:
    static CMap_Manager* Get_Instance()
    {
        if (!m_pInstance)
            m_pInstance = new CMap_Manager;
        return m_pInstance;
    }
    static void Destroy_Instance()
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }

public:
    // 파일에서 맵 로드
    bool Load_Map(const TCHAR* pFilePath);
    // 맵 저장 (에디터용, 나중에)
    bool Save_Map(const TCHAR* pFilePath);
    // 맵 생성 (기본값으로)
    void Create_Map(int iTileCountX, int iTileCountZ);

    void Render(HDC hDC);
    void Release();

    // 길찾기용
    bool        Is_Movable(int iTileX, int iTileZ);
    TILE_TYPE   Get_TileType(int iTileX, int iTileZ);
    void        Set_TileType(int iTileX, int iTileZ, TILE_TYPE eType);

    int Get_TileCountX() { return m_iTileCountX; }
    int Get_TileCountZ() { return m_iTileCountZ; }

private:
    CMap_Manager() {}
    ~CMap_Manager() { Release(); }

private:
    static CMap_Manager* m_pInstance;
    std::vector<CTile*>     m_vecTile;
    int                     m_iTileCountX = 0;
    int                     m_iTileCountZ = 0;
};