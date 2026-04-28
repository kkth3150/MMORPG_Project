#include "pch.h"
#include "Network_Manager.h"
#include "Object_Manager.h"   // 다른 플레이어 오브젝트 관리
//#include "Other_Player.h"     // 다른 플레이어 렌더 객체
#include "Player.h"           // 내 플레이어
#include <iostream>
#include "Level_Manager.h"

CNetwork_Manager* CNetwork_Manager::m_pInstance = nullptr;

// ================================================================
//  Connect
//  TCP 연결 + 수신 스레드 시작
//  클라이언트는 서버 1개와만 통신  워커 스레드 1개면 충분
// ================================================================
bool CNetwork_Manager::Connect(const wchar_t* pszIP, uint16_t nPort)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return false;

    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET) return false;

    // TCP_NODELAY  작은 패킷을 즉시 전송 (게임에서 필수)
    BOOL bNoDelay = TRUE;
    setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY,
        reinterpret_cast<const char*>(&bNoDelay), sizeof(bNoDelay));

    // IP 변환
    char szIP[64] = {};
    WideCharToMultiByte(CP_ACP, 0, pszIP, -1, szIP, sizeof(szIP), NULL, NULL);

    SOCKADDR_IN addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(nPort);
    inet_pton(AF_INET, szIP, &addr.sin_addr);

    if (connect(m_socket,
        reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr)) == SOCKET_ERROR)
    {
        std::cout << "[Network] 연결 실패: " << WSAGetLastError() << std::endl;
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
        return false;
    }

    m_bConnected = true;
    std::cout << "[Network] 서버 연결 완료" << std::endl;

    // 수신 스레드 시작  detach로 백그라운드 실행
    m_recvThread = std::thread(&CNetwork_Manager::RecvThread, this);
    m_recvThread.detach();

    return true;
}

void CNetwork_Manager::Disconnect()
{
    if (m_bConnected.exchange(false) == false) return;

    closesocket(m_socket);
    m_socket = INVALID_SOCKET;
    WSACleanup();
}

// ================================================================
//  RecvThread 백그라운드 수신 스레드
//
//  서버 패킷 수신  ProcessPacket()으로 파싱
//  파싱된 패킷은 람다로 감싸서 큐에 push
//  실제 처리는 메인 스레드 Dispatch()에서
// ================================================================
void CNetwork_Manager::RecvThread()
{
    while (m_bConnected)
    {
        int32_t nReceived = recv(
            m_socket,
            reinterpret_cast<char*>(m_recvBuf) + m_nPrevRemain,
            RECV_BUF_SIZE - m_nPrevRemain,
            0);

        if (nReceived <= 0)
        {
            std::cout << "[Network] 서버 연결 끊김" << std::endl;
            m_bConnected = false;
            break;
        }

        // 서버 코드와 동일한 패킷 경계 파싱
        int32_t  nRemain = m_nPrevRemain + nReceived;
        uint8_t* pCursor = m_recvBuf;

        while (nRemain > 0)
        {
            if (nRemain < static_cast<int32_t>(sizeof(PacketHeader))) break;

            PacketHeader* pHeader = reinterpret_cast<PacketHeader*>(pCursor);
            if (nRemain < pHeader->size) break;

            // 패킷 데이터를 복사해서 람다에 캡처
            // (pCursor는 다음 루프에서 바뀌므로 복사 필수)
            std::vector<uint8_t> vData(pCursor, pCursor + pHeader->size);

            ProcessPacket(pCursor, pHeader->size);

            pCursor += pHeader->size;
            nRemain -= pHeader->size;
        }

        m_nPrevRemain = nRemain;
        if (nRemain > 0)
            memmove(m_recvBuf, pCursor, nRemain);
    }
}

