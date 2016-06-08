/*
*	File					Renderer.h
*	Creation Aurthor:		Nick Hryshko
*	Creation Date:			1/17/16
*	Last Modified Date:		1/21/16
*/

#include "RenderMesh.h"
#include "Renderer.h"
#include "../Asset Manager/AssetManager.h"

CRenderMesh::CRenderMesh(std::vector<unsigned int> vIndexArray, std::vector<TVertex3D> vVertexArray, std::string szName, Vector3D vWorldPosition, ID3D11VertexShader* d3dVS, ID3D11PixelShader* d3dPS, const wchar_t* chtexturePath, bool bRender)
{
	m_bRender = bRender;
	m_szName = szName;
	m_vsVertexShader = d3dVS;
	m_psPixelShader = d3dPS;
	m_d3dBaseTexture = nullptr;

	//set world matrix
	m_mWorldMatrix.MatrixIdentity();
	m_mWorldMatrix.SetMatrixPosition(vWorldPosition.x, vWorldPosition.y, vWorldPosition.z);

	//create index buffer
	m_unIndexCount = vIndexArray.size();

	D3D11_SUBRESOURCE_DATA d3dIndexBufferResource;
	ZeroMemory(&d3dIndexBufferResource, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dIndexBufferResource.pSysMem = &vIndexArray[0];
	d3dIndexBufferResource.SysMemPitch = NULL;
	d3dIndexBufferResource.SysMemSlicePitch = NULL;

	D3D11_BUFFER_DESC d3dIndexBufferDesc;
	ZeroMemory(&d3dIndexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d3dIndexBufferDesc.ByteWidth = (UINT)vIndexArray.size() * sizeof(unsigned int);
	d3dIndexBufferDesc.CPUAccessFlags = NULL;
	d3dIndexBufferDesc.MiscFlags = NULL;
	d3dIndexBufferDesc.StructureByteStride = sizeof(unsigned int);
	d3dIndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	CRenderer::GetInstance()->GetDevice()->CreateBuffer(&d3dIndexBufferDesc, &d3dIndexBufferResource, &m_d3dIndexBuffer);

	//create vertex buffer
	D3D11_SUBRESOURCE_DATA d3dVertexBufferResource;
	ZeroMemory(&d3dVertexBufferResource, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dVertexBufferResource.pSysMem = &vVertexArray[0];
	d3dVertexBufferResource.SysMemPitch = NULL;
	d3dVertexBufferResource.SysMemSlicePitch = NULL;

	D3D11_BUFFER_DESC d3dVertexBufferDesc;
	ZeroMemory(&d3dVertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dVertexBufferDesc.ByteWidth = (UINT)vVertexArray.size() * sizeof(TVertex3D);
	d3dVertexBufferDesc.CPUAccessFlags = NULL;
	d3dVertexBufferDesc.MiscFlags = NULL;
	d3dVertexBufferDesc.StructureByteStride = sizeof(TVertex3D);
	d3dVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	CRenderer::GetInstance()->GetDevice()->CreateBuffer(&d3dVertexBufferDesc, &d3dVertexBufferResource, &m_d3dVertexBuffer);

	//load texture
	SetTexture(chtexturePath);
}

CRenderMesh::~CRenderMesh()
{
	SAFE_RELEASE(m_d3dIndexBuffer);
	SAFE_RELEASE(m_d3dVertexBuffer);
}

CRenderMesh::CRenderMesh(const CRenderMesh& mesh)
{
	m_szName = mesh.m_szName;
	m_bRender = mesh.m_bRender;
	m_vsVertexShader = mesh.m_vsVertexShader;
	m_psPixelShader = mesh.m_psPixelShader;
	m_mWorldMatrix = mesh.m_mWorldMatrix;

}

CRenderMesh& CRenderMesh::operator=(const CRenderMesh& mesh)
{
	if (this != &mesh)
	{
		m_szName = mesh.m_szName;
		m_bRender = mesh.m_bRender;
		m_vsVertexShader = mesh.m_vsVertexShader;
		m_psPixelShader = mesh.m_psPixelShader;
		m_mWorldMatrix = mesh.m_mWorldMatrix;

	}

	return *this;
}

void CRenderMesh::Draw()
{
	CRenderer* pcRender = CRenderer::GetInstance();

	//set index/vertex buffers
	unsigned int unOffset = 0;
	unsigned int unStrideSize = sizeof(TVertex3D);

	pcRender->GetDeviceContext()->IASetIndexBuffer(m_d3dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pcRender->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_d3dVertexBuffer, &unStrideSize, &unOffset);

	//map buffers 
	MapBuffers();

	//set shaders
	pcRender->GetDeviceContext()->VSSetShader(m_vsVertexShader, 0, 0);
	pcRender->GetDeviceContext()->PSSetShader(m_psPixelShader, 0, 0);

	//set shader resource views
	pcRender->GetDeviceContext()->PSSetShaderResources(0, 1, &m_d3dBaseTexture);

	//draw
	pcRender->GetDeviceContext()->DrawIndexed(m_unIndexCount, 0, 0);
}

void CRenderMesh::SetTexture(const wchar_t* chTexturePath)
{
	CAssetManager* pAssetMan = CAssetManager::GetInstance();
	if (pAssetMan->CheckTextureExists(chTexturePath) == false)
	{
		//does not exist. load texture through asset manager and apply it
		if (pAssetMan->LoadTexture(chTexturePath, m_d3dBaseTexture) == true)
		{
			//texture loaded succesfully so grab it and apply it
			m_d3dBaseTexture = pAssetMan->GetTextureFromList(std::wstring(chTexturePath));
			std::wstring szFailed = L"Texture Set Successfully! File: ";
			szFailed += std::wstring(chTexturePath) + L"\n";
			DEBUG_OUTPUTW(szFailed);
		}
		else
		{
			//texture failed to load. apply error texture
			bool error = pAssetMan->LoadTexture(L"../Assets/Art/2D/Textures/Error.dds", m_d3dBaseTexture);
			if (pAssetMan->LoadTexture(L"../Assets/Art/2D/Textures/Error.dds", m_d3dBaseTexture) == false)
			{
				//failed to load error texture
				DEBUG_OUTPUT("Error Texture Failed to load. RenderMesh::SetTexture failed!\n");
				return;
			}

			m_d3dBaseTexture = pAssetMan->GetTextureFromList(L"../Assets/Art/2D/Textures/Error.dds");
			std::wstring szFailed = L"Texture Set Failed! Applied Error Texture instead of File: ";
			szFailed += std::wstring(chTexturePath) + L"\n";
			DEBUG_OUTPUTW(szFailed);
		}
	}
	else
	{
		//it alrdy exists so grab it
		m_d3dBaseTexture = pAssetMan->GetTextureFromList(std::wstring(chTexturePath));
		std::wstring szFailed = L"Texture Set Successfully! File: ";
		szFailed += std::wstring(chTexturePath) + L"\n";
		DEBUG_OUTPUTW(szFailed);
	}
}



//Private Functions
void CRenderMesh::MapBuffers()
{
	CRenderer* pcRender = CRenderer::GetInstance();

	//Transform Buffer
	D3D11_MAPPED_SUBRESOURCE d3dMapSubresource;
	ZeroMemory(&d3dMapSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	pcRender->GetDeviceContext()->Map(pcRender->GetTransformConstBuffer(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &d3dMapSubresource);
	memcpy(d3dMapSubresource.pData, &CMathLib::MatrixConvert4x4(m_mWorldMatrix), sizeof(CMathLib::MatrixConvert4x4(m_mWorldMatrix)));
	pcRender->GetDeviceContext()->Unmap(pcRender->GetTransformConstBuffer(), 0);

}


//Accessors
Vector3D CRenderMesh::GetPosition()
{
	Vector3D temp;
	temp.x = m_mWorldMatrix.e41;
	temp.y = m_mWorldMatrix.e42;
	temp.z = m_mWorldMatrix.e43;
	temp.w = 0.0f;
	return temp;
}

//Mutators
void CRenderMesh::SetPosition(Vector3D vPos)
{
	m_mWorldMatrix.e41 = vPos.x;
	m_mWorldMatrix.e42 = vPos.y;
	m_mWorldMatrix.e43 = vPos.z;
}
