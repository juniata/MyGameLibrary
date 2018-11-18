#include	"DxLibrary\DX_Library.h"
#include	"Enemy.h"
#include	"EnemyManager.h"

EnemyManager::EnemyManager()
{
	m_pEnemy = new EnemyDiamond[10];
}

EnemyManager::~EnemyManager()
{
	DELETE_OBJ_ARRAY(m_pEnemy);
}

bool EnemyManager::Update(const DirectX::XMFLOAT2& playerPos)
{
	m_pEnemy[0].Update(playerPos);
	return true;
}
bool EnemyManager::Render()
{
	m_pEnemy[0].Render();
	return true;
}