// ================================================================
//  ProcessPacket
//  패킷 ID 확인  해당 핸들러 호출
//  핸들러는 람다로 감싸서 큐에 push
// ================================================================
void CNetwork_Manager::ProcessPacket(uint8_t* pBuffer, int32_t nSize)
{
    PacketHeader* pHeader = reinterpret_cast<PacketHeader*>(pBuffer);

    // 패킷 데이터를 vector로 복사  람다가 캡처할 수 있게
    std::vector<uint8_t> vData(pBuffer, pBuffer + nSize);

    switch (pHeader->id)
    {
    case SC_LOGIN_OK:
        PushTask([this, vData]() mutable {
            Handle_SC_LOGIN_OK(vData.data(), static_cast<int32_t>(vData.size()));
            });
        break;

    case SC_LOGIN_FAIL:
        PushTask([this, vData]() mutable {
            Handle_SC_LOGIN_FAIL(vData.data(), static_cast<int32_t>(vData.size()));
            });
        break;

    case SC_ENTER_GAME:
        PushTask([this, vData]() mutable {
            Handle_SC_ENTER_GAME(vData.data(), static_cast<int32_t>(vData.size()));
            });
        break;

    case SC_ADD_PLAYER:
        PushTask([this, vData]() mutable {
            Handle_SC_ADD_PLAYER(vData.data(), static_cast<int32_t>(vData.size()));
            });
        break;

    case SC_REMOVE_PLAYER:
        PushTask([this, vData]() mutable {
            Handle_SC_REMOVE_PLAYER(vData.data(), static_cast<int32_t>(vData.size()));
            });
        break;

    case SC_MOVE_PLAYER:
        PushTask([this, vData]() mutable {
            Handle_SC_MOVE_PLAYER(vData.data(), static_cast<int32_t>(vData.size()));
            });
        break;

    default:
        std::cout << "[Network] 알 수 없는 패킷: " << pHeader->id << std::endl;
        break;
    }
}

// ================================================================
//  Dispatch  메인 스레드에서 매 프레임 호출
//
//  큐에 쌓인 태스크를 전부 처리
//  락을 짧게 잡는 방법
//  전체 큐를 로컬 큐로 swap  락 해제  처리
//   처리 중 새 패킷이 와도 큐에 쌓을 수 있음
// ================================================================
void CNetwork_Manager::Dispatch()
{
    // 큐 전체를 로컬로 가져옴 (락 최소화)
    std::queue<FPacketTask> localQueue;
    {
        std::lock_guard<std::mutex> lock(m_queueLock);
        std::swap(localQueue, m_taskQueue);
    }

    // 락 없이 처리 이미 메인 스레드에 있으므로 D2D 안전
    while (!localQueue.empty())
    {
        localQueue.front().m_handler();
        localQueue.pop();
    }
}

// ================================================================
//  패킷별 핸들러  메인 스레드에서 실행됨
// ================================================================
void CNetwork_Manager::Handle_SC_LOGIN_OK(uint8_t* pBuffer, int32_t nSize)
{
    SC_LOGIN_OK_PACKET* pPkt = reinterpret_cast<SC_LOGIN_OK_PACKET*>(pBuffer);
    m_nMyPlayerID = pPkt->playerID;
    std::cout << "[Network] 로그인 성공. PlayerID=" << m_nMyPlayerID << std::endl;

    // TODO: 로그인 성공 UI 처리
    CLevel_Manager::Get_Instance()->Level_Change(LEVEL_TEST);
}

void CNetwork_Manager::Handle_SC_LOGIN_FAIL(uint8_t* pBuffer, int32_t nSize)
{
    SC_LOGIN_FAIL_PACKET* pPkt = reinterpret_cast<SC_LOGIN_FAIL_PACKET*>(pBuffer);
    std::cout << "[Network] 로그인 실패. reason=" << (int)pPkt->reason << std::endl;

    // TODO: 실패 메시지 UI 표시
}

void CNetwork_Manager::Handle_SC_ENTER_GAME(uint8_t* pBuffer, int32_t nSize)
{
    SC_ENTER_GAME_PACKET* pPkt = reinterpret_cast<SC_ENTER_GAME_PACKET*>(pBuffer);

    //std::cout << "[Network] 게임 진입. X=" << pPkt->fCurX
    //    << " Z=" << pPkt->fCurZ << std::endl;

    // 내 캐릭터 초기 위치 세팅
    // CPlayer가 싱글톤이거나 Object_Manager에서 찾아서 세팅
    // TODO: 내 플레이어 스폰 처리
}

