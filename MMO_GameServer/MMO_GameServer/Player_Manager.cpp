#include "pch.h"
#include "Player_Manager.h"

CPlayer_Manager* CPlayer_Manager::m_pInstance = nullptr;

PlayerRef CPlayer_Manager::Create(int32_t nSessionID)
{
    std::lock_guard<std::mutex> lock(m_lock);

    // 세션 ID와 동일한 슬롯 사용
    // → CSession_Manager와 인덱스가 맞아서 조회가 단순해짐
    if (nSessionID < 0 || nSessionID >= MAX_PLAYER) return nullptr;
    if (m_players[nSessionID] != nullptr) return nullptr;

    PlayerRef pPlayer = std::make_shared<CPlayer>();
    pPlayer->m_nPlayerID = nSessionID;
    pPlayer->m_nSessionID = nSessionID;
    m_players[nSessionID] = pPlayer;

    return pPlayer;
}

PlayerRef CPlayer_Manager::Get_Player(int32_t nPlayerID)
{
    if (nPlayerID < 0 || nPlayerID >= MAX_PLAYER) return nullptr;

    std::lock_guard<std::mutex> lock(m_lock);
    return m_players[nPlayerID];
}

void CPlayer_Manager::Remove(int32_t nPlayerID)
{
    if (nPlayerID < 0 || nPlayerID >= MAX_PLAYER) return;

    std::lock_guard<std::mutex> lock(m_lock);
    if (m_players[nPlayerID] != nullptr)
    {
        m_players[nPlayerID] = nullptr;
        std::cout << "[CPlayer_Manager] 플레이어 제거. ID=" << nPlayerID << std::endl;
    }
}