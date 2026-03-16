#pragma once
#include "Level.h"

class CLevel_Choice : public CLevel
{
public:
	CLevel_Choice();
	virtual ~CLevel_Choice();

public:

	virtual void Initialize()				override;
	virtual int Update(float dt)			override;
	virtual void Late_Update(float dt)		override;
	virtual void Render(HDC hDC)			override;
	virtual void Release(void)				override;


private:

};

