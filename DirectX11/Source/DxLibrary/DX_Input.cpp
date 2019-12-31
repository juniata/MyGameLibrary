#include	"DX_Library.h"

//-----------------------------------------------------------------------------------------
//
//  staticメンバ変数
//
//-----------------------------------------------------------------------------------------
bool			DX_Input::m_bKeys[INPUT_KEY_MAX]				= { false };
bool			DX_Input::m_bPrevKeys[INPUT_KEY_MAX]			= { false };
int				DX_Input::m_bMouseButtons[MOUSE_BUTTON_MAX]		= { 0 };
int				DX_Input::m_bPrevMouseButtons[MOUSE_BUTTON_MAX] = { 0 };
int				DX_Input::m_mouseWheelRotateCount				= 0;
DirectX::XMINT2	DX_Input::m_mousePos							= DirectX::XMINT2(0, 0);
DirectX::XMINT2	DX_Input::m_mouseClientPos						= DirectX::XMINT2(0, 0);

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
LPDIRECTINPUT8 DX_Input::m_pDirectInput = nullptr;
LPDIRECTINPUTDEVICE8 DX_Input::m_pDirectInputDevice = nullptr;

bool DX_Input::Initialize(HWND hWnd, HINSTANCE hInstance)
{
	if (!DX_Debug::GetInstance()->CheckHresult(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL))) {
		TRACE("failed to DirectInput8Create()");
		return false;
	}

	if (!DX_Debug::GetInstance()->CheckHresult(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDirectInputDevice, NULL))) {
		TRACE("failed to CreateDevice()");
		return false;
	}


	if (!DX_Debug::GetInstance()->CheckHresult(m_pDirectInputDevice->SetDataFormat(&c_dfDIKeyboard))) {
		TRACE("failed to SetDataFormat()");
		return false;
	}

	if (!DX_Debug::GetInstance()->CheckHresult(m_pDirectInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
		TRACE("failed to SetCooperativeLevel()");
		return false;
	}

	if (!DX_Debug::GetInstance()->CheckHresult(m_pDirectInputDevice->Acquire())) {
		TRACE("failed to Acquire()");
		return false;
	}

	return true;
}

void DX_Input::Release()
{
	if (m_pDirectInput)
	{
		if (m_pDirectInputDevice)
		{
			m_pDirectInputDevice->Unacquire();
			SAFE_RELEASE(m_pDirectInputDevice);
		}
		SAFE_RELEASE(m_pDirectInput);
	}
}
//-----------------------------------------------------------------------------------------
//
//  押してない時以外の反応を得る
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsKey(DX_INPUT_KEY key)
{
	//	現在、または過去がtrueなら、どれかのアクションをしている
	return ( m_bKeys[DX::CAST::I(key)] || m_bPrevKeys[DX::CAST::I(key)] ) ? true : false;
}


//-----------------------------------------------------------------------------------------
//
//  押している間の反応を得る
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsKeyDown(DX_INPUT_KEY key)
{
	//	現在true : 過去true なら押している間
	return (m_bKeys[DX::CAST::I(key)] && m_bPrevKeys[DX::CAST::I(key)]) ? true : false;
}


//-----------------------------------------------------------------------------------------
//
//  離した瞬間の反応を得る
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsKeyRelease(DX_INPUT_KEY key)
{
	//	現在false : 過去true なら離した瞬間
	return (!m_bKeys[DX::CAST::I(key)] && m_bPrevKeys[DX::CAST::I(key)]) ? true : false;
}

//-----------------------------------------------------------------------------------------
//
//  押した瞬間の反応を得る
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsKeyHit(DX_INPUT_KEY key)
{
	//	現在true : 過去false なら押した瞬間
	return  (m_bKeys[DX::CAST::I(key)] && !m_bPrevKeys[DX::CAST::I(key)]) ? true : false;
}

//-----------------------------------------------------------------------------------------
//
//	マウスボタンへのアクション時の反応を得る
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsMouseButton(DX_MOUSE_BUTTON_KIND kind)
{
	//	現在false : 過去false 以外なら全状態取得
	return (
		(m_bMouseButtons[DX::CAST::I(kind)]) ||
		(m_bPrevMouseButtons[DX::CAST::I(kind)])
		) ? true : false;
}

