#include "pch.h"
#include "Zone.h"
#include "Player_Manager.h"
#include "Session_Manager.h"
#include "Protocol.h"



CZone::CZone(int32_t nZoneID, const char* pszName,
    int32_t nInnerX, int32_t nInnerZ, const int* pBlockMap)
    : m_nZoneID(nZoneID)
{
    strncpy_s(m_szName, pszName, sizeof(m_szName) - 1);

    const int OUTER = 2;
    m_nTileCountX = OUTER + 1 + nInnerX + 1 + OUTER;
    m_nTileCountZ = OUTER + 1 + nInnerZ + 1 + OUTER;

    const int BORDER_START_X = OUTER;
    const int BORDER_END_X = OUTER + 1 + nInnerX;
    const int BORDER_START_Z = OUTER;
    const int BORDER_END_Z = OUTER + 1 + nInnerZ;

    m_tileMap.resize(m_nTileCountX * m_nTileCountZ, TILE_OUTSIDE);

    for (int z = 0; z < m_nTileCountZ; ++z)
    {
        for (int x = 0; x < m_nTileCountX; ++x)
        {
            bool bBorderX = (x == BORDER_START_X || x == BORDER_END_X);
            bool bBorderZ = (z == BORDER_START_Z || z == BORDER_END_Z);
            bool bInsideX = (x > BORDER_START_X && x < BORDER_END_X);
            bool bInsideZ = (z > BORDER_START_Z && z < BORDER_END_Z);

            TILE_TYPE eType = TILE_OUTSIDE;

            if (bBorderZ && bBorderX) eType = TILE_BLOCK;
            else if (bBorderZ && bInsideX)
                eType = (z == BORDER_START_Z) ? TILE_BORDER_RT : TILE_BORDER_LB;
            else if (bBorderX && bInsideZ)
                eType = (x == BORDER_START_X) ? TILE_BORDER_LT : TILE_BORDER_RB;
            else if (bInsideX && bInsideZ)
            {
                int iInnerX = x - BORDER_START_X - 1;
                int iInnerZ = (nInnerZ - 1) - (z - BORDER_START_Z - 1);
                eType = (pBlockMap[iInnerZ * nInnerX + iInnerX] == 1)
                    ? TILE_BLOCK : TILE_GRASS;
            }

            m_tileMap[z * m_nTileCountX + x] = static_cast<uint8_t>(eType);
        }
    }

    std::cout << "[CZone] '" << m_szName << "' 생성. "
        << m_nTileCountX << "x" << m_nTileCountZ << std::endl;
}

CZone::~CZone() {}

bool CZone::IsMovable(int32_t nTileX, int32_t nTileZ) const
{
    if (nTileX < 0 || nTileX >= m_nTileCountX) return false;
    if (nTileZ < 0 || nTileZ >= m_nTileCountZ) return false;
    return Is_MovableTile(static_cast<TILE_TYPE>(
        m_tileMap[nTileZ * m_nTileCountX + nTileX]));
}

// ================================================================
//  EnterZone
// ================================================================
void CZone::EnterZone(PlayerRef pPlayer, float fSpawnX, float fSpawnZ)
{
    pPlayer->m_nZoneID = m_nZoneID;
    pPlayer->m_fCurX = fSpawnX;
    pPlayer->m_fCurZ = fSpawnZ;
    pPlayer->m_fDestX = fSpawnX;
    pPlayer->m_fDestZ = fSpawnZ;
    pPlayer->m_bMoving = false;
    pPlayer->UpdateTilePos();

    {
        std::lock_guard<std::mutex> lock(m_zoneLock);
        m_playerIDs.insert(pPlayer->m_nPlayerID);
    }

    std::vector<int32_t> vNewView = GetNearPlayers(pPlayer);

    {
        std::lock_guard<std::mutex> lock(pPlayer->m_viewLock);
        pPlayer->m_viewList.clear();
        for (int32_t nID : vNewView)
            pPlayer->m_viewList.insert(nID);
    }

    for (int32_t nNearID : vNewView)
    {
        if (nNearID == pPlayer->m_nPlayerID) continue;

        PlayerRef pNear = CPlayer_Manager::Get_Instance()->Get_Player(nNearID);
        if (!pNear) continue;

        // 나에게 상대방 add
        Send_AddPlayer(pPlayer, pNear);

        // 상대방에게 나 add
        Send_AddPlayer(pNear, pPlayer);

        // 상대방 view_list에 나 추가
        {
            std::lock_guard<std::mutex> lock(pNear->m_viewLock);
            pNear->m_viewList.insert(pPlayer->m_nPlayerID);
        }
    }
}

