
#include "Camera.h"
#include "../../Engine/Input/InputManager.h"
#include "../../Engine/Utilities/Time.h"
#include "../../Engine/Renderer/Renderer.h"

CCamera::CCamera(Vector3D vOffset, ECameraType eType) : CObject("Camera")
{
	m_mLocalMatrix.MatrixIdentity();
	m_mLocalMatrix.SetMatrixPosition(vOffset.x, vOffset.y, vOffset.z);

	m_eType = eType;

	m_fCameraAngle = 0.0f;
	m_ptCurrentPosition = { 0, 0 };
	m_ptPreviousPosition = { 0, 0 };
}

bool CCamera::Update()
{
	CInputManager* Input = CInputManager::GetInstacne();
	CTime* Time = CTime::GetInstance();
	Matrix4x4 mViewMatrix = CMathLib::MatrixConvert(CRenderer::GetInstance()->GetSceneViewMatrix());

	//if camera type is 1st person or 3rd person then the camera will move with the object. 
	//When its free roam, the camera is moved here
	if (m_eType == ECameraType::eFREE_ROAM)
	{
#pragma region Camera Keyboard Input 

		float fMoveSpeed = FR_CAM_MOVE_SPEED * Time->DeltaTime();

		if (Input->KeyBoard()->IsKeyDown(EKey::Shift))
			fMoveSpeed = FR_CAM_BOOST_SPEED * Time->DeltaTime();

		if (Input->KeyBoard()->IsKeyDown(EKey::W))// W - forward
			CMathLib::TranslateLocalZ(mViewMatrix, fMoveSpeed);
		if (Input->KeyBoard()->IsKeyDown(EKey::A)) // A - leftward
			CMathLib::TranslateLocalX(mViewMatrix, -fMoveSpeed);
		if (Input->KeyBoard()->IsKeyDown(EKey::S)) // S - backward
			CMathLib::TranslateLocalZ(mViewMatrix, -fMoveSpeed);
		if (Input->KeyBoard()->IsKeyDown(EKey::D)) // D - rightward
			CMathLib::TranslateLocalX(mViewMatrix, fMoveSpeed);
		if (Input->KeyBoard()->IsKeyDown(EKey::Q)) // Q - upward
			CMathLib::TranslateLocalY(mViewMatrix, fMoveSpeed / FR_CAM_VERT_MOVE_DIV);
		if (Input->KeyBoard()->IsKeyDown(EKey::E)) // E - downward
			CMathLib::TranslateLocalY(mViewMatrix, -fMoveSpeed / FR_CAM_VERT_MOVE_DIV);

#pragma endregion

#pragma region Camera Mouse Input

		if (GetCursorPos(&m_ptCurrentPosition))
		{
			float fCursorX = (float)(m_ptCurrentPosition.x - m_ptPreviousPosition.x);
			//float fCursorY = (float)(m_ptCurrentPosition.y - m_ptPreviousPosition.y);

			if (fCursorX /*|| fCursorY*/)
			{
				fCursorX *= FR_CAM_ROT_SENS;
				//fCursorY *= FR_CAM_ROT_SENS;

				Vector3D vViewMatrixPos;
				for (unsigned int i = 0; i < MAX_VECTOR_ELEMENTS; i++)
				{
					vViewMatrixPos.e[i] = mViewMatrix.m[3][i];
					mViewMatrix.m[3][i] = 0.0f;
				}

				CMathLib::RotateLocalY(mViewMatrix, fCursorX);

				for (unsigned int i = 0; i < MAX_VECTOR_ELEMENTS; i++)
				{
					mViewMatrix.m[3][i] = vViewMatrixPos.e[i];
				}
			}
		}

		//snapping
		if (m_ptCurrentPosition.x < 1 || m_ptCurrentPosition.x > WINDOW_WIDTH - 2)
		{
			if (GetCursorPos(&m_ptCurrentPosition) == false)
			{
				DEBUG_OUTPUT("Failed to get cursor position in Camera::Update Free Roam");
				return false;
			}
		}

		m_ptPreviousPosition = m_ptCurrentPosition;

		CRenderer::GetInstance()->SetSceneViewMatrix(CMathLib::MatrixConvert4x4(mViewMatrix));

#pragma endregion

	}

	return true;
}
