#ifndef _PLAYER_H_
#define _PLAYER_H_


struct PlayerStatus
{
	int life;	// 残機
};

class BarrageManager;
//****************************************************************************************************
//
//	Player Class
//
//****************************************************************************************************
class Player
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		初期化
	//
	//------------------------------------------------------------------------------
	Player();
	//------------------------------------------------------------------------------
	//
	//  @brief		解放
	//
	//------------------------------------------------------------------------------
	~Player();

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
	// サイズ
	const float SIZE = 32.0f;
	// 移動速度
	const float MOVE_SPEED = 5.0f;

	// 初期残機
	const int INIT_LIFE = 3;

	// プレイヤーの画像
	DX_2DObject* m_pObj;
	// 座標
	DirectX::XMFLOAT2 m_pos;
	// サイズ
	DirectX::XMFLOAT2 m_size;

	// 弾幕クラス
	BarrageManager* m_pBarrageManager;

	// プレイヤーステータス
	PlayerStatus m_status;

	//------------------------------------------------------------------------------
	//
	//  @brief		移動
	//
	//------------------------------------------------------------------------------
	void Move();

	//------------------------------------------------------------------------------
	//
	//  @brief		移動
	//
	//------------------------------------------------------------------------------
	void Attack();

};

#endif