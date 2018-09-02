#ifndef __DX_INPUT_H_
#define __DX_INPUT_H_

//	�L�[�{�[�h�̃{�^���̎��
#include	"DX_InputDefines.h"

//	�}�E�X�̃{�^���̎��
enum class DX_MOUSE_BUTTON_KIND : int{
	BUTTON_L,	//	���{�^��
	BUTTON_M,	//	�����{�^��
	BUTTON_R,	//	�E�{�^��
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
	//  @brief		�L�[�ւ̃A�N�V�������̔����𓾂�
	//	@param[in]	key		DX_INPUT_KEY
	//	@return		true:������	false:�����ĂȂ�
	//
	//------------------------------------------------------------------------------
	static bool IsKey(DX_INPUT_KEY key);

	//------------------------------------------------------------------------------
	//
	//  @brief		�����Ă���Ԃ̔����𓾂�
	//	@param[in]	key		DX_INPUT_KEY
	//	@return		true:�����Ă���		false:�����Ă��Ȃ�
	//
	//------------------------------------------------------------------------------
	static bool IsKeyDown(DX_INPUT_KEY key);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		�������u�Ԃ̔����𓾂�
	//	@param[in]	key		DX_INPUT_KEY
	//	@return		true:������		false:�����Ă��Ȃ�
	//
	//------------------------------------------------------------------------------
	static bool IsKeyRelease(DX_INPUT_KEY key);

	//------------------------------------------------------------------------------
	//
	//  @brief		�������u�Ԃ̔����𓾂�
	//	@param[in]	key		DX_INPUT_KEY
	//	@return		true:������		false:�����Ă��Ȃ�
	//
	//------------------------------------------------------------------------------
	static bool IsKeyHit(DX_INPUT_KEY key);

	//------------------------------------------------------------------------------
	//
	//  @brief		�}�E�X�{�^���ւ̃A�N�V�������̔����𓾂�
	//	@param[in]	kind		�}�E�X�̃{�^���̎��
	//	@return		true:������		false:�����Ă��Ȃ�
	//
	//------------------------------------------------------------------------------
	static bool IsMouseButton(DX_MOUSE_BUTTON_KIND kind);

	//------------------------------------------------------------------------------
	//
	//  @brief		�}�E�X�{�^���������Ă���Ԃ̔����𓾂�
	//	@param[in]	kind		�}�E�X�̃{�^���̎��
	//	@return		true:�����Ă���		false:�����Ă��Ȃ�
	//
	//------------------------------------------------------------------------------
	static bool IsMouseButtonDown(DX_MOUSE_BUTTON_KIND kind);

	//------------------------------------------------------------------------------
	//
	//  @brief		�}�E�X�{�^���𗣂������̔����𓾂�
	//	@param[in]	kind		�}�E�X�̃{�^���̎��
	//	@return		true:������		false:�����Ă��Ȃ�
	//
	//------------------------------------------------------------------------------
	static bool IsMouseButtonRelease(DX_MOUSE_BUTTON_KIND kind);

	//------------------------------------------------------------------------------
	//
	//  @brief		�}�E�X�{�^���̃N���b�N�̔����𓾂�
	//	@param[in]	kind		�}�E�X�̃{�^���̎��
	//	@return		true:������		false:�����Ă��Ȃ�
	//	@note		�L�[��hit�֐��Ɠ�������
	//
	//------------------------------------------------------------------------------
	static bool IsMouseButtonClick(DX_MOUSE_BUTTON_KIND kind);

	//------------------------------------------------------------------------------
	//
	//  @brief		�}�E�X�{�^���̃_�u���N���b�N�̔����𓾂�
	//	@param[in]	kind		�}�E�X�̃{�^���̎��
	//	@return		true:������		false:�����Ă��Ȃ�
	//
	//------------------------------------------------------------------------------
	static bool IsMouseButtonDoubleClick(DX_MOUSE_BUTTON_KIND kind);

	//------------------------------------------------------------------------------
	//
	//  @brief	�}�E�X���W���擾
	//	@return �E�B���h�E���̃}�E�X���W
	//
	//------------------------------------------------------------------------------
	static DirectX::XMFLOAT2 GetMousePos();

	//------------------------------------------------------------------------------
	//
	//  @brief	�}�E�X�z�C�[����O(��)�ɓ���������]�����擾
	//	@return	��]��
	//
	//------------------------------------------------------------------------------
	static int GetMouseWheelUp();

	//------------------------------------------------------------------------------
	//
	//  @brief	�}�E�X�z�C�[�������(��O)�ɓ���������]�����擾
	//	@return	��]��
	//
	//------------------------------------------------------------------------------
	static int GetMouseWheelDown();

	//------------------------------------------------------------------------------
	//
	//  @brief	�}�E�X���W���擾����
	//	@param[out]	pPos	�}�E�X���W
	//
	//------------------------------------------------------------------------------
	static void GetMousePos(DirectX::XMINT2* pPos);
	static void GetMousePos(DirectX::XMFLOAT2* pPos);


	//------------------------------------------------------------------------------
	//
	//  @brief	�N���C�A���g�̈�̃}�E�X���W���擾����
	//	@param[out]	pPos	�}�E�X���W
	//
	//------------------------------------------------------------------------------
	static void GetMouseClientPos(DirectX::XMINT2* pPos);
	static void GetMouseClientPos(DirectX::XMFLOAT2* pPos);

	//------------------------------------------------------------------------------
	//
	//  @brief		�f�o�C�X�̓��͏����X�V����
	//	@param[in]	hWnd	�E�B���h�E�n���h��
	//	@param[in]	message	MSG.message(windows�̃��b�Z�[�W)
	//	@param[in]	wParam	MSG.wParam
	//
	//------------------------------------------------------------------------------
	static void Update(const HWND hWnd, const unsigned int message, const WPARAM	wParam);

private:
	//	�L�[�{�[�h�̎󂯕t����
	static const int INPUT_KEY_MAX = 254;

	//	�{�^���̐������L�[�����������ǂ���
	static bool m_bKeys[INPUT_KEY_MAX];

	//	�ߋ��̃L�[�t���O
	static bool m_bPrevKeys[INPUT_KEY_MAX];

	//	�}�E�X�̃{�^����
	static const int MOUSE_BUTTON_MAX = 3;

	//	�}�E�X�̐������L�[���������ғ�����
	static int m_bMouseButtons[MOUSE_BUTTON_MAX];

	//	�ߋ��̃}�E�X�t���O
	static int m_bPrevMouseButtons[MOUSE_BUTTON_MAX];

	//	�}�E�X�z�C�[���̉�]��
	static int m_mouseWheelRotateCount;

	// �}�E�X�̍��W
	static DirectX::XMINT2 m_mousePos;
	static DirectX::XMINT2 m_mouseClientPos;

	//------------------------------------------------------------------------------
	//
	//  @brief		�L�[�̍X�V���s��
	//
	//------------------------------------------------------------------------------
	static void KeyUpdate();

	//------------------------------------------------------------------------------
	//
	//  @brief		�}�E�X�̍X�V���s��
	//	@param[in]	hWnd	�E�B���h�E�n���h��
	//	@param[in]	message	DX_Input::Update�̈��������̂܂ܓ����
	//	@param[in]	wParam	DX_Input::Uddate�̈��������̂܂ܓ����
	//
	//------------------------------------------------------------------------------
	static void MouseUpdate(const HWND hWnd, const unsigned int message, const WPARAM wParam);
};
#endif // !__DX_INPUT_H_
