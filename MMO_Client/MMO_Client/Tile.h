#pragma once
#include "define.h"


class CTile
{
public:
    CTile() {}
    ~CTile() {}

public:
    void Render(HDC hDC);

public:
    void      Set_TileIndex(int iX, int iZ) { m_iTileX = iX; m_iTileZ = iZ; }
    void      Set_TileType(TILE_TYPE eType) { m_eType = eType; }
    int       Get_TileX() { return m_iTileX; }
    int       Get_TileZ() { return m_iTileZ; }
    TILE_TYPE Get_TileType() { return m_eType; }

private:
    int         m_iTileX = 0;
    int         m_iTileZ = 0;
    TILE_TYPE   m_eType = TILE_NORMAL;
};