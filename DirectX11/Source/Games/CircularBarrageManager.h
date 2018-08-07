#ifndef _BULLETMANAGER_H_
#define _BULLETMANAGER_H_
class CircularBarrage;

//****************************************************************************************************
//
//	CircularBarrageManager
//
//****************************************************************************************************
class CircularBarrageManager
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		初期化
	//
	//------------------------------------------------------------------------------
	CircularBarrageManager();

	//------------------------------------------------------------------------------
	//
	//  @brief		解放
	//
	//------------------------------------------------------------------------------
	~CircularBarrageManager();

	//------------------------------------------------------------------------------
	//
	//  @brief		更新
	//
	//------------------------------------------------------------------------------
	void Update();

	//------------------------------------------------------------------------------
	//
	//  @brief		描画
	//
	//------------------------------------------------------------------------------
	void Render();

private:
	// 生成する円形弾幕の数
	static const int CIRCULARBARRAGE_MAX = 15;

	// xフレームおきに円形弾幕を生成する
	static const int CREATE_CIRCULAR_BARRAGE_FRAME = 5;

	DX_2DObject* m_pCircle;

	// サークルオブジェクト一覧
	CircularBarrage* m_pCircularBarrageList[CircularBarrageManager::CIRCULARBARRAGE_MAX];

	// 2Dインスタンス描画オブジェクト
	DX_Instance2DObject* m_pInstance2d;

	// 弾有効化インターバル
	int m_interval;

	// 有効化する弾のインデックス
	int m_enabledBulletIndex;

};

#endif