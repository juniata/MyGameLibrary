#include	"DxLibrary\DX_Library.h"
#include	"Bullet.h"
#include	"Enemy.h"
using namespace DirectX;

Enemy::Enemy() :
	m_pObj(nullptr),
	m_pos(XMFLOAT2(0.0f, 0.0f)),
	m_move(XMFLOAT2(0.0f, 0.0f)),
	m_pBullets(nullptr)
{}

Enemy::~Enemy()
{
	DELETE_OBJ(m_pObj);
	DELETE_OBJ_ARRAY(m_pBullets);
}
