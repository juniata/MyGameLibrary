#include "DX_Library.h"

DX_GameObject::DX_GameObject() : m_pCollider(nullptr)
{

}

DX_GameObject::~DX_GameObject()
{
	DELETE_OBJ(m_pCollider);
	DX_ColliderManager::Release(this);
}

DX_Collider* DX_GameObject::GetCollider()
{
	return m_pCollider;
}
