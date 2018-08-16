#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ���������
//
//-----------------------------------------------------------------------------------------
DX_PixelShader::DX_PixelShader() :
	m_pPixelShader(nullptr)
{
}



//-----------------------------------------------------------------------------------------
//
//  ���̂�����Ή��
//
//-----------------------------------------------------------------------------------------
DX_PixelShader::~DX_PixelShader()
{
	SAFE_RELEASE(m_pPixelShader);
}



//-----------------------------------------------------------------------------------------
//
//  �V�F�[�_�[���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_PixelShader::CreateShader(ID3D11Device* pDevice, const char* pFilepath)
{
	try{

		//	�V�F�[�_�[�t�@�C�����R���p�C������
		CompileFromFile(pFilepath, PS_ENTRY_POINT, PS_VERSION);

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
void DX_PixelShader::Begin(ID3D11DeviceContext* pDeviceContext, const unsigned int classInstanceCount)
{
	pDeviceContext->PSSetShader(m_pPixelShader, &m_pClassInstance, classInstanceCount);
}

//-----------------------------------------------------------------------------------------
//
//  �V�F�[�_�[�̗��p���I����
//
//-----------------------------------------------------------------------------------------
void DX_PixelShader::End(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->PSSetShader(nullptr, nullptr, 0);
}

//-----------------------------------------------------------------------------------------
//
//  �V�F�[�_�[�I�u�W�F�N�g���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_PixelShader::CreateShaderObject(ID3D11Device* pDevice)
{
	//	���I�V�F�[�_�[�@�����N��L���ɂ���N���X
	CreateClassLinkage(pDevice);

	//	�V�F�[�_�[�I�u�W�F�N�g���쐬
	HRESULT hr = pDevice->CreatePixelShader(m_pBytecord->GetBufferPointer(), m_pBytecord->GetBufferSize(), m_pClassLinkage, &m_pPixelShader);

	//	ShaderObject�̍쐬�Ɏ��s�����ꍇ��o�C�g�R�[�h���������
	DX_Debug::GetInstance()->ThrowIfFailed(hr, "PixelShader�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���");
}

