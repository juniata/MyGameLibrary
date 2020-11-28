#include	"DX_Library.h"

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
 
/// <summary>
/// �����o�ϐ�������������
/// </summary>
DX_Input::DX_Input() : m_mouseWheelRotateCount(0), m_mousePos(DirectX::XMINT2(0, 0)), m_mouseClientPos(DirectX::XMINT2(0, 0))
{
	ZeroMemory(m_pressedKeys, sizeof(m_pressedKeys));
	ZeroMemory(m_pressedPrevKeys, sizeof(m_pressedPrevKeys));
	ZeroMemory(m_mouseButtons, sizeof(m_mouseButtons));
	ZeroMemory(m_prevMouseButtons, sizeof(m_prevMouseButtons));
}

/// <summary>
/// �f�o�C�X���̏��������s��
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
/// �����o�ϐ�������������
/// </summary>
/// <param name="hWnd">�E�B���h�E�n���h��</param>
/// <param name="hInstance">�C���X�^���g�n���h��</param>
/// <returns>����</returns>
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
/// �L�[�ւ̃A�N�V�������̔����𓾂�
/// </summary>
/// <param name="key">DX_INPUT_KEY</param>
/// <returns>�A�N�V���������������ǂ���</returns>
bool DX_Input::IsKey(DX_INPUT_KEY key)
{
	//	���݁A�܂��͉ߋ���true�Ȃ�A�ǂꂩ�̃A�N�V���������Ă���
	return (m_pressedKeys[DX::CAST::I(key)] || m_pressedPrevKeys[DX::CAST::I(key)] ) ? true : false;
}

/// <summary>
/// �L�[�������Ă���Ԃ̔������擾����
/// </summary>
/// <param name="key">DX_INPUT_KEY</param>
bool DX_Input::IsKeyDown(DX_INPUT_KEY key)
{
	//	����true : �ߋ�true �Ȃ牟���Ă����
	return (m_pressedKeys[DX::CAST::I(key)] && m_pressedPrevKeys[DX::CAST::I(key)]) ? true : false;
}

/// <summary>
/// �L�[�𗣂����u�Ԃ̔������擾����
/// </summary>
/// <param name="key">DX_INPUT_KEY</param>
bool DX_Input::IsKeyRelease(DX_INPUT_KEY key)
{
	//	����false : �ߋ�true �Ȃ痣�����u��
	return (!m_pressedKeys[DX::CAST::I(key)] && m_pressedPrevKeys[DX::CAST::I(key)]) ? true : false;
}

/// <summary>
/// �L�[���������u�Ԃ̔������擾����
/// </summary>
/// <param name="key">DX_INPUT_KEY</param>
bool DX_Input::IsKeyHit(DX_INPUT_KEY key)
{
	//	����true : �ߋ�false �Ȃ牟�����u��
	return  (m_pressedKeys[DX::CAST::I(key)] && !m_pressedPrevKeys[DX::CAST::I(key)]) ? true : false;
}

/// <summary>
/// �}�E�X�{�^���ւ̃A�N�V�������̔������擾����
/// </summary>
/// <param name="kind">�}�E�X�̃{�^���̎��</param>
bool DX_Input::IsMouseButton(DX_MOUSE_BUTTON_KIND kind)
{
	//	����false : �ߋ�false �ȊO�Ȃ�S��Ԏ擾
	return ((m_mouseButtons[DX::CAST::I(kind)]) || (m_prevMouseButtons[DX::CAST::I(kind)])) ? true : false;
}

/// <summary>
/// �}�E�X�{�^���������Ă���Ԃ̔������擾����
/// </summary>
/// <param name="kind">�}�E�X�̃{�^���̎��</param>
bool DX_Input::IsMouseButtonDown(DX_MOUSE_BUTTON_KIND kind)
{
	//	����true : �ߋ�true �Ȃ牟���Ă����
	return ((m_mouseButtons[DX::CAST::I(kind)]) && (m_prevMouseButtons[DX::CAST::I(kind)])) ? true : false;
}

