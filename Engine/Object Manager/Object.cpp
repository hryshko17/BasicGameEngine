
#include "Object.h"
#include "../Renderer/RenderMesh.h"
#include "../Utilities/Time.h"

CObject::CObject()
{
	m_szTag = "Nameless Object";
	m_cpRenderMesh = nullptr;
	m_mWorldMatrix.MatrixIdentity();
	m_vVelocity.ZeroOut();
}

CObject::CObject(std::string szTag)
{
	m_szTag = szTag;
	m_cpRenderMesh = nullptr;
	m_mWorldMatrix.MatrixIdentity();
	m_vVelocity.ZeroOut();
}

CObject::CObject(std::string szTag, CRenderMesh* cpRenderMesh, Matrix4x4 mWorldMatrix, Vector3D vVelocity)
{
	m_szTag = szTag;
	m_cpRenderMesh = cpRenderMesh;
	m_mWorldMatrix = mWorldMatrix;
	m_vVelocity = vVelocity;
}

CObject::~CObject()
{
	//SAFE_DELETE(m_cpRenderMesh);
}

CObject::CObject(const CObject& cObject)
{
	m_szTag = cObject.m_szTag;
	m_cpRenderMesh = cObject.m_cpRenderMesh;
	m_mWorldMatrix = cObject.m_mWorldMatrix;
}

CObject& CObject::operator=(const CObject& cObject)
{
	if (this != &cObject)
	{
		m_szTag = cObject.m_szTag;
		m_cpRenderMesh = cObject.m_cpRenderMesh;
		m_mWorldMatrix = cObject.m_mWorldMatrix;
	}

	return *this;
}

bool CObject::UpdatePosition()
{
	CTime* cpTime = CTime::GetInstance();

	//update the world matix
	Matrix4x4 mOldWorld = m_mWorldMatrix;
	XMMATRIX mTranslate = XMMatrixTranslation(m_vVelocity.x * cpTime->DeltaTime(), m_vVelocity.y * cpTime->DeltaTime(), m_vVelocity.z * cpTime->DeltaTime());
	XMStoreFloat4x4(&CMathLib::MatrixConvert4x4(m_mWorldMatrix), XMMatrixMultiply(mTranslate, XMLoadFloat4x4(&CMathLib::MatrixConvert4x4(m_mWorldMatrix))));

	//update the rendermesh
	if (m_cpRenderMesh != nullptr)
		m_cpRenderMesh->SetWorldMatrix(m_mWorldMatrix);

	//update velocity
	Vector3D vVelocity = GetWorldVelocity();

	return true;
}

Vector3D CObject::GetWorldVelocity()
{
	Matrix4x4 mOldWorld = m_mWorldMatrix;
	Matrix4x4 mNewWorld; mNewWorld.MatrixIdentity();

	//get local velocity as world velocity
	XMMATRIX mTranslate = XMMatrixTranslation(m_vVelocity.x, m_vVelocity.y, m_vVelocity.z);
	XMStoreFloat4x4(&CMathLib::MatrixConvert4x4(mNewWorld), 
		XMMatrixMultiply(mTranslate, XMLoadFloat4x4(&CMathLib::MatrixConvert4x4(m_mWorldMatrix))));

	//update velocity
	Vector3D vVelocity;
	vVelocity.Set(mNewWorld.e41 - mOldWorld.e41, mNewWorld.e42 - mOldWorld.e42, mNewWorld.e43 - mOldWorld.e43, 0.0f);

	return vVelocity;
}

void CObject::SetWorldVelocity(Vector3D vVelocity)
{
	XMMATRIX mWorld = XMLoadFloat4x4(&CMathLib::MatrixConvert4x4(m_mWorldMatrix));
	mWorld.r[0].m128_f32[3] = 0.0f;
	mWorld.r[1].m128_f32[3] = 0.0f;
	mWorld.r[2].m128_f32[3] = 0.0f;
	mWorld.r[3].m128_f32[0] = 0.0f;
	mWorld.r[3].m128_f32[1] = 0.0f;
	mWorld.r[3].m128_f32[2] = 0.0f;
	mWorld = XMMatrixInverse(&XMMatrixDeterminant(mWorld), mWorld);

	Matrix4x4 mLocal; mLocal.MatrixIdentity();
	XMMATRIX mTranslate = XMMatrixTranslation(vVelocity.x, vVelocity.y, vVelocity.z);

	XMStoreFloat4x4(&CMathLib::MatrixConvert4x4(mLocal), XMMatrixMultiply(mTranslate, mWorld));

	//update velocity
	Vector3D vLocalVelocity;
	vLocalVelocity.Set(mLocal.e41, mLocal.e42, mLocal.e43, 0.0f);
	m_vVelocity = vLocalVelocity;
}
