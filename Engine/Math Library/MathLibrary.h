
#ifndef CMATHLIB_H
#define CMATHLIB_H

#include "Vector3D.h"
#include "Matrix4x4.h"

#include <d3d11.h>
#include <DirectXMath.h>
#pragma comment(lib, "d3d11.lib")
using namespace DirectX;

#define PI 3.14159f


class CMathLib
{
public:

	CMathLib() = default;
	~CMathLib() = default;

	//Returns the greater of the 2 parameters
	static float Max(float fA, float fB);

	//Returns the lesser of the 2 parameters
	static float Min(float fA, float fB);

	//Returns the converted parameter as radians
	static float DegreesToRadians(float fDegree);

	//Returns the converted parameter as degrees
	static float RadiansToDegrees(float fRadians);

	//Cross Product
	static Vector3D CrossProduct(Vector3D vVectorA, Vector3D vVectorB);

	//Dot Product
	static float DotProduct(Vector3D vVectorA, Vector3D vVectorB);

	//converts a xmfloat4x4 into a Matrix4x4
	static Matrix4x4 MatrixConvert(XMFLOAT4X4 mMatrix);

	//converts a xmmatrix int0 a matrix4x4
	static Matrix4x4 MatrixConvert(XMMATRIX& mMatrix);

	//converts a Matrix4x4 into a xmfloat4x4
	static XMFLOAT4X4 MatrixConvert4x4(Matrix4x4 mMatrix);

	//converts a matrix4x4 into a xmmatrix
	static XMMATRIX MatrixConvertMatrix(Matrix4x4 mMatrix);

	/*	Matrix Math	*/

	//Matrix Create Functions
	static Matrix4x4 CreateZeroMatrix();//creates a zero 4x4 matrix
	static Matrix4x4 CreateMatrixTranslate(float X, float Y, float Z);
	static Matrix4x4 CreateMatrixScaler(float X, float Y, float Z);
	static Matrix4x4 CreateMatrixRotateX(float fDegree);
	static Matrix4x4 CreateMatrixRotateY(float fDegree);
	static Matrix4x4 CreateMatrixRotateZ(float fDegree);
	static Matrix4x4 MatrixAddMatrix(Matrix4x4 mMatrixA, Matrix4x4 mMatrixB);
	static Matrix4x4 MatrixSubtractMatrix(Matrix4x4 mMatrixA, Matrix4x4 mMatrixB);
	static Matrix4x4 MatrixMultiplyMatrix(Matrix4x4 mMatrixA, Matrix4x4 mMatrixB);

	static Vector3D MatrixMultiplyVector(Matrix4x4 mMatrix, Vector3D vVector);
	static void VectorMultiplyMatrix(Vector3D& vVector, Matrix4x4 mMatrix);
	static Vector3D VectorMultiplyMatrix(Vector3D vVector, Matrix4x4 mMatrix);
	static void MatrixScale(Matrix4x4& mMatrix, Vector3D vScale);

	//Matrix Local Functions
	static void TranslateLocalX(Matrix4x4& mMatrix, float fDistance);
	static void TranslateLocalX(XMFLOAT4X4& mMatrix, float fDistance);

	static void TranslateLocalY(Matrix4x4& mMatrix, float fDistance);
	static void TranslateLocalY(XMFLOAT4X4& mMatrix, float fDistance);

	static void TranslateLocalZ(Matrix4x4& mMatrix, float fDistance);
	static void TranslateLocalZ(XMFLOAT4X4& mMatrix, float fDistance);

	static void RotateLocalX(Matrix4x4& mMatrix, float fAngle);
	static void RotateLocalX(XMFLOAT4X4& mMatrix, float fAngle);

	static void RotateLocalY(Matrix4x4& mMatrix, float fAngle);
	static void RotateLocalY(XMFLOAT4X4& mMatrix, float fAngle);

	static void RotateLocalZ(Matrix4x4& mMatrix, float fAngle);
	static void RotateLocalZ(XMFLOAT4X4& mMatrix, float fAngle);


	//Matrix Global Functions
	static void TranslateGlobalX(Matrix4x4& mMatrix, float fDistance);
	static void TranslateGlobalX(XMFLOAT4X4& mMatrix, float fDistance);

	static void TranslateGlobalY(Matrix4x4& mMatrix, float fDistance);
	static void TranslateGlobalY(XMFLOAT4X4& mMatrix, float fDistance);

	static void TranslateGlobalZ(Matrix4x4& mMatrix, float fDistance);
	static void TranslateGlobalZ(XMFLOAT4X4& mMatrix, float fDistance);

	static void RotateGlobalX(Matrix4x4& mMatrix, float fAngle);
	static void RotateGlobalX(XMFLOAT4X4& mMatrix, float fAngle);

	static void RotateGlobalY(Matrix4x4& mMatrix, float fAngle);
	static void RotateGlobalY(XMFLOAT4X4& mMatrix, float fAngle);

	static void RotateGlobalZ(Matrix4x4& mMatrix, float fAngle);
	static void RotateGlobalZ(XMFLOAT4X4& mMatrix, float fAngle);

};

#endif