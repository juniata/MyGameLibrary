#ifndef __DX_COLLIDER_H_
#define __DX_COLLIDER_H_

enum class COLLIDER_TYPE : int
{
	__NONE,
	BOX_COLLIDER,
	SPHERE_COLLIDER,
};

class Collider
{
protected:
	dx_library::GameObject* m_pGameObject;
	COLLIDER_TYPE m_colliderType;

public:
	Collider(){}
	virtual ~Collider(){}
	dx_library::GameObject* GetGameObject();
	COLLIDER_TYPE GetCollider();
	
};

class BoxCollider : public dx_library::Collider
{

};

class SphereCollider : public dx_library::Collider
{

};

class ColliderManager
{
private:
public:

	static void BindGameObject(COLLIDER_TYPE colliderType, dx_library::GameObject* pGameObject);
};
#endif // !__DX_COLLIDER_H_
