#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ���������
//
//-----------------------------------------------------------------------------------------
DX_ComputeShader::DX_ComputeShader()
{
}



//-----------------------------------------------------------------------------------------
//
//  ���̂�����Ή��
//
//-----------------------------------------------------------------------------------------
DX_ComputeShader::~DX_ComputeShader()
{
}



//-----------------------------------------------------------------------------------------
//
//  �V�F�[�_�[���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_ComputeShader::CreateShader(const char* pFilepath)
{
	try{

		//	�V�F�[�_�[�t�@�C�����R���p�C������
		CompileFromFile(pFilepath, CS_ENTRY_POINT, CS_VERSION);

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
void DX_ComputeShader::Begin(ID3D11DeviceContext* pDeviceContext, const unsigned int classInstanceCount)
{
	pDeviceContext->CSSetShader(m_computeShader.Get(), &m_classInstance, classInstanceCount);
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
void DX_ComputeShader::CreateShaderObject()
{
	//	���I�V�F�[�_�[�@�����N��L���ɂ���N���X
	CreateClassLinkage();

	//	�V�F�[�_�[�I�u�W�F�N�g���쐬
	HRESULT l_hr = DX_System::GetInstance()->GetDevice()->CreateComputeShader(
		m_bytecord->GetBufferPointer(),
		m_bytecord->GetBufferSize(),
		m_classLinkage.Get(),
		&m_computeShader
		);

	//	ShaderObject�̍쐬�Ɏ��s�����ꍇ��o�C�g�R�[�h���������
	if (!DX_Debug::GetInstance()->IsHresultCheck(l_hr)){
		throw "ComputeShader�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���";
	}

}

