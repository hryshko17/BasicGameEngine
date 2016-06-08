/*
*	File					Renderer.cpp
*	Creation Aurthor:		Nick Hryshko
*	Creation Date:			1/17/16
*	Last Modified Date:		1/17/16
*/

#include "Renderer.h"
#include "RenderMesh.h"
#include "../Math Library/MathLibrary.h"
#include "../Utilities/Time.h"
#include "../Input/InputManager.h"
#include "../Object Manager/Object.h"

//shaders
#include "Shaders\Compiled Shaders\Standard3DVS.csh"
#include "Shaders\Compiled Shaders\Standard3DPS.csh"
#include "Shaders\Compiled Shaders\StandardSkyBoxVS.csh"
#include "Shaders\Compiled Shaders\StandardSkyBoxPS.csh"


CRenderer* CRenderer::m_pcInstance = nullptr;


CRenderer::CRenderer()
{
#if _DEBUG
	m_bDebugMode = true;
#else
	m_bDebugMode = false;
#endif

	//window
	m_Window = nullptr;
	m_vClearColor.ZeroOut();

	//scene
	m_vSceneVMStartPos.ZeroOut();

	//directx variables
	m_d3dSwapChain = nullptr;
	m_d3dDevice = nullptr;
	m_d3dDeviceContext = nullptr;
	m_d3dRenderTargetView = nullptr;
	m_d3dInputLayout = nullptr;
	m_d3dSamplerState = nullptr;

	//buffers
	m_d3dBackBuffer = nullptr;
	m_d3dBackBufferRTV = nullptr;
	m_d3dDepthBuffer = nullptr;
	m_d3dDepthBufferDSV = nullptr;
	m_d3dSceneTexture = nullptr;
	m_d3dSceneBufferRTV = nullptr;
	m_d3dSceneBufferSRV = nullptr;

	//constant buffers
	m_d3dTransformConstBuffer = nullptr;
	m_d3dSceneConstBuffer = nullptr;

	//shaders
	m_vsStandard3D = nullptr;
	m_psStandard3D = nullptr;
	m_vsStandardSkyBox = nullptr;
	m_psStandardSkyBox = nullptr;

	//camera
	m_bDebugCamera = false;
	m_fCameraAngle = 0.0f;
	m_ptCurrentPosition = { 0, 0 };
	m_ptPreviousPosition = { 0, 0 };

}

CRenderer* CRenderer::GetInstance()
{
	if (m_pcInstance == nullptr)
		m_pcInstance = new CRenderer();

	return m_pcInstance;
}

void CRenderer::DeleteInstance()
{
	if (m_pcInstance != nullptr)
		SAFE_DELETE(m_pcInstance);
}

