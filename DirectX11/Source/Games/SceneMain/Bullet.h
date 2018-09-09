#ifndef __BULLET_H_
#define __BULLET_H_

struct Bullet
{
	DX_2DObject*	pObj;
	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT2 move;
	bool isActive;
};


struct tagInstanceBullet
{
	DirectX::XMFLOAT2* pos;
	DirectX::XMFLOAT2 move;
};
#endif // !__BULLET_H_
