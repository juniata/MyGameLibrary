#ifndef __DX_COLLIDER_3D_H_
#define __DX_COLLIDER_3D_H_



class DX_Collider3D : public DX_Collider
{
public:
};


class DX_BoxCollider3D : public DX_Collider
{
public:
	COLLIDER_TYPE GetColliderType() { return COLLIDER_TYPE::BOX_COLLIDER_3D; }
};

class DX_SphereCollider3D : public DX_Collider
{
public:
	COLLIDER_TYPE GetColliderType() { return COLLIDER_TYPE::SPHERE_COLLIDER_3D; }
};

#endif