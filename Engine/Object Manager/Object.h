
#ifndef COBJECT_H
#define COBJECT_H

#include "../../Game/Core/stdafx.h"

class CRenderMesh;

class CObject
{
protected:

	std::string m_szTag;
	CRenderMesh* m_cpRenderMesh;
	Matrix4x4 m_mWorldMatrix;
	Vector3D m_vVelocity;

public:

	CObject();
	CObject(std::string szTag);
	CObject(std::string szTag, CRenderMesh* cpRenderMesh, Matrix4x4 mWorldMatrix, Vector3D vVelocity);
	CObject(const CObject& cObject);
	CObject& operator=(const CObject& cObject);

	virtual ~CObject();
	virtual bool Update() = 0;
	virtual bool UpdatePosition();

	/*	Accessors	*/
	std::string GetTag() { return m_szTag; }
	CRenderMesh* GetRenderMesh() { return m_cpRenderMesh; }
	Matrix4x4 GetWorldMatrix() {return m_mWorldMatrix; }
	Vector3D GetLocalVelocity() { return m_vVelocity; }
	virtual Vector3D GetWorldVelocity();//gets the local velocity as world velocity

	/*	Mutators	*/
	void SetTag(std::string szTag) { m_szTag = szTag; }
	void SetRenderMesh(CRenderMesh* cpRenderMesh) { m_cpRenderMesh = cpRenderMesh; }
	void SetWorldMatrix(Matrix4x4 mWorld) { m_mWorldMatrix = mWorld; }
	void SetLocalVelocity(Vector3D vVelocity) { m_vVelocity = vVelocity; }
	virtual void SetWorldVelocity(Vector3D vVelocity);//sets the local velocity to world velocity

};

#endif
