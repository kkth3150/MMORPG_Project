#pragma once
#include "define.h"

class CLevel
{
public:
	CLevel();
	virtual ~CLevel();

public:
	virtual void Initialize()							PURE;
	virtual int	 Update(float dt)						PURE;
	virtual void Late_Update(float dt)					PURE;
	virtual void Render(ID2D1RenderTarget* pRT)			PURE;
	virtual void Release(void)							PURE;
};
