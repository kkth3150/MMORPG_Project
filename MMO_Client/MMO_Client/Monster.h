#pragma once
#include "GameObject.h"
enum MONSTER_STATE : int
{
    MON_IDLE,
    MON_WALK,
    MON_ATTACK_0,
    MON_ATTACK_1,
    MON_HIT,
    MON_DEAD,
    MON_END
};

class CMonster : public CGameObject
{
public:
    CMonster() = default;
    virtual ~CMonster() = default;
public:
    virtual void    Initialize()                        override;
    virtual int     Update(float dt)                    override;
    virtual void    Late_Update(float dt)               override;
    virtual void    Render(ID2D1RenderTarget* pRT)      override;
    virtual void    Release()                           override;
public:
    // 순수 가상 - 각 몬스터가 구현
    virtual void    Motion_Change(MONSTER_STATE eState) PURE;
    // 서버에서 받은 정보 적용
    void    Set_ServerPos(float fX, float fZ)
    {
#ifdef NO_SERVER
        m_tIsoInfo.fWorldX = fX;
        m_tIsoInfo.fWorldZ = fZ;
#else
        // 서버 모드 : Lerp 목표만 저장
        m_fServerX = fX;
        m_fServerZ = fZ;
#endif
    }
    void    Set_MonsterState(MONSTER_STATE eState)
    {
        if (m_eState == eState) return;
        Motion_Change(eState);
    }
    void    Set_MonsterName(const TCHAR* pName) { lstrcpy(m_szName, pName); }
    // HP는 CGameObject 것 그대로 사용
    // Get_HP() / Set_Hp() / m_iHp / m_iMaxHp → 이미 있음
    MONSTER_STATE   Get_MonsterState() const { return m_eState; }
protected:
    void    Render_Sprite(ID2D1RenderTarget* pRT, ID2D1Bitmap* pBitmap);
    void    Render_HpBar(ID2D1RenderTarget* pRT);
    void    Render_NameTag(ID2D1RenderTarget* pRT);
    void    Update_Lerp(float dt);
#ifdef GAME_DEBUG
    void Debug_Render(ID2D1RenderTarget* pRT);
    void Debug_DrawCollider(ID2D1RenderTarget* pRT);
    void Debug_DrawMouseCollider(ID2D1RenderTarget* pRT);
    void Debug_DrawText(ID2D1RenderTarget* pRT);
#endif
protected:
    MONSTER_STATE   m_eState = MON_IDLE;
    TCHAR           m_szName[64] = {};
    // 서버 목표 위치 (보간용)
    float           m_fServerX = 0.f;
    float           m_fServerZ = 0.f;
    float           m_fLerpSpeed = 10.f;
};