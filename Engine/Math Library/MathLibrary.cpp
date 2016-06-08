
#include "MathLibrary.h"


Matrix4x4 CMathLib::MatrixConvert(XMFLOAT4X4 matrix)
{
	Matrix4x4 newMatrix;
	for (unsigned int x = 0; x < MAX_MATRIX_ROW_ELEMENTS; x++)
	{
		for (unsigned int y = 0; y < MAX_MATRIX_ROW_ELEMENTS; y++)
			newMatrix.m[x][y] = matrix.m[x][y];
	}

	return newMatrix;
}

Matrix4x4 CMathLib::MatrixConvert(XMMATRIX& mMatrix)
{
	Matrix4x4 newMatrix;
	for (unsigned int x = 0; x < MAX_MATRIX_ROW_ELEMENTS; x++)
	{
		for (unsigned int y = 0; y < MAX_MATRIX_ROW_ELEMENTS; y++)
			newMatrix.m[x][y] = mMatrix.r[x].m128_f32[y];
	}

	return newMatrix;
}

XMFLOAT4X4 CMathLib::MatrixConvert4x4(Matrix4x4 matrix)
{
	XMFLOAT4X4 newMatrix;
	for (unsigned int x = 0; x < MAX_MATRIX_ROW_ELEMENTS; x++)
	{
		for (unsigned int y = 0; y < MAX_MATRIX_ROW_ELEMENTS; y++)
			newMatrix.m[x][y] = matrix.m[x][y];
	}

	return newMatrix;
}

XMMATRIX CMathLib::MatrixConvertMatrix(Matrix4x4 mMatrix)
{
	XMMATRIX newMatrix;
	for (unsigned int x = 0; x < MAX_MATRIX_ROW_ELEMENTS; x++)
	{
		for (unsigned int y = 0; y < MAX_MATRIX_ROW_ELEMENTS; y++)
			newMatrix.r[x].m128_f32[y] = mMatrix.m[x][y];
	}

	return newMatrix;
}



/*	public static function	*/

float CMathLib::Max(float a, float b)
{
	return (a > b) ? a : b;
}

float CMathLib::Min(float a, float b)
{
	return (a < b) ? a : b;
}

float CMathLib::DegreesToRadians(float degree)
{
	return degree * PI / 180.0f;
}

float CMathLib::RadiansToDegrees(float radians)
{
	return radians * 180.0f / PI;
}

Vector3D CMathLib::CrossProduct(Vector3D vectorA, Vector3D vectorB)
{
	Vector3D tempVect =
	{
		(vectorA.y * vectorB.z) - (vectorB.y * vectorA.z), //x
		(vectorA.x * vectorB.z) - (vectorB.x * vectorA.z), //y
		(vectorA.x * vectorB.y) - (vectorB.x * vectorA.y), //z
		0 //w
	};
	return tempVect;
}

float CMathLib::DotProduct(Vector3D vectorA, Vector3D vectorB)
{
	return (vectorA.x * vectorB.x) + (vectorA.y * vectorB.y) + (vectorA.z * vectorB.z) + (vectorA.w * vectorB.w);
}


Matrix4x4 CMathLib::CreateZeroMatrix()
{
	Matrix4x4 tempMatrix;
	for (unsigned int i = 0; i < MAX_MATRIX_ELEMENTS; i++)
		tempMatrix.e[i] = 0.0f;

	return tempMatrix;
}

Matrix4x4 CMathLib::CreateMatrixTranslate(float X, float Y, float Z)
{
	Matrix4x4 tempMatrix;
	tempMatrix.MatrixIdentity();
	tempMatrix.SetMatrixPosition(X, Y, Z);
	return tempMatrix;
}

Matrix4x4 CMathLib::CreateMatrixScaler(float X, float Y, float Z)
{
	Matrix4x4 tempMatrix;
	tempMatrix.MatrixIdentity();
	tempMatrix.e11 = X;
	tempMatrix.e22 = Y;
	tempMatrix.e33 = Z;

	return tempMatrix;
}