void CNetwork_Manager::Handle_SC_ADD_PLAYER(uint8_t* pBuffer, int32_t nSize)
{
    SC_ADD_PLAYER_PACKET* pPkt = reinterpret_cast<SC_ADD_PLAYER_PACKET*>(pBuffer);

    // 내 캐릭터면 무시
    if (pPkt->playerID == m_nMyPlayerID) return;

    std::cout << "[Network] 플레이어 추가. ID=" << pPkt->playerID << std::endl;

    // COther_Player 생성 → Object_Manager에 추가
    // TODO: COther_Player 생성 후 초기위치 + 목적지 세팅
    // COther_Player* pOther = new COther_Player;
    // pOther->SetPlayerID(pPkt->playerID);
    // pOther->SetPos(pPkt->fCurX, pPkt->fCurZ);
    // pOther->SetDest(pPkt->fDestX, pPkt->fDestZ);
    // pOther->SetSpeed(pPkt->fSpeed);
    // CObject_Manager::Get_Instance()->Add_Object(OBJ_OTHER_PLAYER, pOther);
}

void CNetwork_Manager::Handle_SC_REMOVE_PLAYER(uint8_t* pBuffer, int32_t nSize)
{
    SC_REMOVE_PLAYER_PACKET* pPkt = reinterpret_cast<SC_REMOVE_PLAYER_PACKET*>(pBuffer);

    std::cout << "[Network] 플레이어 제거. ID=" << pPkt->playerID << std::endl;

    // TODO: Object_Manager에서 해당 ID의 COther_Player 제거
    // CObject_Manager::Get_Instance()->Remove_OtherPlayer(pPkt->playerID);
}

void CNetwork_Manager::Handle_SC_MOVE_PLAYER(uint8_t* pBuffer, int32_t nSize)
{
    SC_MOVE_PLAYER_PACKET* pPkt = reinterpret_cast<SC_MOVE_PLAYER_PACKET*>(pBuffer);

    if (pPkt->playerID == m_nMyPlayerID)
    {
        // 내 캐릭터 위치 보정 (서버 검증 결과)
        // TODO: 내 플레이어 위치 보정
        return;
    }

    // 다른 플레이어 Dead Reckoning 갱신
    // TODO: Object_Manager에서 해당 ID의 COther_Player 찾아서
    //       현재위치 보정 + 목적지 갱신
    // COther_Player* pOther = Find(pPkt->playerID);
    // if (pOther) pOther->OnRecv_MovePlayer(pPkt);
}

// ================================================================
//  송신 함수들
// ================================================================
bool CNetwork_Manager::SendRaw(const void* pData, int32_t nSize)
{
    if (!m_bConnected) return false;
    return send(m_socket,
        reinterpret_cast<const char*>(pData), nSize, 0) != SOCKET_ERROR;
}

void CNetwork_Manager::SendLogin(const char* pszID, const char* pszPW)
{
    CS_LOGIN_PACKET pkt = {};
    pkt.header.size = sizeof(pkt);
    pkt.header.id = CS_LOGIN;
    strncpy_s(pkt.id, pszID, sizeof(pkt.id) - 1);
    strncpy_s(pkt.pw, pszPW, sizeof(pkt.pw) - 1);
    SendRaw(&pkt, sizeof(pkt));
}

void CNetwork_Manager::SendMoveDest(float fDestX, float fDestZ, uint32_t nMoveTime)
{
    CS_MOVE_DEST_PACKET pkt = {};
    pkt.header.size = sizeof(pkt);
    pkt.header.id = CS_MOVE_DEST;
    pkt.fDestX = fDestX;
    pkt.fDestZ = fDestZ;
    pkt.moveTime = nMoveTime;
    SendRaw(&pkt, sizeof(pkt));
}

void CNetwork_Manager::SendMovePos(float fCurX, float fCurZ, uint32_t nMoveTime)
{
    CS_MOVE_POS_PACKET pkt = {};
    pkt.header.size = sizeof(pkt);
    pkt.header.id = CS_MOVE_POS;
    pkt.fCurX = fCurX;
    pkt.fCurZ = fCurZ;
    pkt.moveTime = nMoveTime;
    SendRaw(&pkt, sizeof(pkt));
}