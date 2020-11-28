#include	"DX_Library.h"


/// <summary>
/// �����o�ϐ���������
/// </summary>
DX_ComputeShader::DX_ComputeShader() : DX_Shader(SHADER_TYPE::COMPUTE_SHADER)
{}


/// <summary>
/// �f�X�g���N�^
/// </summary>
DX_ComputeShader::~DX_ComputeShader()
{
}

/// �V�F�[�_�[�𗘗p����
/// </summary>
/// <param name="classInstanceCount">�N���X�C���X�^���X�̐�</param>
void DX_ComputeShader::Begin(const unsigned int classInstanceCount)
{
	DX_System::GetInstance()->GetDeviceContext()->CSSetShader(m_conmputeShader.Get(), m_classInstance.GetAddressOf(), classInstanceCount);
}


/// <summary>
/// �V�F�[�_�[�I�u�W�F�N�g���쐬����
/// </summary>
/// <returns>����</returns>
bool DX_ComputeShader::CreateShaderObject()
{
	auto succeed = false;

	do
	{
		if (CreateClassLinkage())
		{
			break;
		}

		HRESULT hr = DX_System::GetInstance()->GetDevice()->CreateComputeShader(m_bytecord->GetBufferPointer(), m_bytecord->GetBufferSize(), m_classLinkage.Get(), m_conmputeShader.GetAddressOf());
	
		if (DX_Debug::GetInstance()->IsFailedHresult(hr))
		{
			TRACE("ComputeShader�I�u�W�F�N�g�̍쐬�Ɏ��s���܂����B");
			break;
		}

		succeed = true;
	} while (false);

	return succeed;
}

