#pragma once
#include "Npc.h"


enum SHOP_TYPE
{
    SHOP_TRADER_0,  // 107x71
    SHOP_TRADER_1,  // 108x68
    SHOP_END
};

enum SHOP_STATE
{
    SHOP_IDLE,
    SHOP_TALK,
    SHOP_STATE_END
};

class CNPC_Shop : public CNPC
{
public:
    CNPC_Shop();
    virtual ~CNPC_Shop();

public:
    virtual void Initialize()                   override;
    virtual int  Update(float dt)               override;
    virtual void Late_Update(float dt)          override;
    virtual void Render(ID2D1RenderTarget* pRT) override;
    virtual void Release()                      override;

    virtual void On_Click()    override;
    virtual void On_Interact() override;

public:
    void Set_ShopType(SHOP_TYPE eType) { m_eShopType = eType; }

private:
    void Motion_Change(SHOP_STATE eState);
    void Load_Sprites();

    // 트레이더별 스프라이트 키 반환
    const TCHAR* Get_IdleKey()  const;
    const TCHAR* Get_TalkKey()  const;

#ifdef GAME_DEBUG
    void Debug_Render(ID2D1RenderTarget* pRT);
    void Debug_DrawCollider(ID2D1RenderTarget* pRT);
    void Debug_DrawMouseCollider(ID2D1RenderTarget* pRT);
    void Debug_DrawText(ID2D1RenderTarget* pRT);
#endif

private:
    SHOP_TYPE   m_eShopType = SHOP_TRADER_0;
    SHOP_STATE  m_eShopState = SHOP_IDLE;
};