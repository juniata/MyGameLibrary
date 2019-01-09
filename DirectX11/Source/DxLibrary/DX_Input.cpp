#include	"DX_Library.h"

//-----------------------------------------------------------------------------------------
//
//  static�����o�ϐ�
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
//  �}�N����`
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
	HRESULT hReuslt = S_FALSE;

	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL))) {
		return false;
	}

	if (FAILED(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDirectInputDevice, NULL))) {
		return false;
	}

	

	if (FAILED(hReuslt = m_pDirectInputDevice->SetDataFormat(&c_dfDIKeyboard))) {
		return false;
	}

	if (FAILED(m_pDirectInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
		return false;
	}

	m_pDirectInputDevice->Acquire();

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
//  �����ĂȂ����ȊO�̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsKey(DX_INPUT_KEY key)
{
	//	���݁A�܂��͉ߋ���true�Ȃ�A�ǂꂩ�̃A�N�V���������Ă���
	return ( m_bKeys[CAST_I(key)] || m_bPrevKeys[CAST_I(key)] ) ? true : false;
}


//-----------------------------------------------------------------------------------------
//
//  �����Ă���Ԃ̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsKeyDown(DX_INPUT_KEY key)
{
	//	����true : �ߋ�true �Ȃ牟���Ă����
	return (m_bKeys[CAST_I(key)] && m_bPrevKeys[CAST_I(key)]) ? true : false;
}


//-----------------------------------------------------------------------------------------
//
//  �������u�Ԃ̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsKeyRelease(DX_INPUT_KEY key)
{
	//	����false : �ߋ�true �Ȃ痣�����u��
	return (!m_bKeys[CAST_I(key)] && m_bPrevKeys[CAST_I(key)]) ? true : false;
}

//-----------------------------------------------------------------------------------------
//
//  �������u�Ԃ̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsKeyHit(DX_INPUT_KEY key)
{
	//	����true : �ߋ�false �Ȃ牟�����u��
	return  (m_bKeys[CAST_I(key)] && !m_bPrevKeys[CAST_I(key)]) ? true : false;
}

//-----------------------------------------------------------------------------------------
//
//	�}�E�X�{�^���ւ̃A�N�V�������̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsMouseButton(DX_MOUSE_BUTTON_KIND kind)
{
	//	����false : �ߋ�false �ȊO�Ȃ�S��Ԏ擾
	return (
		(m_bMouseButtons[CAST_I(kind)]) ||
		(m_bPrevMouseButtons[CAST_I(kind)])
		) ? true : false;
}

//-----------------------------------------------------------------------------------------
//
//	�}�E�X�{�^���������Ă���Ԃ̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsMouseButtonDown(DX_MOUSE_BUTTON_KIND kind)
{

	//	����true : �ߋ�true �Ȃ牟���Ă����
	return(
		(m_bMouseButtons[CAST_I(kind)]) &&
		(m_bPrevMouseButtons[CAST_I(kind)])
		) ? true : false;
}

//-----------------------------------------------------------------------------------------
//
//	�}�E�X�{�^���𗣂������̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsMouseButtonRelease(DX_MOUSE_BUTTON_KIND kind)
{
	//	����false : �ߋ�true �Ȃ痣�����u��
	return (
		(!m_bMouseButtons[CAST_I(kind)]) &&
		(m_bPrevMouseButtons[CAST_I(kind)])
		) ? true : false;
}

//-----------------------------------------------------------------------------------------
//
//	�}�E�X�{�^���̃N���b�N�̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsMouseButtonClick(DX_MOUSE_BUTTON_KIND kind)
{
	//	����true : �ߋ�false �Ȃ牟�����u�ԏu��
	return(
		(m_bMouseButtons[CAST_I(kind)]) &&
		(!m_bPrevMouseButtons[CAST_I(kind)])
		) ? true : false;
		 
}

//-----------------------------------------------------------------------------------------
//
//	�}�E�X�{�^���̃_�u���N���b�N�̔����𓾂�
//
//-----------------------------------------------------------------------------------------
bool DX_Input::IsMouseButtonDoubleClick(DX_MOUSE_BUTTON_KIND kind)
{
	return  (m_bMouseButtons[CAST_I(kind)] == MOUSE_BUTTON_DOUBLE_CLICK) ? true : false;
}

//-----------------------------------------------------------------------------------------
//
//	�}�E�X���W���擾����
//
//-----------------------------------------------------------------------------------------
DirectX::XMFLOAT2 DX_Input::GetMousePos()
{
	DX_System* pSystem = DX_System::GetInstance();

	POINT pos;

	//	�}�E�X�̃X�N���[�����W���擾
	GetCursorPos(&pos);

	//	�X�N���[�����W����E�B���h�̈�ւ̕ϊ�
	ClientToScreen(pSystem->GetWindowHandle(), &pos);

	//	�E�B���h�E���W
	LONG width	= CAST_L(pSystem->GetWindowWidth());
	LONG height	= CAST_L(pSystem->GetWindowHeight());

	if (pos.x < 0){ pos.x = 0; }
	else if (pos.x > width){ pos.x = width; }

	if (pos.y < 0){ pos.y = 0; }
	else if (pos.y > height){ pos.y = height; }

	return DirectX::XMFLOAT2(CAST_F(pos.x), CAST_F(pos.y));
}


//-----------------------------------------------------------------------------------------
//
//	�}�E�X�z�C�[����O(��)�ɓ���������]�����擾
//
//-----------------------------------------------------------------------------------------
int DX_Input::GetMouseWheelUp()
{
	return m_mouseWheelRotateCount;
}


//-----------------------------------------------------------------------------------------
//
//	�}�E�X�z�C�[�������(��O)�ɓ���������]�����擾
//
//-----------------------------------------------------------------------------------------
int DX_Input::GetMouseWheelDown()
{
	return m_mouseWheelRotateCount;
}

//-----------------------------------------------------------------------------------------
//
//	�}�E�X���W���擾����
//
//-----------------------------------------------------------------------------------------
void DX_Input::GetMousePos(DirectX::XMINT2* pPos)
{
	pPos->x = m_mousePos.x;
	pPos->y = m_mousePos.y;
}
void DX_Input::GetMousePos(DirectX::XMFLOAT2* pPos)
{
	pPos->x = CAST_F(m_mousePos.x);
	pPos->y = CAST_F(m_mousePos.y);
}

//-----------------------------------------------------------------------------------------
//
//	�Q�[����ʂł̃}�E�X���W���擾����
//
//-----------------------------------------------------------------------------------------
void DX_Input::GetMouseClientPos(DirectX::XMINT2* pPos)
{
	pPos->x = m_mouseClientPos.x;
	pPos->y = m_mouseClientPos.y;
}
void DX_Input::GetMouseClientPos(DirectX::XMFLOAT2* pPos)
{
	pPos->x = CAST_F(m_mouseClientPos.x);
	pPos->y = CAST_F(m_mouseClientPos.y);
}
//-----------------------------------------------------------------------------------------
//
//  �f�o�C�X�̓��͏����X�V����
//
//-----------------------------------------------------------------------------------------
void DX_Input::Update(const HWND hWnd, const unsigned int message,const WPARAM wParam)
{

	//	�L�[�̍X�V���s��
	KeyUpdate();

	//	�}�E�X�̍X�V���s��
	MouseUpdate(hWnd, message, wParam);

}

//-----------------------------------------------------------------------------------------
//
//  �L�[�̍X�V���s��
//
//-----------------------------------------------------------------------------------------
void DX_Input::KeyUpdate()
{
	char buffer[INPUT_KEY_MAX];
	
	if (SUCCEEDED(m_pDirectInputDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer)))
	{
		for (int i = 0; i < INPUT_KEY_MAX; ++i)
		{
			//	���݂̏�Ԃ��R�s�[
			m_bPrevKeys[i] = m_bKeys[i];

			//	�L�[���������Ȃ�true�A�����ĂȂ��Ȃ�false
			m_bKeys[i] = buffer[i] & 0x80;
		}

	}
	else {
		// �f�o�C�X�̃A�N�Z�X�����擾�ł���܂Ŏ擾��������B
		while (m_pDirectInputDevice->Acquire() == DIERR_INPUTLOST);
	}
}


