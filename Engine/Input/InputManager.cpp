
#include "InputManager.h"
#include "../../Game/Core/stdafx.h"


CInputManager* CInputManager::m_pcInstance = nullptr;


CInputManager::CInputManager()
{
	m_hInputWindow = nullptr;

	m_cKeyboard = new CKeyboard();

}

CInputManager* CInputManager::GetInstacne()
{
	if (m_pcInstance == nullptr)
		m_pcInstance = new CInputManager();

	return m_pcInstance;
}

void CInputManager::DeleteInstance()
{
	if (m_pcInstance != nullptr)
		SAFE_DELETE(m_pcInstance);
}

bool CInputManager::Initialize()
{
#pragma region Keyboard/Mouse

	//get active window
	m_hInputWindow = GetActiveWindow();
	if (m_hInputWindow == nullptr)
	{
		DEBUG_OUTPUT("INPUT_MANAGER::Initialize: Failed to get active window!\n");
		return false;
	}

	//get mouse position
	BOOL error = GetCursorPos(m_cKeyboard->GetMousePosition());
	if (error == false)
	{
		DEBUG_OUTPUT("INPUT_MANAGER::Initialize: Failed to get mouse position!\n");
		return false;
	}

	error = ScreenToClient(m_hInputWindow, m_cKeyboard->GetMousePosition());
	if (error == false)
	{
		DEBUG_OUTPUT("INPUT_MANAGER::Initialize: Failed screen to client!\n");
		return false;
	}

	//clip mouse position
	RECT rTempRect = {};
	error = GetClipCursor(&rTempRect);
	if (error == false)
	{
		DEBUG_OUTPUT("INPUT_MANAGER::Initialize: Failed to get clip cursor!\n");
		return false;
	}
	m_cKeyboard->GetMousePosition()->x -= rTempRect.left;
	m_cKeyboard->GetMousePosition()->y -= rTempRect.top;

	//set cursor pos to center of screen
	//SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	SetCursorPos((rTempRect.left + rTempRect.right) / 2, (rTempRect.top + rTempRect.bottom) / 2);

#pragma endregion

	return true;
}

bool CInputManager::Update()
{
	//keyboard
	bool error = m_cKeyboard->Update(m_hInputWindow);
	if (error == false)
	{
		DEBUG_OUTPUT("INPUT_MANAGER::UPDATE: Failed to Update the Keyboard!\n");
		return false;
	}

	return true;
}

void CInputManager::Shutdown()
{
	SAFE_DELETE(m_cKeyboard);

	m_umRegisteredKeys.clear();
}


void CInputManager::RegisterKey(std::string szName, EKey eKey1, EKey eKey2, EKey eKey3)
{
	std::vector<EKey> veKeys;

	//check if the name alrdy exists as a registered key
	std::unordered_map<std::string, std::vector<EKey>>::iterator umIter = m_umRegisteredKeys.find(szName);
	if (umIter != m_umRegisteredKeys.end())//the name alrdy exists as a registered key. Overwrite it with new data
	{
		veKeys = (*umIter).second;
		m_umRegisteredKeys.erase(umIter);
	}

	//set first key
	veKeys.push_back(eKey1);

	//set second key if valid
	if (eKey2 != EKey::None)
		veKeys.push_back(eKey2);

	//set third key if valid
	if (eKey3 != EKey::None)
		veKeys.push_back(eKey3);

	//set the registered keys
	m_umRegisteredKeys.insert(std::pair<std::string, std::vector<EKey>>(szName, veKeys));
}

bool CInputManager::IsRegisteredKeyDown(std::string szName)
{
	if (m_umRegisteredKeys.find(szName) != m_umRegisteredKeys.end())
	{
		std::vector<EKey> veKeys = m_umRegisteredKeys[szName];
		for (unsigned int i = 0; i < veKeys.size(); i++)
		{
			if (m_cKeyboard->IsKeyDown(veKeys[i]))
				return true;
		}
	}

	return false;
}

bool CInputManager::IsRegisteredKeyUp(std::string szName)
{
	if (m_umRegisteredKeys.find(szName) != m_umRegisteredKeys.end())
	{
		std::vector<EKey> veKeys = m_umRegisteredKeys[szName];
		for (unsigned int i = 0; i < veKeys.size(); i++)
		{
			if (m_cKeyboard->IsKeyUp(veKeys[i]))
				return true;
		}
	}

	return false;
}

bool CInputManager::IsregisteredKeyPressed(std::string szName)
{
	if (m_umRegisteredKeys.find(szName) != m_umRegisteredKeys.end())
	{
		std::vector<EKey> veKeys = m_umRegisteredKeys[szName];
		for (unsigned int i = 0; i < veKeys.size(); i++)
		{
			if (m_cKeyboard->IsKeyPressed(veKeys[i]))
				return true;
		}
	}

	return false;
}

bool CInputManager::IsRegisteredKeyReleased(std::string szName)
{
	if (m_umRegisteredKeys.find(szName) != m_umRegisteredKeys.end())
	{
		std::vector<EKey> veKeys = m_umRegisteredKeys[szName];
		for (unsigned int i = 0; i < veKeys.size(); i++)
		{
			if (m_cKeyboard->IsKeyReleased(veKeys[i]))
				return true;
		}
	}

	return false;
}
