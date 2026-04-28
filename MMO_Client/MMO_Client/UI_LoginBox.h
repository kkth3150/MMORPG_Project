#pragma once
#include "UI.h"

class CUI_LoginBox : public CUI
{

public:
	CUI_LoginBox();
	virtual ~CUI_LoginBox();

public:
	virtual void Initialize() override;
	virtual int Update(float dt) override;
	virtual void Late_Update(float dt) override;
	virtual void Render(ID2D1RenderTarget* pRT) override;
	virtual void Release(void) override;

	virtual void Process_Event() override;
	void On_Char(wchar_t ch);   // WndProc WM_CHAR에서 호출

private:
	bool		m_bCheckin = false;

	static constexpr float PANEL_W = 400.f;
	static constexpr float PANEL_H = 265.f;

	const std::wstring& Get_ID() const { return m_strID; }
	const std::wstring& Get_PW() const { return m_strPW; }

    static constexpr float FIELD_X = 60.f;
    static constexpr float FIELD_W = 280.f;
    static constexpr float FIELD_H = 34.f;
    static constexpr float ID_Y = 50.f;   // 이미지 내 ID칸 상단 Y
    static constexpr float PW_Y = 110.f;  // 이미지 내 PW칸 상단 Y
    static constexpr float CARET_BLINK = 0.5f;

    int             m_iFocus = -1;     // 0=ID, 1=PW
    std::wstring    m_strID;
    std::wstring    m_strPW;
    float           m_fCaretTimer = 0.f;
    bool            m_bCaretVis = true;

    // D2D 브러시 / DWrite (지연 생성)
    ID2D1SolidColorBrush* m_pBrushText = nullptr;
    ID2D1SolidColorBrush* m_pBrushFocus = nullptr;
    ID2D1SolidColorBrush* m_pBrushNormal = nullptr;
    ID2D1SolidColorBrush* m_pBrushHint = nullptr;
    IDWriteFactory* m_pDWFactory = nullptr;
    IDWriteTextFormat* m_pFmtInput = nullptr;
};