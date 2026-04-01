#pragma once
#include "GameObject.h"

#define PLAYER_FOOT_OFFSET_Y 0.5f

enum PLAYER_STATE {
    PLAYER_IDLE, PLAYER_WALK, PLAYER_HIT, PLAYER_ATTACK, PLAYER_DEAD, PLAYER_END
};

struct CLICK_EFFECT
{
    float    fWorldX;
    float    fWorldZ;
    float    fScale;
    bool     bActive;
    D2D1_COLOR_F color;
};

class CPlayer : public CGameObject
{
public:
    CPlayer();
    virtual ~CPlayer();
public:
    virtual void Initialize()        override;
    virtual int  Update(float dt)    override;
    virtual void Late_Update(float dt) override;
    virtual void Render(ID2D1RenderTarget* pRT) override; // HDC ¡æ RT
    virtual void Release(void)       override;

private:
    // ÀüºÎ HDC ¡æ ID2D1RenderTarget*
    void Render_Sprite(ID2D1RenderTarget* pRT, ID2D1Bitmap* pBitmap);
    void RenderIDLE(ID2D1RenderTarget* pRT);
    void RenderWALK(ID2D1RenderTarget* pRT);
    void RenderHIT(ID2D1RenderTarget* pRT);
    void RenderATTACK(ID2D1RenderTarget* pRT);
    void RenderDEAD(ID2D1RenderTarget* pRT);
    void Render_ClickEffect(ID2D1RenderTarget* pRT);

    void Motion_Change(PLAYER_STATE eState);
    void Direction_Change(DIRECTION eDir);
    void Key_Input(float dt);
    void Move_To_Dest(float dt);
    void Decide_Direction(float fNX, float fNZ);
    void Update_ClickEffect(float dt);

private:
    PLAYER_STATE m_eCurState;
    CLICK_EFFECT m_tClickEffect = {};
    float        m_fDestWorldX = 0.f;
    float        m_fDestWorldZ = 0.f;
    bool         m_bMoving = false;

#ifdef GAME_DEBUG
private:
    void Debug_Render(ID2D1RenderTarget* pRT);
    void Debug_DrawClickedTile(ID2D1RenderTarget* pRT);
    void Debug_DrawClickPoint(ID2D1RenderTarget* pRT);
    void Debug_DrawCollider(ID2D1RenderTarget* pRT);
    void Debug_DrawText(ID2D1RenderTarget* pRT);

    int   m_iDebugTileX = 0;
    int   m_iDebugTileZ = 0;
    float m_fDebugLocalX = 0.f;
    float m_fDebugLocalZ = 0.f;
#endif
};