Matrix4x4 CMathLib::CreateMatrixRotateX(float degree)
{
	float tempCOS = cos(DegreesToRadians(degree));
	float tempSIN = sin(DegreesToRadians(degree));

	Matrix4x4 tempMatrix;
	tempMatrix.MatrixIdentity();
	tempMatrix.e22 = tempCOS;
	tempMatrix.e23 = -tempSIN;
	tempMatrix.e32 = tempSIN;
	tempMatrix.e33 = tempCOS;

	return tempMatrix;
}

Matrix4x4 CMathLib::CreateMatrixRotateY(float degree)
{
	float tempCOS = cos(DegreesToRadians(degree));
	float tempSIN = sin(DegreesToRadians(degree));

	Matrix4x4 tempMatrix;
	tempMatrix.MatrixIdentity();
	tempMatrix.e11 = tempCOS;
	tempMatrix.e13 = tempSIN;
	tempMatrix.e31 = -tempSIN;
	tempMatrix.e33 = tempCOS;

	return tempMatrix;
}

Matrix4x4 CMathLib::CreateMatrixRotateZ(float degree)
{
	float tempCOS = cos(DegreesToRadians(degree));
	float tempSIN = sin(DegreesToRadians(degree));

	Matrix4x4 tempMatrix;
	tempMatrix.MatrixIdentity();
	tempMatrix.e11 = tempCOS;
	tempMatrix.e12 = -tempSIN;
	tempMatrix.e21 = tempSIN;
	tempMatrix.e22 = tempCOS;

	return tempMatrix;
}

Matrix4x4 CMathLib::MatrixAddMatrix(Matrix4x4 matrixA, Matrix4x4 matrixB)
{
	Matrix4x4 tempMatrix = CreateZeroMatrix();

	for (unsigned int i = 0; i < MAX_MATRIX_ELEMENTS; i++)
		tempMatrix.e[i] = matrixA.e[i] + matrixB.e[i];

	return tempMatrix;
}

Matrix4x4 CMathLib::MatrixSubtractMatrix(Matrix4x4 matrixA, Matrix4x4 matrixB)
{
	Matrix4x4 tempMatrix = CreateZeroMatrix();

	for (unsigned int i = 0; i < MAX_MATRIX_ELEMENTS; i++)
		tempMatrix.e[i] = matrixA.e[i] - matrixB.e[i];

	return tempMatrix;
}

Matrix4x4 CMathLib::MatrixMultiplyMatrix(Matrix4x4 matrixA, Matrix4x4 matrixB)
{
	Matrix4x4 tempMatrix = CreateZeroMatrix();

	tempMatrix.e11 = (matrixA.e11 * matrixB.e11) + (matrixA.e12 * matrixB.e21) + (matrixA.e13 * matrixB.e31) + (matrixA.e14 * matrixB.e41);
	tempMatrix.e12 = (matrixA.e11 * matrixB.e12) + (matrixA.e12 * matrixB.e22) + (matrixA.e13 * matrixB.e31) + (matrixA.e14 * matrixB.e41);
	tempMatrix.e13 = (matrixA.e11 * matrixB.e13) + (matrixA.e12 * matrixB.e23) + (matrixA.e13 * matrixB.e31) + (matrixA.e14 * matrixB.e41);
	tempMatrix.e14 = (matrixA.e11 * matrixB.e14) + (matrixA.e12 * matrixB.e24) + (matrixA.e13 * matrixB.e31) + (matrixA.e14 * matrixB.e41);

	tempMatrix.e21 = (matrixA.e21 * matrixB.e11) + (matrixA.e22 * matrixB.e21) + (matrixA.e23 * matrixB.e31) + (matrixA.e24 * matrixB.e41);
	tempMatrix.e22 = (matrixA.e21 * matrixB.e12) + (matrixA.e22 * matrixB.e22) + (matrixA.e23 * matrixB.e31) + (matrixA.e24 * matrixB.e41);
	tempMatrix.e23 = (matrixA.e21 * matrixB.e13) + (matrixA.e22 * matrixB.e23) + (matrixA.e23 * matrixB.e31) + (matrixA.e24 * matrixB.e41);
	tempMatrix.e24 = (matrixA.e21 * matrixB.e14) + (matrixA.e22 * matrixB.e24) + (matrixA.e23 * matrixB.e31) + (matrixA.e24 * matrixB.e41);

	tempMatrix.e31 = (matrixA.e31 * matrixB.e11) + (matrixA.e32 * matrixB.e21) + (matrixA.e33 * matrixB.e31) + (matrixA.e34 * matrixB.e41);
	tempMatrix.e32 = (matrixA.e31 * matrixB.e12) + (matrixA.e32 * matrixB.e22) + (matrixA.e33 * matrixB.e31) + (matrixA.e34 * matrixB.e41);
	tempMatrix.e33 = (matrixA.e31 * matrixB.e13) + (matrixA.e32 * matrixB.e23) + (matrixA.e33 * matrixB.e31) + (matrixA.e34 * matrixB.e41);
	tempMatrix.e34 = (matrixA.e31 * matrixB.e14) + (matrixA.e32 * matrixB.e24) + (matrixA.e33 * matrixB.e31) + (matrixA.e34 * matrixB.e41);

	tempMatrix.e41 = (matrixA.e41 * matrixB.e11) + (matrixA.e42 * matrixB.e21) + (matrixA.e43 * matrixB.e31) + (matrixA.e44 * matrixB.e41);
	tempMatrix.e42 = (matrixA.e41 * matrixB.e12) + (matrixA.e42 * matrixB.e22) + (matrixA.e43 * matrixB.e31) + (matrixA.e44 * matrixB.e41);
	tempMatrix.e43 = (matrixA.e41 * matrixB.e13) + (matrixA.e42 * matrixB.e23) + (matrixA.e43 * matrixB.e31) + (matrixA.e44 * matrixB.e41);
	tempMatrix.e44 = (matrixA.e41 * matrixB.e14) + (matrixA.e42 * matrixB.e24) + (matrixA.e43 * matrixB.e31) + (matrixA.e44 * matrixB.e41);

	return tempMatrix;
}

