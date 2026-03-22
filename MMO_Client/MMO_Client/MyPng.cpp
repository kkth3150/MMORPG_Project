#include "pch.h"
#include "MyPng.h"

CMyPng::CMyPng() {}
CMyPng::~CMyPng() { Release(); }

void CMyPng::Load_Png(const TCHAR* pFilePath)
{
    // 1. GDI+로 PNG 로드
    Gdiplus::Bitmap gdiBitmap(pFilePath);
    m_iWidth = gdiBitmap.GetWidth();
    m_iHeight = gdiBitmap.GetHeight();

    // 2. 32bit DIB 섹션 직접 생성 (알파채널 보존)
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = m_iWidth;
    bmi.bmiHeader.biHeight = -m_iHeight; // 음수 = 상단부터 저장
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* pPixels = nullptr;
    HDC hDC = GetDC(g_hWnd);
    m_hBitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, &pPixels, NULL, 0);
    ReleaseDC(g_hWnd, hDC);

    // 3. GDI+ Bitmap → DIBSection 픽셀 복사 (알파값 포함)
    Gdiplus::BitmapData bmpData;
    Gdiplus::Rect rect(0, 0, m_iWidth, m_iHeight);
    gdiBitmap.LockBits(&rect, Gdiplus::ImageLockModeRead,
        PixelFormat32bppARGB, &bmpData);

    memcpy(pPixels, bmpData.Scan0, m_iWidth * m_iHeight * 4);
    gdiBitmap.UnlockBits(&bmpData);

    // 4. MemDC에 선택 (기존 CMyBmp와 동일한 구조)
    hDC = GetDC(g_hWnd);
    m_hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(g_hWnd, hDC);
    m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}

void CMyPng::Release()
{
    SelectObject(m_hMemDC, m_hOldBmp);
    DeleteObject(m_hBitmap);
    DeleteDC(m_hMemDC);
}