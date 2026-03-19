#pragma once

#define WINCX 1280
#define WINCY 720

#define OBJ_DEAD 1
#define OBJ_NOEVENT 0

#define UI_EVENT 1
#define UI_NOEVENT 0

#define VK_MAX	0xff

enum LEVEL_ID {LEVEL_MENU, LEVEL_LOGIN, LEVEL_CHOICE, LEVEL_END};
enum OBJ_ID {OBJ_PLAYER,OBJ_END};
enum UI_ID {UI_BUTTON, UI_END};
enum DIRECTION {DIR_LEFT, DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_END};

enum MOUSE_BUTTON {MBUTTON_L,MBUTTON_R,MBUTTON_WHEEL,MBUTTON_END};

enum ButtonID { BUTTON_LOGIN, BUTTON_EXIT, BUTTON_END };
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

}INFO;



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