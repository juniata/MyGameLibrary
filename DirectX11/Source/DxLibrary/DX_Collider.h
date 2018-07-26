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


class DX_GameObject;
class DX_ColliderManager
{
public:
	static DX_Collider* BindGameObject(DX_GameObject* gameObject, COLLIDER_TYPE colliderType);
	static void Update();
	static void Release(DX_GameObject* gameObject);

private:
	static std::list<DX_GameObject*> m_gameObjects;

	//------------------------------------------------------------------------------
	//
	//  @brief		DX_RectCollder2D�Ƃ̓����蔻��
	//	@param		collider		�Փ˂�����̃R���C�_�[
	//	@param		targetCollider	�Փ˂����ق��̃R���C�_�[
	//
	//------------------------------------------------------------------------------
	static void TargetRectCollision(DX_Collider* collider, DX_RectCollder2D* targetCollider);
};
#endif // !__DX_COLLIDER_H_
