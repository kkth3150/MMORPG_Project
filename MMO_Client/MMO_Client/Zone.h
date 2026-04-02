#pragma once
#include "Tile.h"
#include<unordered_map>

class CZone
{
public:
    CZone() = default;
    virtual ~CZone() { Release(); }

    // 자식이 반드시 구현
    // 이미지 로드 + Build_TileGrid() + ImgKey 세팅 + NpcSpawn 세팅
    virtual void    Build() PURE;
    virtual ZONE_ID Get_ZoneID()    const PURE;
    virtual void    Spawn_Objects() PURE;   // 추가 - 각 존이 직접 구현
    virtual void    Clear_Objects();       // 기본 구현 제공 (NPC 전체 삭제)

    // 공통 인터페이스 (기반 클래스에서 처리, 오버라이드 불필요)
    bool        Is_Movable(int x, int z)    const;
    TILE_TYPE   Get_TileType(int x, int z)  const;
    void        Set_TileType(int x, int z, TILE_TYPE eType);
    void        Render(ID2D1RenderTarget* pRT);
    void        Release();

    int     Get_TileCountX() const { return m_iTileCountX; }
    int     Get_TileCountZ() const { return m_iTileCountZ; }
    const   std::vector<FNpcSpawnInfo>& Get_NpcSpawnList() const { return m_vecNpcSpawn; }

protected:
    // 타일 배치만 담당
    // ImgKey 세팅은 하지 않음 → 자식이 Build()에서 직접 세팅
    void    Build_TileGrid(int innerX, int innerZ, const int* pBlockMap);
    void    Apply_ImgKeys(const std::unordered_map<TILE_TYPE, const TCHAR*>& keyMap); // 추가
protected:
    int                     m_iTileCountX = 0;
    int                     m_iTileCountZ = 0;
    std::vector<CTile*>          m_vecTile;
    std::vector<FNpcSpawnInfo>   m_vecNpcSpawn;
};