#pragma once
#include "Player.h"
#include <array>
#include <mutex>

constexpr int32_t MAX_PLAYER = 1000;

class CPlayer_Manager
{
private:
    CPlayer_Manager() = default;
    ~CPlayer_Manager() = default;

public:
    static CPlayer_Manager* Get_Instance()
    {
        if (!m_pInstance)
            m_pInstance = new CPlayer_Manager;
        return m_pInstance;
    }

    static void Destroy_Instance()
    {
        if (m_pInstance)
        {
            delete m_pInstance;
            m_pInstance = nullptr;
        }
    }

    CPlayer_Manager(const CPlayer_Manager&) = delete;
    CPlayer_Manager& operator=(const CPlayer_Manager&) = delete;

    // 플레이어 생성 \ 세션 ID와 동일한 슬롯에 배치
    PlayerRef Create(int32_t nSessionID);

    // 조회
    PlayerRef Get_Player(int32_t nPlayerID);

    // 제거 (로그아웃)
    void      Remove(int32_t nPlayerID);

private:
    static CPlayer_Manager* m_pInstance;

    std::array<PlayerRef, MAX_PLAYER> m_players;
    std::mutex                        m_lock;
};