
#include "ObjectManager.h"
#include "Object.h"

CObjectManager::CObjectManager()
{
	m_cpTheCamera = nullptr;
}

CObjectManager::~CObjectManager()
{
	for (unsigned int i = 0; i < m_vpObjects.size(); i++)
	{
		if (m_vpObjects[i] != nullptr)
		{
			SAFE_DELETE(m_vpObjects[i]);
		}
	}
}

void CObjectManager::AddObject(CObject* pObject, bool bIsCamera)
{
	if (pObject != nullptr)
	{
		if (bIsCamera == true)
		{
			m_cpTheCamera = pObject;
			DEBUG_OUTPUT("Successfully Added the Camera to the Object Manager\n");
		}

		m_vpObjects.push_back(pObject);

		std::string szSuccess = "Successfully Added Object to the ObjectManger! Object Tag = ";
		szSuccess += pObject->GetTag().c_str();
		szSuccess += "\n";
		DEBUG_OUTPUT(szSuccess);
	}
	else
	{
		DEBUG_OUTPUT("Failed to Add Object to ObjectManager! Object is NULL!\n");
	}
}

bool CObjectManager::RemoveObject(CObject* pObject)
{
	if (pObject == nullptr)
	{
		DEBUG_OUTPUT("Failed to Remove Object from ObjectManager! Object is NULL!\n");
		return false;
	}

	for (std::vector<CObject*>::iterator iter = m_vpObjects.begin(); iter != m_vpObjects.end(); iter++)
	{
		if ((*iter) == pObject)
		{
			std::string szTempTag = pObject->GetTag();

			m_vpObjects.erase(iter);

			std::string szSuccess = "Successfully Removed Object from ObjectManger! Object Tag = ";
			szSuccess += szTempTag.c_str();
			szSuccess += "\n";
			DEBUG_OUTPUT(szSuccess);
			return true;
		}
	}

	DEBUG_OUTPUT("Failed to Remove Object from ObjectManager! Object was not Found in ObjectManager!\n");
	return false;
}

std::vector<CObject*> CObjectManager::FindObjectsWithTag(std::string szTag)
{
	std::vector<CObject*> vpFoundObjects;

	for (unsigned int i = 0; i < m_vpObjects.size(); i++)
	{
		if (m_vpObjects[i]->GetTag() == szTag)
			vpFoundObjects.push_back(m_vpObjects[i]);
	}

	return vpFoundObjects;
}

bool CObjectManager::Update()
{
	for (unsigned int i = 0; i < m_vpObjects.size(); i++)
	{
		bool error = m_vpObjects[i]->Update();
		if (error == false)
		{
			DEBUG_OUTPUT("Failed to Update Object in ObjectManager!\n");
			return false;
		}

		error = m_vpObjects[i]->UpdatePosition();
		if (error == false)
		{
			DEBUG_OUTPUT("Failed to Update Object Position in ObjectManager!\n");
			return false;
		}
	}

	return true;
}
