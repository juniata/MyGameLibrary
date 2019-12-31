#ifndef __SCENE_TITLE_H_
#define __SCENE_TITLE_H_

//****************************************************************************************************
//
//	SceneMain
//
//****************************************************************************************************
class SceneMain : public DX_Scene
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		コンストラクタ
	//
	//------------------------------------------------------------------------------
	SceneMain();

	//------------------------------------------------------------------------------
	//
	//  @brief		デストラクタ
	//
	//------------------------------------------------------------------------------
	~SceneMain();

	//------------------------------------------------------------------------------
	//
	//  @brief		初期化
	//	@return		falseの場合エラーが起きたと判断し、ゲームを終了します。
	//
	//------------------------------------------------------------------------------
	bool Initialize();

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
};
#endif // !__SCENE_TITLE_H_
