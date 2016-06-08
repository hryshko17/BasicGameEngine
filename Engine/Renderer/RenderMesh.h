/*
*	File					Renderer.h
*	Creation Aurthor:		Nick Hryshko
*	Creation Date:			1/17/16
*	Last Modified Date:		1/21/16
*/

#ifndef CRENDERMESH_H
#define CRENDERMESH_H

#include "../../Game/Core/stdafx.h"
#include "../Utilities/Geometry.h"

class CRenderMesh
{
private:

	bool m_bRender;//if true then this object will get rendered
	std::string m_szName;
	Matrix4x4 m_mWorldMatrix;
	
	//buffers
	unsigned int m_unIndexCount;
	ID3D11Buffer* m_d3dIndexBuffer;
	ID3D11Buffer* m_d3dVertexBuffer;

	//shaders
	ID3D11VertexShader* m_vsVertexShader;
	ID3D11PixelShader* m_psPixelShader;

	ID3D11ShaderResourceView* m_d3dBaseTexture;


	//helper functions
	void MapBuffers();

public:

	CRenderMesh() = default;
	CRenderMesh(std::vector<unsigned int> vIndexArray, std::vector<TVertex3D> vVertexArray, std::string szName, 
		Vector3D vWorldPosition, ID3D11VertexShader* d3dVS, ID3D11PixelShader* d3dPS, const wchar_t* chtexturePath,
		bool bRender = true);

	~CRenderMesh();
	CRenderMesh(const CRenderMesh& mesh);
	CRenderMesh& operator=(const CRenderMesh& mesh);

	void Draw();
	void SetTexture(const wchar_t* chTexturePath);

	//Accessors
	bool GetRenderable() { return m_bRender; };
	std::string GetName() { return m_szName; };
	Matrix4x4 GetWorldMatrix() { return m_mWorldMatrix; };
	Vector3D GetPosition();
	ID3D11ShaderResourceView* GetBaseTexture() { return m_d3dBaseTexture; };

	//Mutators
	void SetRenderable(bool bRender){ m_bRender = bRender; };
	void SetWorldMatrix(Matrix4x4 mMatrix) { m_mWorldMatrix = mMatrix; };
	void SetPosition(Vector3D vPos);
};

#endif
