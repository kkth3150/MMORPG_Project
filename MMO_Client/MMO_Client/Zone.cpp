#include "pch.h"
#include "Zone.h"
#include "Object_Manager.h"

// Build_TileGrid : 타일 배치만 담당
// ImgKey는 세팅하지 않음 → 자식 Build()에서 타입별로 직접 세팅
void CZone::Build_TileGrid(int innerX, int innerZ, const int* pBlockMap)
{
    const int OUTER = 2;    // 테두리 밖 두께

    m_iTileCountX = OUTER + 1 + innerX + 1 + OUTER;
    m_iTileCountZ = OUTER + 1 + innerZ + 1 + OUTER;

    const int BORDER_START_X = OUTER;
    const int BORDER_END_X = OUTER + 1 + innerX;
    const int BORDER_START_Z = OUTER;
    const int BORDER_END_Z = OUTER + 1 + innerZ;

    m_vecTile.reserve(m_iTileCountX * m_iTileCountZ);

    for (int z = 0; z < m_iTileCountZ; ++z)
    {
        for (int x = 0; x < m_iTileCountX; ++x)
        {
            TILE_TYPE eType = TILE_OUTSIDE;

            bool bBorderX = (x == BORDER_START_X || x == BORDER_END_X);
            bool bBorderZ = (z == BORDER_START_Z || z == BORDER_END_Z);
            bool bInsideX = (x > BORDER_START_X && x < BORDER_END_X);
            bool bInsideZ = (z > BORDER_START_Z && z < BORDER_END_Z);

            if (bBorderZ && bBorderX)
            {
                if (z == BORDER_START_Z && x == BORDER_START_X) eType = TILE_BLOCK;
                else if (z == BORDER_START_Z && x == BORDER_END_X)   eType = TILE_BLOCK;
                else if (z == BORDER_END_Z && x == BORDER_END_X)   eType = TILE_BLOCK;
                else if (z == BORDER_END_Z && x == BORDER_START_X) eType = TILE_BLOCK;
            }
            else if (bBorderZ && bInsideX)
            {
                eType = (z == BORDER_START_Z) ? TILE_BORDER_RT : TILE_BORDER_LB; //RT 모서리, 
            }
            else if (bBorderX && bInsideZ)
            {
                eType = (x == BORDER_START_X) ? TILE_BORDER_LT : TILE_BORDER_RB; //LT 모서리, 
            }
            else if (bInsideX && bInsideZ)
            {
                int iInnerX = x - BORDER_START_X - 1;
                int iInnerZ = (innerZ - 1) - (z - BORDER_START_Z - 1);
                eType = (pBlockMap[iInnerZ * innerX + iInnerX] == 1)
                    ? TILE_BLOCK : TILE_GRASS;
            }

            CTile* pTile = new CTile;
            pTile->Set_TileIndex(x, z);
            pTile->Set_TileType(eType);
            // ImgKey는 여기서 세팅 안 함
            // 자식 Build()에서 Build_TileGrid() 호출 후 직접 세팅
            m_vecTile.push_back(pTile);
        }
    }
}

// 타입별 ImgKey 일괄 세팅 헬퍼
// 자식 Build()에서 호출, 키 이름은 자식이 결정
void CZone::Apply_ImgKeys(const std::unordered_map<TILE_TYPE, const TCHAR*>& keyMap)
{
    for (auto& pTile : m_vecTile)
    {
        auto it = keyMap.find(pTile->Get_TileType());
        if (it != keyMap.end())
            pTile->Set_ImgKey(it->second);
    }
}

void CZone::Clear_Objects()
{
    CObject_Manager::Get_Instance()->DeleteID(OBJ_NPC);
}

bool CZone::Is_Movable(int x, int z) const
{
    if (x < 0 || x >= m_iTileCountX) return false;
    if (z < 0 || z >= m_iTileCountZ) return false;
    return Is_MovableTile(m_vecTile[z * m_iTileCountX + x]->Get_TileType());
}

TILE_TYPE CZone::Get_TileType(int x, int z) const
{
    return m_vecTile[z * m_iTileCountX + x]->Get_TileType();
}

void CZone::Set_TileType(int x, int z, TILE_TYPE eType)
{
    m_vecTile[z * m_iTileCountX + x]->Set_TileType(eType);
}

void CZone::Render(ID2D1RenderTarget* pRT)
{
    // 렌더는 기반 클래스에서 전담
    // CTile이 자신의 ImgKey로 알아서 렌더하므로 존마다 오버라이드 불필요
    for (auto& pTile : m_vecTile)
        pTile->Render(pRT);
}

void CZone::Release()
{
    for (auto& pTile : m_vecTile)
        delete pTile;
    m_vecTile.clear();
    m_vecNpcSpawn.clear();
    m_iTileCountX = m_iTileCountZ = 0;
}