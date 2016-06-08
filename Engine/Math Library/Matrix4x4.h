
#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <cmath>

#define MAX_MATRIX_ELEMENTS 16
#define MAX_MATRIX_ROW_ELEMENTS 4

typedef union Matrix4x4
{
	float e[MAX_MATRIX_ELEMENTS];
	float m[MAX_MATRIX_ROW_ELEMENTS][MAX_MATRIX_ROW_ELEMENTS];
	struct
	{
		float e11, e12, e13, e14;
		float e21, e22, e23, e24;
		float e31, e32, e33, e34;
		float e41, e42, e43, e44;
	};

	void MatrixZero();
	void MatrixIdentity();
	void MatrixNegate();
	void MatrixTranspose();
	void MatrixAddMatrix(Matrix4x4 addMatrix);
	void MatrixSubtractMatrix(Matrix4x4 subMatrix);
	void MatrixMultiplyMatrix(Matrix4x4 multMatrix);

	void SetMatrixPosition(float x, float y, float z);


} *LPMatrix4x4;


#endif