//-----------------------------------------------------------------------------------------
//
//	マウスボタンを押している間の反応を得る
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsMouseButtonDown(DX_MOUSE_BUTTON_KIND kind)
{

	//	現在true : 過去true なら押している間
	return(
		(m_bMouseButtons[DX::CAST::I(kind)]) &&
		(m_bPrevMouseButtons[DX::CAST::I(kind)])
		) ? true : false;
}

//-----------------------------------------------------------------------------------------
//
//	マウスボタンを離した時の反応を得る
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsMouseButtonRelease(DX_MOUSE_BUTTON_KIND kind)
{
	//	現在false : 過去true なら離した瞬間
	return (
		(!m_bMouseButtons[DX::CAST::I(kind)]) &&
		(m_bPrevMouseButtons[DX::CAST::I(kind)])
		) ? true : false;
}

//-----------------------------------------------------------------------------------------
//
//	マウスボタンのクリックの反応を得る
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsMouseButtonClick(DX_MOUSE_BUTTON_KIND kind)
{
	//	現在true : 過去false なら押した瞬間瞬間
	return(
		(m_bMouseButtons[DX::CAST::I(kind)]) &&
		(!m_bPrevMouseButtons[DX::CAST::I(kind)])
		) ? true : false;
		 
}

//-----------------------------------------------------------------------------------------
//
//	マウスボタンのダブルクリックの反応を得る
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsMouseButtonDoubleClick(DX_MOUSE_BUTTON_KIND kind)
{
	return  (m_bMouseButtons[DX::CAST::I(kind)] == MOUSE_BUTTON_DOUBLE_CLICK) ? true : false;
}

//-----------------------------------------------------------------------------------------
//
//	マウス座標を取得する
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT2 DX_Input::GetMousePos()
{
	DX_System* pSystem = DX_System::GetInstance();

	POINT pos;

	//	マウスのスクリーン座標を取得
	GetCursorPos(&pos);

	//	スクリーン座標からウィンド領域への変換
	ClientToScreen(pSystem->GetWindowHandle(), &pos);

	//	ウィンドウ座標
	LONG width	= DX::CAST::L(pSystem->GetScreenWidth());
	LONG height	= DX::CAST::L(pSystem->GetScreenHeight());

	if (pos.x < 0){ pos.x = 0; }
	else if (pos.x > width){ pos.x = width; }

	if (pos.y < 0){ pos.y = 0; }
	else if (pos.y > height){ pos.y = height; }

	return DirectX::XMFLOAT2(DX::CAST::F(pos.x), DX::CAST::F(pos.y));
}


//-----------------------------------------------------------------------------------------
//
//	マウスホイールを前(奥)に動かした回転数を取得
//
//-----------------------------------------------------------------------------------------
int DX_Input::GetMouseWheelUp()
{
	return m_mouseWheelRotateCount;
}


//-----------------------------------------------------------------------------------------
//
//	マウスホイールを後ろ(手前)に動かした回転数を取得
//
//-----------------------------------------------------------------------------------------
int DX_Input::GetMouseWheelDown()
{
	return m_mouseWheelRotateCount;
}

//-----------------------------------------------------------------------------------------
//
//	マウス座標を取得する
//
//-----------------------------------------------------------------------------------------
void DX_Input::GetMousePos(DirectX::XMINT2* pPos)
{
	pPos->x = m_mousePos.x;
	pPos->y = m_mousePos.y;
}
void DX_Input::GetMousePos(DirectX::XMFLOAT2* pPos)
{
	pPos->x = DX::CAST::F(m_mousePos.x);
	pPos->y = DX::CAST::F(m_mousePos.y);
}

