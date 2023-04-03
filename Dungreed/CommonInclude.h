#pragma once
// WIndows Header File
#include <windows.h>
// C Runtime Header File
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
//#include <wchar.h>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include <filesystem>

#pragma comment(lib, "Msimg32.lib")

#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

#define TILE_SIZE_X 64
#define TILE_SIZE_Y 64

#include <ole2.h>
#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")
