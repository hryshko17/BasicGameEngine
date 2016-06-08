
#ifndef CGAME_H
#define CGAME_H

#include "stdafx.h"

class CRenderer;
class CRenderMesh;
class CObjectManager;
class CGameObject;
class CCamera;


class CGame
{
private:

	static CGame* m_cpInstance;

	//Renderer Instance
	CRenderer* m_cpRenderer;

	//this states objectmanager
	CObjectManager* m_cpObjectManager;

	//TEMP:: 
	CGameObject* m_cpGroundPlane;
	CCamera* m_cpCamera;
	CRenderMesh* m_pcSkyBoxMesh;

public:

	CGame() = default;
	~CGame() = default;

	static CGame* GetInstance();
	static void DeleteInstance();

	bool Initialize();
	bool Update();
	void Shutdown();

};

#endif
