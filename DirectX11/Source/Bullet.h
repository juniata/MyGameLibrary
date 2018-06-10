#ifndef __BULLET_H_
#define __BULLET_H_

//****************************************************************************************************
//
//	Bullet Class
//
//****************************************************************************************************
class Bullet
{
private:
	// 弾の画像
	DX_2DObject* obj;
	// 基点
	DirectX::XMFLOAT2 basePos;
	// 座標
	DirectX::XMFLOAT2 pos;
	// サイズ
	DirectX::XMFLOAT2 size;

	// 中心からの距離
	float distance;
	// 角度
	float angle;
	// 有効フラグ
	bool isEnabled;
	// 死亡フラグ
	bool isDead;
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		初期化
	//
	//------------------------------------------------------------------------------
	Bullet();
	//------------------------------------------------------------------------------
	//
	//  @brief		解放
	//
	//------------------------------------------------------------------------------
	~Bullet();

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

	//------------------------------------------------------------------------------
	//
	//  @brief		有効フラグセット
	//  @param[in]  isEnabled セットする有効フラグ(bool)
	//  @detail     引数の値によってUpdateとRenderの動作制御
	//
	//------------------------------------------------------------------------------
	void SetEnabled(bool isEnabled);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		死亡フラグ取得
	//  @return     死亡フラグを返す(bool)
	//
	//------------------------------------------------------------------------------
	bool IsDead();

	//------------------------------------------------------------------------------
	//
	//  @brief		復活させる
	//
	//------------------------------------------------------------------------------
	void Resurrection();

	//------------------------------------------------------------------------------
	//
	//  @brief	    距離加算
	//
	//------------------------------------------------------------------------------
	void SetDistance(float distance);

	//------------------------------------------------------------------------------
	//
	//  @brief	    距離取得
	//
	//------------------------------------------------------------------------------
	float GetDistance();

	//------------------------------------------------------------------------------
	//
	//  @brief		角度加算
	//
	//------------------------------------------------------------------------------
	void SetAngle(float angle);

	//------------------------------------------------------------------------------
	//
	//  @brief		角度取得
	//
	//------------------------------------------------------------------------------
	float GetAngle();
private:
	//------------------------------------------------------------------------------
	//
	//  @brief		移動
	//
	//------------------------------------------------------------------------------
	void Move();

	//------------------------------------------------------------------------------
	//
	//  @brief		リスポーン
	//  @detail     画面外に出るか何かに当たったらリスポーンする
	//
	//------------------------------------------------------------------------------
	void Response();

	//------------------------------------------------------------------------------
	//
	//  @brief	    死亡記録
	//
	//------------------------------------------------------------------------------
	void DeathRecord();
};

#endif