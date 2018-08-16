#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ���������
//
//-----------------------------------------------------------------------------------------
DX_VertexShader::DX_VertexShader() :
	m_pVertexShader(nullptr)
{
}



//-----------------------------------------------------------------------------------------
//
//  ���̂�����Ή��
//
//-----------------------------------------------------------------------------------------
DX_VertexShader::~DX_VertexShader()
{
	SAFE_RELEASE(m_pVertexShader);
}

//-----------------------------------------------------------------------------------------
//
//  �V�F�[�_�[���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_VertexShader::CreateShader(ID3D11Device* pDevice, const char* pFilepath)
{
	try{
		//	�V�F�[�_�[�t�@�C�����R���p�C������
		CompileFromFile(pFilepath, VS_ENTRY_POINT, VS_VERSION);

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
void DX_VertexShader::Begin(ID3D11DeviceContext* pDeviceContext, const unsigned int classInstanceCount)
{
	pDeviceContext->VSSetShader(m_pVertexShader, &m_pClassInstance, classInstanceCount);
}

//-----------------------------------------------------------------------------------------
//
//  �V�F�[�_�[�̗��p���I����
//
//-----------------------------------------------------------------------------------------
void DX_VertexShader::End(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->VSSetShader(nullptr, nullptr, 0);
}

//-----------------------------------------------------------------------------------------
//
//  �V�F�[�_�[�I�u�W�F�N�g���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_VertexShader::CreateShaderObject(ID3D11Device* pDevice)
{
	//	���I�V�F�[�_�[�@�����N��L���ɂ���N���X
	CreateClassLinkage(pDevice);

	//	�V�F�[�_�[�I�u�W�F�N�g���쐬
	HRESULT hr = pDevice->CreateVertexShader(m_pBytecord->GetBufferPointer(), m_pBytecord->GetBufferSize(), m_pClassLinkage, &m_pVertexShader);

	//	ShaderObject�̍쐬�Ɏ��s�����ꍇ��o�C�g�R�[�h���������
	DX_Debug::GetInstance()->ThrowIfFailed(hr, "VertexShader�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���");
}

