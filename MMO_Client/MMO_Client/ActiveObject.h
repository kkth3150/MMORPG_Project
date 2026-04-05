#pragma once
#include "GameObject.h"

class CActiveObject : public CGameObject
{
public:
    CActiveObject() = default;
    virtual ~CActiveObject() = default;

public:
    virtual void Initialize()                       override;
    virtual int  Update(float dt)                   override;
    virtual void Late_Update(float dt)              override;
    virtual void Render(ID2D1RenderTarget* pRT)     override;
    virtual void Release()                          override;

public:
    // 순수 가상 - 최하위 클래스에서 반드시 구현
    virtual void On_Click()     PURE;
    virtual void On_Interact()  PURE;

    // 선택적 오버라이드
    virtual void On_PlayerNear() {}
    virtual void On_PlayerFar() {}

    virtual void On_Collision(CGameObject* pOther)  override;
    virtual void On_CollisionEnd()                  override;

protected:
    virtual void Render_Indicator(ID2D1RenderTarget* pRT) {}

protected:
    bool    m_bInteractable = false;
    bool    m_bClick = false;
};