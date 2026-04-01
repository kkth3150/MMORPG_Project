#pragma once

#include <algorithm>
#include <list>
#include <map>
#include <vector>
#include <cmath>

#include <windows.h>
#include <TCHAR.h>
#include <timeapi.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>        // DirectWrite (텍스트)
#include <wincodec.h>      // WIC (PNG 로드)



#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")


#ifdef _DEBUG
#define GAME_DEBUG
#endif