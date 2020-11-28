#include	"DX_Library.h"

//-----------------------------------------------------------------------------------------
//
//  マクロ定義
//
//-----------------------------------------------------------------------------------------
#define MOUSE_L_BUTTON_NUM 0 
#define MOUSE_M_BUTTON_NUM 1
#define MOUSE_R_BUTTON_NUM 2

#define MOUSE_BUTTON_FALSE 0x00
#define MOUSE_BUTTON_TRUE  0x03
#define MOUSE_BUTTON_CLICK 0x01
#define MOUSE_BUTTON_DOUBLE_CLICK 0x02
 
/// <summary>
/// メンバ変数を初期化する
/// </summary>
DX_Input::DX_Input() : m_mouseWheelRotateCount(0), m_mousePos(DirectX::XMINT2(0, 0)), m_mouseClientPos(DirectX::XMINT2(0, 0))
{
	ZeroMemory(m_pressedKeys, sizeof(m_pressedKeys));
	ZeroMemory(m_pressedPrevKeys, sizeof(m_pressedPrevKeys));
	ZeroMemory(m_mouseButtons, sizeof(m_mouseButtons));
	ZeroMemory(m_prevMouseButtons, sizeof(m_prevMouseButtons));
}

/// <summary>
/// デバイス等の初期化を行う
/// </summary>
DX_Input::~DX_Input()
{
	if (m_directInput)
	{
		if (m_directInputDevice)
		{
			m_directInputDevice->Unacquire();
		}
	}
}

