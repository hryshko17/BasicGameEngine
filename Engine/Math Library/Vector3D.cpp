
#include "Vector3D.h"

void Vector3D::ZeroOut()
{
	for (unsigned int i = 0; i < MAX_VECTOR_ELEMENTS; i++)
		this->e[i] = 0.0f;
}

void Vector3D::Set(float x, float y, float z, float w)
{
	this->e[0] = x;
	this->e[1] = y;
	this->e[2] = z;
	this->e[3] = w;
}

void Vector3D::Scale(float scaler)
{
	this->e[0] *= scaler;
	this->e[1] *= scaler;
	this->e[2] *= scaler;
	this->e[3] *= scaler;
}

void Vector3D::Negate()
{
	for (unsigned int i = 0; i < MAX_VECTOR_ELEMENTS; i++)
		this->e[i] *= -1.0f;
}

void Vector3D::Normalize()
{
	float normalize = sqrt((this->e[0] * this->e[0]) + (this->e[1] * this->e[1]) + (this->e[2] * this->e[2]) + (this->e[3] * this->e[3]));

	if (this->e[0] != 0)
		this->e[0] /= normalize;
	if (this->e[1] != 0)
		this->e[1] /= normalize;
	if (this->e[2] != 0)
		this->e[2] /= normalize;
	if (this->e[3] != 0)
		this->e[3] /= normalize;
}

void Vector3D::VectorAdd(Vector3D vector)
{
	this->e[0] += vector.x;
	this->e[1] += vector.y;
	this->e[2] += vector.z;
	this->e[3] += vector.w;
}

void Vector3D::VectorSubtract(Vector3D vector)
{
	this->e[0] -= vector.x;
	this->e[1] -= vector.y;
	this->e[2] -= vector.z;
	this->e[3] -= vector.w;
}