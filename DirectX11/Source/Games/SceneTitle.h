#ifndef __SCENE_TITLE_H_
#define __SCENE_TITLE_H_

class SceneTitle : public DX_Scene
{
public:
	SceneTitle();
	~SceneTitle();
	bool Initialize();

	bool Update();
	void Render();
private:
	DX_2DObject* m_pTitle;
};
#endif // !__SCENE_TITLE_H_