/// <summary>
/// メンバ変数を初期化する
/// </summary>
/// <param name="hWnd">ウィンドウハンドル</param>
/// <param name="hInstance">インスタントハンドル</param>
/// <returns>成否</returns>
bool DX_Input::Initialize(HWND hWnd, HINSTANCE hInstance)
{
	bool succeed = false;

	DX_Debug* debug = DX_Debug::GetInstance();

	do
	{
		if (debug->IsFailedHresult(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL)))
		{
			TRACE("failed to DirectInput8Create()");
			break;
		}

		if (debug->IsFailedHresult(m_directInput->CreateDevice(GUID_SysKeyboard, &m_directInputDevice, NULL)))
		{
			TRACE("failed to CreateDevice()");
			break;
		}

		if (debug->IsFailedHresult(m_directInputDevice->SetDataFormat(&c_dfDIKeyboard)))
		{
			TRACE("failed to SetDataFormat()");
			break;
		}

		if (debug->IsFailedHresult(m_directInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		{
			TRACE("failed to SetCooperativeLevel()");
			break;
		}

		if (debug->IsFailedHresult(m_directInputDevice->Acquire()))
		{
			TRACE("failed to Acquire()");
			break;
		}
		succeed = true;
	} while (false);

	return true;
}

/// <summary>
/// キーへのアクション時の反応を得る
/// </summary>
/// <param name="key">DX_INPUT_KEY</param>
/// <returns>アクションがあったかどうか</returns>
bool DX_Input::IsKey(DX_INPUT_KEY key)
{
	//	現在、または過去がtrueなら、どれかのアクションをしている
	return (m_pressedKeys[DX::CAST::I(key)] || m_pressedPrevKeys[DX::CAST::I(key)] ) ? true : false;
}

/// <summary>
/// キーを押している間の反応を取得する
/// </summary>
/// <param name="key">DX_INPUT_KEY</param>
bool DX_Input::IsKeyDown(DX_INPUT_KEY key)
{
	//	現在true : 過去true なら押している間
	return (m_pressedKeys[DX::CAST::I(key)] && m_pressedPrevKeys[DX::CAST::I(key)]) ? true : false;
}

/// <summary>
/// キーを離した瞬間の反応を取得する
/// </summary>
/// <param name="key">DX_INPUT_KEY</param>
bool DX_Input::IsKeyRelease(DX_INPUT_KEY key)
{
	//	現在false : 過去true なら離した瞬間
	return (!m_pressedKeys[DX::CAST::I(key)] && m_pressedPrevKeys[DX::CAST::I(key)]) ? true : false;
}

/// <summary>
/// キーを押した瞬間の反応を取得する
/// </summary>
/// <param name="key">DX_INPUT_KEY</param>
bool DX_Input::IsKeyHit(DX_INPUT_KEY key)
{
	//	現在true : 過去false なら押した瞬間
	return  (m_pressedKeys[DX::CAST::I(key)] && !m_pressedPrevKeys[DX::CAST::I(key)]) ? true : false;
}

/// <summary>
/// マウスボタンへのアクション時の反応を取得する
/// </summary>
/// <param name="kind">マウスのボタンの種類</param>
bool DX_Input::IsMouseButton(DX_MOUSE_BUTTON_KIND kind)
{
	//	現在false : 過去false 以外なら全状態取得
	return ((m_mouseButtons[DX::CAST::I(kind)]) || (m_prevMouseButtons[DX::CAST::I(kind)])) ? true : false;
}

/// <summary>
/// マウスボタンを押している間の反応を取得する
/// </summary>
/// <param name="kind">マウスのボタンの種類</param>
bool DX_Input::IsMouseButtonDown(DX_MOUSE_BUTTON_KIND kind)
{
	//	現在true : 過去true なら押している間
	return ((m_mouseButtons[DX::CAST::I(kind)]) && (m_prevMouseButtons[DX::CAST::I(kind)])) ? true : false;
}

/// <summary>
/// マウスボタンを離した時の反応を取得する
/// </summary>
/// <param name="kind">マウスのボタンの種類</param>
bool DX_Input::IsMouseButtonRelease(DX_MOUSE_BUTTON_KIND kind)
{
	//	現在false : 過去true なら離した瞬間
	return ((!m_mouseButtons[DX::CAST::I(kind)]) && (m_prevMouseButtons[DX::CAST::I(kind)])) ? true : false;
}


/// <summary>
/// マウスボタンのクリックの反応を取得する
/// </summary>
/// <param name="kind">マウスのボタンの種類</param>
bool DX_Input::IsMouseButtonClick(DX_MOUSE_BUTTON_KIND kind)
{
	//	現在true : 過去false なら押した瞬間瞬間
	return ((m_mouseButtons[DX::CAST::I(kind)]) && (!m_prevMouseButtons[DX::CAST::I(kind)])) ? true : false;
		 
}

/// <summary>
/// マウスボタンのダブルクリックの反応を取得する
/// </summary>
/// <param name="kind">マウスのボタンの種類</param>
bool DX_Input::IsMouseButtonDoubleClick(DX_MOUSE_BUTTON_KIND kind)
{
	return  (m_mouseButtons[DX::CAST::I(kind)] == MOUSE_BUTTON_DOUBLE_CLICK) ? true : false;
}

/// <summary>
/// マウス座標を取得する
/// </summary>
/// <returns>座標</returns>
DirectX::XMFLOAT2 DX_Input::GetMousePos()
{
	DX_System* system = DX_System::GetInstance();

	POINT pos;

	//	マウスのスクリーン座標を取得
	GetCursorPos(&pos);

	//	スクリーン座標からウィンド領域への変換
	ClientToScreen(system->GetWindowHandle(), &pos);

	//	ウィンドウ座標
	LONG width	= DX::CAST::L(system->GetScreenWidth());
	LONG height	= DX::CAST::L(system->GetScreenHeight());

	if (pos.x < 0)
	{
		pos.x = 0; 
	}
	else if (pos.x > width)
	{ 
		pos.x = width;
	}

	if (pos.y < 0)
	{ 
		pos.y = 0;
	}
	else if (pos.y > height)
	{ 
		pos.y = height;
	}

	return DirectX::XMFLOAT2(DX::CAST::F(pos.x), DX::CAST::F(pos.y));
}

/// <summary>
/// マウスホイールを前(奥)に動かした回転数を取得する
/// </summary>
/// <returns>回転数</returns>
int DX_Input::GetMouseWheelUp()
{
	return m_mouseWheelRotateCount;
}

/// <summary>
/// マウスホイールを後ろ(手前)に動かした回転数を取得する
/// </summary>
/// <returns>回転数</returns>
int DX_Input::GetMouseWheelDown()
{
	return m_mouseWheelRotateCount;
}

/// <summary>
/// マウス座標を取得する
/// </summary>
/// <param name="outPos">マウス座標を格納する</param>
void DX_Input::GetMousePos(DirectX::XMINT2* outPos)
{
	outPos->x = m_mousePos.x;
	outPos->y = m_mousePos.y;
}

/// <summary>
/// マウス座標を取得する
/// </summary>
/// <param name="outPos">マウス座標を格納する</param>
void DX_Input::GetMousePos(DirectX::XMFLOAT2* outPos)
{
	outPos->x = DX::CAST::F(m_mousePos.x);
	outPos->y = DX::CAST::F(m_mousePos.y);
}

/// <summary>
/// クライアント領域のマウス座標を取得する
/// </summary>
/// <param name="outPos">マウス座標を格納する</param>
void DX_Input::GetMouseClientPos(DirectX::XMINT2* outPos)
{
	outPos->x = m_mouseClientPos.x;
	outPos->y = m_mouseClientPos.y;
}

/// <summary>
/// クライアント領域のマウス座標を取得する
/// </summary>
/// <param name="outPos">マウス座標を格納する</param>
void DX_Input::GetMouseClientPos(DirectX::XMFLOAT2* outPos)
{
	outPos->x = DX::CAST::F(m_mouseClientPos.x);
	outPos->y = DX::CAST::F(m_mouseClientPos.y);
}

/// <summary>
/// デバイスの入力情報を更新する
/// </summary>
/// <param name="hWnd">ウィンドウハンドル</param>
/// <param name="message">MSG.message(windowsのメッセージ)</param>
/// <param name="wParam">MSG.wParam</param>
void DX_Input::Update(const HWND hWnd, const unsigned int message,const WPARAM wParam)
{
	KeyUpdate();
	MouseUpdate(hWnd, message, wParam);
}

/// <summary>
/// キー、マウスの入力の更新する
/// </summary>
void DX_Input::KeyUpdate()
{
	char buffer[INPUT_KEY_MAX];
	
	if (SUCCEEDED(m_directInputDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer)))
	{
		for (int i = 0; i < INPUT_KEY_MAX; ++i)
		{
			//	現在の状態をコピー
			m_pressedPrevKeys[i] = m_pressedKeys[i];

			//	キーを押したならtrue、押してないならfalse
			m_pressedKeys[i] = buffer[i] & 0x80;
		}

	}
	else
	{
		// デバイスのアクセス権が取得できるまで取得し続ける。
		while (m_directInputDevice->Acquire() == DIERR_INPUTLOST);
	}
}


