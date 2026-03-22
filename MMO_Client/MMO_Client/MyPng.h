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
    void Load_Png(const TCHAR* pFilePath);
    void Release();

public:
    HDC     GetMemDC() { return m_hMemDC; }
    int     GetWidth() { return m_iWidth; }
    int     GetHeight() { return m_iHeight; }

private:
    HDC     m_hMemDC = nullptr;
    HBITMAP m_hBitmap = nullptr;
    HBITMAP m_hOldBmp = nullptr;
    int     m_iWidth = 0;
    int     m_iHeight = 0;
};