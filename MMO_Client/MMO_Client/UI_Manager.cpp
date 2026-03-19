#include "pch.h"
#include "UI_Manager.h"
#include "Bmp_Manager.h"

CUI_Manager* CUI_Manager::m_pInstance = nullptr;

CUI_Manager::CUI_Manager()
{

}

CUI_Manager::~CUI_Manager()
{
	Release();
}

void CUI_Manager::Add_UI(UI_ID eID, CUI* pUI)
{
	if (UI_END <= eID || nullptr == pUI)
		return;

	m_UIList[eID].push_back(pUI);
}

int CUI_Manager::Update(float dt)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_UIList[i].begin();
			iter != m_UIList[i].end(); )
		{
			int iResult = (*iter)->Update(dt);

			if (UI_EVENT == iResult)
			{
				(*iter)->Process_Event();
				Safe_Delete<CUI*>(*iter);
				iter = m_UIList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return OBJ_NOEVENT;
}

void CUI_Manager::Late_Update(float dt)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_UIList[i])
		{
			iter->Late_Update(dt);

			if (m_UIList[i].empty())
				break;

		}
	}

}

void CUI_Manager::Render(HDC hDC)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_UIList[i])
		{
			iter->Render(hDC);
		}
	}
}

void CUI_Manager::Release(void)
{

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for_each(m_UIList[i].begin(), m_UIList[i].end(), Safe_Delete<CUI*>);
		m_UIList[i].clear();
	}
}

void CUI_Manager::DeleteID(OBJ_ID eID)
{
	for (auto& iter : m_UIList[eID])
		Safe_Delete(iter);

	m_UIList[eID].clear();
}
