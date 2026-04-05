#pragma once
#include "ActiveObject.h"

class CPortal : public CActiveObject
{
public:
    CPortal() = default;
    virtual ~CPortal() = default;

    virtual void    Initialize()                            override;
    virtual int     Update(float dt)                        override;
    virtual void    Late_Update(float dt)                   override;
    virtual void    Render(ID2D1RenderTarget* pRT)          override;
    virtual void    Release()                               override;
public:
    virtual void    On_Click()      override;
    virtual void    On_Interact()   override;

    // 선택적 오버라이드
    virtual void    On_PlayerNear() override;
    virtual void    On_PlayerFar()  override;

    void    Set_TargetZone(ZONE_ID eZone, float fSpawnX, float fSpawnZ)
    {
        m_eTargetZone = eZone;
        m_fSpawnX = fSpawnX;
        m_fSpawnZ = fSpawnZ;
    }


protected:
    virtual void    Render_Indicator(ID2D1RenderTarget* pRT) override;

private:
    void    Render_Gate(ID2D1RenderTarget* pRT, POINT tScreen);
    void    Render_Frame(ID2D1RenderTarget* pRT, POINT tScreen);

#ifdef GAME_DEBUG
    void    Debug_Render(ID2D1RenderTarget* pRT);
    void    Debug_DrawCollider(ID2D1RenderTarget* pRT);
    void    Debug_DrawMouseCollider(ID2D1RenderTarget* pRT);
#endif

private:
    // 테두리 껍데기 상수 (변수 필요 없음)
    static constexpr float BORDER_CX = 155.f;
    static constexpr float BORDER_CY = 233.f;

    // 존 전환
    ZONE_ID m_eTargetZone = ZONE_MAX;
    float   m_fSpawnX = 0.f;
    float   m_fSpawnZ = 0.f;
    bool    m_bActivated = false;
    float   m_fActivateDelay = 0.5f;
};