// ================================================================
//  LeaveZone
// ================================================================
void CZone::LeaveZone(PlayerRef pPlayer)
{
    {
        std::lock_guard<std::mutex> lock(m_zoneLock);
        m_playerIDs.erase(pPlayer->m_nPlayerID);
    }

    std::unordered_set<int32_t> viewCopy;
    {
        std::lock_guard<std::mutex> lock(pPlayer->m_viewLock);
        viewCopy = pPlayer->m_viewList;
        pPlayer->m_viewList.clear();
    }

    for (int32_t nNearID : viewCopy)
    {
        PlayerRef pNear = CPlayer_Manager::Get_Instance()->Get_Player(nNearID);
        if (!pNear) continue;

        Send_RemovePlayer(pNear, pPlayer->m_nPlayerID);

        {
            std::lock_guard<std::mutex> lock(pNear->m_viewLock);
            pNear->m_viewList.erase(pPlayer->m_nPlayerID);
        }
    }
}

// ================================================================
//  OnMoveDest ? 마우스 클릭 시 1번 호출
//
//  1) 목적지 타일이 이동 가능한지 검증
//  2) 플레이어 목적지 저장
//  3) 현재 view_list에 브로드캐스트
//     (다른 클라이언트들이 이 플레이어의 이동을 보간 시작)
// ================================================================
void CZone::OnMoveDest(PlayerRef pPlayer,
    float fDestX, float fDestZ, uint32_t nMoveTime)
{
    int32_t nDestTileX = static_cast<int32_t>(floorf(fDestX));
    int32_t nDestTileZ = static_cast<int32_t>(floorf(fDestZ));

    // 목적지 블록 검증
    if (!IsMovable(nDestTileX, nDestTileZ))
    {
        // 이동 불가 타일 ? 현재 위치로 되돌림
        Send_MovePlayer(pPlayer, pPlayer, nMoveTime);
        return;
    }

    // 목적지 + 이동 시작 정보 저장
    // m_fMoveStartX/Z와 m_nMoveStartTime을 저장해두면
    // 서버가 언제든지 GetCurrentPos()로 정확한 위치 역산 가능
    float fCalcX, fCalcZ;
    pPlayer->GetCurrentPos(nMoveTime, fCalcX, fCalcZ);

    pPlayer->m_fMoveStartX = fCalcX;    // 클릭 시점의 실제 위치
    pPlayer->m_fMoveStartZ = fCalcZ;
    pPlayer->m_nMoveStartTime = nMoveTime;
    pPlayer->m_fDestX = fDestX;
    pPlayer->m_fDestZ = fDestZ;
    pPlayer->m_bMoving = true;
    pPlayer->m_nLastMoveTime = nMoveTime;

    // 현재 view_list에 브로드캐스트
    // 시야 재계산은 하지 않음 ? 아직 위치 안 바뀌었으니까
    {
        std::lock_guard<std::mutex> lock(pPlayer->m_viewLock);
        for (int32_t nNearID : pPlayer->m_viewList)
        {
            PlayerRef pNear = CPlayer_Manager::Get_Instance()->Get_Player(nNearID);
            if (pNear) Send_MovePlayer(pNear, pPlayer, nMoveTime);
        }
    }
}

