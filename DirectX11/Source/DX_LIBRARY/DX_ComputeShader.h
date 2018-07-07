#ifndef __DX_COMPUTE_SHADER_H_
#define __DX_COMPUTE_SHADER_H_


class DX_ComputeShader : public DX_Shader
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�ϐ���������
	//
	//------------------------------------------------------------------------------
	DX_ComputeShader();

	//------------------------------------------------------------------------------
	//
	//  @brief		���̂�����Ή��
	//
	//------------------------------------------------------------------------------
	~DX_ComputeShader();

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[���쐬����
	//	@param[in]	pFilepath	�V�F�[�_�[�t�@�C���̃p�X
	//
	//------------------------------------------------------------------------------
	void CreateShader(
		const char* pFilepath
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[�̎g�p���J�n
	//	@param[in]	classInstanceCount	�N���X�C���X�^���X�̐�
	//	@param[in]	pDeviceContext		DX_System::GetDeviceContext()
	//
	//------------------------------------------------------------------------------
	void Begin(
		ID3D11DeviceContext*	pDeviceContext,
		const unsigned int		classInstanceCount = 0
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[�̗��p���I����
	//	@param[in]	pDeviceContext		DX_System::GetDeviceContext()
	//
	//------------------------------------------------------------------------------
	void End(
		ID3D11DeviceContext*	pDeviceContext
		);

private:
	ID3D11ComputeShader*	m_pComputeShader;

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[�I�u�W�F�N�g���쐬����
	//
	//------------------------------------------------------------------------------
	void CreateShaderObject();
};
#endif // !__DX_COMPUTE_SHADER_H_
