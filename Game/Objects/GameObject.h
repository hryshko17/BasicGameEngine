
#ifndef CGAMEOBJECT_H
#define CGAMEOBJECT_H

#include "../../Engine/Object Manager/Object.h"

class CGameObject : public CObject
{
private:

public:

	CGameObject(std::string szTag);
	~CGameObject();

	bool Update();

};

#endif
