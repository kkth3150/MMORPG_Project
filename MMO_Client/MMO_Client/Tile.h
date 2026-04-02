#pragma once
#include "define.h"

class CTile
{
public:
    CTile() = default;
    ~CTile() = default;

    // 인덱스 / 타입 세팅
    void        Set_TileIndex(int x, int z) { m_iTileX = x; m_iTileZ = z; }
    void        Set_TileType(TILE_TYPE eType) { m_eType = eType; }

    // 렌더 이미지 키 - 존이 Build()에서 직접 세팅
    void        Set_ImgKey( const TCHAR* wKey) { m_pImgKey = wKey; }

    // Getter
    TILE_TYPE   Get_TileType()  const { return m_eType; }
    int         Get_TileX()     const { return m_iTileX; }
    int         Get_TileZ()     const { return m_iTileZ; }

    void        Render(ID2D1RenderTarget* pRT);

private:
    int         m_iTileX = 0;
    int         m_iTileZ = 0;
    TILE_TYPE   m_eType = TILE_GRASS;
    const TCHAR* m_pImgKey = L"";
};