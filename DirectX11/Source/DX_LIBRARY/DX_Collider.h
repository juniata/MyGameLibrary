#ifndef __DX_COLLIDER_H_
#define __DX_COLLIDER_H_

enum class COLLIDER_TYPE : int
{
	// 2D
	RECT_COLLIDER_2D,
	CIRCLE_COLLIDER_2D,

	// 3D
	BOX_COLLIDER_3D,
	SPHERE_COLLIDER_3D,

};

class DX_Collider
{
public:
	DX_Collider(){}
	virtual ~DX_Collider(){}
	virtual COLLIDER_TYPE GetColliderType() = 0;
	
};

#include "DX_Collider2D.h"
#include "DX_Collider3D.h"



class DX_ColliderManager
{
private:
	static std::list<DX_GameObject*> m_gameObjects;
public:

	static DX_Collider* BindGameObject(DX_GameObject* gameObject, COLLIDER_TYPE colliderType);
	static void Update();
	static void Release(DX_GameObject* gameObject);
};
#endif // !__DX_COLLIDER_H_
