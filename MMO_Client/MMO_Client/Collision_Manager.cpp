#include "pch.h"
#include "Collision_Manager.h"

CCollision_Manager::CCollision_Manager()
{
}

CCollision_Manager::~CCollision_Manager()
{
}

void CCollision_Manager::Collision_Rect(std::list<CGameObject*> _Dst, std::list<CGameObject*> _Src)
{
	RECT	rc{};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, Dst->Get_Rect(), Src->Get_Rect()))
			{

			}
		}
	}
}

void CCollision_Manager::Collision_RectEx(std::list<CGameObject*> _Dst, std::list<CGameObject*> _Src)
{
	float	fX = 0.f, fY = 0.f;

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Rect(Dst, Src, &fX, &fY))
			{
				if (fX > fY)	// 상하 충돌
				{
					// 상 충돌
					if (Dst->Get_Info().fY < Src->Get_Info().fY)
					{
					}
					else // 하 충돌
					{
					}
				}

				else		// 좌우 충돌
				{
					// 좌 충돌
					if (Dst->Get_Info().fX < Src->Get_Info().fX)
					{

					}
					else // 우 충돌
					{

					}
				}

			}
		}
	}
}

bool CCollision_Manager::Check_Rect(CGameObject* pDst, CGameObject* pSrc, float* _pX, float* _pY)
{
	float	fWidth = abs(pDst->Get_Info().fX - pSrc->Get_Info().fX);
	float	fHeight = abs(pDst->Get_Info().fY - pSrc->Get_Info().fY);

	float	fRadiusX = (pDst->Get_Info().fCX + pSrc->Get_Info().fCX) * 0.5f;
	float	fRadiusY = (pDst->Get_Info().fCY + pSrc->Get_Info().fCY) * 0.5f;

	if ((fRadiusX >= fWidth) && (fRadiusY >= fHeight))
	{
		*_pX = fRadiusX - fWidth;
		*_pY = fRadiusY - fHeight;

		return true;
	}

	return false;
}

void CCollision_Manager::Collision_Sphere(std::list<CGameObject*> _Dst, std::list<CGameObject*> _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dst, Src))
			{

			}
		}
	}
}

bool CCollision_Manager::Check_Sphere(CGameObject* pDst, CGameObject* pSrc)
{
	float	fWidth = abs(pDst->Get_Info().fX - pSrc->Get_Info().fX);
	float	fHeight = abs(pDst->Get_Info().fY - pSrc->Get_Info().fY);

	float	fDiagonal = sqrt(fWidth * fWidth + fHeight * fHeight);

	float	fRadius = (pDst->Get_Info().fCX + pSrc->Get_Info().fCX) * 0.5f;

	return fRadius >= fDiagonal;
}