// ================================================================
//  OnMovePos ? 클라이언트 타일 변경 시마다 호출
//
//  1) 현재 위치 업데이트
//  2) 타일 좌표 갱신
//  3) 타일이 바뀌었으면 시야 재계산
//     → 새로 보이는 플레이어: SC_ADD_PLAYER 양방향
//     → 시야 밖 플레이어:    SC_REMOVE_PLAYER 양방향
//     → 계속 보이는 플레이어: SC_MOVE_PLAYER
//  4) 타일이 안 바뀌었으면 move 브로드캐스트만
// ================================================================
void CZone::OnMovePos(PlayerRef pPlayer,
    float fCurX, float fCurZ, uint32_t nMoveTime)
{
    // ---- 속도 검증 ----
    // 서버가 계산한 현재 위치와 클라이언트가 보낸 위치를 비교
    // 허용 오차 이상이면 핵 의심 → 서버 계산 위치로 보정
    float fServerX, fServerZ;
    pPlayer->GetCurrentPos(nMoveTime, fServerX, fServerZ);

    float fDiffX = fCurX - fServerX;
    float fDiffZ = fCurZ - fServerZ;
    float fDiff = sqrtf(fDiffX * fDiffX + fDiffZ * fDiffZ);

    constexpr float MAX_TOLERANCE = 1.5f;  // 허용 오차 (타일 단위)

    if (fDiff > MAX_TOLERANCE)
    {
        // 오차 초과 → 서버 계산 위치로 강제 보정
        std::cout << "[경고] 위치 오차 초과. PlayerID="
            << pPlayer->m_nPlayerID
            << " 오차=" << fDiff << std::endl;
        fCurX = fServerX;
        fCurZ = fServerZ;
    }

    // 현재 위치 업데이트
    pPlayer->m_fCurX = fCurX;
    pPlayer->m_fCurZ = fCurZ;
    pPlayer->m_nLastMoveTime = nMoveTime;

    // 목적지 도착 여부 체크
    if (pPlayer->IsArrived(nMoveTime))
    {
        pPlayer->m_fCurX = pPlayer->m_fDestX;
        pPlayer->m_fCurZ = pPlayer->m_fDestZ;
        pPlayer->m_bMoving = false;
    }

    bool bTileChanged = pPlayer->UpdateTilePos();

    if (!bTileChanged)
    {
        // 타일 안에서만 움직임 ? 시야 변화 없음
        // move 패킷만 현재 view_list에 브로드캐스트
        std::lock_guard<std::mutex> lock(pPlayer->m_viewLock);
        for (int32_t nNearID : pPlayer->m_viewList)
        {
            PlayerRef pNear = CPlayer_Manager::Get_Instance()->Get_Player(nNearID);
            if (pNear) Send_MovePlayer(pNear, pPlayer, nMoveTime);
        }
        return;
    }

    // ---- 타일 바뀜 → 시야 재계산 ----
    std::vector<int32_t> vOldView;
    {
        std::lock_guard<std::mutex> lock(pPlayer->m_viewLock);
        vOldView.assign(pPlayer->m_viewList.begin(), pPlayer->m_viewList.end());
    }

    std::vector<int32_t> vNewView = GetNearPlayers(pPlayer);

    {
        std::lock_guard<std::mutex> lock(pPlayer->m_viewLock);
        pPlayer->m_viewList.clear();
        for (int32_t nID : vNewView)
            pPlayer->m_viewList.insert(nID);
    }

    UpdateViewAndBroadcast(pPlayer, vOldView, vNewView, nMoveTime);
}

// ================================================================
//  GetNearPlayers ? 존 전체 순회 + 타일 거리로 시야 판정
// ================================================================
std::vector<int32_t> CZone::GetNearPlayers(PlayerRef pPlayer)
{
    std::vector<int32_t> vResult;

    std::lock_guard<std::mutex> lock(m_zoneLock);
    for (int32_t nID : m_playerIDs)
    {
        if (nID == pPlayer->m_nPlayerID) continue;

        PlayerRef pOther = CPlayer_Manager::Get_Instance()->Get_Player(nID);
        if (!pOther) continue;

        if (CanSee(pPlayer, pOther))
            vResult.push_back(nID);
    }
    return vResult;
}

bool CZone::CanSee(PlayerRef pA, PlayerRef pB)
{
    int32_t nDX = abs(pA->m_nTileX - pB->m_nTileX);
    int32_t nDZ = abs(pA->m_nTileZ - pB->m_nTileZ);
    return nDX <= VIEW_RANGE && nDZ <= VIEW_RANGE;
}