/// <summary>
/// マウスの更新を行う
/// </summary>
/// <param name="hWnd">ウィンドウハンドル</param>
/// <param name="message">MSG.message(windowsのメッセージ)</param>
/// <param name="wParam">MSG.wParam</param>
void DX_Input::MouseUpdate(HWND hWnd, const unsigned int message, const WPARAM	 wParam)
{
	//	回転数を０にする
	m_mouseWheelRotateCount = 0;

	//	現在の状態をコピー
	CopyMemory(m_prevMouseButtons, m_mouseButtons, sizeof(m_mouseButtons));

	//	マウスの入力更新を行う
	switch (message){

		//	左ボタンをクリック
	case WM_LBUTTONDOWN:
		m_mouseButtons[MOUSE_L_BUTTON_NUM] = MOUSE_BUTTON_CLICK;
		break;

		//	左ボタンをダブルクリック
	case WM_LBUTTONDBLCLK:
		m_mouseButtons[MOUSE_L_BUTTON_NUM] = MOUSE_BUTTON_DOUBLE_CLICK;
		break;

		//	左ボタンを離す
	case WM_LBUTTONUP:	
		m_mouseButtons[MOUSE_L_BUTTON_NUM] = MOUSE_BUTTON_FALSE;
		break;

		//	中央ボタンをクリック
	case WM_MBUTTONDOWN:	
		m_mouseButtons[MOUSE_M_BUTTON_NUM] = MOUSE_BUTTON_CLICK;
		break;

		//	中央ボタンをダブルクリック
	case WM_MBUTTONDBLCLK:	
		m_mouseButtons[MOUSE_M_BUTTON_NUM] = MOUSE_BUTTON_DOUBLE_CLICK;
		break;

		//	中央ボタンを離す
	case WM_MBUTTONUP:
		m_mouseButtons[MOUSE_M_BUTTON_NUM] = MOUSE_BUTTON_FALSE;
		break;

		//	右ボタンをクリック
	case WM_RBUTTONDOWN:	
		m_mouseButtons[MOUSE_R_BUTTON_NUM] = MOUSE_BUTTON_CLICK;
		break;

		//	右ボタンをダブルクリック
	case WM_RBUTTONDBLCLK:	
		m_mouseButtons[MOUSE_R_BUTTON_NUM] = MOUSE_BUTTON_DOUBLE_CLICK;
		break;

		//	右ボタンを離す
	case WM_RBUTTONUP:
		m_mouseButtons[MOUSE_R_BUTTON_NUM] = MOUSE_BUTTON_FALSE;
		break;

		//	マウスホイールが動いた
	case WM_MOUSEWHEEL:
		m_mouseWheelRotateCount = GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	}

	//カーソル位置を取得する
	tagPOINT point;
	GetCursorPos(&point);
	m_mousePos.x = (int32_t)point.x;
	m_mousePos.y = (int32_t)point.y;

	// クライアント領域の座標に変換する
	ScreenToClient(hWnd,  &point);
	m_mouseClientPos.x = (int32_t)point.x;
	m_mouseClientPos.y = (int32_t)point.y;
}