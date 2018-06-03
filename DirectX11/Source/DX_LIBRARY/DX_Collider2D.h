#ifndef __DX_COLLIDER_2D_H_
#define __DX_COLLIDER_2D_H_

class DX_Collider2D : public DX_Collider
{
public:
};

class DX_RectCollder2D : public DX_Collider2D
{
public:
	COLLIDER_TYPE GetColliderType() { return COLLIDER_TYPE::RECT_COLLIDER_2D; }
};

class DX_CircleCollider2D : public DX_Collider2D
{
public:
	COLLIDER_TYPE GetColliderType() { return COLLIDER_TYPE::CIRCLE_COLLIDER_2D;  }
};
#endif