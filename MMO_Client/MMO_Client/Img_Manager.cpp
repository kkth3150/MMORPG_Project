#include "pch.h"
#include "Img_Manager.h"
#include "MyPng.h"


CImg_Manager* CImg_Manager::m_pInstance = nullptr;

CImg_Manager::CImg_Manager()
{
}

CImg_Manager::~CImg_Manager()
{
	Release();
}

void CImg_Manager::Insert_Png(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	auto iter = std::find_if(m_mapPng.begin(), m_mapPng.end(), CTagFinder(pImgKey));
	if (iter == m_mapPng.end())
	{
		CMyPng* pPng = new CMyPng;
		pPng->Load_Png(pFilePath, m_pRT);  //rtÀü´Þ
		m_mapPng.insert({ pImgKey, pPng });
	}
}

ID2D1Bitmap* CImg_Manager::Find_Png(const TCHAR* pImgKey)
{
	auto iter = std::find_if(m_mapPng.begin(), m_mapPng.end(), CTagFinder(pImgKey));
	if (iter == m_mapPng.end())
		return nullptr;
	return iter->second->Get_Bitmap();
}
void CImg_Manager::Release(void)
{
	std::for_each(m_mapPng.begin(), m_mapPng.end(), CDeleteMap());
	m_mapPng.clear();

}
