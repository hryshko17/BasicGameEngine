
#include "Matrix4x4.h"

void Matrix4x4::MatrixZero()
{
	for (unsigned int i = 0; i < MAX_MATRIX_ELEMENTS; i++)
		e[i] = 0.0f;
}

void Matrix4x4::MatrixIdentity()
{
	MatrixZero();
	e11 = 1.0f;
	e22 = 1.0f;
	e33 = 1.0f;
	e44 = 1.0f;
}

void Matrix4x4::MatrixNegate()
{
	for (unsigned int i = 0; i < MAX_MATRIX_ELEMENTS; i++)
		e[i] *= -1.0f;
}

void Matrix4x4::MatrixTranspose()
{
	Matrix4x4 tempMatrix;
	for (unsigned int i = 0; i < MAX_MATRIX_ELEMENTS; i++)
		tempMatrix.e[i] = e[i];

	e12 = tempMatrix.e21;
	e13 = tempMatrix.e31;
	e14 = tempMatrix.e41;

	e21 = tempMatrix.e12;
	e23 = tempMatrix.e32;
	e24 = tempMatrix.e42;

	e31 = tempMatrix.e13;
	e32 = tempMatrix.e23;
	e34 = tempMatrix.e43;

	e41 = tempMatrix.e14;
	e42 = tempMatrix.e24;
	e43 = tempMatrix.e34;
}

void Matrix4x4::MatrixAddMatrix(Matrix4x4 addMatrix)
{
	for (unsigned int i = 0; i < MAX_MATRIX_ELEMENTS; i++)
		e[i] += addMatrix.e[i];
}

void Matrix4x4::MatrixSubtractMatrix(Matrix4x4 subMatrix)
{
	for (unsigned int i = 0; i < MAX_MATRIX_ELEMENTS; i++)
		e[i] -= subMatrix.e[i];
}

void Matrix4x4::MatrixMultiplyMatrix(Matrix4x4 multMatrix)
{
	e11 = (e11 * multMatrix.e11) + (e12 * multMatrix.e21) + (e13 * multMatrix.e31) + (e14 * multMatrix.e41);
	e12 = (e11 * multMatrix.e12) + (e12 * multMatrix.e22) + (e13 * multMatrix.e31) + (e14 * multMatrix.e41);
	e13 = (e11 * multMatrix.e13) + (e12 * multMatrix.e23) + (e13 * multMatrix.e31) + (e14 * multMatrix.e41);
	e14 = (e11 * multMatrix.e14) + (e12 * multMatrix.e24) + (e13 * multMatrix.e31) + (e14 * multMatrix.e41);

	e21 = (e21 * multMatrix.e11) + (e22 * multMatrix.e21) + (e23 * multMatrix.e31) + (e24 * multMatrix.e41);
	e22 = (e21 * multMatrix.e12) + (e22 * multMatrix.e22) + (e23 * multMatrix.e31) + (e24 * multMatrix.e41);
	e23 = (e21 * multMatrix.e13) + (e22 * multMatrix.e23) + (e23 * multMatrix.e31) + (e24 * multMatrix.e41);
	e24 = (e21 * multMatrix.e14) + (e22 * multMatrix.e24) + (e23 * multMatrix.e31) + (e24 * multMatrix.e41);

	e31 = (e31 * multMatrix.e11) + (e32 * multMatrix.e21) + (e33 * multMatrix.e31) + (e34 * multMatrix.e41);
	e32 = (e31 * multMatrix.e12) + (e32 * multMatrix.e22) + (e33 * multMatrix.e31) + (e34 * multMatrix.e41);
	e33 = (e31 * multMatrix.e13) + (e32 * multMatrix.e23) + (e33 * multMatrix.e31) + (e34 * multMatrix.e41);
	e34 = (e31 * multMatrix.e14) + (e32 * multMatrix.e24) + (e33 * multMatrix.e31) + (e34 * multMatrix.e41);

	e41 = (e41 * multMatrix.e11) + (e42 * multMatrix.e21) + (e43 * multMatrix.e31) + (e44 * multMatrix.e41);
	e42 = (e41 * multMatrix.e12) + (e42 * multMatrix.e22) + (e43 * multMatrix.e31) + (e44 * multMatrix.e41);
	e43 = (e41 * multMatrix.e13) + (e42 * multMatrix.e23) + (e43 * multMatrix.e31) + (e44 * multMatrix.e41);
	e44 = (e41 * multMatrix.e14) + (e42 * multMatrix.e24) + (e43 * multMatrix.e31) + (e44 * multMatrix.e41);
}

void Matrix4x4::SetMatrixPosition(float x, float y, float z)
{
	e41 = x;
	e42 = y;
	e43 = z;
}