bool CRenderer::Initialize(HWND window)
{
	//set window
	m_Window = window;

	//set clear color
	m_vClearColor.Set(0.5f, 0.0f, 0.5f, 0.0f);//rgba (magenta)

#pragma region Swap Chain and Feature Levels Initalization

	//set swap chain
	DXGI_SWAP_CHAIN_DESC d3dSwapChainDesc;
	ZeroMemory(&d3dSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	d3dSwapChainDesc.BufferCount = 1;
	d3dSwapChainDesc.BufferDesc.Width = (UINT)WINDOW_WIDTH;
	d3dSwapChainDesc.BufferDesc.Height = (UINT)WINDOW_HEIGHT;
	d3dSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dSwapChainDesc.BufferDesc.RefreshRate.Numerator = (UINT)60;
	d3dSwapChainDesc.BufferDesc.RefreshRate.Denominator = (UINT)1;
	d3dSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	d3dSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	d3dSwapChainDesc.OutputWindow = m_Window;
	d3dSwapChainDesc.SampleDesc.Count = (UINT)1;
	d3dSwapChainDesc.SampleDesc.Quality = (UINT)0;
	d3dSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	d3dSwapChainDesc.Windowed = true;


	//set up feature levels
	const D3D_FEATURE_LEVEL d3dFeatureLevels[D3D_FEATURE_LEVEL_VERSIONS] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};


	//set device and swap chain descriptors
	D3D11CreateDeviceAndSwapChain(nullptr,				//Default adapter
		D3D_DRIVER_TYPE_HARDWARE,						//Render with hardware
		nullptr,										//No software rasterizer
		(m_bDebugMode) ? D3D11_CREATE_DEVICE_DEBUG : 0,	//debug mode
		d3dFeatureLevels,								//DirectX feature level versions array
		D3D_FEATURE_LEVEL_VERSIONS,						//Number of DirectX feature level versions
		D3D11_SDK_VERSION,								//Best SDK option
		&d3dSwapChainDesc,								//Swap chain descriptor
		&m_d3dSwapChain,								//Swap chain OUT param
		&m_d3dDevice,									//Device OUT param
		nullptr,										//Current DirectX version
		&m_d3dDeviceContext);							//Device Contexxt OUT param

#pragma endregion

#pragma region View Port Initialization

	//view port creation
	ZeroMemory(&m_d3dViewPort, sizeof(D3D11_VIEWPORT));
	m_d3dViewPort.Width = (FLOAT)WINDOW_WIDTH;
	m_d3dViewPort.Height = (FLOAT)WINDOW_HEIGHT;
	m_d3dViewPort.MinDepth = (FLOAT)0;
	m_d3dViewPort.MaxDepth = (FLOAT)1;
	m_d3dViewPort.TopLeftX = (FLOAT)0;
	m_d3dViewPort.TopLeftY = (FLOAT)0;
	m_d3dDeviceContext->RSSetViewports(1, &m_d3dViewPort);

#pragma endregion

#pragma region Buffer Initialization

	//set up back buffer
	HRESULT error = m_d3dSwapChain->GetBuffer(NULL, __uuidof(m_d3dBackBuffer), (void**)(&m_d3dBackBuffer));
	if (FAILED(error))
	{
		DEBUG_OUTPUT("Back Buffer Creation Failed!\n");
		return false;
	}

	error = m_d3dDevice->CreateRenderTargetView(m_d3dBackBuffer, nullptr, &m_d3dBackBufferRTV);
	if (FAILED(error))
	{
		DEBUG_OUTPUT("Back Buffer Render Target View(RTV) Creation Failed!\n");
		return false;
	}


	//set up depth buffer
	D3D11_TEXTURE2D_DESC d3dDepthTextureDesc;
	CreateTextureDesc(WINDOW_WIDTH, WINDOW_HEIGHT, DXGI_FORMAT_D32_FLOAT, 1, 0, D3D11_BIND_DEPTH_STENCIL, NULL, NULL, d3dDepthTextureDesc);
	error = m_d3dDevice->CreateTexture2D(&d3dDepthTextureDesc, NULL, &m_d3dDepthBuffer);
	if (FAILED(error))
	{
		DEBUG_OUTPUT("Depth Buffer Failed\n");
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthDSVDesc;
	CreateDSVDesc(NULL, DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2D, d3dDepthDSVDesc);
	error = m_d3dDevice->CreateDepthStencilView(m_d3dDepthBuffer, &d3dDepthDSVDesc, &m_d3dDepthBufferDSV);
	if (FAILED(error))
	{
		DEBUG_OUTPUT("Depth Buffer Depth Stencil View(DSV) Failed\n");
		return false;
	}


	//set up scene buffer
	D3D11_TEXTURE2D_DESC d3dSceneTextureDesc;
	CreateTextureDesc(WINDOW_WIDTH, WINDOW_HEIGHT, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, NULL, NULL, d3dSceneTextureDesc);
	error = m_d3dDevice->CreateTexture2D(&d3dSceneTextureDesc, NULL, &m_d3dSceneTexture);
	if (FAILED(error))
	{
		DEBUG_OUTPUT("Scene Buffer Failed\n");
		return false;
	}

	error = m_d3dDevice->CreateRenderTargetView(m_d3dSceneTexture, nullptr, &m_d3dSceneBufferRTV);
	if (FAILED(error))
	{
		DEBUG_OUTPUT("Scene Buffer RTV Failed\n");
		return false;
	}

	error = m_d3dDevice->CreateShaderResourceView(m_d3dSceneTexture, nullptr, &m_d3dSceneBufferSRV);
	if (FAILED(error))
	{
		DEBUG_OUTPUT("Scene Buffer SRV Failed\n");
		return false;
	}

#pragma endregion

#pragma region Shader Creation and Input Layout Initialization

	//set up shaders
	m_d3dDevice->CreateVertexShader(Standard3DVS, sizeof(Standard3DVS), nullptr, &m_vsStandard3D);
	m_d3dDevice->CreatePixelShader(Standard3DPS, sizeof(Standard3DPS), nullptr, &m_psStandard3D);
	m_d3dDevice->CreateVertexShader(StandardSkyBoxVS, sizeof(StandardSkyBoxVS), nullptr, &m_vsStandardSkyBox);
	m_d3dDevice->CreatePixelShader(StandardSkyBoxPS, sizeof(StandardSkyBoxPS), nullptr, &m_psStandardSkyBox);

	//set up input layout for 3D
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout3D[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORDS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_d3dDevice->CreateInputLayout(d3dInputLayout3D, _countof(d3dInputLayout3D), Standard3DVS, sizeof(Standard3DVS), &m_d3dInputLayout);

#pragma endregion

#pragma region Sampler State Initialization

	//set up sampler state
	CD3D11_SAMPLER_DESC d3dSamplerStateDesc = CD3D11_SAMPLER_DESC(CD3D11_DEFAULT());
	d3dSamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerStateDesc.MinLOD = -D3D11_FLOAT32_MAX;
	d3dSamplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;
	m_d3dDevice->CreateSamplerState(&d3dSamplerStateDesc, &m_d3dSamplerState);
	m_d3dDeviceContext->PSSetSamplers(0, 1, &m_d3dSamplerState);

#pragma endregion

#pragma region Constant Buffers Initialization

	//set up transform constant buffer
	D3D11_BUFFER_DESC d3dTransformBufferDesc;
	ZeroMemory(&d3dTransformBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dTransformBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dTransformBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dTransformBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dTransformBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	d3dTransformBufferDesc.MiscFlags = 0;
	d3dTransformBufferDesc.StructureByteStride = 0;
	m_d3dDevice->CreateBuffer(&d3dTransformBufferDesc, NULL, &m_d3dTransformConstBuffer);
	m_d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_d3dTransformConstBuffer);
	m_d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_d3dTransformConstBuffer);

	//set up scene constant buffer
	D3D11_BUFFER_DESC d3dSceneBufferDesc;
	ZeroMemory(&d3dSceneBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dSceneBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dSceneBufferDesc.ByteWidth = sizeof(TScene);
	d3dSceneBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dSceneBufferDesc.MiscFlags = 0;
	d3dSceneBufferDesc.StructureByteStride = 0;
	d3dSceneBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_d3dDevice->CreateBuffer(&d3dSceneBufferDesc, NULL, &m_d3dSceneConstBuffer);
	m_d3dDeviceContext->VSSetConstantBuffers(1, 1, &m_d3dSceneConstBuffer);

#pragma endregion

#pragma region Camera Set Up (View & Proj Matrix)

	//set start position for scene view matrix
	m_vSceneVMStartPos.Set(0.0f, 2.0f, -5.0f, 1.0f);

	//set up camera(view matrix)
	m_tScene.m_mViewMatrix =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		m_vSceneVMStartPos.x, m_vSceneVMStartPos.y, m_vSceneVMStartPos.z, m_vSceneVMStartPos.w,
	};

	//set up camera(proj matrix)
	float yScale = 1.0f / (tan(CMathLib::DegreesToRadians(FOV) / 2.0f));
	float xScale = yScale / RATIO;
	m_tScene.m_mProjMatrix =
	{
		xScale, 0, 0, 0,
		0, yScale, 0, 0,
		0, 0, (FAR_PLANE / (FAR_PLANE - NEAR_PLANE)), 1,
		0, 0, -(FAR_PLANE * NEAR_PLANE) / (FAR_PLANE - NEAR_PLANE), 0
	};

#pragma endregion

	return true;
}

bool CRenderer::Update()
{
	//debug camera
	if (m_bDebugCamera == true)
		UpdateCamera();

	//map buffers
	MapBuffers();

	//draw all 
	ClearScreen(m_d3dBackBufferRTV);
	ClearDepth(m_d3dDepthBufferDSV);

	m_d3dDeviceContext->OMSetRenderTargets(1, &m_d3dBackBufferRTV, 0);
	m_d3dDeviceContext->IASetInputLayout(m_d3dInputLayout);
	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//iterate through all rendermeshes and draw them
	for (std::unordered_map<std::string, CRenderMesh*>::iterator AllRMIter = m_umAllRenderMeshes.begin(); AllRMIter != m_umAllRenderMeshes.end(); AllRMIter++)
	{
		if (m_umAllRenderMeshes[AllRMIter->first]->GetRenderable() == true)
			m_umAllRenderMeshes[AllRMIter->first]->Draw();
	}

	//present scene
	m_d3dSwapChain->Present(0, 0);

	return true;
}

void CRenderer::Shutdown()
{
	//directx variables
	SAFE_RELEASE(m_d3dSwapChain);
	SAFE_RELEASE(m_d3dDevice);
	SAFE_RELEASE(m_d3dDeviceContext);
	SAFE_RELEASE(m_d3dRenderTargetView);
	SAFE_RELEASE(m_d3dInputLayout);
	SAFE_RELEASE(m_d3dSamplerState);

	//buffers
	SAFE_RELEASE(m_d3dBackBuffer);
	SAFE_RELEASE(m_d3dBackBufferRTV);
	SAFE_RELEASE(m_d3dDepthBuffer);
	SAFE_RELEASE(m_d3dDepthBufferDSV);
	SAFE_RELEASE(m_d3dSceneTexture);
	SAFE_RELEASE(m_d3dSceneBufferRTV);
	SAFE_RELEASE(m_d3dSceneBufferSRV);

	//constant buffers
	SAFE_RELEASE(m_d3dTransformConstBuffer);
	SAFE_RELEASE(m_d3dSceneConstBuffer);

	//shaders
	SAFE_RELEASE(m_vsStandard3D);
	SAFE_RELEASE(m_psStandard3D);
	SAFE_RELEASE(m_vsStandardSkyBox);
	SAFE_RELEASE(m_psStandardSkyBox);

	//rendermeshes
	MAP_DELETE_ALL(m_umAllRenderMeshes);
}

void CRenderer::AddRenderMesh(std::string szName, CRenderMesh* pcNewMesh)
{
	if (pcNewMesh != nullptr && CheckRenderMeshExists(szName) == false)
	{
		m_umAllRenderMeshes.insert(std::pair<std::string, CRenderMesh*>(szName, pcNewMesh));
		std::string szSuccess = "Successfully Added Rendermesh to Renderer! Object tag = \n";
		szSuccess += szName.c_str();
		szSuccess += "\n";
		DEBUG_OUTPUT(szSuccess);
	}
	else
	{
		DEBUG_OUTPUT("Failed to AddRenderMesh! RenderMesh is NULL or Tag already exists!\n");
	}
}

void CRenderer::AddRenderMesh(CObject* cpObject)
{
	if (cpObject != nullptr)
	{
		if (cpObject->GetRenderMesh() != nullptr && CheckRenderMeshExists(cpObject->GetTag()) == false)
		{
			m_umAllRenderMeshes.insert(std::pair<std::string, CRenderMesh*>(cpObject->GetTag(), cpObject->GetRenderMesh()));
		
			std::string szSuccess = "Successfully Added Rendermesh to Renderer! Object tag = \n";
			szSuccess += cpObject->GetTag().c_str();
			szSuccess += "\n";
			DEBUG_OUTPUT(szSuccess);
		}
		else
		{
			DEBUG_OUTPUT("Failed to AddRenderMesh! RenderMesh is NULL or Tag already exists!\n");
		}
	}
	else
	{
		DEBUG_OUTPUT("Failed to AddRenderMesh! Object is NULL!\n");
	}
}



/*	Helper Functions	*/
void CRenderer::CreateTextureDesc(unsigned int unWindowWidth, unsigned int unWindowHeight, DXGI_FORMAT d3dFormat,
	unsigned int unSampleCount, unsigned int unSampleQuality, unsigned int unBindFlags, unsigned int unCPUFlags,
	unsigned int unMiscFlags, D3D11_TEXTURE2D_DESC& d3dTextureDesc)
{
	ZeroMemory(&d3dTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dTextureDesc.ArraySize = (UINT)1;
	d3dTextureDesc.BindFlags = unBindFlags;
	d3dTextureDesc.CPUAccessFlags = unCPUFlags;
	d3dTextureDesc.Format = d3dFormat;
	d3dTextureDesc.Height = unWindowHeight;
	d3dTextureDesc.MipLevels = (UINT)1;
	d3dTextureDesc.MiscFlags = unMiscFlags;
	d3dTextureDesc.SampleDesc.Count = unSampleCount;
	d3dTextureDesc.SampleDesc.Quality = unSampleQuality;
	d3dTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTextureDesc.Width = unWindowWidth;
}

void CRenderer::CreateDSVDesc(unsigned int unFlags, DXGI_FORMAT d3dFormat, D3D11_DSV_DIMENSION d3dDimension,
	D3D11_DEPTH_STENCIL_VIEW_DESC& d3dDSVDesc)
{
	ZeroMemory(&d3dDSVDesc, sizeof(d3dDSVDesc));
	d3dDSVDesc.Flags = unFlags;
	d3dDSVDesc.Format = d3dFormat;
	d3dDSVDesc.ViewDimension = d3dDimension;
}

void CRenderer::ClearScreen(ID3D11RenderTargetView* d3dRTV)
{
	float clear[4] = { m_vClearColor.x, m_vClearColor.y, m_vClearColor.z, m_vClearColor.w };
	m_d3dDeviceContext->ClearRenderTargetView(d3dRTV, clear);
}

void CRenderer::ClearDepth(ID3D11DepthStencilView* d3dDSV)
{
	m_d3dDeviceContext->ClearDepthStencilView(d3dDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CRenderer::MapBuffers()
{
	//map the scene (send scene to GPU)
	XMStoreFloat4x4(&m_tScene.m_mViewMatrix, XMMatrixInverse(NULL, XMLoadFloat4x4(&m_tScene.m_mViewMatrix)));//inverse the view matrix

	D3D11_MAPPED_SUBRESOURCE d3dMapRes;
	ZeroMemory(&d3dMapRes, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_d3dDeviceContext->Map(m_d3dSceneConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &d3dMapRes);
	memcpy(d3dMapRes.pData, &m_tScene, sizeof(TScene));
	m_d3dDeviceContext->Unmap(m_d3dSceneConstBuffer, 0);

	XMStoreFloat4x4(&m_tScene.m_mViewMatrix, XMMatrixInverse(NULL, XMLoadFloat4x4(&m_tScene.m_mViewMatrix)));//inverse it back

}

bool CRenderer::CheckRenderMeshExists(std::string szName)
{
	std::unordered_map<std::string, CRenderMesh*>::iterator iter = m_umAllRenderMeshes.find(szName);
	if (iter != m_umAllRenderMeshes.end())
		return true;

	return false;
}

//TODO: replace input logic with input manager functionality
void CRenderer::UpdateCamera()
{
	//set move speed based on if boost is on or not
	float fMoveSpeed = DEBUG_CAMERA_MOVE_SPEED * CTime::GetInstance()->DeltaTime();

	CInputManager* pInput = CInputManager::GetInstacne();

	if (pInput->KeyBoard()->IsKeyDown(EKey::Shift)) //Shift - Boost
		fMoveSpeed = DEBUG_CAMERA_BOOST_SPEED * CTime::GetInstance()->DeltaTime();

	//camera movement (6 directional movement)
	if (pInput->KeyBoard()->IsKeyDown(EKey::W)) // W - forward
		CMathLib::TranslateLocalZ(m_tScene.m_mViewMatrix, fMoveSpeed);
	if (pInput->KeyBoard()->IsKeyDown(EKey::A)) // A - leftward
		CMathLib::TranslateLocalX(m_tScene.m_mViewMatrix, -fMoveSpeed);
	if (pInput->KeyBoard()->IsKeyDown(EKey::S)) // S - backward
		CMathLib::TranslateLocalZ(m_tScene.m_mViewMatrix, -fMoveSpeed);
	if (pInput->KeyBoard()->IsKeyDown(EKey::D)) // D - rightward
		CMathLib::TranslateLocalX(m_tScene.m_mViewMatrix, fMoveSpeed);
	if (pInput->KeyBoard()->IsKeyDown(EKey::Q)) // Q - upward
		CMathLib::TranslateLocalY(m_tScene.m_mViewMatrix, fMoveSpeed / DEBUG_CAMERA_VERTICAL_DIVISOR);
	if (pInput->KeyBoard()->IsKeyDown(EKey::E)) // E - downward
		CMathLib::TranslateLocalY(m_tScene.m_mViewMatrix, -fMoveSpeed / DEBUG_CAMERA_VERTICAL_DIVISOR);


	//arrow keys to rotate camera
	if (pInput->KeyBoard()->IsKeyDown(EKey::UpArrow))//up arrow - rotate upward(pos X)
		CMathLib::RotateLocalX(m_tScene.m_mViewMatrix, fMoveSpeed / DEBUG_CAMERA_ROTATE_DIVISOR);
	if (pInput->KeyBoard()->IsKeyDown(EKey::DownArrow))//down arrow - rotate downward(neg X)
		CMathLib::RotateLocalX(m_tScene.m_mViewMatrix, -fMoveSpeed / DEBUG_CAMERA_ROTATE_DIVISOR);
	if (pInput->KeyBoard()->IsKeyDown(EKey::LeftArrow))//left arrow - rotate leftward(neg Y)
		CMathLib::RotateLocalY(m_tScene.m_mViewMatrix, -fMoveSpeed / DEBUG_CAMERA_ROTATE_DIVISOR);
	if (pInput->KeyBoard()->IsKeyDown(EKey::RightArrow))//right arrow - rotate rightward(pos Y)
		CMathLib::RotateLocalY(m_tScene.m_mViewMatrix, fMoveSpeed / DEBUG_CAMERA_ROTATE_DIVISOR);


#pragma region Mouse Rotation

	if (GetCursorPos(&m_ptCurrentPosition) /*&& GetAsyncKeyState(0x02)*/)
	{
		float fCursorX = (float)(m_ptCurrentPosition.x - m_ptPreviousPosition.x);//(float)pInput->KeyBoard()->GetMousePosition()->x;
		float fCursorY = (float)(m_ptCurrentPosition.y - m_ptPreviousPosition.y);//(float)pInput->KeyBoard()->GetMousePosition()->y;

		if (fCursorX || fCursorY)
		{
			fCursorX *= DEBUG_CAMERA_ROTATE_SENSITIVITY;
			fCursorY *= DEBUG_CAMERA_ROTATE_SENSITIVITY;

			//store position
			float fViewMatrixPosX = m_tScene.m_mViewMatrix.m[3][0];
			float fViewMatrixPosY = m_tScene.m_mViewMatrix.m[3][1];
			float fViewMatrixPosZ = m_tScene.m_mViewMatrix.m[3][2];
			float fViewMatrixPosW = m_tScene.m_mViewMatrix.m[3][3];

			//set position to world center
			m_tScene.m_mViewMatrix.m[3][0] = 0.0f;
			m_tScene.m_mViewMatrix.m[3][1] = 0.0f;
			m_tScene.m_mViewMatrix.m[3][2] = 0.0f;
			m_tScene.m_mViewMatrix.m[3][3] = 0.0f;

			//rotate globally
			CMathLib::RotateLocalY(m_tScene.m_mViewMatrix, fCursorX);

			//prevent x-axis 360's
//			if (m_fCameraAngle + fCursorY < DEBUG_CAMERA_VERTICAL_ANGLE_LOCK && 
//				m_fCameraAngle + fCursorY > -DEBUG_CAMERA_VERTICAL_ANGLE_LOCK)
//			{
//				//update angle
//				m_fCameraAngle += fCursorY;
//			
//				//rotate locally
//				CMathLib::RotateLocalX(m_tScene.m_mViewMatrix, fCursorY);
//			}

			//restore position
			m_tScene.m_mViewMatrix.m[3][0] = fViewMatrixPosX;
			m_tScene.m_mViewMatrix.m[3][1] = fViewMatrixPosY;
			m_tScene.m_mViewMatrix.m[3][2] = fViewMatrixPosZ;
			m_tScene.m_mViewMatrix.m[3][3] = fViewMatrixPosW;
		}
	}

	//snapping
	if (m_ptCurrentPosition.x < 1 || m_ptCurrentPosition.y < 1 ||
		m_ptCurrentPosition.x > WINDOW_WIDTH - 2 || m_ptCurrentPosition.y > WINDOW_HEIGHT - 2)
	{
		BOOL bError = GetCursorPos(&m_ptCurrentPosition);
		if (bError == false)
			DEBUG_OUTPUT("Failed to get cursor pos in Renderer::UpdateCamera!\n");
	}

	m_ptPreviousPosition = m_ptCurrentPosition;

#pragma endregion

	//camera reset
	if (pInput->KeyBoard()->IsKeyDown(EKey::R)) // R - Reset Camera to Start Position
	{
		m_tScene.m_mViewMatrix =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			m_vSceneVMStartPos.x, m_vSceneVMStartPos.y, m_vSceneVMStartPos.z, m_vSceneVMStartPos.w,
		};
	}
}

