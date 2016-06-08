/*
*	File					Renderer.h
*	Creation Aurthor:		Nick Hryshko
*	Creation Date:			1/17/16
*	Last Modified Date:		1/17/16
*/

#ifndef CRENDERER_H
#define CRENDERER_H

#include "../../Game/Core/stdafx.h"

#define D3D_FEATURE_LEVEL_VERSIONS 2
#define FAR_PLANE 1000.0f
#define NEAR_PLANE 0.1f
#define DEBUG_CAMERA_MOVE_SPEED 30.0f
#define DEBUG_CAMERA_BOOST_SPEED 90.0f
#define DEBUG_CAMERA_VERTICAL_DIVISOR 5.0f
#define DEBUG_CAMERA_ROTATE_DIVISOR 10.0f
#define DEBUG_CAMERA_ROTATE_SENSITIVITY 0.015f
#define DEBUG_CAMERA_VERTICAL_ANGLE_LOCK 45.0f

class CRenderMesh;
class CObject;

struct TScene
{
	XMFLOAT4X4 m_mViewMatrix;
	XMFLOAT4X4 m_mProjMatrix;
};

class CRenderer
{
private:

	static CRenderer* m_pcInstance;

	/*	Debug variables	*/
	bool m_bDebugMode;

	/*	Window variables	*/
	HWND m_Window;
	Vector3D m_vClearColor;

	/*	Scene	*/
	TScene m_tScene;
	Vector3D m_vSceneVMStartPos;//the start position of the scene's view matrix

	/*	DirectX data memebers	*/
	IDXGISwapChain* m_d3dSwapChain;
	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dDeviceContext;
	ID3D11RenderTargetView* m_d3dRenderTargetView;
	ID3D11InputLayout* m_d3dInputLayout;
	ID3D11SamplerState* m_d3dSamplerState;
	D3D11_VIEWPORT m_d3dViewPort;

	/*	Buffers	*/
	//back buffer
	ID3D11Texture2D* m_d3dBackBuffer;
	ID3D11RenderTargetView* m_d3dBackBufferRTV;
	//depth buffer
	ID3D11Texture2D* m_d3dDepthBuffer;
	ID3D11DepthStencilView* m_d3dDepthBufferDSV;
	//Scene buffer
	ID3D11Texture2D* m_d3dSceneTexture;
	ID3D11RenderTargetView* m_d3dSceneBufferRTV;
	ID3D11ShaderResourceView* m_d3dSceneBufferSRV;

	//Constant Buffers
	ID3D11Buffer* m_d3dTransformConstBuffer;
	ID3D11Buffer* m_d3dSceneConstBuffer;

	/*	Shaders	*/
	ID3D11VertexShader* m_vsStandard3D;
	ID3D11PixelShader* m_psStandard3D;
	ID3D11VertexShader* m_vsStandardSkyBox;
	ID3D11PixelShader* m_psStandardSkyBox;


	/*	RenderMeshes	*/
	std::unordered_map<std::string, CRenderMesh*> m_umAllRenderMeshes;

	/*	Debug Camera(free roam)	*/
	bool m_bDebugCamera;
	float m_fCameraAngle;
	POINT m_ptCurrentPosition;
	POINT m_ptPreviousPosition;

	/*	Helper Fuctions	*/
	void CreateTextureDesc(unsigned int unWindowWidth, unsigned int unWindowHeight, DXGI_FORMAT d3dFormat, 
		unsigned int unSampleCount, unsigned int unSampleQuality, unsigned int unBindFlags, unsigned int unCPUFlags,
		unsigned int unMiscFlags, D3D11_TEXTURE2D_DESC& d3dTextureDesc);
	void CreateDSVDesc(unsigned int unFlags, DXGI_FORMAT d3dFormat, D3D11_DSV_DIMENSION d3dDimension,
		D3D11_DEPTH_STENCIL_VIEW_DESC& d3dDSVDesc);
	void ClearScreen(ID3D11RenderTargetView* d3dRTV);
	void ClearDepth(ID3D11DepthStencilView* d3dDSV);
	void MapBuffers(); 
	bool CheckRenderMeshExists(std::string szName);
	void UpdateCamera();

public:

	CRenderer();
	~CRenderer() = default;

	static CRenderer* GetInstance();
	static void DeleteInstance();

	bool Initialize(HWND window);
	bool Update();
	void Shutdown();

	void AddRenderMesh(std::string szName, CRenderMesh* pcNewMesh);
	void AddRenderMesh(CObject* cpObject);


	//Accessors
	ID3D11Device* GetDevice() { return m_d3dDevice; };
	ID3D11DeviceContext* GetDeviceContext() { return m_d3dDeviceContext; };
	ID3D11VertexShader* GetStandard3DVS() { return m_vsStandard3D; };
	ID3D11PixelShader* GetStandard3DPS() { return m_psStandard3D; };
	ID3D11VertexShader* GetStandardSkyBoxVS() { return m_vsStandardSkyBox; };
	ID3D11PixelShader* GetStandardSkyBoxPS() { return m_psStandardSkyBox; };
	ID3D11Buffer* GetTransformConstBuffer() { return m_d3dTransformConstBuffer; };
	XMFLOAT4X4 GetSceneViewMatrix() { return m_tScene.m_mViewMatrix; };


	//Mutators
	void SetSceneViewMatrix(XMFLOAT4X4 viewMatrix) { m_tScene.m_mViewMatrix = viewMatrix; }
};

#endif
