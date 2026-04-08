#pragma once
#include "define.h"

class CUI
{
public:
	CUI();
	virtual ~CUI();

public:
	virtual void Initialize()							PURE;
	virtual int	 Update(float dt)						PURE;
	virtual void Late_Update(float dt)					PURE;
	virtual void Render(ID2D1RenderTarget* pRT)			PURE;
	virtual void Release(void)							PURE;

public:
	virtual void Process_Event()						PURE;

	void Set_Pos(float _fX, float _fY)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
	}
	void Set_Size(float fCX, float fCY) { m_tInfo.fCX = fCX, m_tInfo.fCY = fCY; };
	void Set_FrameKey(const TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }

protected:
	void		Update_Rect();

protected:
	INFO			m_tInfo;
	RECT			m_tRect;
	const TCHAR*	m_pFrameKey;
	
	bool			m_bClick;
	bool			m_bMouseOn;

};

