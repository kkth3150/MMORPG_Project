#pragma once
#include "define.h"
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

class CMyPng
{
public:
    CMyPng();
    virtual ~CMyPng();

public:
    void Load_Png(const TCHAR* pFilePath, ID2D1RenderTarget* pRT);
    void Release();

    ID2D1Bitmap* Get_Bitmap() { return m_pBitmap; }
    int Get_Width() { return m_iWidth; }
    int Get_Height() { return m_iHeight; }

private:
    //HDC     m_hMemDC = nullptr;
    //HBITMAP m_hBitmap = nullptr;
    //HBITMAP m_hOldBmp = nullptr;

    ID2D1Bitmap* m_pBitmap = nullptr;

    int m_iWidth = 0;
    int m_iHeight = 0;

};