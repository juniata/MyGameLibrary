#include	"DxLibrary\DX_Library.h"
#include	"Enemy.h"

Enemy::Enemy() :
	m_pObj(nullptr)
{}

Enemy::~Enemy()
{
	DELETE_OBJ(m_pObj);
}
