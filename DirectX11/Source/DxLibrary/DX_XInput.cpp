#include	"DX_Library.h"
#pragma comment(lib, "XInput.lib")
//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ���������
//
//-----------------------------------------------------------------------------------------
DX_XInput::DX_XInput() :
m_connectCount(0)
{
	//	������
	for (int i = 0; i < XUSER_MAX_COUNT; ++i){
		ZeroMemory(&m_players[i].state,		sizeof(m_players[i].state));
		ZeroMemory(&m_players[i].keys,		sizeof(m_players[i].keys));
		ZeroMemory(&m_players[i].prevKeys,	sizeof(m_players[i].prevKeys));
		m_players[i].id = NOT_CONNECT;
	}

	// XINPUT_KEY�Ƃ̕R�Â�
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
DX_XInput::~DX_XInput()
{

}


//-----------------------------------------------------------------------------------------
//
//  �����ĂȂ����ȊO�̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_XInput::IsKey(XINPUT_KEY key, const int id)
{
	if (m_players[id].id == NOT_CONNECT) {
		return false;
	}
	
	tagInputInfo& player = m_players[id];
	const int index = static_cast<int>(key);
	
	//	����false : �ߋ�false �ȊO�Ȃ�S��Ԏ擾
	return (!player.keys[index] && !player.prevKeys[index]) ? false : true;
}

//-----------------------------------------------------------------------------------------
//
//  �����Ă���Ԃ̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_XInput::IsKeyDown(XINPUT_KEY key, const int id)
{
	if (m_players[id].id == NOT_CONNECT) {
		return false;
	}

	tagInputInfo& player = m_players[id];
	const int index = static_cast<int>(key);

	//	����true : �ߋ�true �Ȃ牟���Ă����
	return (player.keys[index] && player.prevKeys[index]) ? true : false;

}

//-----------------------------------------------------------------------------------------
//
//  �������u�Ԃ̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_XInput::IsKeyRelease(XINPUT_KEY key, const int id)
{
	if (m_players[id].id == NOT_CONNECT) {
		return false;
	}

	tagInputInfo& player = m_players[id];
	const int index = static_cast<int>(key);


	//	����false : �ߋ�true �Ȃ痣�����u��
	return (!player.keys[index] && player.prevKeys[index]) ? true : false;

}


//-----------------------------------------------------------------------------------------
//
//  �������u�Ԃ̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_XInput::IsKeyHit(XINPUT_KEY key, const int id)
{
	if (m_players[id].id == NOT_CONNECT) {
		return false;
	}

	tagInputInfo& player = m_players[id];
	const int index = static_cast<int>(key);

	//	����false : �ߋ�true �Ȃ痣�����u��
	return (player.keys[index] && !player.prevKeys[index]) ? true : false;

}

//-----------------------------------------------------------------------------------------
//
//  ���ׂẴ{�^���̓��͂��󂯕t����
//
//-----------------------------------------------------------------------------------------
void DX_XInput::Update()
{
	tagInputInfo* pPlayer = nullptr;

	for (int i = 0; i < XUSER_MAX_COUNT; ++i){

		pPlayer = &m_players[i];

		//	�R���g���[���[�̓��͂��m�F����
		if (XInputGetState(i, &pPlayer->state) == ERROR_SUCCESS){

			//	���͂��m�F�����̂łh�c��U�蕪����
			pPlayer->id = i;

			//	�r�b�g���Z�œ��͂���Ă�����̂𔻒肷��
			for (int j = 0; j < KEY_MAX; ++j){
				pPlayer->prevKeys[j] = pPlayer->keys[j];
				if (pPlayer->state.Gamepad.wButtons & m_buttonBitJuage[j]) {
					pPlayer->keys[j] = true;
				} else {
					pPlayer->keys[j] = false;
				}
			}
		} else {
			//	�ڑ�����Ă��Ȃ��̂Ŗ��ڑ�ID�ɂ���
			pPlayer->id = NOT_CONNECT;
		}
	}
}
