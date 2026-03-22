#pragma once
#include "GameObject.h"

class CCollision_Manager
{
public:
    static CCollision_Manager* Get_Instance()
    {
        if (!m_pInstance)
            m_pInstance = new CCollision_Manager;
        return m_pInstance;
    }
    static void Destroy_Instance()
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }

public:
    // 기본 rect 충돌 (IntersectRect 기반)
    void Collision_Rect(std::list<CGameObject*> _Dst, std::list<CGameObject*> _Src);

    // 방향별 충돌 처리
    void Collision_RectEx(std::list<CGameObject*> _Dst, std::list<CGameObject*> _Src);

    // 원형 충돌
    void Collision_Sphere(std::list<CGameObject*> _Dst, std::list<CGameObject*> _Src);

private:
    bool Check_Rect(CGameObject* pDst, CGameObject* pSrc, float* _pX, float* _pY);
    bool Check_Sphere(CGameObject* pDst, CGameObject* pSrc);

private:
    CCollision_Manager() {}
    ~CCollision_Manager() {}

private:
    static CCollision_Manager* m_pInstance;
};