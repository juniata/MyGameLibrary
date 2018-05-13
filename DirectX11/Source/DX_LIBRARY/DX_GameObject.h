#ifndef __DX_GAME_OBJECT_H_
#define __DX_GAME_OBJECT_H_

class GameObject
{
private:
public:
	GameObject(){}
	virtual ~GameObject(){}

	virtual void Initialize(){}
	virtual void Destroy(){}
};
#endif // !__DX_GAME_OBJECT_H_
