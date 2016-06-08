
#include "Keyboard.h"


CKeyboard::CKeyboard()
{
	ClearKeys();

	m_ptMousePosition = { 0, 0 };
	m_vMouseDirection.ZeroOut();
}

bool CKeyboard::Update(HWND window)
{
	if (GetForegroundWindow() == window)
	{
#pragma region Keyboard

		unsigned char uchTempKeyboard[NUM_KEYBOARD_KEYS];
		BOOL error = GetKeyboardState(uchTempKeyboard);
		if (error == false)
		{
			DEBUG_OUTPUT("INPUT_MANAGER: Failed to get Keyboard State!\n");
			return false;
		}

		for (unsigned int i = 0; i < NUM_KEYBOARD_KEYS; i++)
			m_chKeyboard[i] = ((m_chKeyboard[i] & KEY_PRESSED) >> 1) | (uchTempKeyboard[i]);

#pragma endregion

#pragma region Mouse

		//cursor position
		POINT ptTempMouse = {};
		error = GetCursorPos(&ptTempMouse);
		if (error == false)
		{
			DEBUG_OUTPUT("KEYBOARD::UPDATE: Failed to get cursor position!\n");
			return false;
		}

		//cursor direction
		m_vMouseDirection.x = float(ptTempMouse.x - m_ptMousePosition.x);
		m_vMouseDirection.y = float(ptTempMouse.y - m_ptMousePosition.y);

		//cursor wrap
		RECT rClientRect = {};
		error = GetWindowRect(window, &rClientRect);
		if (error == false)
		{
			DEBUG_OUTPUT("KEYBOARD::UPDATE: Failed to get window rect!\n");
			return false;
		}

		if (ptTempMouse.x < rClientRect.left + 2) // Left
		{
			SetCursorPos(rClientRect.right - 2, ptTempMouse.y);
			GetCursorPos(&ptTempMouse);
		}
		if (ptTempMouse.y < rClientRect.top + 2) // Top
		{
			SetCursorPos(ptTempMouse.x, rClientRect.bottom - 2);
			GetCursorPos(&ptTempMouse);
		}
		if (ptTempMouse.x > rClientRect.right - 2) // Right
		{
			SetCursorPos(rClientRect.left + 2, ptTempMouse.y);
			GetCursorPos(&ptTempMouse);
		}
		if (ptTempMouse.y > rClientRect.bottom - 2) // Bottom
		{
			SetCursorPos(ptTempMouse.x, rClientRect.top + 2);
			GetCursorPos(&ptTempMouse);
		}

		//lock cursor to center of screen
//		error = SetCursorPos((rClientRect.left + rClientRect.right) / 2, (rClientRect.top + rClientRect.bottom) / 2);
//		if (error == false)
//		{
//			DEBUG_OUTPUT("KEYBOARD::UPDATE: Failed to set cursor pos!\n");
//			return false;
//		}
//
//		error = GetCursorPos(&ptTempMouse);
//		if (error == false)
//		{
//			DEBUG_OUTPUT("KEYBOARD::UPDATE: Failed to get cursor position(2nd)!\n");
//			return false;
//		}
//		
//		m_ptMousePosition = ptTempMouse;

#pragma endregion
	}
	else
		ClearKeys();

	return true;
}

bool CKeyboard::IsAnyKeyPressed()
{
	for (unsigned int i = 0; i < NUM_KEYBOARD_KEYS; i++)
	{
		if ((m_chKeyboard[i] & KEY_OFFSET) == KEY_PRESSED)
			return true;
	}

	return false;
}

bool CKeyboard::IsKeyPressed(EKey key)
{
	if ((m_chKeyboard[key] & KEY_OFFSET) == KEY_PRESSED)
		return true;

	return false;
}

bool CKeyboard::IsKeyDown(EKey key)
{
	if ((m_chKeyboard[key] & KEY_PRESSED) == KEY_PRESSED)
		return true;

	return false;
}

bool CKeyboard::IsKeyReleased(EKey key)
{
	if ((m_chKeyboard[key] & KEY_OFFSET) != KEY_RELEASED)
		return true;

	return false;
}

bool CKeyboard::IsKeyUp(EKey key)
{
	if ((m_chKeyboard[key] & KEY_PRESSED) != KEY_PRESSED)
		return true;

	return false;
}

void CKeyboard::ClearKeys()
{
	memset(m_chKeyboard, 0, sizeof(m_chKeyboard));
	BOOL error = SetKeyboardState(m_chKeyboard);
	if (error == false)
		DEBUG_OUTPUT("KEYBOARD::ClearKeys: Failed to set keyboard state!\n");
}
