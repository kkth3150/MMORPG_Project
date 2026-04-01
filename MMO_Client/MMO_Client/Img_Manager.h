#pragma once
#include "define.h"


class CMyPng;

class CImg_Manager
{
private:
    CImg_Manager();
    ~CImg_Manager();

public:
    void         Set_RenderTarget(ID2D1RenderTarget* pRT) { m_pRT = pRT; }
    void         Insert_Png(const TCHAR* pFilePath, const TCHAR* pImgKey);
    ID2D1Bitmap* Find_Png(const TCHAR* pImgKey);
    void         Release(void);



public:
    static CImg_Manager* Get_Instance()
    {
        if (!m_pInstance)
            m_pInstance = new CImg_Manager;
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

public:
    IDWriteTextFormat* Get_DebugFont() { return m_pDebugFont; }
    
    void Create_DebugFont(IDWriteFactory* pDW)
    {
        pDW->CreateTextFormat(
            L"Arial", nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            14.f, L"ko-KR",
            &m_pDebugFont
        );
    }


private:
    static CImg_Manager* m_pInstance;
    
    IDWriteTextFormat* m_pDebugFont = nullptr;
    ID2D1RenderTarget* m_pRT = nullptr;
    
    std::map<const TCHAR*, CMyPng*> m_mapPng;
};