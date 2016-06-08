
#ifndef CCAMERA_H
#define CCAMERA_H

#include "../../Game/Core/stdafx.h"
#include "../../Engine/Object Manager/Object.h"

#define FR_CAM_MOVE_SPEED 30.0f		//Free Roam Camera Move Speed
#define FR_CAM_BOOST_SPEED 90.0f	//Free Roam Camera Boost Speed
#define FR_CAM_VERT_MOVE_DIV 5.0f	//Free Roam Camera Vertical Movement Divisor
#define FR_CAM_ROT_MOVE_DIV 10.0f	//Free Roam Camera Rotate Movement Divisor
#define FR_CAM_ROT_SENS 0.015f		//Free Roam Camera Rotate Sensitivity
#define FR_CAM_VERT_MOVE_ANGLE_LOCK 45.0f	//Free Roam Camera Vertical Movement Angle Lock

class CCamera : public CObject
{
public:

	enum ECameraType { eFREE_ROAM = 0, eATTACHED_FIRST_PERSON, eATTACHED_THIRD_PERSON };


private:

	Matrix4x4 m_mLocalMatrix;
	ECameraType m_eType;

	float m_fCameraAngle;
	POINT m_ptCurrentPosition;
	POINT m_ptPreviousPosition;

public:

	CCamera() = default;
	~CCamera() = default;
	CCamera(Vector3D vOffset, ECameraType eType);

	bool Update() override;


};

#endif
