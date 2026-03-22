#pragma once
#include "define.h"
class CMyBmp;
class CMyPng;

class CImg_Manager
{
private:
	CImg_Manager();
	~CImg_Manager();

public:
	void		Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey);
	HDC			Find_Bmp(const TCHAR* pImgKey);

	void		Insert_Png(const TCHAR* pFilePath, const TCHAR* pImgKey);
	HDC			Find_Png(const TCHAR* pImgKey);

	void		Release(void);

public:
	static	CImg_Manager* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CImg_Manager;
		}
		return m_pInstance;
	}

	static void			Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static	CImg_Manager* m_pInstance;

	std::map<const TCHAR*, CMyBmp*>		m_mapBit;
	std::map<const TCHAR*, CMyPng*>     m_mapPng;

};

