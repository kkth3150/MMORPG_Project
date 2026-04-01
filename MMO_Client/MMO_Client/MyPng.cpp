#include "pch.h"
#include "MyPng.h"

CMyPng::CMyPng()
{
}

CMyPng::~CMyPng()
{
    Release();
}

void CMyPng::Load_Png(const TCHAR* pFilePath, ID2D1RenderTarget* pRT)
{
    IWICImagingFactory* pWIC = nullptr;
    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory, nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pWIC)
    );
    if (FAILED(hr) || !pWIC)
    {
        MessageBox(g_hWnd, L"WIC 팩토리 생성 실패", L"오류", MB_OK);
        return;
    }

    // 2. 파일 디코딩
    IWICBitmapDecoder* pDecoder = nullptr;
    hr = pWIC->CreateDecoderFromFilename(
        pFilePath, nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder
    );
    if (FAILED(hr) || !pDecoder)
    {
        //어떤 파일이 문제인지 바로 알 수 있음
        MessageBox(g_hWnd, pFilePath, L"PNG 파일 로드 실패 - 경로 확인", MB_OK);
        pWIC->Release();
        return;
    }

    // 3. 첫 번째 프레임 추출
    IWICBitmapFrameDecode* pFrame = nullptr;
    pDecoder->GetFrame(0, &pFrame);
    if (!pFrame)
    {
        pDecoder->Release();
        pWIC->Release();
        return;
    }

    // 4. 포맷 변환
    IWICFormatConverter* pConverter = nullptr;
    pWIC->CreateFormatConverter(&pConverter);
    pConverter->Initialize(
        pFrame,
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        nullptr, 0.f,
        WICBitmapPaletteTypeCustom
    );

    // 5. ID2D1Bitmap 생성
    hr = pRT->CreateBitmapFromWicBitmap(pConverter, nullptr, &m_pBitmap);
    if (FAILED(hr) || !m_pBitmap)
    {
        MessageBox(g_hWnd, pFilePath, L"D2D Bitmap 생성 실패", MB_OK);
    }
    else
    {
        auto size = m_pBitmap->GetPixelSize();
        m_iWidth = size.width;
        m_iHeight = size.height;
    }

    // 6. COM 해제
    pConverter->Release();
    pFrame->Release();
    pDecoder->Release();
    pWIC->Release();
}

void CMyPng::Release()
{
    if (m_pBitmap) { m_pBitmap->Release(); m_pBitmap = nullptr; }
}