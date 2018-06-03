#ifndef __DX_GAME_OBJECT_H_
#define __DX_GAME_OBJECT_H_

class DX_Collider;
class DX_GameObject
{
protected:
	DX_Collider* m_pCollider;

public:
	DX_GameObject();
	virtual ~DX_GameObject();
	virtual void Initialize(){}
	virtual void Destroy(){}
	DX_Collider* GetCollider();
};
#endif // !__DX_GAME_OBJECT_H_
