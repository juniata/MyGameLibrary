#ifndef __DX_X_INPUT_H_
#define __DX_X_INPUT_H_

enum class XINPUT_KEY{
	UP,			
	DOWN,		
	LEFT,
	RIGHT,
	START,
	BACK,
	LEFT_THUMB,
	RIGHT_THUMB,
	LEFT_SHOULDER,
	RIGHT_SHOULDER,
	A,
	B,
	X,
	Y
};

/// <summary>
/// キー、マウス入力を管理
/// </summary>
class DX_XInput
{
public:
	/// <summary>
	/// メンバ変数を初期化
	/// </summary>
	DX_XInput();

	//------------------------------------------------------------------------------
	//
	//  @brief		後始末
	//
	//------------------------------------------------------------------------------
	~DX_XInput();

	//------------------------------------------------------------------------------
	//
	//  @brief		押してない時以外の反応を得る
	//	@param[in]	key		DX_XINPUT_KEY
	//	@param[in]	id		接続ID
	//	@return		true:押した	false:押してない
	//
	//------------------------------------------------------------------------------
	bool IsKey(XINPUT_KEY key, const int id = 0);

	//------------------------------------------------------------------------------
	//
	//  @brief		押している間の反応を得る
	//	@param[in]	key		DX_XINPUT_KEY
	//	@param[in]	id		接続ID
	//	@return		true:押している		false:押していない
	//
	//------------------------------------------------------------------------------
	bool IsKeyDown(XINPUT_KEY key, const int id = 0);

	//------------------------------------------------------------------------------
	//
	//  @brief		離した瞬間の反応を得る
	//	@param[in]	key		DX_XINPUT_KEY
	//	@return		true:離した		false:離していない
	//
	//------------------------------------------------------------------------------
	bool IsKeyRelease(XINPUT_KEY key, const int id = 0);

	//------------------------------------------------------------------------------
	//
	//  @brief		押した瞬間の反応を得る
	//	@param[in]	key		DX_XINPUT_KEY
	//	@return		true:押した		false:押していない
	//
	//------------------------------------------------------------------------------
	bool IsKeyHit(XINPUT_KEY key, const int id = 0);

	//------------------------------------------------------------------------------
	//
	//  @brief		すべてのボタンの入力を受付
	//
	//------------------------------------------------------------------------------
	void Update();


private:
	static const int KEY_MAX = 14;
	static const int NOT_CONNECT = -1;

	struct tagInputInfo {
		XINPUT_STATE state;
		int id;
		bool keys[KEY_MAX];
		bool prevKeys[KEY_MAX];
	} m_players[XUSER_MAX_COUNT];

	int m_connectCount;
	int m_buttonBitJuage[KEY_MAX];

};
#endif // !__DX_X_INPUT_H_