Vector3D CMathLib::MatrixMultiplyVector(Matrix4x4 matrix, Vector3D vector)
{
	Vector3D tempVect = { 0.0f, 0.0f, 0.0f, 0.0f };
	tempVect.x = (matrix.e11 * vector.x) + (matrix.e12 * vector.y) + (matrix.e13 * vector.z) + (matrix.e14 * vector.w);
	tempVect.y = (matrix.e21 * vector.x) + (matrix.e22 * vector.y) + (matrix.e23 * vector.z) + (matrix.e24 * vector.w);
	tempVect.z = (matrix.e31 * vector.x) + (matrix.e32 * vector.y) + (matrix.e33 * vector.z) + (matrix.e34 * vector.w);
	tempVect.w = (matrix.e41 * vector.x) + (matrix.e42 * vector.y) + (matrix.e43 * vector.z) + (matrix.e44 * vector.w);
	return tempVect;
}


void CMathLib::VectorMultiplyMatrix(Vector3D& vector, Matrix4x4 matrix)
{
	vector.x = (vector.x * matrix.e11) + (vector.y * matrix.e21) + (vector.z * matrix.e31) + (vector.w * matrix.e41);
	vector.y = (vector.x * matrix.e12) + (vector.y * matrix.e22) + (vector.z * matrix.e32) + (vector.w * matrix.e42);
	vector.z = (vector.x * matrix.e13) + (vector.y * matrix.e23) + (vector.z * matrix.e33) + (vector.w * matrix.e43);
	vector.w = (vector.x * matrix.e14) + (vector.y * matrix.e24) + (vector.z * matrix.e34) + (vector.w * matrix.e44);
}

Vector3D CMathLib::VectorMultiplyMatrix(Vector3D vector, Matrix4x4 matrix)
{
	Vector3D tempVect = { 0.0f, 0.0f, 0.0f, 0.0f };
	tempVect.x = (vector.x * matrix.e11) + (vector.y * matrix.e21) + (vector.z * matrix.e31) + (vector.w * matrix.e41);
	tempVect.y = (vector.x * matrix.e12) + (vector.y * matrix.e22) + (vector.z * matrix.e32) + (vector.w * matrix.e42);
	tempVect.z = (vector.x * matrix.e13) + (vector.y * matrix.e23) + (vector.z * matrix.e33) + (vector.w * matrix.e43);
	tempVect.w = (vector.x * matrix.e14) + (vector.y * matrix.e24) + (vector.z * matrix.e34) + (vector.w * matrix.e44);
	return tempVect;
}

