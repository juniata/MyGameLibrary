#ifndef __ENEMY_H_
#define __ENEMY_H_

struct tagBullet;
class Enemy
{
public:
	Enemy();
	virtual ~Enemy();
	
	virtual bool Update(const DirectX::XMFLOAT2& playerPos) = 0;
	virtual bool Render() = 0;
protected:
	DX_2DObject* m_pObj;
	DirectX::XMFLOAT2 m_pos;
	DirectX::XMFLOAT2 m_move;
	tagBullet* m_pBullets;
};

#include	"EnemyDiamond.h"
#endif // !__ENEMY_H_
