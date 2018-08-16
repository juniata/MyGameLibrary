#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ���������
//
//-----------------------------------------------------------------------------------------
DX_GeometryShader::DX_GeometryShader() :
	m_pOutputGeometryShader(nullptr),
	m_pGeometryShader(nullptr)
{
}

//-----------------------------------------------------------------------------------------
//
//  ���̂�����Ή��
//
//-----------------------------------------------------------------------------------------
DX_GeometryShader::~DX_GeometryShader()
{
	SAFE_RELEASE(m_pGeometryShader);
	SAFE_RELEASE(m_pOutputGeometryShader);
}

//-----------------------------------------------------------------------------------------
//
//  �V�F�[�_�[���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_GeometryShader::CreateShader(ID3D11Device* pDevice, const char* pFilepath)
{
	try{

		//	�V�F�[�_�[�t�@�C�����R���p�C������
		CompileFromFile(pFilepath, GS_ENTRY_POINT, GS_VERSION);

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
void DX_GeometryShader::Begin(ID3D11DeviceContext* pDeviceContext, const unsigned int classInstanceCount)
{
	if (m_pOutputGeometryShader){
		pDeviceContext->GSSetShader(m_pOutputGeometryShader, &m_pClassInstance, classInstanceCount);
	}
	else{
		pDeviceContext->GSSetShader(m_pGeometryShader, &m_pClassInstance, classInstanceCount);
	}
}

//-----------------------------------------------------------------------------------------
//
//  �V�F�[�_�[�̗��p���I����
//
//-----------------------------------------------------------------------------------------
void DX_GeometryShader::End(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->GSSetShader(nullptr, nullptr, 0);
}

//-----------------------------------------------------------------------------------------
//
//  �V�F�[�_�[�I�u�W�F�N�g���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_GeometryShader::CreateShaderObject(ID3D11Device* pDevice)
{
	//	���I�V�F�[�_�[�@�����N��L���ɂ���N���X
	CreateClassLinkage(pDevice);

	//	�V�F�[�_�[�I�u�W�F�N�g���쐬
	HRESULT hr = pDevice->CreateGeometryShader(m_pBytecord->GetBufferPointer(), m_pBytecord->GetBufferSize(), m_pClassLinkage, &m_pGeometryShader);

	//	ShaderObject�̍쐬�Ɏ��s�����ꍇ��o�C�g�R�[�h���������
	DX_Debug::GetInstance()->ThrowIfFailed(hr, "GeometryShader�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���");
}


//-----------------------------------------------------------------------------------------
//
//  �W�I���g���[�V�F�[�_�[����o�͂����f�[�^���擾
//
//-----------------------------------------------------------------------------------------
void DX_GeometryShader::CreateGeometryShaderWithStreamOutput(D3D11_SO_DECLARATION_ENTRY	decreation[], const UINT decreationElementCount, unsigned int* pBufferStrides, const UINT stridesElementCount)
{
	HRESULT hr = DX_System::GetInstance()->GetDevice()->CreateGeometryShaderWithStreamOutput(
		m_pBytecord->GetBufferPointer(),
		m_pBytecord->GetBufferSize(),
		decreation,
		decreationElementCount,
		pBufferStrides,
		stridesElementCount,
		0,
		nullptr,
		&m_pOutputGeometryShader
		);

	//	ShaderObject�̍쐬�Ɏ��s�����ꍇ��o�C�g�R�[�h���������
	DX_Debug::GetInstance()->ThrowIfFailed(hr, "GeometryShaderWithStreamOutput�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���");
}