void CMathLib::MatrixScale(Matrix4x4& matrix, Vector3D scale)
{
	XMFLOAT4X4 newMatrix = MatrixConvert4x4(matrix);
	XMStoreFloat4x4(&newMatrix, XMMatrixScaling(scale.x, scale.y, scale.z) * XMLoadFloat4x4(&newMatrix));
	matrix = MatrixConvert(newMatrix);
}

#pragma region Translate Functions

//translate X
void CMathLib::TranslateLocalX(Matrix4x4& matrix, float distance)
{
	XMFLOAT4X4 newMatrix = MatrixConvert4x4(matrix);
	XMStoreFloat4x4(&newMatrix, XMMatrixTranslation(distance, 0.0f, 0.0f) * XMLoadFloat4x4(&newMatrix));
	matrix = MatrixConvert(newMatrix);
}
void CMathLib::TranslateLocalX(XMFLOAT4X4& matrix, float distance)
{
	XMStoreFloat4x4(&matrix, XMMatrixTranslation(distance, 0.0f, 0.0f) * XMLoadFloat4x4(&matrix));
}

void CMathLib::TranslateGlobalX(Matrix4x4& matrix, float distance)
{
	XMFLOAT4X4 newMatrix = MatrixConvert4x4(matrix);
	XMStoreFloat4x4(&newMatrix, XMLoadFloat4x4(&newMatrix) * XMMatrixTranslation(distance, 0.0f, 0.0f));
	matrix = MatrixConvert(newMatrix);
}
void CMathLib::TranslateGlobalX(XMFLOAT4X4& matrix, float distance)
{
	XMStoreFloat4x4(&matrix, XMLoadFloat4x4(&matrix) * XMMatrixTranslation(distance, 0.0f, 0.0f));
}



//tanslate Y
void CMathLib::TranslateLocalY(Matrix4x4& matrix, float distance)
{
	XMFLOAT4X4 newMatrix = MatrixConvert4x4(matrix);
	XMStoreFloat4x4(&newMatrix, XMMatrixTranslation(0.0f, distance, 0.0f) * XMLoadFloat4x4(&newMatrix));
	matrix = MatrixConvert(newMatrix);
}
void CMathLib::TranslateLocalY(XMFLOAT4X4& matrix, float distance)
{
	XMStoreFloat4x4(&matrix, XMMatrixTranslation(0.0f, distance, 0.0f) * XMLoadFloat4x4(&matrix));
}

void CMathLib::TranslateGlobalY(Matrix4x4& matrix, float distance)
{
	XMFLOAT4X4 newMatrix = MatrixConvert4x4(matrix);
	XMStoreFloat4x4(&newMatrix, XMLoadFloat4x4(&newMatrix) * XMMatrixTranslation(0.0f, distance, 0.0f));
	matrix = MatrixConvert(newMatrix);
}
void CMathLib::TranslateGlobalY(XMFLOAT4X4& matrix, float distance)
{
	XMStoreFloat4x4(&matrix, XMLoadFloat4x4(&matrix) * XMMatrixTranslation(0.0f, distance, 0.0f));
}


//translate Z
void CMathLib::TranslateLocalZ(Matrix4x4& matrix, float distance)
{
	XMFLOAT4X4 newMatrix = MatrixConvert4x4(matrix);
	XMStoreFloat4x4(&newMatrix, XMMatrixTranslation(0.0f, 0.0f, distance) * XMLoadFloat4x4(&newMatrix));
	matrix = MatrixConvert(newMatrix);
}
void CMathLib::TranslateLocalZ(XMFLOAT4X4& matrix, float distance)
{
	XMStoreFloat4x4(&matrix, XMMatrixTranslation(0.0f, 0.0f, distance) * XMLoadFloat4x4(&matrix));
}

