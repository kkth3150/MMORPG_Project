#pragma once
#include "Player.h"
#include <vector>
#include <unordered_set>
#include <mutex>

enum TILE_TYPE : uint8_t
{
    TILE_GRASS = 0,
    TILE_BLOCK = 1,
    TILE_BORDER_LT = 2,
    TILE_BORDER_RT = 3,
    TILE_BORDER_RB = 4,
    TILE_BORDER_LB = 5,
    TILE_BORDER_T = 6,
    TILE_BORDER_R = 7,
    TILE_BORDER_B = 8,
    TILE_BORDER_L = 9,
    TILE_OUTSIDE = 10,
};

inline bool Is_MovableTile(TILE_TYPE eType)
{
    return eType == TILE_GRASS;
}

constexpr int32_t VIEW_RANGE = 10;

class CZone
{
public:
    CZone(int32_t nZoneID, const char* pszName,
        int32_t nInnerX, int32_t nInnerZ, const int* pBlockMap);
    ~CZone();

    // ---- 입/퇴장 ----
    void EnterZone(PlayerRef pPlayer, float fSpawnX, float fSpawnZ);
    void LeaveZone(PlayerRef pPlayer);

    // ---- 이동 처리 ----

    // 마우스 클릭 시 호출 목적지 검증 + 브로드캐스트
    void OnMoveDest(PlayerRef pPlayer,
        float fDestX, float fDestZ, uint32_t nMoveTime);

    // 클라이언트 타일 변경 시 호출 위치 업데이트 + 시야 재계산
    void OnMovePos(PlayerRef pPlayer,
        float fCurX, float fCurZ, uint32_t nMoveTime);

    // ---- 유틸 ----
    bool IsMovable(int32_t nTileX, int32_t nTileZ) const;
    int32_t GetZoneID() { return m_nZoneID; }

private:
    // ---- 시야 ----
    std::vector<int32_t> GetNearPlayers(PlayerRef pPlayer);
    bool CanSee(PlayerRef pA, PlayerRef pB);
    void UpdateViewAndBroadcast(PlayerRef pPlayer,
        const std::vector<int32_t>& vOldView,
        const std::vector<int32_t>& vNewView,
        uint32_t nMoveTime);

    // ---- 패킷 전송 헬퍼 ----
    void Send_AddPlayer(PlayerRef pTo, PlayerRef pTarget);
    void Send_RemovePlayer(PlayerRef pTo, int32_t nTargetID);
    void Send_MovePlayer(PlayerRef pTo, PlayerRef pMoved, uint32_t nMoveTime);

private:
    int32_t  m_nZoneID = -1;
    char     m_szName[32] = {};

    int32_t              m_nTileCountX = 0;
    int32_t              m_nTileCountZ = 0;
    std::vector<uint8_t> m_tileMap;

    std::unordered_set<int32_t> m_playerIDs;
    std::mutex                  m_zoneLock;
};