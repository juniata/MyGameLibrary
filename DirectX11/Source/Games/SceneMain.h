#ifndef __SCENE_MAIN_H_
#define __SCENE_MAIN_H_

class CircularBarrageManager;
class Player;

class SceneMain : public DX_Scene
{
private:
	CircularBarrageManager* circularBarrageManager;
	Player* player;
public:
	SceneMain();
	~SceneMain();
	bool Initialize();

	bool Update(DX_System* pSystem, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Render(DX_System* pSystem, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);


	
};
#endif // !__SCENE_MAIN_H_
