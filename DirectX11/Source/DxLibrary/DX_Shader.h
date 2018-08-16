#ifndef __DX_SHADER_H_
#define __DX_SHADER_H_

//	�e�V�F�[�_�[�̃G���g���[�|�C���g
#define VS_ENTRY_POINT "VS_Main"
#define HS_ENTRY_POINT "HS_Main"
#define DS_ENTRY_POINT "DS_Main"
#define GS_ENTRY_POINT "GS_Main"
#define PS_ENTRY_POINT "PS_Main"
#define CS_ENTRY_POINT "CS_Main"

//	�e�V�F�[�_�[�̃o�[�W����
#define VS_VERSION "vs_5_0"
#define HS_VERSION "hs_5_0"
#define DS_VERSION "ds_5_0"
#define GS_VERSION "gs_5_0"
#define PS_VERSION "ps_5_0"
#define CS_VERSION "cs_5_0"

//****************************************************************************************************
//
//	Shader
//
//****************************************************************************************************
class DX_Shader
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		�����o�ϐ���������
	//
	//------------------------------------------------------------------------------
	DX_Shader();

	//------------------------------------------------------------------------------
	//
	//  @brief		���̂�����Ή��
	//
	//------------------------------------------------------------------------------
	virtual ~DX_Shader();

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[���쐬����
	//	@param[in]	pDevice		DirectX11�̃f�o�C�X
	//	@param[in]	pFilepath	�V�F�[�_�[�t�@�C���̃p�X
	//
	//------------------------------------------------------------------------------
	virtual void CreateShader(ID3D11Device* pDevice, const char* pFilepath) = 0;

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[�̎g�p���J�n
	//	@param[in]	pDeviceContext		DX_System::GetDeviceContext()
	//	@param[in]	classInstanceCount	�N���X�C���X�^���X�̐�
	//
	//------------------------------------------------------------------------------
	virtual void Begin(ID3D11DeviceContext*	pDeviceContext, const unsigned int classInstanceCount = 0) = 0;

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[�̗��p���I����
	//	@param[in]	pDeviceContext		DX_System::GetDeviceContext()
	//
	//------------------------------------------------------------------------------
	virtual void End(ID3D11DeviceContext*	pDeviceContext) = 0;

	//------------------------------------------------------------------------------
	//
	//  @brief		�R���p�C���������̃o�C�g�R�[�h���擾
	//	@return		�R���p�C���������̃o�C�g�R�[�h
	//
	//------------------------------------------------------------------------------
	ID3DBlob* GetByteCord();

protected:
	ID3D11ClassLinkage*		m_pClassLinkage;
	ID3D11ClassInstance*	m_pClassInstance;
	ID3D11InputLayout*		m_pInputLayout;
	ID3DBlob*				m_pBytecord;

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[�t�@�C�����R���p�C������
	//	@param[in]	pFilepath		�V�F�[�_�[�t�@�C���̃p�X
	//	@param[in]	pEntryPoint		�V�F�[�_�[�̃G���g���[�|�C���g
	//	@param[in]	pShaderVersion	�V�F�[�_�[�̃o�[�W����
	//
	//------------------------------------------------------------------------------
	void CompileFromFile(const char* pFilepath, const char* pEntryPoint, const char* pShaderVersion);

	//------------------------------------------------------------------------------
	//
	//  @brief		�V�F�[�_�[�I�u�W�F�N�g���쐬����
	//	@param[in]	pDevice	DirectX11�̃f�o�C�X
	//
	//------------------------------------------------------------------------------
	virtual void CreateShaderObject(ID3D11Device* pDevice) = 0;

	//------------------------------------------------------------------------------
	//
	//  @brief		���I�V�F�[�_�[��L���ɂ��郊���N���쐬
	//	@param[in]	pDevice	DirectX11�̃f�o�C�X
	//
	//------------------------------------------------------------------------------
	void CreateClassLinkage(ID3D11Device* pDevice);
};

//	�e�V�F�[�_�[�w�b�_�[���C���N���[�h
#include	"DX_VertexShader.h"
#include	"DX_GeometryShader.h"
#include	"DX_PixelShader.h"
#include	"DX_ComputeShader.h"
#endif // !__DX_SHADER_H_
