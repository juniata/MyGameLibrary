#include	"DX_Library.h"
#pragma comment(lib, "XInput.lib")
//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ���������
//
//-----------------------------------------------------------------------------------------
DX_XInput::XInput() :
m_connectCount(0)
{
	//	������
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
//  ��n��
//
//-----------------------------------------------------------------------------------------
DX_XInput::~XInput()
{

}


//-----------------------------------------------------------------------------------------
//
//  �����ĂȂ����ȊO�̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_XInput::IsKey(dx_library::XINPUT_KEY key, const int id)
{
	const int l_key = CAST_I(key);

	//	���ڑ��Ȃ�false��Ԃ�
	if (m_players[id].id == -1){ return false; }

	
	//	����false : �ߋ�false �ȊO�Ȃ�S��Ԏ擾
	return (!m_players[id].keys[l_key] && !m_players[id].prevKeys[l_key]) ? false : true;
}

//-----------------------------------------------------------------------------------------
//
//  �����Ă���Ԃ̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_XInput::IsKeyDown(dx_library::XINPUT_KEY key, const int id)
{
	const int l_key = CAST_I(key);

	//	���ڑ��Ȃ�false��Ԃ�
	if (m_players[id].id == -1){ return false; }

	//	����true : �ߋ�true �Ȃ牟���Ă����
	return (m_players[id].keys[l_key] && m_players[id].prevKeys[l_key]) ? true : false;

}

//-----------------------------------------------------------------------------------------
//
//  �������u�Ԃ̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_XInput::IsKeyRelease(dx_library::XINPUT_KEY key, const int id)
{
	const int l_key = CAST_I(key);

	//	���ڑ��Ȃ�false��Ԃ�
	if (m_players[id].id == -1){ return false; }

	//	����false : �ߋ�true �Ȃ痣�����u��
	return (!m_players[id].keys[l_key] && m_players[id].prevKeys[l_key]) ? true : false;

}


//-----------------------------------------------------------------------------------------
//
//  �������u�Ԃ̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_XInput::IsKeyHit(dx_library::XINPUT_KEY key, const int id)
{
	const int l_key = CAST_I(key);

	//	���ڑ��Ȃ�false��Ԃ�
	if (m_players[id].id == -1){ return false; }

	//	����false : �ߋ�true �Ȃ痣�����u��
	return (m_players[id].keys[l_key] && !m_players[id].prevKeys[l_key]) ? true : false;

}

//-----------------------------------------------------------------------------------------
//
//  ���ׂẴ{�^���̓��͂��󂯕t����
//
//-----------------------------------------------------------------------------------------
void DX_XInput::Update()
{
	for (int i = 0; i < XUSER_MAX_COUNT; ++i){

		//	�R���g���[���[�̓��͂��m�F����
		if (XInputGetState(i, &m_players[i].state) == ERROR_SUCCESS){

			//	���͂��m�F�����̂łh�c��U�蕪����
			m_players[i].id = i;

			//	�r�b�g���Z�œ��͂���Ă�����̂𔻒肷��
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
			//	�ڑ�����Ă��Ȃ��̂Ŗ��ڑ�ID�ɂ���
			m_players[i].id = -1;
		}
	}
}
