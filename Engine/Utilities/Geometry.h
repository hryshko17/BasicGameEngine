
#ifndef CGEOMETRY_H
#define CGEOMETRY_H


#include "../../Game/Core/stdafx.h"


struct TVertex3D
{
	float m_fPosition[4];
	float m_fNormal[4];
	float m_fTexCoord[4];
};

struct TPlane3D
{
	Vector3D m_vCenterPosition;
	Vector3D m_vExtents;
	std::vector<unsigned int> m_vIndexArray;
	std::vector<TVertex3D> m_vVertexArray;
};


class CRenderMesh;

class CGeometry
{
private:

public:

	//this is for the face direction of the plane in local space 
	enum EPlaneFaceDir { eUpPlane, eDownPlane, eRightPlane, eLeftPlane, eForwardPlane, eBackwardPlane };

	CGeometry() = default;
	~CGeometry() = default;

	//eFaceDir = the facing direction of the plane
	//vWorldCenterPos = the center position of the plane in world space
	//vExtents = the top left extents of the plane. Should have all positive elements 
	//Notes: The plane's top edge will always be on the positive side of the axis(the axis changes based on the face direction)
	static TPlane3D CreatePlane3D(EPlaneFaceDir eFaceDir, Vector3D vWorldCenterPos, Vector3D vExtents);

	static CRenderMesh* CreateSkybox(std::string szName, Matrix4x4 mCamera, const wchar_t* chFilePath);

};

#endif
