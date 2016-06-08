
#ifndef MAIN_H
#define MAIN_H

#include "stdafx.h"
#include "../../Engine/Renderer/Renderer.h"
#include "../../Engine/Utilities/Time.h"
#include "../../Engine/Input/InputManager.h"
#include "Game.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//if (wParam == VK_ESCAPE)
	//	message = WM_DESTROY;

	switch (message)
	{
	case WM_QUIT:
		message = WM_DESTROY;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int cmdShow)
{
	// Memory Leak Detection
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1);
#endif

	// Window Initialization 
	HWND hWindow;
	MSG message;
	RECT rScreen;
	WNDCLASSEX windowClass;

	// Grab Screen Resolution 
	GetWindowRect(GetDesktopWindow(), &rScreen);

	// Fill Window Properties 
	ZeroMemory(&message, sizeof(message));
	ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(107));
	windowClass.hCursor = LoadCursorW(NULL, (LPWSTR)IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)(1 + COLOR_WINDOW);
	windowClass.lpszMenuName = MAKEINTRESOURCE(107);
	windowClass.lpszClassName = L"Game Engine";

	RegisterClassEx(&windowClass);

	STICKYKEYS wStickyKeys;
	wStickyKeys.cbSize = sizeof(STICKYKEYS);
	wStickyKeys.dwFlags = NULL;
	HRESULT result = SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &wStickyKeys, NULL);

	// Create Window 
	hWindow = CreateWindow(L"Game Engine",
		L"Engine App", 
		WS_VISIBLE | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		((SCREEN_WIDTH - WINDOW_WIDTH) / 2), ((SCREEN_HEIGHT - WINDOW_HEIGHT) / 2),/*250, 100,*/ 
		WINDOW_WIDTH, WINDOW_HEIGHT, 
		NULL, NULL, hInstance, NULL);

	if (!hWindow) // If creation of window fails
		return false;

	ShowWindow(hWindow, cmdShow);
	UpdateWindow(hWindow);



	// Intialize Singleton Classes		
	//Note: Do not change the order of initialization, updates, and shutdown/deletes of these singletons
	if (CTime::GetInstance()->Initialize() == false)
	{
		DEBUG_OUTPUT("Time Class Failed to Initialize!\n");
		return 0;
	}
	if (CInputManager::GetInstacne()->Initialize() == false)
	{
		DEBUG_OUTPUT("Input Manager Failed to Initialize!\n");
		return 0;
	}
	if (CRenderer::GetInstance()->Initialize(hWindow) == false)
	{
		DEBUG_OUTPUT("Renderer Failed to Initialize!\n");
		return 0;
	}
	if (CGame::GetInstance()->Initialize() == false)
	{
		DEBUG_OUTPUT("Game Failed to Initialize!\n");
		return 0;
	}

	//Gane Loop
	while (message.message != WM_QUIT)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
			break;

		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			//Game Time
			if (CTime::GetInstance()->Update() == false)
			{
				DEBUG_OUTPUT("Time Class Failed to Update!\n");
				break;
			}

			//Input Manager
			if (CInputManager::GetInstacne()->Update() == false)
			{
				DEBUG_OUTPUT("Input Manager Failed to Update!\n");
				break;
			}

			//Renderer
			if (CRenderer::GetInstance()->Update() == false)
			{
				DEBUG_OUTPUT("Renderer Failed to Update!\n");
				break;
			}

			//The Game Update
			if (CGame::GetInstance()->Update() == false)
			{
				DEBUG_OUTPUT("Game Failed to Update!\n");
				break;
			}
		}
	}

	//Shutdown Singleton Classes
	CGame::GetInstance()->Shutdown();
	CRenderer::GetInstance()->Shutdown();
	CInputManager::GetInstacne()->Shutdown();

	//Delete Singleton Classes
	CGame::DeleteInstance();
	CRenderer::DeleteInstance();
	CInputManager::GetInstacne()->DeleteInstance();
	CTime::DeleteInstance();

	return 0;
}

#endif
