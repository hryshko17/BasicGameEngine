/*
*	File					stdafx.h
*	Creation Aurthor:		Nick Hryshko
*	Creation Date:			1/17/16
*	Last Modified Date:		1/21/16
*/

#ifndef STDAFX_H
#define STDAFX_H


//Standard includes
#include <Windows.h>
#include <array>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>

//directX includes
#include <d3d11.h>
#include <DirectXMath.h>
#pragma comment(lib, "d3d11.lib")

//engine includes
#include "../../Engine/Math Library/MathLibrary.h"

//namespaces
using namespace std;
using namespace DirectX;

//general macros
#define FOV 75.0f//field of view angle for camera

//game window macros
#define SCREEN_WIDTH 1980
#define SCREEN_HEIGHT 1080
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define RATIO ((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)

//print a string to the output window in debug macro
#define DEBUG_OUTPUT(x) OutputDebugStringA(std::string(x).c_str())
#define DEBUG_OUTPUTW(x) OutputDebugStringW(std::wstring(x).c_str())

//COM object release macros
#define SAFE_RELEASE(x) { if((x)) { (x)->Release(); (x) = nullptr; } }
#define VECTOR_RELEASE_ALL(vector) { auto iter = (vector).begin(); auto end = (vector).end(); while (iter != end) { SAFE_RELEASE(*iter); iter++; } (vector).clear(); }
#define MAP_RELEASE_ALL(map) { auto iter = (map).begin(); auto end = (map).end(); while (iter != end) { SAFE_RELEASE(iter->second); iter++; } (map).clear(); }

//pointer delete macros
#define SAFE_DELETE(x) { if((x)) { delete (x); (x) = nullptr; } }
#define VECTOR_DELETE(vector, x) { auto iter = (vector).begin(); auto end = (vector).end(); while (iter != end) { if (*iter == (x)) { SAFE_DELETE(*iter); (x) = nullptr; (vector).erase(iter); break; } iter++; } }
#define VECTOR_DELETE_ALL(vector) { auto iter = (vector).begin(); auto end = (vector).end(); while (iter != end) { SAFE_DELETE(*iter); iter++; } (vector).clear(); }
#define MAP_DELETE(map, x) { auto iter = (map).begin(); auto end = (map).end(); while (iter != end) { if (iter->second == (x)) { SAFE_DELETE(iter->second); (x) = nullptr; (map).erase(iter); break; } iter++; } }
#define MAP_DELETE_ALL(map) { auto iter = (map).begin(); auto end = (map).end(); while (iter != end) { SAFE_DELETE(iter->second); iter++; } (map).clear(); }


#endif
