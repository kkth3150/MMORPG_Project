#pragma once
#include "GameObject.h"

class CCollision_Manager
{
public:
	CCollision_Manager();
	~CCollision_Manager();

public:
	static void Collision_Rect(std::list<CGameObject*> _Dst, std::list<CGameObject*> _Src);
	static void Collision_RectEx(std::list<CGameObject*> _Dst, std::list<CGameObject*> _Src);
	static bool Check_Rect(CGameObject* pDst, CGameObject* pSrc, float* _pX, float* _pY);

	static void Collision_Sphere(std::list<CGameObject*> _Dst, std::list<CGameObject*> _Src);
	static bool Check_Sphere(CGameObject* pDst, CGameObject* pSrc);

};