//-----------------------------------------------------------------------------------------
//
//  �}�E�X�̍X�V���s��
//
//-----------------------------------------------------------------------------------------
void DX_Input::MouseUpdate(HWND hWnd, const unsigned int message, const WPARAM	 wParam)
{
	//	��]�����O�ɂ���
	m_mouseWheelRotateCount = 0;

	//	���݂̏�Ԃ��R�s�[
	CopyMemory(m_bPrevMouseButtons, m_bMouseButtons, sizeof(m_bMouseButtons));

	//	�}�E�X�̓��͍X�V���s��
	switch (message){

		//	���{�^�����N���b�N
	case WM_LBUTTONDOWN:
		m_bMouseButtons[MOUSE_L_BUTTON_NUM] = MOUSE_BUTTON_CLICK; 
		break;

		//	���{�^�����_�u���N���b�N
	case WM_LBUTTONDBLCLK:
		m_bMouseButtons[MOUSE_L_BUTTON_NUM] = MOUSE_BUTTON_DOUBLE_CLICK;
		break;

		//	���{�^���𗣂�
	case WM_LBUTTONUP:	
		m_bMouseButtons[MOUSE_L_BUTTON_NUM] = MOUSE_BUTTON_FALSE;
		break;

		//	�����{�^�����N���b�N
	case WM_MBUTTONDOWN:	
		m_bMouseButtons[MOUSE_M_BUTTON_NUM] = MOUSE_BUTTON_CLICK;
		break;

		//	�����{�^�����_�u���N���b�N
	case WM_MBUTTONDBLCLK:	
		m_bMouseButtons[MOUSE_M_BUTTON_NUM] = MOUSE_BUTTON_DOUBLE_CLICK;
		break;

		//	�����{�^���𗣂�
	case WM_MBUTTONUP:
		m_bMouseButtons[MOUSE_M_BUTTON_NUM] = MOUSE_BUTTON_FALSE;
		break;

		//	�E�{�^�����N���b�N
	case WM_RBUTTONDOWN:	
		m_bMouseButtons[MOUSE_R_BUTTON_NUM] = MOUSE_BUTTON_CLICK;
		break;

		//	�E�{�^�����_�u���N���b�N
	case WM_RBUTTONDBLCLK:	
		m_bMouseButtons[MOUSE_R_BUTTON_NUM] = MOUSE_BUTTON_DOUBLE_CLICK;
		break;

		//	�E�{�^���𗣂�
	case WM_RBUTTONUP:
		m_bMouseButtons[MOUSE_R_BUTTON_NUM] = MOUSE_BUTTON_FALSE;
		break;

		//	�}�E�X�z�C�[����������
	case WM_MOUSEWHEEL:

		break;
	}

	//�J�[�\���ʒu���擾����
	tagPOINT point;
	GetCursorPos(&point);
	m_mousePos.x = (int32_t)point.x;
	m_mousePos.y = (int32_t)point.y;

	// �N���C�A���g�̈�̍��W�ɕϊ�����
	ScreenToClient(hWnd,  &point);
	m_mouseClientPos.x = (int32_t)point.x;
	m_mouseClientPos.y = (int32_t)point.y;
}