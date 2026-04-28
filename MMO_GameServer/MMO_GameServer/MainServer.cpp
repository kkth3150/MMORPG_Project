#include "pch.h"
#include "IOCP_Server.h"
#include "Session_Manager.h"
#include <iostream>

int main()
{
    std::cout << "=== MMO GameServer ===" << std::endl;

    // 워커 스레드 뜨기 전에 미리 생성
    // 멀티스레드 경합 없이 안전하게 초기화
    CSession_Manager::Get_Instance();

    CIOCP_Server server;
    if (!server.Start(7777))
    {
        std::cout << "서버 시작 실패" << std::endl;
        CSession_Manager::Destroy_Instance();
        return -1;
    }

    server.Run();

    // 서버 종료 시 명시적 해제
    CSession_Manager::Destroy_Instance();
    return 0;
}