void CMathLib::TranslateGlobalZ(Matrix4x4& matrix, float distance)
{
	XMFLOAT4X4 newMatrix = MatrixConvert4x4(matrix);
	XMStoreFloat4x4(&newMatrix, XMLoadFloat4x4(&newMatrix) * XMMatrixTranslation(0.0f, 0.0f, distance));
	matrix = MatrixConvert(newMatrix);
}
void CMathLib::TranslateGlobalZ(XMFLOAT4X4& matrix, float distance)
{
	XMStoreFloat4x4(&matrix, XMLoadFloat4x4(&matrix) * XMMatrixTranslation(0.0f, 0.0f, distance));
}

#pragma endregion

#pragma region Rotation Functions

//rotate X
void CMathLib::RotateLocalX(Matrix4x4& matrix, float angle)
{
	XMFLOAT4X4 newMatrix = MatrixConvert4x4(matrix);
	XMStoreFloat4x4(&newMatrix, XMMatrixRotationX(angle) * XMLoadFloat4x4(&newMatrix));
	matrix = MatrixConvert(newMatrix);
}
void CMathLib::RotateLocalX(XMFLOAT4X4& matrix, float angle)
{
	XMStoreFloat4x4(&matrix, XMMatrixRotationX(angle) * XMLoadFloat4x4(&matrix));
}

void CMathLib::RotateGlobalX(Matrix4x4& matrix, float angle)
{
	XMFLOAT4X4 newMatrix = MatrixConvert4x4(matrix);
	XMStoreFloat4x4(&newMatrix, XMLoadFloat4x4(&newMatrix) * XMMatrixRotationX(angle));
	matrix = MatrixConvert(newMatrix);
}
void CMathLib::RotateGlobalX(XMFLOAT4X4& matrix, float angle)
{
	XMStoreFloat4x4(&matrix, XMLoadFloat4x4(&matrix) * XMMatrixRotationX(angle));
}



//rotate Y
void CMathLib::RotateLocalY(Matrix4x4& matrix, float angle)
{
	XMFLOAT4X4 newMatrix = MatrixConvert4x4(matrix);
	XMStoreFloat4x4(&newMatrix, XMMatrixRotationY(angle) * XMLoadFloat4x4(&newMatrix));
	matrix = MatrixConvert(newMatrix);
}
void CMathLib::RotateLocalY(XMFLOAT4X4& matrix, float angle)
{
	XMStoreFloat4x4(&matrix, XMMatrixRotationY(angle) * XMLoadFloat4x4(&matrix));
}

void CMathLib::RotateGlobalY(Matrix4x4& matrix, float angle)
{
	XMFLOAT4X4 newMatrix = MatrixConvert4x4(matrix);
	XMStoreFloat4x4(&newMatrix, XMLoadFloat4x4(&newMatrix) * XMMatrixRotationY(angle));
	matrix = MatrixConvert(newMatrix);
}
void CMathLib::RotateGlobalY(XMFLOAT4X4& matrix, float angle)
{
	XMStoreFloat4x4(&matrix, XMLoadFloat4x4(&matrix) * XMMatrixRotationY(angle));
}


//rotate Z
void CMathLib::RotateLocalZ(Matrix4x4& matrix, float angle)
{
	XMFLOAT4X4 newMatrix = MatrixConvert4x4(matrix);
	XMStoreFloat4x4(&newMatrix, XMMatrixRotationZ(angle) * XMLoadFloat4x4(&newMatrix));
	matrix = MatrixConvert(newMatrix);
}
void CMathLib::RotateLocalZ(XMFLOAT4X4& matrix, float angle)
{
	XMStoreFloat4x4(&matrix, XMMatrixRotationZ(angle) * XMLoadFloat4x4(&matrix));
}

void CMathLib::RotateGlobalZ(Matrix4x4& matrix, float angle)
{
	XMFLOAT4X4 newMatrix = MatrixConvert4x4(matrix);
	XMStoreFloat4x4(&newMatrix, XMLoadFloat4x4(&newMatrix) * XMMatrixRotationZ(angle));
	matrix = MatrixConvert(newMatrix);
}
void CMathLib::RotateGlobalZ(XMFLOAT4X4& matrix, float angle)
{
	XMStoreFloat4x4(&matrix, XMLoadFloat4x4(&matrix) * XMMatrixRotationZ(angle));

}

#pragma endregion
