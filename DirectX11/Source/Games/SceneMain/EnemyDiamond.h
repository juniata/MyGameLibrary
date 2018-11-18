#ifndef __ENEMY_DIAMOND_H_
#define __ENEMY_DIAMOND_H_

class EnemyDiamond : public Enemy
{
public:
	EnemyDiamond();
	~EnemyDiamond();

	bool Update(const DirectX::XMFLOAT2& playerPos);
	bool Render();
};
#endif // !__ENEMY_DIAMOND_H_

