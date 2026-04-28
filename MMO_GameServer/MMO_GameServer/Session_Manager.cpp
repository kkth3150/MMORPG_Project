#include "pch.h"
#include "Session_Manager.h"
#include <iostream>

// 정적 멤버 초기화 클라이언트 코드와 동일한 패턴
CSession_Manager* CSession_Manager::m_pInstance = nullptr;

CSession_Manager::CSession_Manager()
{
    // m_sessions 배열은 shared_ptr 기본값(nullptr)으로 자동 초기화
}

int32_t CSession_Manager::Assign()
{
    std::lock_guard<std::mutex> lock(m_lock);

    for (int32_t i = 0; i < MAX_SESSION; ++i)
    {
        if (m_sessions[i] == nullptr)
        {
            m_sessions[i] = std::make_shared<CSession>();
            m_sessions[i]->SetID(i);
            m_count++;
            return i;
        }
    }
    return -1;
}

SessionRef CSession_Manager::Get_Session(int32_t nID)
{
    if (nID < 0 || nID >= MAX_SESSION) return nullptr;

    std::lock_guard<std::mutex> lock(m_lock);
    return m_sessions[nID];
}

void CSession_Manager::Release(int32_t nID)
{
    if (nID < 0 || nID >= MAX_SESSION) return;

    std::lock_guard<std::mutex> lock(m_lock);

    if (m_sessions[nID] != nullptr)
    {
        m_sessions[nID] = nullptr;
        m_count--;
        std::cout << "[CSessionManager] 세션 반납. ID=" << nID
            << " 현재 접속=" << m_count << std::endl;
    }
}