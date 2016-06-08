
#include "GameObject.h"

CGameObject::CGameObject(std::string szTag) : CObject(szTag)
{

}

CGameObject::~CGameObject()
{

}

bool CGameObject::Update()
{
	return true;
}
