#include "pch.h"
#include "Img_Manager.h"
#include "MyBmp.h"
#include "MyPng.h"


CImg_Manager* CImg_Manager::m_pInstance = nullptr;

CImg_Manager::CImg_Manager()
{
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusInput, NULL);
}

CImg_Manager::~CImg_Manager()
{
	Release();
}

void CImg_Manager::Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	auto	iter = std::find_if(m_mapBit.begin(), m_mapBit.end(), CTagFinder(pImgKey));

	if (iter == m_mapBit.end())
	{
		CMyBmp* pBmp = new CMyBmp;
		pBmp->Load_Bmp(pFilePath);

		m_mapBit.insert({ pImgKey, pBmp });
	}
}

HDC CImg_Manager::Find_Bmp(const TCHAR* pImgKey)
{
	auto	iter = std::find_if(m_mapBit.begin(), m_mapBit.end(), CTagFinder(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second->Get_MemDC();
}


void CImg_Manager::Insert_Png(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	auto iter = std::find_if(m_mapPng.begin(), m_mapPng.end(), CTagFinder(pImgKey));
	if (iter == m_mapPng.end())
	{
		CMyPng* pPng = new CMyPng;
		pPng->Load_Png(pFilePath);
		m_mapPng.insert({ pImgKey, pPng });
	}
}

HDC CImg_Manager::Find_Png(const TCHAR* pImgKey)
{
	auto iter = std::find_if(m_mapPng.begin(), m_mapPng.end(), CTagFinder(pImgKey));
	if (iter == m_mapPng.end())
		return nullptr;
	return iter->second->GetMemDC();
}

void CImg_Manager::Release(void)
{
	std::for_each(m_mapBit.begin(), m_mapBit.end(), CDeleteMap());
	m_mapBit.clear();

	std::for_each(m_mapPng.begin(), m_mapPng.end(), CDeleteMap());
	m_mapPng.clear();

}
