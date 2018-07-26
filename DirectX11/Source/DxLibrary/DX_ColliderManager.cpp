#include	"DX_Library.h"

std::list<DX_GameObject*> DX_ColliderManager::m_gameObjects;

DX_Collider* DX_ColliderManager::BindGameObject(DX_GameObject* gameObject, COLLIDER_TYPE colliderType)
{
	DX_Collider* collider = nullptr;

	switch (colliderType)
	{
	case COLLIDER_TYPE::CIRCLE_COLLIDER_2D:
		collider = new DX_CircleCollider2D();
		break;
	case COLLIDER_TYPE::RECT_COLLIDER_2D:
		collider = new DX_RectCollder2D();
		break;
	case COLLIDER_TYPE::BOX_COLLIDER_3D:
		collider = new DX_BoxCollider3D();
		break;
	case COLLIDER_TYPE::SPHERE_COLLIDER_3D:
		collider = new DX_SphereCollider3D();
		break;
	}
	m_gameObjects.push_back(gameObject);

	return collider;
}

void DX_ColliderManager::Update()
{
	for each (auto gameObject1 in m_gameObjects)
	{
		// ゲームオブジェクトにアタッチしたコライダーを取得する
		DX_Collider* collider1 = gameObject1->GetCollider();

		// アタッチされてなければcontinue
		if (collider1 == nullptr) {
			continue;
		}

		for each (auto gameObject2 in m_gameObjects)
		{
			// ゲームオブジェクトにアタッチしたコライダーを取得する
			DX_Collider* collider2 = gameObject2->GetCollider();
			// アタッチされてなければcontinue
			if (collider1 == nullptr) {
				continue;
			}

			// 自分自身ならcontinue
			if (collider1 == collider2) {
				continue;
			}

			//　自身のコライダータイプをチェックし、衝突判定方法を切り替える
			switch (collider2->GetColliderType())
			{
			case COLLIDER_TYPE::RECT_COLLIDER_2D: TargetRectCollision(collider1, dynamic_cast<DX_RectCollder2D*>(collider2)); break;
				case COLLIDER_TYPE::CIRCLE_COLLIDER_2D: // ToCirCleCollision();
					break;
				case COLLIDER_TYPE::BOX_COLLIDER_3D:	// ToBoxCollision();
					break;
				case COLLIDER_TYPE::SPHERE_COLLIDER_3D: // ToSphereCollision(); 
					break;
			}
		}
	}
}

void DX_ColliderManager::Release(DX_GameObject* gameObject)
{
	m_gameObjects.remove(gameObject);
}

void DX_ColliderManager::TargetRectCollision(DX_Collider* collider, DX_RectCollder2D* targetCollider)
{
	switch (collider->GetColliderType())
	{
	case COLLIDER_TYPE::RECT_COLLIDER_2D:
			break;
	case COLLIDER_TYPE::CIRCLE_COLLIDER_2D:
		break;
	case COLLIDER_TYPE::BOX_COLLIDER_3D:
		break;
	case COLLIDER_TYPE::SPHERE_COLLIDER_3D:
		break;
	}
}