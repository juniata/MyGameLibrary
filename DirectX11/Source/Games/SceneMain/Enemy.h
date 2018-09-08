#ifndef __ENEMY_H_
#define __ENEMY_H_

class Enemy
{
public:
	Enemy();
	virtual ~Enemy();
	
protected:
	DX_2DObject* m_pObj;
};
#endif // !__ENEMY_H_
