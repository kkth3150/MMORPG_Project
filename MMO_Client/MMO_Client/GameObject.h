#pragma once
#include "Define.h"

class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

public:
	virtual void Initialize()PURE;
	virtual int	 Update(float dt)PURE;
	virtual void Late_Update(float dt)PURE;
	virtual void Render(HDC hDC)PURE;
	virtual void Release(void)PURE;

public:
	void Set_WorldPos(float fWorldX, float fWorldZ)
	{
		m_tIsoInfo.fWorldX = fWorldX;
		m_tIsoInfo.fWorldZ = fWorldZ;
	}
	void		Set_Direction(DIRECTION eDir) { m_eDir = eDir; }
	void		Set_Dead() { m_bDead = true; }
	void		Set_DeadMotion() { m_bDeadMotion = true; }
	void		Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void Set_Size(float fCX, float fCY)
	{
		m_tIsoInfo.fCX = fCX;
		m_tIsoInfo.fCY = fCY;
	};

	void		Set_Target(CGameObject* _pTarget) { m_pTarget = _pTarget; }
	void		Set_FrameKey(const TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }
	void		Set_Frame(int iFrameEnd, DWORD FrameSpeed) {

		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = iFrameEnd;
		m_tFrame.dwSpeed = FrameSpeed;
		m_tFrame.dwTime = GetTickCount64();
	};
	const RECT* Get_Rect() { return &m_tRect; }
	ISO_INFO    Get_IsoInfo() { return m_tIsoInfo; }
	bool		Get_Dead() { return m_bDead; }
	int			Get_HP() { return m_iHp; };
	void		Set_Hp(int iAtk) { m_iHp -= iAtk; };
	int			Get_Atk() { return m_iAttack; }

	void Set_Collider(float fRadiusX, float fRadiusZ,
		float fOffsetX = 0.f, float fOffsetZ = 0.f)
	{
		m_tCollider.fRadiusX = fRadiusX;
		m_tCollider.fRadiusZ = fRadiusZ;
		m_tCollider.fOffsetX = fOffsetX;
		m_tCollider.fOffsetZ = fOffsetZ;
	}
	COLLIDER Get_Collider() { return m_tCollider; }

	// 충돌 중심 논리좌표
	float Get_ColliderX() { return m_tIsoInfo.fWorldX + m_tCollider.fOffsetX; }
	float Get_ColliderZ() { return m_tIsoInfo.fWorldZ + m_tCollider.fOffsetZ; }


protected:
	void		Update_Rect();
	void		Move_Frame();

protected:
	CGameObject* m_pTarget;
	ISO_INFO	m_tIsoInfo;
	COLLIDER    m_tCollider;
	RECT		m_tRect;
	DIRECTION	m_eDir;
	FRAME		m_tFrame;

	bool		m_bDead;

	float		m_fSpeed;
	float		m_fAngle;
	int			m_iHp;
	int			m_iAttack;
	float		m_fSpeed_Y;

	bool		m_bDeadMotion;

	const TCHAR* m_pFrameKey;

};

