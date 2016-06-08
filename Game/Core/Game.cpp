
#include "Game.h"
#include "../../Engine/Asset Manager/AssetManager.h"
#include "../../Engine/Utilities/Geometry.h"
#include "../../Engine/Renderer/Renderer.h"
#include "../../Engine/Renderer/RenderMesh.h"
#include "../../Engine/Object Manager/ObjectManager.h"
#include "../../Engine/Object Manager/Object.h"
#include "../Objects/GameObject.h"
#include "../Camera/Camera.h"

CGame* CGame::m_cpInstance = nullptr;


CGame* CGame::GetInstance()
{
	if (m_cpInstance == nullptr)
		m_cpInstance = new CGame();

	return m_cpInstance;
}

void CGame::DeleteInstance()
{
	if (m_cpInstance != nullptr)
		SAFE_DELETE(m_cpInstance);
}

bool CGame::Initialize()
{
	//set renderer instance
	m_cpRenderer = CRenderer::GetInstance();

	//initialize all engine singletons besides the renderer
	if (CAssetManager::GetInstance()->Initialize() == false)//asset manager
	{
		DEBUG_OUTPUT("Asset Manager Failed to Initialize!\n");
		return 0;
	}

#pragma region TEMP::

	//initialize the object manager
	m_cpObjectManager = new CObjectManager();

#pragma region Ground Plane Set Up

	//create plane object
	Vector3D vCenterPos = { 0.0f, 0.0f, 0.0f, 0.0f };
	Vector3D vExtents = { 10.0f, 10.0f, 10.0f };
	TPlane3D tGround = CGeometry::CreatePlane3D(CGeometry::EPlaneFaceDir::eUpPlane, vCenterPos, vExtents);

	m_cpGroundPlane = new CGameObject("Ground Plane");
	m_cpGroundPlane->SetRenderMesh(new CRenderMesh(tGround.m_vIndexArray, tGround.m_vVertexArray, "Ground Plane",
		vCenterPos, m_cpRenderer->GetStandard3DVS(), m_cpRenderer->GetStandard3DPS(), 
		L"../Assets/Art/2D/Textures/Grass_Ground.dds"));

	//add the object to the object manager
	m_cpObjectManager->AddObject(m_cpGroundPlane);

#pragma endregion

#pragma region Camera Set Up

	Vector3D vOffset = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_cpCamera = new CCamera(vOffset, CCamera::ECameraType::eFREE_ROAM);

	m_cpObjectManager->AddObject(m_cpCamera, true);

#pragma endregion

#pragma region Skybox Set Up
	
	m_pcSkyBoxMesh = CGeometry::CreateSkybox("Sky Box", m_cpCamera->GetWorldMatrix(), L"../Assets/Art/2D/Textures/SkyBox.dds");
	m_pcSkyBoxMesh->GetWorldMatrix() = m_cpCamera->GetWorldMatrix();
	m_cpRenderer->AddRenderMesh(m_pcSkyBoxMesh->GetName(), m_pcSkyBoxMesh);

#pragma endregion

	//add all the objects in the objectmanager rendermesh to the renderer
	for (unsigned int i = 0; i < m_cpObjectManager->GetObjects().size(); i++)
	{
		if (m_cpObjectManager->GetObjects()[i] != nullptr)
		{
			m_cpRenderer->AddRenderMesh(m_cpObjectManager->GetObjects()[i]);
		}
	}

#pragma endregion

	//state machine

	return true;
}

bool CGame::Update()
{
	//object manager
	m_cpObjectManager->Update();

	//state machine 

	//m_pcSkyBoxMesh->SetWorldMatrix(m_cpCamera->GetWorldMatrix());
	//Vector3D tempPos;
	//for (unsigned int i = 0; i < MAX_VECTOR_ELEMENTS; i++)
	//{
	//	tempPos.e[i] = m_cpCamera->GetWorldMatrix().m[3][i];
	//}
	//m_pcSkyBoxMesh->SetPosition(tempPos);

	return true;
}
 
void CGame::Shutdown()
{
	//object manager
	SAFE_DELETE(m_cpObjectManager);

	//asset manager
	CAssetManager::GetInstance()->Shutdown();
	CAssetManager::DeleteInstance();

	//state machine

}
