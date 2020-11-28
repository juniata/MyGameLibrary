#ifndef __DX_GRAPHICS_H_
#define __DX_GRAPHICS_H_

/// <summary>
/// �`��̊J�n�I���y�сA���j�^�[�̏ڍׂ��Ǘ����Ă��܂�
/// </summary>
class DX_Graphics : public DX_Singleton<DX_Graphics>
{
public:
	/// <summary>
	/// </summary>
	/// ���j�^�[�Ɋւ�������擾���s���B
	/// <returns>����</returns>
	bool Initialize();

	/// <summary>
	/// �`��J�n
	/// </summary>
	void BeginRender();

	/// <summary>
	/// �`��I��
	/// </summary>
	void EndRender();

	/// <summary>
	/// ���t���b�V���V�[�g�̕�����擾
	/// </summary>
	/// <returns>���t���b�V���V�[�g�̕���</returns>
	unsigned int GetRefreshRateN();

	/// <summary>
	/// ���t���b�V���V�[�g�̕��q���擾
	/// </summary>
	/// <returns>���t���b�V���V�[�g�̕��q</returns>
	unsigned int GetRefreshRateD();

	/// <summary>
	/// �r�f�I�J�[�h�̃������ʂ̎擾
	/// </summary>
	/// <returns>�r�f�I�J�[�h�̃�������</returns>
	unsigned int GetVieoCardMemory();

	/// <summary>
	///�@�r�f�I�J�[�h�̖��O���擾
	/// </summary>
	/// <returns>�r�f�I�J�[�h�̖��O</returns>
	const char* GetVideoCardDescription();

	/// <summary>
	/// �X�L�������C���I�[�_�[���擾
	/// </summary>
	/// <returns>�X�L�������C���I�[�_�[</returns>
	DXGI_MODE_SCANLINE_ORDER GetScanLineOrder();

	/// <summary>
	/// �X�P�[�����O���擾
	/// </summary>
	/// <returns>�X�P�[�����O</returns>
	DXGI_MODE_SCALING GetScaling();

	/// <summary>
	/// �t�H�[�}�b�g���擾
	/// </summary>
	/// <returns>�t�H�[�}�b�g</returns>
	DXGI_FORMAT	GetFortmat();

private:
	friend class DX_Singleton<DX_Graphics>;
	//	��ʂ��J����Ă��鎞�����`����s�����߂̃t���O
	bool m_standByMode;

	//	���t���b�V���V�[�g�̕���ƕ��q
	unsigned int m_refreshRateN;	//	���q nolecule
	unsigned int m_refreshRateD;	//	���� denominator

	//	�r�f�I�J�[�h�̃�������			
	unsigned int m_videoCardMemory;

	//	�r�f�I�J�[�h�̖��O
	char m_videoCardDescription[128];

	//	�X�L�������C��
	DXGI_MODE_SCANLINE_ORDER m_scanlineOrder;

	//	�X�P�[�����O
	DXGI_MODE_SCALING		m_scaling;

	//	�t�H�[�}�b�g
	DXGI_FORMAT				m_format;

	/// <summary>
	/// �����o�ϐ��̏��������s��
	/// </summary>
		DX_Graphics();
};
#endif // !__DX_GRAPHICS_H_
