
#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>

#define MAX_VECTOR_ELEMENTS 4

typedef union Vector3D
{
	float e[MAX_VECTOR_ELEMENTS];
	struct
	{
		float x, y, z, w;
	};

	void ZeroOut();//sets this vector elements to 0
	void Set(float x, float y, float z, float w);//sets all the elements of the array
	void Scale(float scaler);//scales this vector elements by the scaler value
	void Negate();//negates this vector element data
	void Normalize();//normalizes this vector
	void VectorAdd(Vector3D vector);//adds a vector to this vector
	void VectorSubtract(Vector3D vector);//subtracts a vector from this vector

} *LPVector3D;

#endif