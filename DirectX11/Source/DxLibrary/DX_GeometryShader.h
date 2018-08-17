#ifndef __DX_GEOMETRY_SHADER_H_
#define __DX_GEOMETRY_SHADER_H_

class DX_GeometryShader : public DX_Shader
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�ϐ���������
	//
	//------------------------------------------------------------------------------
	DX_GeometryShader();

	//------------------------------------------------------------------------------
	//
	//  @brief		���̂�����Ή��
	//
	//------------------------------------------------------------------------------
	~DX_GeometryShader();

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
		const unsigned int classInstanceCount = 0
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


	//------------------------------------------------------------------------------
	//
	//  @brief		�W�I���g���[�V�F�[�_�[����o�͂����f�[�^���擾
	//	@param[in]	decreation				�W�I���g���Ɠ����Z�}���e�B�N�X�̂��̂Ƀf�[�^���o��
	//	@param[in]	decreationElementCount	decreation�̗v�f��
	//	@param[in]	pBufferStrides			�f�[�^�𕪊����������
	//	@param[in]	stridesElementCount		pBufferStrides�̗v�f��
	//
	//------------------------------------------------------------------------------
	void CreateGeometryShaderWithStreamOutput(
		D3D11_SO_DECLARATION_ENTRY	decreation[],	
		const UINT					decreationElementCount,
		unsigned int*				pBufferStrides,
		const UINT					stridesElementCount
		);
private:
	ID3D11GeometryShader*	m_pGeometryShader;
	ID3D11GeometryShader*	m_pOutputGeometryShader;

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[�I�u�W�F�N�g���쐬����
	//
	//------------------------------------------------------------------------------
	void CreateShaderObject();
};
#endif // !__DX_GEOMETRY_SHADER_H_
