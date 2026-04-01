#include "pch.h"
#include "Collision_Manager.h"

CCollision_Manager* CCollision_Manager::m_pInstance = nullptr;

void CCollision_Manager::Collision_Rect(std::list<CGameObject*> _Dst,
    std::list<CGameObject*> _Src)
{
    RECT rc{};
    for (auto& Dst : _Dst)
    {
        for (auto& Src : _Src)
        {
            if (IntersectRect(&rc, Dst->Get_Rect(), Src->Get_Rect()))
            {
                // 충돌 처리 (추후 구현)
            }
        }
    }
}

void CCollision_Manager::Collision_RectEx(std::list<CGameObject*> _Dst,
    std::list<CGameObject*> _Src)
{
    for (auto& pObj : _Dst) pObj->Set_Colliding(false);
    for (auto& pObj : _Src) pObj->Set_Colliding(false);

    float fX = 0.f, fY = 0.f;
    for (auto& Dst : _Dst)
    {
        for (auto& Src : _Src)
        {
            if (Check_Rect(Dst, Src, &fX, &fY))
            {
                Dst->Set_Colliding(true);
                Src->Set_Colliding(true);
                Dst->On_Collision(Src);
                Src->On_Collision(Dst);
            }
            else
            {
                if (Dst->Is_Colliding()) Dst->On_CollisionEnd();
                if (Src->Is_Colliding()) Src->On_CollisionEnd();
            }
        }
    }
}

bool CCollision_Manager::Check_Rect(CGameObject* pDst, CGameObject* pSrc,
    float* _pX, float* _pY)
{
    // 충돌 중심 논리좌표
    float fDstX = pDst->Get_ColliderX();
    float fDstZ = pDst->Get_ColliderZ();
    float fSrcX = pSrc->Get_ColliderX();
    float fSrcZ = pSrc->Get_ColliderZ();

    float fWidth = abs(fDstX - fSrcX);
    float fHeight = abs(fDstZ - fSrcZ);
    float fRadiusX = pDst->Get_Collider().fRadiusX + pSrc->Get_Collider().fRadiusX;
    float fRadiusZ = pDst->Get_Collider().fRadiusZ + pSrc->Get_Collider().fRadiusZ;

    if ((fRadiusX >= fWidth) && (fRadiusZ >= fHeight))
    {
        *_pX = fRadiusX - fWidth;
        *_pY = fRadiusZ - fHeight;
        return true;
    }
    return false;
}

void CCollision_Manager::Collision_Sphere(std::list<CGameObject*> _Dst,
    std::list<CGameObject*> _Src)
{
    for (auto& Dst : _Dst)
    {
        for (auto& Src : _Src)
        {
            if (Check_Sphere(Dst, Src))
            {
                // 충돌 처리 (추후 구현)
            }
        }
    }
}

bool CCollision_Manager::Check_Sphere(CGameObject* pDst, CGameObject* pSrc)
{
    float fDX = pDst->Get_ColliderX() - pSrc->Get_ColliderX();
    float fDZ = pDst->Get_ColliderZ() - pSrc->Get_ColliderZ();
    float fDist = sqrtf(fDX * fDX + fDZ * fDZ);
    float fRadius = pDst->Get_Collider().fRadiusX + pSrc->Get_Collider().fRadiusX;

    return fRadius >= fDist;
}