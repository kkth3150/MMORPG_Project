#include "pch.h"
#include "UI_Manager.h"
#include "Img_Manager.h"
#include "UI_LoginBox.h"

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
	for (size_t i = 0; i < UI_END; ++i)
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
	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto& iter : m_UIList[i])
		{
			iter->Late_Update(dt);

			if (m_UIList[i].empty())
				break;

		}
	}

}

void CUI_Manager::Render(ID2D1RenderTarget* pRT)
{
	for (size_t i = 0; i < UI_END; ++i)
	{
		for (auto& iter : m_UIList[i])
		{
			iter->Render(pRT);
		}
	}
}

void CUI_Manager::On_Char(wchar_t ch)
{
	for (auto* pUI : m_UIList[UI_BOX])
	{
		auto* pLoginBox = dynamic_cast<CUI_LoginBox*>(pUI);
		if (pLoginBox)
		{
			pLoginBox->On_Char(ch);
			break;
		}
	}
}

void CUI_Manager::Release(void)
{

	for (size_t i = 0; i < UI_END; ++i)
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