// ================================================================
//  UpdateViewAndBroadcast
//
//  old/new 비교:
//  new에 있고 old에 없음 → 새로 보임 → SC_ADD_PLAYER 양방향
//  old에 있고 new에 없음 → 시야 밖   → SC_REMOVE_PLAYER 양방향
//  둘 다 있음            → 계속 보임 → SC_MOVE_PLAYER
// ================================================================
void CZone::UpdateViewAndBroadcast(PlayerRef pPlayer,
    const std::vector<int32_t>& vOldView,
    const std::vector<int32_t>& vNewView,
    uint32_t nMoveTime)
{
    std::unordered_set<int32_t> setOld(vOldView.begin(), vOldView.end());
    std::unordered_set<int32_t> setNew(vNewView.begin(), vNewView.end());

    // 나 자신에게 이동 확인 패킷
    Send_MovePlayer(pPlayer, pPlayer, nMoveTime);

    for (int32_t nNearID : setNew)
    {
        PlayerRef pNear = CPlayer_Manager::Get_Instance()->Get_Player(nNearID);
        if (!pNear) continue;

        if (setOld.count(nNearID) == 0)
        {
            // ---- 새로 시야에 들어옴 ----
            Send_AddPlayer(pPlayer, pNear);   // 나에게 상대방 add
            Send_AddPlayer(pNear, pPlayer);   // 상대방에게 나 add

            {
                std::lock_guard<std::mutex> lock(pNear->m_viewLock);
                pNear->m_viewList.insert(pPlayer->m_nPlayerID);
            }
        }
        else
        {
            // ---- 계속 시야 안 ----
            Send_MovePlayer(pNear, pPlayer, nMoveTime);
        }
    }

    for (int32_t nNearID : setOld)
    {
        if (setNew.count(nNearID) != 0) continue;

        PlayerRef pNear = CPlayer_Manager::Get_Instance()->Get_Player(nNearID);
        if (!pNear) continue;

        // ---- 시야 밖으로 나감 ----
        Send_RemovePlayer(pPlayer, nNearID);           // 나에게 상대방 remove
        Send_RemovePlayer(pNear, pPlayer->m_nPlayerID); // 상대방에게 나 remove

        {
            std::lock_guard<std::mutex> lock(pNear->m_viewLock);
            pNear->m_viewList.erase(pPlayer->m_nPlayerID);
        }
    }
}

// ================================================================
//  패킷 전송 헬퍼
// ================================================================
void CZone::Send_AddPlayer(PlayerRef pTo, PlayerRef pTarget)
{
    auto pSession = CSession_Manager::Get_Instance()->Get_Session(pTo->m_nSessionID);
    if (!pSession) return;

    SC_ADD_PLAYER_PACKET pkt = {};
    pkt.header.size = sizeof(pkt);
    pkt.header.id = SC_ADD_PLAYER;
    pkt.playerID = pTarget->m_nPlayerID;
    pkt.fCurX = pTarget->m_fCurX;
    pkt.fCurZ = pTarget->m_fCurZ;
    pkt.fDestX = pTarget->m_fDestX;  // 이동 중이면 목적지도 전달
    pkt.fDestZ = pTarget->m_fDestZ;  // 클라이언트가 보간 시작
    pkt.fSpeed = pTarget->m_fSpeed;
    strncpy_s(pkt.name, pTarget->m_szName, sizeof(pkt.name) - 1);
    pSession->Send(&pkt, sizeof(pkt));
}

void CZone::Send_RemovePlayer(PlayerRef pTo, int32_t nTargetID)
{
    auto pSession = CSession_Manager::Get_Instance()->Get_Session(pTo->m_nSessionID);
    if (!pSession) return;

    SC_REMOVE_PLAYER_PACKET pkt = {};
    pkt.header.size = sizeof(pkt);
    pkt.header.id = SC_REMOVE_PLAYER;
    pkt.playerID = nTargetID;
    pSession->Send(&pkt, sizeof(pkt));
}

void CZone::Send_MovePlayer(PlayerRef pTo, PlayerRef pMoved, uint32_t nMoveTime)
{
    auto pSession = CSession_Manager::Get_Instance()->Get_Session(pTo->m_nSessionID);
    if (!pSession) return;

    SC_MOVE_PLAYER_PACKET pkt = {};
    pkt.header.size = sizeof(pkt);
    pkt.header.id = SC_MOVE_PLAYER;
    pkt.playerID = pMoved->m_nPlayerID;
    pkt.fCurX = pMoved->m_fCurX;
    pkt.fCurZ = pMoved->m_fCurZ;
    pkt.fDestX = pMoved->m_fDestX;
    pkt.fDestZ = pMoved->m_fDestZ;
    pkt.fSpeed = pMoved->m_fSpeed;
    pkt.moveTime = nMoveTime;
    pSession->Send(&pkt, sizeof(pkt));
}