/// <summary>
/// �}�E�X�{�^���𗣂������̔������擾����
/// </summary>
/// <param name="kind">�}�E�X�̃{�^���̎��</param>
bool DX_Input::IsMouseButtonRelease(DX_MOUSE_BUTTON_KIND kind)
{
	//	����false : �ߋ�true �Ȃ痣�����u��
	return ((!m_mouseButtons[DX::CAST::I(kind)]) && (m_prevMouseButtons[DX::CAST::I(kind)])) ? true : false;
}


/// <summary>
/// �}�E�X�{�^���̃N���b�N�̔������擾����
/// </summary>
/// <param name="kind">�}�E�X�̃{�^���̎��</param>
bool DX_Input::IsMouseButtonClick(DX_MOUSE_BUTTON_KIND kind)
{
	//	����true : �ߋ�false �Ȃ牟�����u�ԏu��
	return ((m_mouseButtons[DX::CAST::I(kind)]) && (!m_prevMouseButtons[DX::CAST::I(kind)])) ? true : false;
		 
}

/// <summary>
/// �}�E�X�{�^���̃_�u���N���b�N�̔������擾����
/// </summary>
/// <param name="kind">�}�E�X�̃{�^���̎��</param>
bool DX_Input::IsMouseButtonDoubleClick(DX_MOUSE_BUTTON_KIND kind)
{
	return  (m_mouseButtons[DX::CAST::I(kind)] == MOUSE_BUTTON_DOUBLE_CLICK) ? true : false;
}

/// <summary>
/// �}�E�X���W���擾����
/// </summary>
/// <returns>���W</returns>
DirectX::XMFLOAT2 DX_Input::GetMousePos()
{
	DX_System* system = DX_System::GetInstance();

	POINT pos;

	//	�}�E�X�̃X�N���[�����W���擾
	GetCursorPos(&pos);

	//	�X�N���[�����W����E�B���h�̈�ւ̕ϊ�
	ClientToScreen(system->GetWindowHandle(), &pos);

	//	�E�B���h�E���W
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
/// �}�E�X�z�C�[����O(��)�ɓ���������]�����擾����
/// </summary>
/// <returns>��]��</returns>
int DX_Input::GetMouseWheelUp()
{
	return m_mouseWheelRotateCount;
}

/// <summary>
/// �}�E�X�z�C�[�������(��O)�ɓ���������]�����擾����
/// </summary>
/// <returns>��]��</returns>
int DX_Input::GetMouseWheelDown()
{
	return m_mouseWheelRotateCount;
}

/// <summary>
/// �}�E�X���W���擾����
/// </summary>
/// <param name="outPos">�}�E�X���W���i�[����</param>
void DX_Input::GetMousePos(DirectX::XMINT2* outPos)
{
	outPos->x = m_mousePos.x;
	outPos->y = m_mousePos.y;
}

/// <summary>
/// �}�E�X���W���擾����
/// </summary>
/// <param name="outPos">�}�E�X���W���i�[����</param>
void DX_Input::GetMousePos(DirectX::XMFLOAT2* outPos)
{
	outPos->x = DX::CAST::F(m_mousePos.x);
	outPos->y = DX::CAST::F(m_mousePos.y);
}

/// <summary>
/// �N���C�A���g�̈�̃}�E�X���W���擾����
/// </summary>
/// <param name="outPos">�}�E�X���W���i�[����</param>
void DX_Input::GetMouseClientPos(DirectX::XMINT2* outPos)
{
	outPos->x = m_mouseClientPos.x;
	outPos->y = m_mouseClientPos.y;
}

/// <summary>
/// �N���C�A���g�̈�̃}�E�X���W���擾����
/// </summary>
/// <param name="outPos">�}�E�X���W���i�[����</param>
void DX_Input::GetMouseClientPos(DirectX::XMFLOAT2* outPos)
{
	outPos->x = DX::CAST::F(m_mouseClientPos.x);
	outPos->y = DX::CAST::F(m_mouseClientPos.y);
}

/// <summary>
/// �f�o�C�X�̓��͏����X�V����
/// </summary>
/// <param name="hWnd">�E�B���h�E�n���h��</param>
/// <param name="message">MSG.message(windows�̃��b�Z�[�W)</param>
/// <param name="wParam">MSG.wParam</param>
void DX_Input::Update(const HWND hWnd, const unsigned int message,const WPARAM wParam)
{
	KeyUpdate();
	MouseUpdate(hWnd, message, wParam);
}

/// <summary>
/// �L�[�A�}�E�X�̓��͂̍X�V����
/// </summary>
void DX_Input::KeyUpdate()
{
	char buffer[INPUT_KEY_MAX];
	
	if (SUCCEEDED(m_directInputDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer)))
	{
		for (int i = 0; i < INPUT_KEY_MAX; ++i)
		{
			//	���݂̏�Ԃ��R�s�[
			m_pressedPrevKeys[i] = m_pressedKeys[i];

			//	�L�[���������Ȃ�true�A�����ĂȂ��Ȃ�false
			m_pressedKeys[i] = buffer[i] & 0x80;
		}

	}
	else
	{
		// �f�o�C�X�̃A�N�Z�X�����擾�ł���܂Ŏ擾��������B
		while (m_directInputDevice->Acquire() == DIERR_INPUTLOST);
	}
}


