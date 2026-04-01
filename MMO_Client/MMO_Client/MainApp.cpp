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

    //m_hDC = GetDC(g_hWnd);
   /* CImg_Manager::Get_Instance()->Insert_Bmp(L"../Resource/BackBuffer/BackBuffer.bmp", L"BackBuffer");
    CLevel_Manager::Get_Instance()->Level_Change(LEVEL_TEST);*/
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);

    //2. 렌더타겟 생성 (BackBuffer 역할을 D2D가 대체)
    RECT rc;
    GetClientRect(g_hWnd, &rc);
    m_pFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            g_hWnd,
            D2D1::SizeU(rc.right, rc.bottom)
        ),
        &m_pRenderTarget
    );

    //3. DirectWrite 팩토리 생성 (텍스트용)
    DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
    );

    //4. FPS 텍스트 포맷 생성
    m_pDWriteFactory->CreateTextFormat(
        L"Arial", nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        16.f, L"ko-KR",
        &m_pTextFormat
    );

    //5. Img_Manager에 RenderTarget 전달 후 이미지 로드
    CImg_Manager::Get_Instance()->Set_RenderTarget(m_pRenderTarget);
    CImg_Manager::Get_Instance()->Create_DebugFont(m_pDWriteFactory);

    //6. 레벨 초기화 (그대로)
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
    //HDC	hBackDC = CImg_Manager::Get_Instance()->Find_Bmp(L"BackBuffer");
    //CLevel_Manager::Get_Instance()->Render(hBackDC);
    //BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);

    m_pRenderTarget->BeginDraw();
    m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    // 레벨 렌더 (인자만 RT로 변경)
    CLevel_Manager::Get_Instance()->Render(m_pRenderTarget);

    m_pRenderTarget->EndDraw();

}

void CMainApp::Release(void)
{

    //COM 객체 순서대로 해제(생성 역순)
    if (m_pTextFormat) { m_pTextFormat->Release();     m_pTextFormat = nullptr; }
    if (m_pDWriteFactory) { m_pDWriteFactory->Release();  m_pDWriteFactory = nullptr; }
    if (m_pRenderTarget) { m_pRenderTarget->Release();   m_pRenderTarget = nullptr; }
    if (m_pFactory) { m_pFactory->Release();        m_pFactory = nullptr; }
}