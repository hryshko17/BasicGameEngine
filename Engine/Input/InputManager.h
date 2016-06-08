
#ifndef CINPUTMANAGER_H
#define CINPUTMANAGER_H

#include "Keyboard.h"
#include "../../Game/Core/stdafx.h"

class CInputManager
{
private:

	static CInputManager* m_pcInstance;

	HWND m_hInputWindow;

	CKeyboard* m_cKeyboard;
	std::unordered_map<std::string, std::vector<EKey>> m_umRegisteredKeys;


public:

	CInputManager();
	~CInputManager() = default;

	static CInputManager* GetInstacne();
	static void DeleteInstance();

	bool Initialize();
	bool Update();
	void Shutdown();

	///Register up to 3 keys to one name
	void RegisterKey(std::string szName, EKey eKey1, EKey eKey2 = EKey::None, EKey eKey3 = EKey::None);
	bool IsRegisteredKeyDown(std::string szName);
	bool IsRegisteredKeyUp(std::string szName);
	bool IsregisteredKeyPressed(std::string szName);
	bool IsRegisteredKeyReleased(std::string szName);

	//Accessors
	CKeyboard* KeyBoard() { return m_cKeyboard; };

};

#endif