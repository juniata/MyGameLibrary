#ifndef __SCENE_MAIN_H_
#define __SCENE_MAIN_H_

class Player;

class SceneMain : public DX_Scene
{
private:
	Player* player;
public:
	SceneMain();
	~SceneMain();
	bool Initialize();

	bool Update();
	void Render();


	
};
#endif // !__SCENE_MAIN_H_
