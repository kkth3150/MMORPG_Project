#pragma once
#include "Npc.h"

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

    //NPC 순수가상 구현
    virtual void On_Click()    override;
    virtual void On_Interact() override;
};