#ifndef __DX_COLLIDER_H_
#define __DX_COLLIDER_H_

enum class COLLIDER_TYPE : int
{
	__NONE,
	BOX_COLLIDER,
	SPHERE_COLLIDER,
};

class DX_Collider
{
protected:
	DX_GameObject* m_pGameObject;
	COLLIDER_TYPE m_colliderType;

public:
	DX_Collider(){}
	virtual ~DX_Collider(){}
	DX_GameObject* GetGameObject();
	COLLIDER_TYPE GetCollider();
	
};

class BoxCollider : public DX_Collider
{

};

class SphereCollider : public DX_Collider
{

};

class ColliderManager
{
private:
public:

	static void BindGameObject(COLLIDER_TYPE colliderType, DX_GameObject* pGameObject);
};
#endif // !__DX_COLLIDER_H_
