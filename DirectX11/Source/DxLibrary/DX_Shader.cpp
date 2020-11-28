#include	"DX_Library.h"
#include <locale.h>

//	�e�V�F�[�_�[�̃G���g���[�|�C���g
const char* DX_Shader::VS_ENTRY_POINT = "VS_Main";
const char* DX_Shader::HS_ENTRY_POINT = "HS_Main";
const char* DX_Shader::DS_ENTRY_POINT = "DS_Main";
const char* DX_Shader::GS_ENTRY_POINT = "GS_Main";
const char* DX_Shader::PS_ENTRY_POINT = "PS_Main";
const char* DX_Shader::CS_ENTRY_POINT = "CS_Main";

//	�e�V�F�[�_�[�̃o�[�W����
const char* DX_Shader::VS_VERSION = "vs_5_0";
const char* DX_Shader::HS_VERSION = "hs_5_0";
const char* DX_Shader::DS_VERSION = "ds_5_0";
const char* DX_Shader::GS_VERSION = "gs_5_0";
const char* DX_Shader::PS_VERSION = "ps_5_0";
const char* DX_Shader::CS_VERSION = "cs_5_0";

/// <summary>
/// �R���X�g���N�^
/// </summary>
DX_Shader::DX_Shader()
{}

/// �����t���R���X�g���N�^
/// </summary>
/// <param name="type">�V�F�[�_�[�^�C�v</param>
DX_Shader::DX_Shader(SHADER_TYPE type) : m_shaderType(type)
{}

/// <summary>
///	���z�f�X�g���N�^
/// </summary>
DX_Shader::~DX_Shader()
{}

/// <summary>
/// �V�F�[�_�[���쐬����
/// </summary>
/// <param name="filepath">�V�F�[�_�[�̃t�@�C���p�X</param>
/// <returns>����</returns>
bool DX_Shader::CreateShader(const char* filepath)
{
	bool succeed = false;
	
	do
	{
		if (false == Compile(filepath))
		{
			break;
		}

		if (false == CreateShaderObject())
		{
			break;
		}

		succeed = true;
	} while (false);
	

	return succeed;
}

void DX_Shader::End()
{
	ID3D11DeviceContext* deviceContext = DX_System::GetInstance()->GetDeviceContext();

	switch (m_shaderType)
	{
		case SHADER_TYPE::VERTEX_SHADER:
			deviceContext->VSSetShader(nullptr, nullptr, 0);
		break;

		case SHADER_TYPE::PIXEL_SHADER:
			deviceContext->PSSetShader(nullptr, nullptr, 0);
		break;

		case SHADER_TYPE::GEOMETRY_SHADER:
			deviceContext->GSSetShader(nullptr, nullptr, 0);
		break;

		case SHADER_TYPE::COMPUTE_SHADER:
			deviceContext->CSSetShader(nullptr, nullptr, 0);
		break;
	}
}

/// <summary>
/// �V�F�[�_�[�t�@�C�����R���p�C���������̃o�C�g�R�[�h���擾
/// </summary>
/// <returns>�o�C�g�R�[�h</returns>
ID3DBlob* DX_Shader::GetByteCord()
{
	return m_bytecord.Get();
}

/// <summary>
/// �V�F�[�_�[�t�@�C�����R���p�C������
/// </summary>
/// <param name="filepath">�t�@�C���p�X</param>
/// <returns>����</returns>
bool DX_Shader::Compile(const char* pFilepath)
{
	const char* entryPoint	= nullptr;
	const char* version		= nullptr;

	switch (m_shaderType) {
	case SHADER_TYPE::VERTEX_SHADER:
		entryPoint	= VS_ENTRY_POINT;
		version		= VS_VERSION;
		break;

	case SHADER_TYPE::GEOMETRY_SHADER:
		entryPoint	= GS_ENTRY_POINT;
		version		= GS_VERSION;
		break;

	case SHADER_TYPE::PIXEL_SHADER:
		entryPoint	= PS_ENTRY_POINT;
		version		= PS_VERSION;
		break;

	case SHADER_TYPE::COMPUTE_SHADER:
		entryPoint	= CS_ENTRY_POINT;
		version		= CS_VERSION;
		break;
	}

	//	��D��V�F�[�_�[
#if defined(DEBUG) || defined(_DEBUG)
	DWORD shaderFlags =
		D3D10_SHADER_ENABLE_STRICTNESS | 
		D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR |
		D3D10_SHADER_DEBUG;
#else
	DWORD shaderFlags =
		D3D10_SHADER_ENABLE_STRICTNESS | 
		D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;
#endif
	
	//���P�[���w��
	setlocale(LC_ALL, "japanese");

	size_t strlen;
	wchar_t filepath[MAX_PATH];
	mbstowcs_s(&strlen, filepath, strnlen_s(pFilepath, MAX_PATH) + 1, pFilepath, _TRUNCATE);

	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	HRESULT	hr = D3DCompileFromFile(filepath, nullptr, nullptr, entryPoint, version, shaderFlags, 0, &m_bytecord, &errorBlob);


#if defined(DEBUG) || defined(_DEBUG)
	//	�R���p�C�������s������
	if (FAILED(hr))
	{
		TRACE(errorBlob.Get() ? static_cast<const char*>(errorBlob.Get()->GetBufferPointer()) : "Not Found ShaderFile");
		return false;
	}
#endif

	return true;
}

/// <summary>
/// ���I�V�F�[�_�[��L���ɂ��郊���N���쐬
/// </summary>
/// <returns>����</returns>
bool DX_Shader::CreateClassLinkage()
{
	if (DX_Debug::GetInstance()->IsFailedHresult(DX_System::GetInstance()->GetDevice()->CreateClassLinkage(&m_classLinkage))){
		TRACE("�N���X�����N�P�[�W�̍쐬�Ɏ��s���܂����B");
		return false;
	}

	return true;
}
