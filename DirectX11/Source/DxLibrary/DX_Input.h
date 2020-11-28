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


/// <summary>
/// �L�[�A�}�E�X�̓��͂��Ǘ�����
/// </summary>
class DX_Input : public DX_Singleton<DX_Input>
{
public:
	/// <summary>
	/// �f�o�C�X���̏��������s��
	/// </summary>
	~DX_Input();

	/// <summary>
	/// �����o�ϐ�������������
	/// </summary>
	/// <param name="hWnd">�E�B���h�E�n���h��</param>
	/// <param name="hInstance">�C���X�^���g�n���h��</param>
	/// <returns>����</returns>
	bool Initialize(HWND hWnd, HINSTANCE hInstance);

	/// <summary>
	/// �L�[�ւ̃A�N�V�������̔������擾����
	/// </summary>
	/// <param name="key">�擾�����������̃L�[</param>
	/// <returns>�A�N�V���������������ǂ���</returns>
	 bool IsKey(DX_INPUT_KEY key);

	 /// <summary>
	 /// �L�[�������Ă���Ԃ̔������擾����
	 /// </summary>
	 /// <param name="key">�擾�����������̃L�[</param>
	 bool IsKeyDown(DX_INPUT_KEY key);
	
	 /// <summary>
	 /// �L�[�𗣂����u�Ԃ̔������擾����
	 /// </summary>
	 /// <param name="key">�擾�����������̃L�[</param>
	 bool IsKeyRelease(DX_INPUT_KEY key);

	 /// <summary>
	 /// �L�[���������u�Ԃ̔������擾����
	 /// </summary>
	 /// <param name="key">�擾�����������̃L�[</param>
	 bool IsKeyHit(DX_INPUT_KEY key);

	 /// <summary>
	 /// �}�E�X�{�^���ւ̃A�N�V�������̔������擾����
	 /// </summary>
	 /// <param name="kind">�}�E�X�̃{�^���̎��</param>
	 bool IsMouseButton(DX_MOUSE_BUTTON_KIND kind);

	 /// <summary>
	 /// �}�E�X�{�^���������Ă���Ԃ̔������擾����
	 /// </summary>
	 /// <param name="kind">�}�E�X�̃{�^���̎��</param>
	 bool IsMouseButtonDown(DX_MOUSE_BUTTON_KIND kind);

	 /// <summary>
	 /// �}�E�X�{�^���𗣂������̔������擾����
	 /// </summary>
	 /// <param name="kind">�}�E�X�̃{�^���̎��</param>
	 bool IsMouseButtonRelease(DX_MOUSE_BUTTON_KIND kind);

	 /// <summary>
	 /// �}�E�X�{�^���̃N���b�N�̔������擾����
	 /// </summary>
	 /// <param name="kind">�}�E�X�̃{�^���̎��</param>
	 bool IsMouseButtonClick(DX_MOUSE_BUTTON_KIND kind);

	 /// <summary>
	 /// �}�E�X�{�^���̃_�u���N���b�N�̔������擾����
	 /// </summary>
	 /// <param name="kind">�}�E�X�̃{�^���̎��</param>
	 bool IsMouseButtonDoubleClick(DX_MOUSE_BUTTON_KIND kind);

	/// <summary>
	/// �}�E�X�z�C�[����O(��)�ɓ���������]�����擾����
	/// </summary>
	/// <returns>��]��</returns>
	int GetMouseWheelUp();

	/// <summary>
	/// �}�E�X�z�C�[�������(��O)�ɓ���������]�����擾����
	/// </summary>
	/// <returns>��]��</returns>
	int GetMouseWheelDown();

	/// <summary>
	/// �}�E�X���W���擾����
	/// </summary>
	/// <returns>���W</returns>
	DirectX::XMFLOAT2 GetMousePos();

	/// <summary>
	/// �}�E�X���W���擾����
	/// </summary>
	/// <param name="outPos">�}�E�X���W���i�[����</param>
	void GetMousePos(DirectX::XMINT2* outPos);

	/// <summary>
	/// �}�E�X���W���擾����
	/// </summary>
	/// <param name="outPos">�}�E�X���W���i�[����</param>
	void GetMousePos(DirectX::XMFLOAT2* outPos);

	/// <summary>
	/// �N���C�A���g�̈�̃}�E�X���W���擾����
	/// </summary>
	/// <param name="outPos">�}�E�X���W���i�[����</param>
	void GetMouseClientPos(DirectX::XMINT2* outPos);

	/// <summary>
	/// �N���C�A���g�̈�̃}�E�X���W���擾����
	/// </summary>
	/// <param name="outPos">�}�E�X���W���i�[����</param>
	void GetMouseClientPos(DirectX::XMFLOAT2* outPos);

	/// <summary>
	/// �f�o�C�X�̓��͏����X�V����
	/// </summary>
	/// <param name="hWnd">�E�B���h�E�n���h��</param>
	/// <param name="message">MSG.message(windows�̃��b�Z�[�W)</param>
	/// <param name="wParam">MSG.wParam</param>
	void Update(const HWND hWnd, const unsigned int message, const WPARAM	wParam);

private:
	friend class DX_Singleton<DX_Input>;
	Microsoft::WRL::ComPtr<IDirectInput8> m_directInput;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> m_directInputDevice;

	static const int INPUT_KEY_MAX		= 256;
	static const int MOUSE_BUTTON_MAX	= 3;


	//	�{�^���̐������L�[�����������ǂ���
	bool m_pressedKeys[INPUT_KEY_MAX];

	//	�ߋ��̃L�[�t���O
	bool m_pressedPrevKeys[INPUT_KEY_MAX];

	//	�}�E�X�̐������L�[���������ғ�����
	int m_mouseButtons[MOUSE_BUTTON_MAX];

	//	�ߋ��̃}�E�X�t���O
	int m_prevMouseButtons[MOUSE_BUTTON_MAX];

	//	�}�E�X�z�C�[���̉�]��
	int m_mouseWheelRotateCount;

	// �}�E�X�̍��W
	 DirectX::XMINT2 m_mousePos;
	 DirectX::XMINT2 m_mouseClientPos;


	 /// <summary>
	 /// �����o�ϐ�������������
	 /// </summary>
	 DX_Input();


	 /// <summary>
	 /// �L�[�A�}�E�X�̓��͂̍X�V����
	 /// </summary>
	 void KeyUpdate();

	/// <summary>
	/// �}�E�X�̍X�V���s��
	/// </summary>
	/// <param name="hWnd">�E�B���h�E�n���h��</param>
	/// <param name="message">MSG.message(windows�̃��b�Z�[�W)</param>
	/// <param name="wParam">MSG.wParam</param>
	void MouseUpdate(const HWND hWnd, const unsigned int message, const WPARAM wParam);
};
#endif // !__DX_INPUT_H_
