#pragma once
#include "GameObject.h"
#define PLAYER_FOOT_OFFSET_Y  0.5f

enum PLAYER_STATE {
	PLAYER_IDLE, PLAYER_WALK,PLAYER_HIT, PLAYER_ATTACK,PLAYER_DEAD,PLAYER_END
};

struct CLICK_EFFECT
{
	float fWorldX;
	float fWorldZ;
	float fScale;
	bool  bActive;
	COLORREF color;  // 眠啊
};

class CPlayer : public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	virtual void Initialize() override;
	virtual int	 Update(float dt) override;
	virtual void Late_Update(float dt) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

private:

	void Render_Sprite(HDC hDC, HDC PngDC);

	void RenderIDLE(HDC hDC);
	void RenderWALK(HDC hDC);
	void RenderHIT(HDC hDC);
	void RenderATTACK(HDC hDC);
	void RenderDEAD(HDC hDC);

	void Motion_Change(PLAYER_STATE eState);
	void Direction_Change(DIRECTION eDir);

	void Key_Input(float dt);


	void Move_To_Dest(float dt);
	void Decide_Direction(float fNX, float fNZ);

	void Update_ClickEffect(float dt);
	void Render_ClickEffect(HDC hDC);

private:
	PLAYER_STATE m_eCurState;
	CLICK_EFFECT m_tClickEffect = {};

	float           m_fDestWorldX = 0.f;   // 格利瘤 稠府 X
	float           m_fDestWorldZ = 0.f;   // 格利瘤 稠府 Z
	bool            m_bMoving = false;

private:
#ifdef GAME_DEBUG


	void Debug_Render(HDC hDC);
	void Debug_DrawClickedTile(HDC hDC);
	void Debug_DrawClickPoint(HDC hDC);
	void Debug_DrawCollider(HDC hDC);
	void Debug_DrawText(HDC hDC);




	int   m_iDebugTileX = 0;
	int   m_iDebugTileZ = 0;
	float m_fDebugLocalX = 0.f;
	float m_fDebugLocalZ = 0.f;



#endif
};

