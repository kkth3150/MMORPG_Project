#pragma once
#include "UI.h"

class CUI_Manager
{
private:
	CUI_Manager();
	~CUI_Manager();

public:
	void		Add_UI(UI_ID eID, CUI* pUI);
	int			Update(float dt);
	void		Late_Update(float dt);
	void		Render(HDC hDC);
	void		Release(void);

public:

	std::list<CUI*>* Get_List(OBJ_ID eID) {
		if (!List_Empty(eID))
			return &m_UIList[eID];
	}

	bool List_Empty(OBJ_ID eID) {
		if (m_UIList[eID].empty() == true) {
			return true;
		}
		else {
			return false;
		}

	};

	void		DeleteID(OBJ_ID eID);

private:

	std::list<CUI*>		m_UIList[UI_END];

public:
	static	CUI_Manager* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CUI_Manager;
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
	static	CUI_Manager* m_pInstance;


};