//-----------------------------------------------------------------------------------------
//
//	ゲーム画面でのマウス座標を取得する
//
//-----------------------------------------------------------------------------------------
void DX_Input::GetMouseClientPos(DirectX::XMINT2* pPos)
{
	pPos->x = m_mouseClientPos.x;
	pPos->y = m_mouseClientPos.y;
}
void DX_Input::GetMouseClientPos(DirectX::XMFLOAT2* pPos)
{
	pPos->x = DX::CAST::F(m_mouseClientPos.x);
	pPos->y = DX::CAST::F(m_mouseClientPos.y);
}
//-----------------------------------------------------------------------------------------
//
//  デバイスの入力情報を更新する
//
//-----------------------------------------------------------------------------------------
void DX_Input::Update(const HWND hWnd, const unsigned int message,const WPARAM wParam)
{

	//	キーの更新を行う
	KeyUpdate();

	//	マウスの更新を行う
	MouseUpdate(hWnd, message, wParam);

}

//-----------------------------------------------------------------------------------------
//
//  キーの更新を行う
//
//-----------------------------------------------------------------------------------------
void DX_Input::KeyUpdate()
{
	char buffer[INPUT_KEY_MAX];
	
	if (SUCCEEDED(m_pDirectInputDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer)))
	{
		for (int i = 0; i < INPUT_KEY_MAX; ++i)
		{
			//	現在の状態をコピー
			m_bPrevKeys[i] = m_bKeys[i];

			//	キーを押したならtrue、押してないならfalse
			m_bKeys[i] = buffer[i] & 0x80;
		}

	}
	else {
		// デバイスのアクセス権が取得できるまで取得し続ける。
		while (m_pDirectInputDevice->Acquire() == DIERR_INPUTLOST);
	}
}


//-----------------------------------------------------------------------------------------
//
//  マウスの更新を行う
//
//-----------------------------------------------------------------------------------------
void DX_Input::MouseUpdate(HWND hWnd, const unsigned int message, const WPARAM	 wParam)
{
	//	回転数を０にする
	m_mouseWheelRotateCount = 0;

	//	現在の状態をコピー
	CopyMemory(m_bPrevMouseButtons, m_bMouseButtons, sizeof(m_bMouseButtons));

	//	マウスの入力更新を行う
	switch (message){

		//	左ボタンをクリック
	case WM_LBUTTONDOWN:
		m_bMouseButtons[MOUSE_L_BUTTON_NUM] = MOUSE_BUTTON_CLICK; 
		break;

		//	左ボタンをダブルクリック
	case WM_LBUTTONDBLCLK:
		m_bMouseButtons[MOUSE_L_BUTTON_NUM] = MOUSE_BUTTON_DOUBLE_CLICK;
		break;

		//	左ボタンを離す
	case WM_LBUTTONUP:	
		m_bMouseButtons[MOUSE_L_BUTTON_NUM] = MOUSE_BUTTON_FALSE;
		break;

		//	中央ボタンをクリック
	case WM_MBUTTONDOWN:	
		m_bMouseButtons[MOUSE_M_BUTTON_NUM] = MOUSE_BUTTON_CLICK;
		break;

		//	中央ボタンをダブルクリック
	case WM_MBUTTONDBLCLK:	
		m_bMouseButtons[MOUSE_M_BUTTON_NUM] = MOUSE_BUTTON_DOUBLE_CLICK;
		break;

		//	中央ボタンを離す
	case WM_MBUTTONUP:
		m_bMouseButtons[MOUSE_M_BUTTON_NUM] = MOUSE_BUTTON_FALSE;
		break;

		//	右ボタンをクリック
	case WM_RBUTTONDOWN:	
		m_bMouseButtons[MOUSE_R_BUTTON_NUM] = MOUSE_BUTTON_CLICK;
		break;

		//	右ボタンをダブルクリック
	case WM_RBUTTONDBLCLK:	
		m_bMouseButtons[MOUSE_R_BUTTON_NUM] = MOUSE_BUTTON_DOUBLE_CLICK;
		break;

		//	右ボタンを離す
	case WM_RBUTTONUP:
		m_bMouseButtons[MOUSE_R_BUTTON_NUM] = MOUSE_BUTTON_FALSE;
		break;

		//	マウスホイールが動いた
	case WM_MOUSEWHEEL:

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