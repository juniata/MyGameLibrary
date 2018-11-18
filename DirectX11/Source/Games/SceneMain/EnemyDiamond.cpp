#include	"DxLibrary\DX_Library.h"
#include	"Bullet.h"
#include	"Enemy.h"

using namespace DirectX;

EnemyDiamond::EnemyDiamond()
{
	m_pObj = new DX_2DObject();
	m_pObj->Initialize("SceneMain\\Enemy\\enemy1.png");
	m_pBullets = new tagBullet[255];

	ZeroMemory(&m_pBullets[0], sizeof(m_pBullets[0]));
	m_pBullets[0].pObj = new DX_2DObject();
	m_pBullets[0].pObj->Initialize("SceneMain\\Enemy\\Bullet.png");
	tagBullet* pBullet = nullptr;
	for (int i = 1; i < 255; ++i)
	{
		pBullet = &m_pBullets[i];
		pBullet->pObj = m_pBullets[0].pObj->Clone();
	}
}
EnemyDiamond::~EnemyDiamond()
{
	for (int i = 0; i < 255; ++i)
	{
		DELETE_OBJ(m_pBullets[i].pObj);
	}
}
bool EnemyDiamond::Update(const DirectX::XMFLOAT2& playerPos)
{
	XMFLOAT2 target;

	tagBullet* pBullet = nullptr;

	for (int i = 0; i < 255; ++i)
	{
		pBullet = &m_pBullets[i];
		if (pBullet->isActive) {
			continue;
		}

		target.x = playerPos.x - pBullet->pos.x;
		target.y = playerPos.y - pBullet->pos.y;

		XMStoreFloat2(&pBullet->move, XMVector2Normalize(XMLoadFloat2(&target)));
		pBullet->pos;
		pBullet->isActive = true;
	}

	for (int i = 0; i < 255; ++i)
	{
		pBullet = &m_pBullets[i];
		if (pBullet->isActive == false) {
			continue;
		}

		pBullet->pos.x += pBullet->move.x;
		pBullet->pos.y += pBullet->move.y;
	}

	return true;
}
bool EnemyDiamond::Render()
{
	tagBullet* pBullet = nullptr;
	for (int i = 0; i < 255; ++i)
	{
		pBullet = &m_pBullets[i];
		if (pBullet->isActive == false) {
			continue;
		}
		pBullet->pObj->Render(pBullet->pos, XMFLOAT2(32.0f, 32.0f));
	}

	m_pObj->Render(m_pos, XMFLOAT2(32.0f, 32.0f));
	return true;
}