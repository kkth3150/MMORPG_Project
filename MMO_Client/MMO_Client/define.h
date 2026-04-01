#pragma once

#define WINCX 1280
#define WINCY 720

#define OBJ_DEAD 1
#define OBJ_NOEVENT 0

#define UI_EVENT 1
#define UI_NOEVENT 0

#define VK_MAX	0xff
#define TILE_WIDTH   160
#define TILE_HEIGHT  80
#define TILE_HALF_W  (TILE_WIDTH  / 2.f)   // 90.f
#define TILE_HALF_H  (TILE_HEIGHT / 2.f)   // 29.5f

#define ISO_RATIO    (TILE_HALF_W / TILE_HALF_H)  // 90/29.5

// 맵 크기
#define MAP_TILE_X      20
#define MAP_TILE_Z      20


enum LEVEL_ID {LEVEL_MENU, LEVEL_LOGIN, LEVEL_CHOICE, LEVEL_TEST,LEVEL_END};
enum OBJ_ID {OBJ_PLAYER,OBJ_NPC,OBJ_END};
enum UI_ID {UI_BUTTON, UI_END};
enum DIRECTION { DIR_B, DIR_LB, DIR_L, DIR_LT, DIR_T, DIR_RT, DIR_R, DIR_RB, DIR_END };
enum MOUSE_BUTTON {MBUTTON_L,MBUTTON_R,MBUTTON_WHEEL,MBUTTON_END};
enum TILE_TYPE
{
	TILE_NORMAL, // 이동 가능
	TILE_BLOCK,       // 이동 불가
	TILE_TYPE_END
};
enum ButtonID { BUTTON_LOGIN, BUTTON_EXIT, BUTTON_END };
enum CURSOR_MODE
{
	CURSOR_NORMAL,
	CURSOR_ATTACK,      // 공격 가능 커서 (몬스터 위)
	CURSOR_TALK,        // 대화 커서 (NPC 위)
	CURSOR_END
};

extern HWND g_hWnd;

typedef struct tagFrame
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iMotion;
	DWORD	dwSpeed;
	DWORD	dwTime;

}FRAME;


typedef struct tagInfo
{
	float	fX;		// 중점 X
	float	fY;		// 중점 Y
	float	fCX;	// 가로 사이즈
	float	fCY;	// 세로 사이즈
	float   fHeight;

}INFO;

typedef struct tagIosInfo
{
	// 아이소메트릭 논리 월드좌표 (이동/충돌/서버 동기화용)
	float fWorldX;  // 논리 X
	float fWorldZ;  // 논리 Z (앞뒤)
	float fHeight;  // 높이 (점프 등)

	// 스프라이트 크기
	float fCX;
	float fCY;
} ISO_INFO;


typedef struct tagCollider
{
	float fOffsetX;  // 중심에서 오프셋 X
	float fOffsetZ;  // 중심에서 오프셋 Z
	float fRadiusX;  // 충돌 반경 X (논리좌표)
	float fRadiusZ;  // 충돌 반경 Z (논리좌표)
} COLLIDER;

typedef struct tagMouseCollider
{
	float fOffsetX;  // 스프라이트 중심에서 X 오프셋
	float fOffsetY;  // 스프라이트 중심에서 Y 오프셋
	float fWidth;    // 클릭 박스 너비
	float fHeight;   // 클릭 박스 높이
} MOUSE_COLLIDER;

typedef struct tagTile
{
	int         iTileX;    // 타일 논리 X
	int         iTileZ;    // 타일 논리 Z
	TILE_TYPE   eType;     // 이동가능 여부 (길찾기용)
} TILE;

template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}


class CDeleteObj
{
public:
	template<typename T>
	void operator()(T& Temp)
	{
		if (Temp)
		{
			delete Temp;
			Temp = nullptr;
		}
	}
};

class CDeleteMap
{
public:
	template<typename T>
	void operator()(T& MyPair)
	{
		if (MyPair.second)
		{
			delete MyPair.second;
			MyPair.second = nullptr;
		}
	}
};


class CTagFinder
{
public:
	CTagFinder(const TCHAR* pKey) : m_pKey(pKey) {}

public:
	template<typename T>
	bool	operator()(T& Pair)
	{
		if (!lstrcmp(m_pKey, Pair.first))
			return true;

		return false;
	}

private:
	const TCHAR* m_pKey;
};