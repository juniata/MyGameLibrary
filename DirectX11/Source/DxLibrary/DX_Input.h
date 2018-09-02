#ifndef __DX_INPUT_H_
#define __DX_INPUT_H_

//	キーボードのボタンの種類
#include	"DX_InputDefines.h"

//	マウスのボタンの種類
enum class DX_MOUSE_BUTTON_KIND : int{
	BUTTON_L,	//	左ボタン
	BUTTON_M,	//	中央ボタン
	BUTTON_R,	//	右ボタン
};

//****************************************************************************************************
//
//	Input
//
//****************************************************************************************************
class DX_Input
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		キーへのアクション時の反応を得る
	//	@param[in]	key		DX_INPUT_KEY
	//	@return		true:押した	false:押してない
	//
	//------------------------------------------------------------------------------
	static bool IsKey(DX_INPUT_KEY key);

	//------------------------------------------------------------------------------
	//
	//  @brief		押している間の反応を得る
	//	@param[in]	key		DX_INPUT_KEY
	//	@return		true:押している		false:押していない
	//
	//------------------------------------------------------------------------------
	static bool IsKeyDown(DX_INPUT_KEY key);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		離した瞬間の反応を得る
	//	@param[in]	key		DX_INPUT_KEY
	//	@return		true:離した		false:離していない
	//
	//------------------------------------------------------------------------------
	static bool IsKeyRelease(DX_INPUT_KEY key);

	//------------------------------------------------------------------------------
	//
	//  @brief		押した瞬間の反応を得る
	//	@param[in]	key		DX_INPUT_KEY
	//	@return		true:押した		false:押していない
	//
	//------------------------------------------------------------------------------
	static bool IsKeyHit(DX_INPUT_KEY key);

	//------------------------------------------------------------------------------
	//
	//  @brief		マウスボタンへのアクション時の反応を得る
	//	@param[in]	kind		マウスのボタンの種類
	//	@return		true:押した		false:押していない
	//
	//------------------------------------------------------------------------------
	static bool IsMouseButton(DX_MOUSE_BUTTON_KIND kind);

	//------------------------------------------------------------------------------
	//
	//  @brief		マウスボタンを押している間の反応を得る
	//	@param[in]	kind		マウスのボタンの種類
	//	@return		true:押している		false:押していない
	//
	//------------------------------------------------------------------------------
	static bool IsMouseButtonDown(DX_MOUSE_BUTTON_KIND kind);

	//------------------------------------------------------------------------------
	//
	//  @brief		マウスボタンを離した時の反応を得る
	//	@param[in]	kind		マウスのボタンの種類
	//	@return		true:離した		false:押していない
	//
	//------------------------------------------------------------------------------
	static bool IsMouseButtonRelease(DX_MOUSE_BUTTON_KIND kind);

	//------------------------------------------------------------------------------
	//
	//  @brief		マウスボタンのクリックの反応を得る
	//	@param[in]	kind		マウスのボタンの種類
	//	@return		true:押した		false:押していない
	//	@note		キーのhit関数と同じ効果
	//
	//------------------------------------------------------------------------------
	static bool IsMouseButtonClick(DX_MOUSE_BUTTON_KIND kind);

	//------------------------------------------------------------------------------
	//
	//  @brief		マウスボタンのダブルクリックの反応を得る
	//	@param[in]	kind		マウスのボタンの種類
	//	@return		true:押した		false:押していない
	//
	//------------------------------------------------------------------------------
	static bool IsMouseButtonDoubleClick(DX_MOUSE_BUTTON_KIND kind);

	//------------------------------------------------------------------------------
	//
	//  @brief	マウス座標を取得
	//	@return ウィンドウ内のマウス座標
	//
	//------------------------------------------------------------------------------
	static DirectX::XMFLOAT2 GetMousePos();

	//------------------------------------------------------------------------------
	//
	//  @brief	マウスホイールを前(奥)に動かした回転数を取得
	//	@return	回転数
	//
	//------------------------------------------------------------------------------
	static int GetMouseWheelUp();

	//------------------------------------------------------------------------------
	//
	//  @brief	マウスホイールを後ろ(手前)に動かした回転数を取得
	//	@return	回転数
	//
	//------------------------------------------------------------------------------
	static int GetMouseWheelDown();

	//------------------------------------------------------------------------------
	//
	//  @brief	マウス座標を取得する
	//	@param[out]	pPos	マウス座標
	//
	//------------------------------------------------------------------------------
	static void GetMousePos(DirectX::XMINT2* pPos);
	static void GetMousePos(DirectX::XMFLOAT2* pPos);


	//------------------------------------------------------------------------------
	//
	//  @brief	クライアント領域のマウス座標を取得する
	//	@param[out]	pPos	マウス座標
	//
	//------------------------------------------------------------------------------
	static void GetMouseClientPos(DirectX::XMINT2* pPos);
	static void GetMouseClientPos(DirectX::XMFLOAT2* pPos);

	//------------------------------------------------------------------------------
	//
	//  @brief		デバイスの入力情報を更新する
	//	@param[in]	hWnd	ウィンドウハンドル
	//	@param[in]	message	MSG.message(windowsのメッセージ)
	//	@param[in]	wParam	MSG.wParam
	//
	//------------------------------------------------------------------------------
	static void Update(const HWND hWnd, const unsigned int message, const WPARAM	wParam);

private:
	//	キーボードの受け付け数
	static const int INPUT_KEY_MAX = 254;

	//	ボタンの数だけキーを押したかどうか
	static bool m_bKeys[INPUT_KEY_MAX];

	//	過去のキーフラグ
	static bool m_bPrevKeys[INPUT_KEY_MAX];

	//	マウスのボタン数
	static const int MOUSE_BUTTON_MAX = 3;

	//	マウスの数だけキーを押した稼動亜ｋ
	static int m_bMouseButtons[MOUSE_BUTTON_MAX];

	//	過去のマウスフラグ
	static int m_bPrevMouseButtons[MOUSE_BUTTON_MAX];

	//	マウスホイールの回転数
	static int m_mouseWheelRotateCount;

	// マウスの座標
	static DirectX::XMINT2 m_mousePos;
	static DirectX::XMINT2 m_mouseClientPos;

	//------------------------------------------------------------------------------
	//
	//  @brief		キーの更新を行う
	//
	//------------------------------------------------------------------------------
	static void KeyUpdate();

	//------------------------------------------------------------------------------
	//
	//  @brief		マウスの更新を行う
	//	@param[in]	hWnd	ウィンドウハンドル
	//	@param[in]	message	DX_Input::Updateの引数をそのまま入れる
	//	@param[in]	wParam	DX_Input::Uddateの引数をそのまま入れる
	//
	//------------------------------------------------------------------------------
	static void MouseUpdate(const HWND hWnd, const unsigned int message, const WPARAM wParam);
};
#endif // !__DX_INPUT_H_
