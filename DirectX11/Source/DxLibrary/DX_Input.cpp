#include	"DX_Library.h"
using namespace DirectX;

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
DirectX::XMINT2	DX_Input::m_mousePos							= XMINT2(0, 0);
DirectX::XMINT2	DX_Input::m_mouseClientPos						= XMINT2(0, 0);

//-----------------------------------------------------------------------------------------
//
//  �}�N����`
//
//-----------------------------------------------------------------------------------------
#define GET_KEY_STATE_PRESS0 -127
#define GET_KEY_STATE_PRESS1 -128

#define GET_KEY_STATE_NOT_PRESS0 0
#define GET_KEY_STATE_NOT_PRESS1 1

#define MOUSE_L_BUTTON_NUM 0 
#define MOUSE_M_BUTTON_NUM 1
#define MOUSE_R_BUTTON_NUM 2

#define MOUSE_BUTTON_FALSE 0x00
#define MOUSE_BUTTON_TRUE  0x03
#define MOUSE_BUTTON_CLICK 0x01
#define MOUSE_BUTTON_DOUBLE_CLICK 0x02
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
	POINT l_pos;

	//	�}�E�X�̃X�N���[�����W���擾
	GetCursorPos(&l_pos);

	//	�X�N���[�����W����E�B���h�̈�ւ̕ϊ�
	ClientToScreen(DX_System::GetInstance()->GetWindowHandle(), &l_pos);

	//	�E�B���h�E���W
	LONG l_width	= CAST_L(DX_System::GetWindowWidth());
	LONG l_height	= CAST_L(DX_System::GetWindowHeight());

	if (l_pos.x < 0){ l_pos.x = 0; }
	else if (l_pos.x > l_width){ l_pos.x = l_width; }

	if (l_pos.y < 0){ l_pos.y = 0; }
	else if (l_pos.y > l_height){ l_pos.y = l_height; }

	return DirectX::XMFLOAT2(CAST_F(l_pos.x), CAST_F(l_pos.y));
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
	int l_getKeyState = 0;

	//	�S�L�[�̍X�V���s��
	for (int i = 0; i < INPUT_KEY_MAX; ++i){

		//	���݂̏�Ԃ��R�s�[
		m_bPrevKeys[i] = m_bKeys[i];

		//	�L�[�̏�Ԃ��擾
		l_getKeyState = GetKeyState(i);

		//	�L�[���������Ȃ�true�A�����ĂȂ��Ȃ�false
		m_bKeys[i] =
			(	(l_getKeyState == GET_KEY_STATE_PRESS0) ||
				(l_getKeyState == GET_KEY_STATE_PRESS1)) ? true : false;

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