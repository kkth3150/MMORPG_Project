#pragma once
#include "GameObject.h"

enum NPC_TYPE
{
    NPC_SHOP,
    NPC_QUEST,
    NPC_GUARD,
    NPC_END
};

enum NPC_STATE
{
    NPC_IDLE,
    NPC_TALK,
    NPC_WALK,
    NPC_END_STATE
};

class CNPC : public CGameObject
{
public:
    CNPC();
    virtual ~CNPC();

public:
    virtual void Initialize()               override;
    virtual int  Update(float dt)           override;
    virtual void Late_Update(float dt)      override;
    virtual void Render(ID2D1RenderTarget* pRT) override;
    virtual void Release(void)              override;

public:
    virtual void On_Click()     PURE;   // 클릭 이벤트 (각 NPC마다 다름)
    virtual void On_Interact()  PURE;   // 상호작용 (대화/상점/퀘스트)

    //하위 NPC가 선택적으로 오버라이드하는 것
    virtual void On_PlayerNear() {}     // 플레이어 접근 시
    virtual void On_PlayerFar() {}     // 플레이어 멀어질 시


    virtual void On_Collision(CGameObject* pOther) override;
    virtual void On_CollisionEnd() override;

public:
    // 공통 기능 - 하위에서 그대로 사용
    void         Set_NpcName(const TCHAR* pName) { lstrcpy(m_szName, pName); }
    const TCHAR* Get_NpcName() { return m_szName; }
    void        Set_NpcType(NPC_TYPE eType) { m_eType = eType; }
    NPC_TYPE    Get_NpcType() { return m_eType; }

protected:
    // 공통 렌더 헬퍼 - 하위에서 호출
    void Render_Sprite(ID2D1RenderTarget* pRT, ID2D1Bitmap* pBitmap);
    void Render_NameTag(ID2D1RenderTarget* pRT);    // 이름표
    void Render_Indicator(ID2D1RenderTarget* pRT);  // ! 또는 ? 아이콘

private:
    void Debug_Render(ID2D1RenderTarget* pRT);
    void Debug_DrawCollider(ID2D1RenderTarget* pRT);
    void Debug_DrawMouseCollider(ID2D1RenderTarget* pRT);
protected:
    NPC_TYPE    m_eType = NPC_END;
    NPC_STATE   m_eState = NPC_IDLE;
    TCHAR m_szName[64] = {};


    bool        m_bInteractable = false;  // 상호작용 가능 여부
    bool        m_bClick = false;
    float       m_fInteractRange = 2.f;  // 상호작용 가능 거리

    // 말풍선
    bool        m_bShowBubble = false;
    float       m_fBubbleTimer = 0.f;
    TCHAR m_szBubbleText[256] = {};
};