#include	"DX_Library.h"
#pragma comment(lib, "XInput.lib")
//-----------------------------------------------------------------------------------------
//
//  メンバ変数を初期化
//
//-----------------------------------------------------------------------------------------
DX_XInput::XInput() :
m_connectCount(0)
{
	//	初期化
	for (int i = 0; i < XUSER_MAX_COUNT; ++i){
		ZeroMemory(&m_players[i].state,		sizeof(m_players[i].state));
		ZeroMemory(&m_players[i].keys,		sizeof(m_players[i].keys));
		ZeroMemory(&m_players[i].prevKeys,	sizeof(m_players[i].prevKeys));
		m_players[i].id = -1;
	}

	m_buttonBitJuage[0] = XINPUT_GAMEPAD_DPAD_UP;
	m_buttonBitJuage[1] = XINPUT_GAMEPAD_DPAD_DOWN;
	m_buttonBitJuage[2] = XINPUT_GAMEPAD_DPAD_LEFT;
	m_buttonBitJuage[3] = XINPUT_GAMEPAD_DPAD_RIGHT;
	m_buttonBitJuage[4] = XINPUT_GAMEPAD_START;
	m_buttonBitJuage[5] = XINPUT_GAMEPAD_BACK;
	m_buttonBitJuage[6] = XINPUT_GAMEPAD_LEFT_THUMB;
	m_buttonBitJuage[7] = XINPUT_GAMEPAD_RIGHT_THUMB;
	m_buttonBitJuage[8] = XINPUT_GAMEPAD_LEFT_SHOULDER;
	m_buttonBitJuage[9] = XINPUT_GAMEPAD_RIGHT_SHOULDER;
	m_buttonBitJuage[10] = XINPUT_GAMEPAD_A;
	m_buttonBitJuage[11] = XINPUT_GAMEPAD_B;
	m_buttonBitJuage[12] = XINPUT_GAMEPAD_X;
	m_buttonBitJuage[13] = XINPUT_GAMEPAD_Y;

}

//-----------------------------------------------------------------------------------------
//
//  後始末
//
//-----------------------------------------------------------------------------------------
DX_XInput::~XInput()
{

}


//-----------------------------------------------------------------------------------------
//
//  押してない時以外の反応を得る
//
//-----------------------------------------------------------------------------------------
bool DX_XInput::IsKey(dx_library::XINPUT_KEY key, const int id)
{
	const int l_key = CAST_I(key);

	//	未接続ならfalseを返す
	if (m_players[id].id == -1){ return false; }

	
	//	現在false : 過去false 以外なら全状態取得
	return (!m_players[id].keys[l_key] && !m_players[id].prevKeys[l_key]) ? false : true;
}

//-----------------------------------------------------------------------------------------
//
//  押している間の反応を得る
//
//-----------------------------------------------------------------------------------------
bool DX_XInput::IsKeyDown(dx_library::XINPUT_KEY key, const int id)
{
	const int l_key = CAST_I(key);

	//	未接続ならfalseを返す
	if (m_players[id].id == -1){ return false; }

	//	現在true : 過去true なら押している間
	return (m_players[id].keys[l_key] && m_players[id].prevKeys[l_key]) ? true : false;

}

//-----------------------------------------------------------------------------------------
//
//  離した瞬間の反応を得る
//
//-----------------------------------------------------------------------------------------
bool DX_XInput::IsKeyRelease(dx_library::XINPUT_KEY key, const int id)
{
	const int l_key = CAST_I(key);

	//	未接続ならfalseを返す
	if (m_players[id].id == -1){ return false; }

	//	現在false : 過去true なら離した瞬間
	return (!m_players[id].keys[l_key] && m_players[id].prevKeys[l_key]) ? true : false;

}


//-----------------------------------------------------------------------------------------
//
//  押した瞬間の反応を得る
//
//-----------------------------------------------------------------------------------------
bool DX_XInput::IsKeyHit(dx_library::XINPUT_KEY key, const int id)
{
	const int l_key = CAST_I(key);

	//	未接続ならfalseを返す
	if (m_players[id].id == -1){ return false; }

	//	現在false : 過去true なら離した瞬間
	return (m_players[id].keys[l_key] && !m_players[id].prevKeys[l_key]) ? true : false;

}

//-----------------------------------------------------------------------------------------
//
//  すべてのボタンの入力を受け付ける
//
//-----------------------------------------------------------------------------------------
void DX_XInput::Update()
{
	for (int i = 0; i < XUSER_MAX_COUNT; ++i){

		//	コントローラーの入力を確認する
		if (XInputGetState(i, &m_players[i].state) == ERROR_SUCCESS){

			//	入力を確認したのでＩＤを振り分ける
			m_players[i].id = i;

			//	ビット演算で入力されているものを判定する
			for (int j = 0; j < KEY_MAX; ++j){
				m_players[i].prevKeys[j] = m_players[i].keys[j];
				if (m_players[i].state.Gamepad.wButtons & m_buttonBitJuage[j]){
					m_players[i].keys[j] = true;
				}
				else{
					m_players[i].keys[j] = false;
				}
			}
		}
		else{
			//	接続されていないので未接続IDにする
			m_players[i].id = -1;
		}
	}
}
