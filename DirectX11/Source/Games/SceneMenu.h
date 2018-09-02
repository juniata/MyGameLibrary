#ifndef __SCENE_MENU_H_
#define __SCENE_MENU_H_

//****************************************************************************************************
//
//	SceneMenu
//	メニュー画面を管理します。
//
//****************************************************************************************************
class SceneMenu : public DX_Scene
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		コンストラクタ
	//
	//------------------------------------------------------------------------------
	SceneMenu();

	//------------------------------------------------------------------------------
	//
	//  @brief		デストラクタ
	//
	//------------------------------------------------------------------------------
	~SceneMenu();

	//------------------------------------------------------------------------------
	//
	//  @brief		更新
	//	@return		falseの場合エラーが起きたと判断し、ゲームを終了します。
	//
	//------------------------------------------------------------------------------
	bool Update();

	//------------------------------------------------------------------------------
	//
	//  @brief		描画
	//	@return		falseの場合エラーが起きたと判断し、ゲームを終了します。
	//
	//------------------------------------------------------------------------------
	bool Render();
private:
	struct Button {
		DirectX::XMFLOAT2 pos;
		DX_Button*	pButton;
	};
	DX_2DObject* m_pMenu;

	static const int BUTTON_NUM = 10;
	Button m_stageButtonList[BUTTON_NUM];

	//------------------------------------------------------------------------------
	//
	//  @brief		ステージボタンを追加する
	//
	//------------------------------------------------------------------------------
	void AddStageButton();
};
#endif // !__SCENE_MENU_H_
