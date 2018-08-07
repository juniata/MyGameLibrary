#ifndef __DX_GAME_OBJECT_H_
#define __DX_GAME_OBJECT_H_

class DX_GameObject
{
public:
	DX_GameObject();
	virtual ~DX_GameObject();
	virtual void Initialize(){}
	virtual void Destroy(){}
};
#endif // !__DX_GAME_OBJECT_H_
