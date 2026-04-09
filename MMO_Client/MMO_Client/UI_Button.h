#pragma once
#include "UI.h"

class CUI_Button  : public CUI
{

public:
	CUI_Button();
	virtual ~CUI_Button();

public:
	virtual void Initialize() override;
	virtual int Update(float dt) override;
	virtual void Late_Update(float dt) override;
	virtual void Render(ID2D1RenderTarget* pRT) override;
	virtual void Release(void) override;

	virtual void Process_Event() override;

public:
	bool GetButtonDown() { return m_bClick; };
	void SetButtonID(ButtonID eID) {

		m_eButtonID = eID;
	}
private:
	bool		m_bEffect;
	int			m_iDrawID;
	ButtonID	m_eButtonID;

};

