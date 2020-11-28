#include	"DX_Library.h"


/// <summary>
/// �����o�ϐ���������
/// </summary>
DX_VertexShader::DX_VertexShader() : DX_Shader(SHADER_TYPE::VERTEX_SHADER)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
DX_VertexShader::~DX_VertexShader() {}

/// <summary>
/// �V�F�[�_�[�𗘗p����
/// </summary>
/// <param name="classInstanceCount">�N���X�C���X�^���X�̐�</param>
void DX_VertexShader::Begin(const unsigned int classInstanceCount)
{
	DX_System::GetInstance()->GetDeviceContext()->VSSetShader(m_vertexShader.Get(), m_classInstance.GetAddressOf(), classInstanceCount);
}

/// <summary>
/// �V�F�[�_�[�̗��p���I����
/// </summary>
void DX_VertexShader::End()
{
	DX_System::GetInstance()->GetDeviceContext()->VSSetShader(nullptr, nullptr, 0);
}

/// <summary>
/// �V�F�[�_�[�I�u�W�F�N�g���쐬����
/// </summary>
/// <returns>����</returns>
bool DX_VertexShader::CreateShaderObject()
{
	auto succeed = false;

	do
	{
		if (false == CreateClassLinkage())
		{
			break;
		}

		HRESULT hr = DX_System::GetInstance()->GetDevice()->CreateVertexShader(m_bytecord->GetBufferPointer(), m_bytecord->GetBufferSize(), m_classLinkage.Get(), &m_vertexShader);


		if (DX_Debug::GetInstance()->IsFailedHresult(hr)) 
		{
			TRACE("VertexShader�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���");
			break;
		}

		succeed = true;
	} while (false);

	
	return succeed;
}

