#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ���������
//
//-----------------------------------------------------------------------------------------
DX_ComputeShader::DX_ComputeShader() :
	m_pComputeShader(nullptr)
{
}



//-----------------------------------------------------------------------------------------
//
//  ���̂�����Ή��
//
//-----------------------------------------------------------------------------------------
DX_ComputeShader::~DX_ComputeShader()
{
	SAFE_RELEASE(m_pComputeShader);
}



//-----------------------------------------------------------------------------------------
//
//  �V�F�[�_�[���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_ComputeShader::CreateShader(ID3D11Device* pDevice, const char* pFilepath)
{
	try{

		//	�V�F�[�_�[�t�@�C�����R���p�C������
		CompileFromFile(pFilepath, CS_ENTRY_POINT, CS_VERSION);

		//	�V�F�[�_�[�I�u�W�F�N�g���쐬
		CreateShaderObject(pDevice);
	}
	catch (char* pMessage){
		throw pMessage;
	}
}

//-----------------------------------------------------------------------------------------
//
// �V�F�[�_�[�̎g�p���J�n
//
//-----------------------------------------------------------------------------------------
void DX_ComputeShader::Begin(ID3D11DeviceContext* pDeviceContext, const unsigned int classInstanceCount)
{
	pDeviceContext->CSSetShader(m_pComputeShader, &m_pClassInstance, classInstanceCount);
}

//-----------------------------------------------------------------------------------------
//
//  �V�F�[�_�[�̗��p���I����
//
//-----------------------------------------------------------------------------------------
void DX_ComputeShader::End(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->CSSetShader(nullptr, nullptr, 0);
}

//-----------------------------------------------------------------------------------------
//
//  �V�F�[�_�[�I�u�W�F�N�g���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_ComputeShader::CreateShaderObject(ID3D11Device* pDevice)
{
	//	���I�V�F�[�_�[�@�����N��L���ɂ���N���X
	CreateClassLinkage(pDevice);

	//	�V�F�[�_�[�I�u�W�F�N�g���쐬
	HRESULT hr = pDevice->CreateComputeShader(m_pBytecord->GetBufferPointer(), m_pBytecord->GetBufferSize(), m_pClassLinkage, &m_pComputeShader);

	//	ShaderObject�̍쐬�Ɏ��s�����ꍇ��o�C�g�R�[�h���������
	DX_Debug::GetInstance()->ThrowIfFailed(hr, "ComputeShader�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���");
}

