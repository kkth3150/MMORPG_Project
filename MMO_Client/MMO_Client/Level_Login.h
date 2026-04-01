#pragma once
#include "Level.h"

class CLevel_Login : public CLevel
{
public:
	CLevel_Login();
	virtual ~CLevel_Login();

public:

	virtual void Initialize()							override;
	virtual int Update(float dt)						override;
	virtual void Late_Update(float dt)					override;
	virtual void Render(ID2D1RenderTarget* pRT)			override;
	virtual void Release(void)							override;


private:

};
