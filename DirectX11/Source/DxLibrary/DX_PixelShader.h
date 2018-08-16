#ifndef __DX_PIXEL_SHADER_H_
#define __DX_PIXEL_SHADER_H_

class DX_PixelShader : public DX_Shader
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�ϐ���������
	//
	//------------------------------------------------------------------------------
	DX_PixelShader();

	//------------------------------------------------------------------------------
	//
	//  @brief		���̂�����Ή��
	//
	//------------------------------------------------------------------------------
	~DX_PixelShader();

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[���쐬����
	//	@param[in]	pDevice		DirectX11�̃f�o�C�X
	//	@param[in]	pFilepath	�V�F�[�_�[�t�@�C���̃p�X
	//
	//------------------------------------------------------------------------------
	virtual void CreateShader(ID3D11Device* pDevice, const char* pFilepath);

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[�̎g�p���J�n
	//	@param[in]	classInstanceCount	�N���X�C���X�^���X�̐�
	//	@param[in]	pDeviceContext		DX_System::GetDeviceContext()
	//
	//------------------------------------------------------------------------------
	void Begin(ID3D11DeviceContext*	pDeviceContext, const unsigned int classInstanceCount = 0);

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[�̗��p���I����
	//	@param[in]	pDeviceContext		DX_System::GetDeviceContext()
	//
	//------------------------------------------------------------------------------
	void End(ID3D11DeviceContext*	pDeviceContext);

private:
	ID3D11PixelShader*	m_pPixelShader;

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[�I�u�W�F�N�g���쐬����
	//	@param[in]	pDevice	DirectX11�̃f�o�C�X
	//
	//------------------------------------------------------------------------------
	void CreateShaderObject(ID3D11Device* pDevice);
};
#endif // !__DX_PIXEL_SHADER_H_
