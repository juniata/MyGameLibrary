#ifndef __ENEMY_MANAGER_H_
#define __ENEMY_MANAGER_H_

class Enemy;
class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	bool Update(const DirectX::XMFLOAT2& playerPos);
	bool Render();
private:
	Enemy* m_pEnemy;
};
#endif // !__ENEMY_MANAGER_H_
