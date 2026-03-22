#include "pch.h"
#include "MainApp.h"
#include "Img_Manager.h"
#include "Level_Manager.h"
#include "Input_Manager.h"

CMainApp::CMainApp()
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

CMainApp::~CMainApp()
{
	Release();
}


void CMainApp::Initialize(void)
{

    m_hDC = GetDC(g_hWnd);
    CImg_Manager::Get_Instance()->Insert_Bmp(L"../Resource/BackBuffer/BackBuffer.bmp", L"BackBuffer");
    CLevel_Manager::Get_Instance()->Level_Change(LEVEL_TEST);
}

void CMainApp::Update(float dt)
{

    m_fTimeAcc += dt;
    m_iFrameCount++;
    CInput_Manager::Get_Instance()->Update();
    CLevel_Manager::Get_Instance()->Update(dt);
}

void CMainApp::Late_Update(float dt)
{

    CLevel_Manager::Get_Instance()->Late_Update(dt);
}

void CMainApp::Render(void)
{
	
    if (m_fTimeAcc >= 1.f)
    {
        int fps = m_iFrameCount;

        m_iFrameCount = 0;
        m_fTimeAcc = 0.f;

        wchar_t buffer[128];
        swprintf_s(buffer, L"MMO RPG FPS : %d", fps);

        SetWindowText(g_hWnd, buffer);
    }
#pragma region 이중 버퍼
    HDC	hBackDC = CImg_Manager::Get_Instance()->Find_Bmp(L"BackBuffer");
    CLevel_Manager::Get_Instance()->Render(hBackDC);
    BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);

#pragma endregion
}

void CMainApp::Release(void)
{
	ReleaseDC(g_hWnd, m_hDC);
}