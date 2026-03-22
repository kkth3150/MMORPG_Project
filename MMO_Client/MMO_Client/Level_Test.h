#pragma once
#include "Level.h"

class CLevel_Test : public CLevel
{
public:
	CLevel_Test();
	virtual ~CLevel_Test();

public:

	virtual void Initialize()				override;
	virtual int Update(float dt)			override;
	virtual void Late_Update(float dt)		override;
	virtual void Render(HDC hDC)			override;
	virtual void Release(void)				override;

private:

	void Ready_Player();

};

