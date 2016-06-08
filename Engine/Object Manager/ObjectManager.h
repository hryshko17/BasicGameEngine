
#ifndef COBJECTMANAGER_H
#define COBJECTMANAGER_H

#include "../../Game/Core/stdafx.h"

class CObject;

class CObjectManager
{
private:

	std::vector<CObject*> m_vpObjects;
	CObject* m_cpTheCamera;

public:

	CObjectManager();
	~CObjectManager();

	void AddObject(CObject* pObject, bool bIsCamera = false);
	bool RemoveObject(CObject* pObject);
	std::vector<CObject*> FindObjectsWithTag(std::string szTag);

	bool Update();

	/*	Accessors	*/
	std::vector<CObject*> GetObjects() { return m_vpObjects; }
	CObject* GetCamera() { return m_cpTheCamera; }

};

#endif
