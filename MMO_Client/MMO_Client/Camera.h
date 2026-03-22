#pragma once
#include "Define.h"

class CGameObject;

class CCamera
{
public:
    static CCamera* Get_Instance()
    {
        if (!m_pInstance)
            m_pInstance = new CCamera;
        return m_pInstance;
    }
    static void Destroy_Instance()
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }

public:
    void    Update(float dt);

    void    ScreenToIsoWorld(int iScreenX, int iScreenY,
        float& fOutWorldX, float& fOutWorldZ);
    POINT   IsoWorldToScreen(float fWorldX, float fWorldZ);
    POINT   TileToScreen(int iTileX, int iTileZ);
    bool    Is_InViewport(float fWorldX, float fWorldZ, float fCX, float fCY);

    void    Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }
    float   Get_X() { return m_fX; }
    float   Get_Y() { return m_fY; }

private:
    CCamera() {}
    ~CCamera() {}

private:
    static CCamera* m_pInstance;
    float            m_fX = 0.f;
    float            m_fY = 0.f;
    CGameObject* m_pTarget = nullptr;
};