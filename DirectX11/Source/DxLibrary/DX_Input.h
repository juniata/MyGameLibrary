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


/// <summary>
/// キー、マウスの入力を管理する
/// </summary>
class DX_Input : public DX_Singleton<DX_Input>
{
public:
	/// <summary>
	/// デバイス等の初期化を行う
	/// </summary>
	~DX_Input();

	/// <summary>
	/// メンバ変数を初期化する
	/// </summary>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <param name="hInstance">インスタントハンドル</param>
	/// <returns>成否</returns>
	bool Initialize(HWND hWnd, HINSTANCE hInstance);

	/// <summary>
	/// キーへのアクション時の反応を取得する
	/// </summary>
	/// <param name="key">取得したい反応のキー</param>
	/// <returns>アクションがあったかどうか</returns>
	 bool IsKey(DX_INPUT_KEY key);

	 /// <summary>
	 /// キーを押している間の反応を取得する
	 /// </summary>
	 /// <param name="key">取得したい反応のキー</param>
	 bool IsKeyDown(DX_INPUT_KEY key);
	
	 /// <summary>
	 /// キーを離した瞬間の反応を取得する
	 /// </summary>
	 /// <param name="key">取得したい反応のキー</param>
	 bool IsKeyRelease(DX_INPUT_KEY key);

	 /// <summary>
	 /// キーを押した瞬間の反応を取得する
	 /// </summary>
	 /// <param name="key">取得したい反応のキー</param>
	 bool IsKeyHit(DX_INPUT_KEY key);

	 /// <summary>
	 /// マウスボタンへのアクション時の反応を取得する
	 /// </summary>
	 /// <param name="kind">マウスのボタンの種類</param>
	 bool IsMouseButton(DX_MOUSE_BUTTON_KIND kind);

	 /// <summary>
	 /// マウスボタンを押している間の反応を取得する
	 /// </summary>
	 /// <param name="kind">マウスのボタンの種類</param>
	 bool IsMouseButtonDown(DX_MOUSE_BUTTON_KIND kind);

	 /// <summary>
	 /// マウスボタンを離した時の反応を取得する
	 /// </summary>
	 /// <param name="kind">マウスのボタンの種類</param>
	 bool IsMouseButtonRelease(DX_MOUSE_BUTTON_KIND kind);

	 /// <summary>
	 /// マウスボタンのクリックの反応を取得する
	 /// </summary>
	 /// <param name="kind">マウスのボタンの種類</param>
	 bool IsMouseButtonClick(DX_MOUSE_BUTTON_KIND kind);

	 /// <summary>
	 /// マウスボタンのダブルクリックの反応を取得する
	 /// </summary>
	 /// <param name="kind">マウスのボタンの種類</param>
	 bool IsMouseButtonDoubleClick(DX_MOUSE_BUTTON_KIND kind);

	/// <summary>
	/// マウスホイールを前(奥)に動かした回転数を取得する
	/// </summary>
	/// <returns>回転数</returns>
	int GetMouseWheelUp();

	/// <summary>
	/// マウスホイールを後ろ(手前)に動かした回転数を取得する
	/// </summary>
	/// <returns>回転数</returns>
	int GetMouseWheelDown();

	/// <summary>
	/// マウス座標を取得する
	/// </summary>
	/// <returns>座標</returns>
	DirectX::XMFLOAT2 GetMousePos();

	/// <summary>
	/// マウス座標を取得する
	/// </summary>
	/// <param name="outPos">マウス座標を格納する</param>
	void GetMousePos(DirectX::XMINT2* outPos);

	/// <summary>
	/// マウス座標を取得する
	/// </summary>
	/// <param name="outPos">マウス座標を格納する</param>
	void GetMousePos(DirectX::XMFLOAT2* outPos);

	/// <summary>
	/// クライアント領域のマウス座標を取得する
	/// </summary>
	/// <param name="outPos">マウス座標を格納する</param>
	void GetMouseClientPos(DirectX::XMINT2* outPos);

	/// <summary>
	/// クライアント領域のマウス座標を取得する
	/// </summary>
	/// <param name="outPos">マウス座標を格納する</param>
	void GetMouseClientPos(DirectX::XMFLOAT2* outPos);

	/// <summary>
	/// デバイスの入力情報を更新する
	/// </summary>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <param name="message">MSG.message(windowsのメッセージ)</param>
	/// <param name="wParam">MSG.wParam</param>
	void Update(const HWND hWnd, const unsigned int message, const WPARAM	wParam);

private:
	friend class DX_Singleton<DX_Input>;
	Microsoft::WRL::ComPtr<IDirectInput8> m_directInput;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> m_directInputDevice;

	static const int INPUT_KEY_MAX		= 256;
	static const int MOUSE_BUTTON_MAX	= 3;


	//	ボタンの数だけキーを押したかどうか
	bool m_pressedKeys[INPUT_KEY_MAX];

	//	過去のキーフラグ
	bool m_pressedPrevKeys[INPUT_KEY_MAX];

	//	マウスの数だけキーを押した稼動亜ｋ
	int m_mouseButtons[MOUSE_BUTTON_MAX];

	//	過去のマウスフラグ
	int m_prevMouseButtons[MOUSE_BUTTON_MAX];

	//	マウスホイールの回転数
	int m_mouseWheelRotateCount;

	// マウスの座標
	 DirectX::XMINT2 m_mousePos;
	 DirectX::XMINT2 m_mouseClientPos;


	 /// <summary>
	 /// メンバ変数を初期化する
	 /// </summary>
	 DX_Input();


	 /// <summary>
	 /// キー、マウスの入力の更新する
	 /// </summary>
	 void KeyUpdate();

	/// <summary>
	/// マウスの更新を行う
	/// </summary>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <param name="message">MSG.message(windowsのメッセージ)</param>
	/// <param name="wParam">MSG.wParam</param>
	void MouseUpdate(const HWND hWnd, const unsigned int message, const WPARAM wParam);
};
#endif // !__DX_INPUT_H_
