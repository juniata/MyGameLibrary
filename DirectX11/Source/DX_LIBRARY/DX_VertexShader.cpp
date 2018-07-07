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
void DX_VertexShader::CreateShader(const char* pFilepath)
{
	try{
		//	�V�F�[�_�[�t�@�C�����R���p�C������
		CompileFromFile(pFilepath, VS_ENTRY_POINT, VS_VERSION);

		//	�V�F�[�_�[�I�u�W�F�N�g���쐬
		CreateShaderObject();
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
void DX_VertexShader::CreateShaderObject()
{
	//	���I�V�F�[�_�[�@�����N��L���ɂ���N���X
	CreateClassLinkage();

	//	�V�F�[�_�[�I�u�W�F�N�g���쐬
	HRESULT l_hr = DX_System::GetInstance()->GetDevice()->CreateVertexShader(
		m_pBytecord->GetBufferPointer(),
		m_pBytecord->GetBufferSize(),
		m_pClassLinkage,
		&m_pVertexShader
		);

	//	ShaderObject�̍쐬�Ɏ��s�����ꍇ��o�C�g�R�[�h���������
	if (!DX_Debug::GetInstance()->IsHresultCheck(l_hr)){
		throw "VertexShader�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���";
	}
	
}

