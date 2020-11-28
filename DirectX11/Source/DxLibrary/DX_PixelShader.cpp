#include	"DX_Library.h"


/// <summary>
/// �����o�ϐ���������
/// </summary>
DX_PixelShader::DX_PixelShader() : DX_Shader(SHADER_TYPE::PIXEL_SHADER)
{}

/// <summary>
/// �f�X�g���N�^
/// </summary>
DX_PixelShader::~DX_PixelShader() {}


/// �V�F�[�_�[�𗘗p����
/// </summary>
/// <param name="classInstanceCount">�N���X�C���X�^���X�̐�</param>
void DX_PixelShader::Begin(const unsigned int classInstanceCount)
{
	DX_System::GetInstance()->GetDeviceContext()->PSSetShader(m_pixelShader.Get(), m_classInstance.GetAddressOf(), classInstanceCount);
}

/// <summary>
/// �V�F�[�_�[�I�u�W�F�N�g���쐬����
/// </summary>
/// <returns>����</returns>
bool DX_PixelShader::CreateShaderObject()
{
	auto succeed = false;

	do
	{
		if (false == CreateClassLinkage())
		{
			break;
		}

		HRESULT hr = DX_System::GetInstance()->GetDevice()->CreatePixelShader(m_bytecord->GetBufferPointer(), m_bytecord->GetBufferSize(), m_classLinkage.Get(), &m_pixelShader);

		if (DX_Debug::GetInstance()->IsFailedHresult(hr)) {
			TRACE("PixelShader�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���");
			break;
		}

		succeed = true;
	} while (false);

	return succeed;
}

