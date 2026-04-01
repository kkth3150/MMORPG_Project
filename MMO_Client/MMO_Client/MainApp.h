#pragma once
#include "define.h"

class CMainApp
{
public:
	CMainApp();
	~CMainApp();

public:
	void Initialize(void);
	void Update(float dt);
	void Late_Update(float dt);
	void Render(void);
	void Release(void);

private:

	ID2D1Factory* m_pFactory = nullptr;
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;
	IDWriteFactory* m_pDWriteFactory = nullptr;
	IDWriteTextFormat* m_pTextFormat = nullptr; // FPS 텍스트용

	//HDC				m_hDC;

	TCHAR			m_szFPS[32];

	float m_fTimeAcc = 0.f;
	int   m_iFrameCount = 0;
};

