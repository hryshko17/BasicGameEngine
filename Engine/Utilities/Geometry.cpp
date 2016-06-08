
#include "Geometry.h"
#include "../Renderer/RenderMesh.h"
#include "../Renderer/Renderer.h"

TPlane3D CGeometry::CreatePlane3D(EPlaneFaceDir eFaceDir, Vector3D vWorldCenterPos, Vector3D vExtents)
{
	TPlane3D newPlane;
	newPlane.m_vCenterPosition = vWorldCenterPos;
	newPlane.m_vExtents.x = abs(vExtents.x);
	newPlane.m_vExtents.y = abs(vExtents.y);
	newPlane.m_vExtents.z = abs(vExtents.z);

	//set up index array
	newPlane.m_vIndexArray.push_back(0);
	newPlane.m_vIndexArray.push_back(1);
	newPlane.m_vIndexArray.push_back(2);

	newPlane.m_vIndexArray.push_back(0);
	newPlane.m_vIndexArray.push_back(2);
	newPlane.m_vIndexArray.push_back(3);

#pragma region Vertex Array Creation

	switch (eFaceDir)
	{
	case CGeometry::eUpPlane:
	{
#pragma region UpWard Plane

		newPlane.m_vVertexArray.push_back(//top left
		{
			{ newPlane.m_vCenterPosition.x - newPlane.m_vExtents.x, newPlane.m_vCenterPosition.y, newPlane.m_vCenterPosition.z + newPlane.m_vExtents.z, 1.0f },	//position
			{ 0, 1, 0 },	//normals
			{ 0, 0 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//top right
		{
			{ newPlane.m_vCenterPosition.x + newPlane.m_vExtents.x, newPlane.m_vCenterPosition.y, newPlane.m_vCenterPosition.z + newPlane.m_vExtents.z, 1.0f },	//position
			{ 0, 1, 0 },	//normals
			{ 1, 0 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//bottom right
		{
			{ newPlane.m_vCenterPosition.x + newPlane.m_vExtents.x, newPlane.m_vCenterPosition.y, newPlane.m_vCenterPosition.z - newPlane.m_vExtents.z, 1.0f },	//position
			{ 0, 1, 0 },	//normals
			{ 1, 1 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//bottom left
		{
			{ newPlane.m_vCenterPosition.x - newPlane.m_vExtents.x, newPlane.m_vCenterPosition.y, newPlane.m_vCenterPosition.z - newPlane.m_vExtents.z, 1.0f },	//position
			{ 0, 1, 0 },	//normals
			{ 0, 1 }		//tex coords
		});

#pragma endregion
	}
	break;
	case CGeometry::eDownPlane:
	{
#pragma region DownWard Plane

		newPlane.m_vVertexArray.push_back(//top left
		{
			{ newPlane.m_vCenterPosition.x + newPlane.m_vExtents.x, newPlane.m_vCenterPosition.y, newPlane.m_vCenterPosition.z + newPlane.m_vExtents.z, 1.0f },	//position
			{ 0, -1, 0 },	//normals
			{ 0, 0 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//top right
		{
			{ newPlane.m_vCenterPosition.x - newPlane.m_vExtents.x, newPlane.m_vCenterPosition.y, newPlane.m_vCenterPosition.z + newPlane.m_vExtents.z, 1.0f },	//position
			{ 0, -1, 0 },	//normals
			{ 1, 0 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//bottom right
		{
			{ newPlane.m_vCenterPosition.x - newPlane.m_vExtents.x, newPlane.m_vCenterPosition.y, newPlane.m_vCenterPosition.z - newPlane.m_vExtents.z, 1.0f },	//position
			{ 0, -1, 0 },	//normals
			{ 1, 1 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//bottom left
		{
			{ newPlane.m_vCenterPosition.x + newPlane.m_vExtents.x, newPlane.m_vCenterPosition.y, newPlane.m_vCenterPosition.z - newPlane.m_vExtents.z, 1.0f },	//position
			{ 0, -1, 0 },	//normals
			{ 0, 1 }		//tex coords
		});

#pragma endregion
	}
	break;
	case CGeometry::eRightPlane:
	{
#pragma region RightWard Plane

		newPlane.m_vVertexArray.push_back(//top left
		{
			{ newPlane.m_vCenterPosition.x, newPlane.m_vCenterPosition.y + newPlane.m_vExtents.y, newPlane.m_vCenterPosition.z - newPlane.m_vExtents.z, 1.0f },	//position
			{ 1, 0, 0 },	//normals
			{ 0, 0 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//top right
		{
			{ newPlane.m_vCenterPosition.x, newPlane.m_vCenterPosition.y + newPlane.m_vExtents.y, newPlane.m_vCenterPosition.z + newPlane.m_vExtents.z, 1.0f },	//position
			{ 1, 0, 0 },	//normals
			{ 1, 0 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//bottom right
		{
			{ newPlane.m_vCenterPosition.x, newPlane.m_vCenterPosition.y - newPlane.m_vExtents.y, newPlane.m_vCenterPosition.z + newPlane.m_vExtents.z, 1.0f },	//position
			{ 1, 0, 0 },	//normals
			{ 1, 1 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//bottom left
		{
			{ newPlane.m_vCenterPosition.x, newPlane.m_vCenterPosition.y - newPlane.m_vExtents.y, newPlane.m_vCenterPosition.z - newPlane.m_vExtents.z, 1.0f },	//position
			{ 1, 0, 0 },	//normals
			{ 0, 1 }		//tex coords
		});

#pragma endregion
	}
	break;
	case CGeometry::eLeftPlane:
	{
#pragma region LeftWard Plane

		newPlane.m_vVertexArray.push_back(//top left
		{
			{ newPlane.m_vCenterPosition.x, newPlane.m_vCenterPosition.y + newPlane.m_vExtents.y, newPlane.m_vCenterPosition.z + newPlane.m_vExtents.z, 1.0f },	//position
			{ -1, 0, 0 },	//normals
			{ 0, 0 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//top right
		{
			{ newPlane.m_vCenterPosition.x, newPlane.m_vCenterPosition.y + newPlane.m_vExtents.y, newPlane.m_vCenterPosition.z - newPlane.m_vExtents.z, 1.0f },	//position
			{ -1, 0, 0 },	//normals
			{ 1, 0 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//bottom right
		{
			{ newPlane.m_vCenterPosition.x, newPlane.m_vCenterPosition.y - newPlane.m_vExtents.y, newPlane.m_vCenterPosition.z - newPlane.m_vExtents.z, 1.0f },	//position
			{ -1, 0, 0 },	//normals
			{ 1, 1 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//bottom left
		{
			{ newPlane.m_vCenterPosition.x, newPlane.m_vCenterPosition.y - newPlane.m_vExtents.y, newPlane.m_vCenterPosition.z + newPlane.m_vExtents.z, 1.0f },	//position
			{ -1, 0, 0 },	//normals
			{ 0, 1 }		//tex coords
		});

#pragma endregion
	}
	break;
	case CGeometry::eForwardPlane:
	{
#pragma region Forward Plane

		newPlane.m_vVertexArray.push_back(//top left
		{
			{ newPlane.m_vCenterPosition.x - newPlane.m_vExtents.x, newPlane.m_vCenterPosition.y + newPlane.m_vExtents.y, newPlane.m_vCenterPosition.z, 1.0f },	//position
			{ 0, 0, -1 },	//normals
			{ 0, 0 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//top right
		{
			{ newPlane.m_vCenterPosition.x + newPlane.m_vExtents.x, newPlane.m_vCenterPosition.y + newPlane.m_vExtents.y, newPlane.m_vCenterPosition.z, 1.0f },	//position
			{ 0, 0, -1 },	//normals
			{ 1, 0 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//bottom right
		{
			{ newPlane.m_vCenterPosition.x + newPlane.m_vExtents.x, newPlane.m_vCenterPosition.y - newPlane.m_vExtents.y, newPlane.m_vCenterPosition.z, 1.0f },	//position
			{ 0, 0, -1 },	//normals
			{ 1, 1 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//bottom left
		{
			{ newPlane.m_vCenterPosition.x - newPlane.m_vExtents.x, newPlane.m_vCenterPosition.y - newPlane.m_vExtents.y, newPlane.m_vCenterPosition.z, 1.0f },	//position
			{ 0, 0, -1 },	//normals
			{ 0, 1 }		//tex coords
		});

#pragma endregion
	}
	break;
	case CGeometry::eBackwardPlane:
	{
#pragma region Backward Plane

		newPlane.m_vVertexArray.push_back(//top left
		{
			{ newPlane.m_vCenterPosition.x + newPlane.m_vExtents.x, newPlane.m_vCenterPosition.y + newPlane.m_vExtents.y, newPlane.m_vCenterPosition.z, 1.0f },	//position
			{ 0, 0, 1 },	//normals
			{ 0, 0 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//top right
		{
			{ newPlane.m_vCenterPosition.x - newPlane.m_vExtents.x, newPlane.m_vCenterPosition.y + newPlane.m_vExtents.y, newPlane.m_vCenterPosition.z, 1.0f },	//position
			{ 0, 0, 1 },	//normals
			{ 1, 0 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//bottom right
		{
			{ newPlane.m_vCenterPosition.x - newPlane.m_vExtents.x, newPlane.m_vCenterPosition.y - newPlane.m_vExtents.y, newPlane.m_vCenterPosition.z, 1.0f },	//position
			{ 0, 0, 1 },	//normals
			{ 1, 1 }		//tex coords
		});

		newPlane.m_vVertexArray.push_back(//bottom left
		{
			{ newPlane.m_vCenterPosition.x + newPlane.m_vExtents.x, newPlane.m_vCenterPosition.y - newPlane.m_vExtents.y, newPlane.m_vCenterPosition.z, 1.0f },	//position
			{ 0, 0, 1 },	//normals
			{ 0, 1 }		//tex coords
		});

#pragma endregion
	}
	break;
	default:
		break;
	}

#pragma endregion

	return newPlane;
}

CRenderMesh* CGeometry::CreateSkybox(std::string szName, Matrix4x4 mCamera, const wchar_t* chFilePath)
{
	std::vector<unsigned int> viSkyBoxIndecies;
	std::vector<TVertex3D> vtSkyBoxVertecies;

#pragma region Indicies Set Up

	//front face
	viSkyBoxIndecies.push_back(1);
	viSkyBoxIndecies.push_back(0);
	viSkyBoxIndecies.push_back(3);
	viSkyBoxIndecies.push_back(1);
	viSkyBoxIndecies.push_back(3);
	viSkyBoxIndecies.push_back(2);

	//left face
	viSkyBoxIndecies.push_back(0);
	viSkyBoxIndecies.push_back(4);
	viSkyBoxIndecies.push_back(7);
	viSkyBoxIndecies.push_back(0);
	viSkyBoxIndecies.push_back(7);
	viSkyBoxIndecies.push_back(3);

	//right face
	viSkyBoxIndecies.push_back(5);
	viSkyBoxIndecies.push_back(1);
	viSkyBoxIndecies.push_back(2);
	viSkyBoxIndecies.push_back(5);
	viSkyBoxIndecies.push_back(2);
	viSkyBoxIndecies.push_back(6);

	//back face
	viSkyBoxIndecies.push_back(4);
	viSkyBoxIndecies.push_back(5);
	viSkyBoxIndecies.push_back(6);
	viSkyBoxIndecies.push_back(4);
	viSkyBoxIndecies.push_back(6);
	viSkyBoxIndecies.push_back(7);

	//top face
	viSkyBoxIndecies.push_back(0);
	viSkyBoxIndecies.push_back(1);
	viSkyBoxIndecies.push_back(5);
	viSkyBoxIndecies.push_back(0);
	viSkyBoxIndecies.push_back(5);
	viSkyBoxIndecies.push_back(4);

	//bottom face
	viSkyBoxIndecies.push_back(7);
	viSkyBoxIndecies.push_back(6);
	viSkyBoxIndecies.push_back(2);
	viSkyBoxIndecies.push_back(7);
	viSkyBoxIndecies.push_back(2);
	viSkyBoxIndecies.push_back(3);

#pragma endregion

	//set vertecies (only need to set positions)
	vtSkyBoxVertecies.push_back({ { -1.0f, 1.0f, -1.0f, 1.0f } });	// V0	front top left
	vtSkyBoxVertecies.push_back({ { 1.0f, 1.0f, -1.0f, 1.0f } });	// V1	front top right
	vtSkyBoxVertecies.push_back({ { 1.0f, -1.0f, -1.0f, 1.0f } });	// V2	front bottom right
	vtSkyBoxVertecies.push_back({ { -1.0f, -1.0f, -1.0f, 1.0f } });	// V3	front bottom left
	vtSkyBoxVertecies.push_back({ { -1.0f, 1.0f, 1.0f, 1.0f } });	// V4	back top left
	vtSkyBoxVertecies.push_back({ { 1.0f, 1.0f, 1.0f, 1.0f } });	// V5	back top right
	vtSkyBoxVertecies.push_back({ { 1.0f, -1.0f, 1.0f, 1.0f } });	// V6	back bottom right
	vtSkyBoxVertecies.push_back({ { -1.0f, -1.0f, 1.0f, 1.0f } });	// V7	back bottom left

	//set world pos TODO:: create camera and lock skybox to camera
	Vector3D vWorldPos = { mCamera.e41 , mCamera.e42, mCamera.e43, mCamera.e44 };

	//create rendermesh
	CRenderMesh* pcSkyBoxMesh = new CRenderMesh(viSkyBoxIndecies, vtSkyBoxVertecies, szName, vWorldPos,
		CRenderer::GetInstance()->GetStandardSkyBoxVS(), CRenderer::GetInstance()->GetStandardSkyBoxPS(), chFilePath);
	return pcSkyBoxMesh;
}
