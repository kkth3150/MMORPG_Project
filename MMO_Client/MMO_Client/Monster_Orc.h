#pragma once
#include "Monster.h"

class CMonster_Orc : public CMonster
{
public:
    CMonster_Orc() = default;
    virtual ~CMonster_Orc() = default;

    virtual void    Initialize()                        override;
    virtual int     Update(float dt)                    override;
    virtual void    Late_Update(float dt)               override;
    virtual void    Render(ID2D1RenderTarget* pRT)      override;
    virtual void    Release()                           override;

    virtual void    Motion_Change(MONSTER_STATE eState) override;

private:
    void Move_To_Dest(float dt);
    void Decide_Direction(float fNX, float fNZ);
    void Update_MouseCollider();
    void Direction_Change(DIRECTION eDir);



    float m_fDestWorldX = 0.f;
    float m_fDestWorldZ = 0.f;
    bool  m_bMoving = false;

#ifdef GAME_DEBUG
    void            Debug_Render(ID2D1RenderTarget* pRT);
    void            Debug_DrawCollider(ID2D1RenderTarget* pRT);
    void            Debug_DrawMouseCollider(ID2D1RenderTarget* pRT);
    void            Debug_DrawText(ID2D1RenderTarget* pRT);
#endif
};

