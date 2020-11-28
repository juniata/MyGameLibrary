#include	"DX_Library.h"

/// <summary>
/// �����o�ϐ���������
/// </summary>
DX_GeometryShader::DX_GeometryShader() : DX_Shader(SHADER_TYPE::GEOMETRY_SHADER)
{}

/// <summary>
/// �f�X�g���N�^
/// </summary>
DX_GeometryShader::~DX_GeometryShader()
{}

/// �V�F�[�_�[�𗘗p����
/// </summary>
/// <param name="classInstanceCount">�N���X�C���X�^���X�̐�</param>
void DX_GeometryShader::Begin(const unsigned int classInstanceCount)
{
	ID3D11DeviceContext* deviceContext = DX_System::GetInstance()->GetDeviceContext();

	if (m_outputGeometryShader)
	{
		deviceContext->GSSetShader(m_outputGeometryShader.Get(), m_classInstance.GetAddressOf(), classInstanceCount);
	}
	else
	{
		deviceContext->GSSetShader(m_geometryShader.Get(), m_classInstance.GetAddressOf(), classInstanceCount);
	}
}


/// <summary>
/// �V�F�[�_�[�I�u�W�F�N�g���쐬����
/// </summary>
/// <returns>����</returns>
bool DX_GeometryShader::CreateShaderObject()
{
	auto succeed = false;

	do
	{
		if (false == CreateClassLinkage())
		{
			break;
		}

		HRESULT hr = DX_System::GetInstance()->GetDevice()->CreateGeometryShader(m_bytecord->GetBufferPointer(), m_bytecord->GetBufferSize(), m_classLinkage.Get(), &m_geometryShader);

		if (DX_Debug::GetInstance()->IsFailedHresult(hr))
		{
			TRACE("GeometryShader�I�u�W�F�N�g�̍쐬�Ɏ��s���܂����B");
			break;
		}

		succeed = true;

	} while (false);
	
	return succeed;
}


/// <summary>
/// �W�I���g���V�F�[�_�[����o�͂����f�[�^���擾
/// </summary>
/// <param name="decreation">�W�I���g���Ɠ����Z�}���e�B�N�X�̂��̂Ƀf�[�^���o��</param>
/// <param name="decreationElementCount">decreation�̗v�f��</param>
/// <param name="bufferStrides">�f�[�^�𕪊����������</param>
/// <param name="stridesElementCount">bufferStrides�̗v�f��</param>
void DX_GeometryShader::CreateGeometryShaderWithStreamOutput(D3D11_SO_DECLARATION_ENTRY decreation[], const UINT decreationElementCount, unsigned int* pBufferStrides, const UINT stridesElementCount)
{
	HRESULT hr = DX_System::GetInstance()->GetDevice()->CreateGeometryShaderWithStreamOutput(m_bytecord->GetBufferPointer(), m_bytecord->GetBufferSize(), decreation, decreationElementCount, pBufferStrides, stridesElementCount, 0, nullptr, &m_outputGeometryShader);

	//	ShaderObject�̍쐬�Ɏ��s�����ꍇ��o�C�g�R�[�h���������
	if (DX_Debug::GetInstance()->IsFailedHresult(hr)){
		TRACE("GeometryShaderWithStreamOutput�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���");
	}

}
