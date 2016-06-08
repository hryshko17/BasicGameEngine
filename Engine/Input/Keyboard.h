
#ifndef CKEYBOARD_H
#define CKEYBOARD_H

#include "../../Game/Core/stdafx.h"

#define NUM_KEYBOARD_KEYS 256


enum EKey
{
	None = 0,

	LeftMouse = 1,
	RightMouse = 2,
	MiddleMouse = 4,

	Backspace = 8,
	Tab = 9,
	Enter = 13,
	Shift = 16,
	Control = 17,
	Alt = 18,
	CapsLock = 20,

	escape = 27,
	Spacebar = 32,

	LeftArrow = 37,
	UpArrow = 38,
	RightArrow = 39,
	DownArrow = 40,

	Num0 = 48,
	Num1 = 49,
	Num2 = 50,
	Num3 = 51,
	Num4 = 52,
	Num5 = 53,
	Num6 = 54,
	Num7 = 55,
	Num8 = 56,
	Num9 = 57,

	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,

	NumPad0 = 96,
	NumPad1 = 97,
	NumPad2 = 98,
	NumPad3 = 99,
	NumPad4 = 100,
	NumPad5 = 101,
	NumPad6 = 102,
	NumPad7 = 103,
	NumPad8 = 104,
	NumPad9 = 105,

	F1 = 112,
	F2 = 113,
	F3 = 114,
	F4 = 115,
	F5 = 116,
	F6 = 117,
	F7 = 118,
	F8 = 119,
	F9 = 120,
	F10 = 121,
	F11 = 122,
	F12 = 123,

};


class CKeyboard
{
private:

	enum EKeyStates
	{
		KEY_NONE = 0,
		KEY_RELEASED = 64,
		KEY_PRESSED = 128,
		KEY_OFFSET = 192
	};

	///Array of all mouse and keyboard keys
	unsigned char m_chKeyboard[NUM_KEYBOARD_KEYS];

	POINT m_ptMousePosition;
	Vector3D m_vMouseDirection;

public:

	CKeyboard();
	~CKeyboard() = default;

	bool Update(HWND window);

	bool IsAnyKeyPressed();
	bool IsKeyPressed(EKey key);
	bool IsKeyDown(EKey key);
	bool IsKeyReleased(EKey key);
	bool IsKeyUp(EKey key);
	void ClearKeys();

	//Accessors
	unsigned char* GetKeyboard() { return m_chKeyboard; };
	POINT* GetMousePosition() { return &m_ptMousePosition; };
	Vector3D GetMouseDirection() { return m_vMouseDirection; };

	//Mutators
	void SetMousePosition(POINT pos) { m_ptMousePosition = pos; };
	
};

#endif