/// <summary>
/// �}�E�X�̍X�V���s��
/// </summary>
/// <param name="hWnd">�E�B���h�E�n���h��</param>
/// <param name="message">MSG.message(windows�̃��b�Z�[�W)</param>
/// <param name="wParam">MSG.wParam</param>
void DX_Input::MouseUpdate(HWND hWnd, const unsigned int message, const WPARAM	 wParam)
{
	//	��]�����O�ɂ���
	m_mouseWheelRotateCount = 0;

	//	���݂̏�Ԃ��R�s�[
	CopyMemory(m_prevMouseButtons, m_mouseButtons, sizeof(m_mouseButtons));

	//	�}�E�X�̓��͍X�V���s��
	switch (message){

		//	���{�^�����N���b�N
	case WM_LBUTTONDOWN:
		m_mouseButtons[MOUSE_L_BUTTON_NUM] = MOUSE_BUTTON_CLICK;
		break;

		//	���{�^�����_�u���N���b�N
	case WM_LBUTTONDBLCLK:
		m_mouseButtons[MOUSE_L_BUTTON_NUM] = MOUSE_BUTTON_DOUBLE_CLICK;
		break;

		//	���{�^���𗣂�
	case WM_LBUTTONUP:	
		m_mouseButtons[MOUSE_L_BUTTON_NUM] = MOUSE_BUTTON_FALSE;
		break;

		//	�����{�^�����N���b�N
	case WM_MBUTTONDOWN:	
		m_mouseButtons[MOUSE_M_BUTTON_NUM] = MOUSE_BUTTON_CLICK;
		break;

		//	�����{�^�����_�u���N���b�N
	case WM_MBUTTONDBLCLK:	
		m_mouseButtons[MOUSE_M_BUTTON_NUM] = MOUSE_BUTTON_DOUBLE_CLICK;
		break;

		//	�����{�^���𗣂�
	case WM_MBUTTONUP:
		m_mouseButtons[MOUSE_M_BUTTON_NUM] = MOUSE_BUTTON_FALSE;
		break;

		//	�E�{�^�����N���b�N
	case WM_RBUTTONDOWN:	
		m_mouseButtons[MOUSE_R_BUTTON_NUM] = MOUSE_BUTTON_CLICK;
		break;

		//	�E�{�^�����_�u���N���b�N
	case WM_RBUTTONDBLCLK:	
		m_mouseButtons[MOUSE_R_BUTTON_NUM] = MOUSE_BUTTON_DOUBLE_CLICK;
		break;

		//	�E�{�^���𗣂�
	case WM_RBUTTONUP:
		m_mouseButtons[MOUSE_R_BUTTON_NUM] = MOUSE_BUTTON_FALSE;
		break;

		//	�}�E�X�z�C�[����������
	case WM_MOUSEWHEEL:
		m_mouseWheelRotateCount = GET_WHEEL_DELTA_WPARAM(wParam);
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