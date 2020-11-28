#include	"DX_Library.h"
#include <locale.h>

//	各シェーダーのエントリーポイント
const char* DX_Shader::VS_ENTRY_POINT = "VS_Main";
const char* DX_Shader::HS_ENTRY_POINT = "HS_Main";
const char* DX_Shader::DS_ENTRY_POINT = "DS_Main";
const char* DX_Shader::GS_ENTRY_POINT = "GS_Main";
const char* DX_Shader::PS_ENTRY_POINT = "PS_Main";
const char* DX_Shader::CS_ENTRY_POINT = "CS_Main";

//	各シェーダーのバージョン
const char* DX_Shader::VS_VERSION = "vs_5_0";
const char* DX_Shader::HS_VERSION = "hs_5_0";
const char* DX_Shader::DS_VERSION = "ds_5_0";
const char* DX_Shader::GS_VERSION = "gs_5_0";
const char* DX_Shader::PS_VERSION = "ps_5_0";
const char* DX_Shader::CS_VERSION = "cs_5_0";

/// <summary>
/// コンストラクタ
/// </summary>
DX_Shader::DX_Shader()
{}

/// 引数付きコンストラクタ
/// </summary>
/// <param name="type">シェーダータイプ</param>
DX_Shader::DX_Shader(SHADER_TYPE type) : m_shaderType(type)
{}

/// <summary>
///	仮想デストラクタ
/// </summary>
DX_Shader::~DX_Shader()
{}

/// <summary>
/// シェーダーを作成する
/// </summary>
/// <param name="filepath">シェーダーのファイルパス</param>
/// <returns>成否</returns>
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
/// シェーダーファイルをコンパイルした時のバイトコードを取得
/// </summary>
/// <returns>バイトコード</returns>
ID3DBlob* DX_Shader::GetByteCord()
{
	return m_bytecord.Get();
}

/// <summary>
/// シェーダーファイルをコンパイルする
/// </summary>
/// <param name="filepath">ファイルパス</param>
/// <returns>成否</returns>
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

	//	列優先シェーダー
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
	
	//ロケール指定
	setlocale(LC_ALL, "japanese");

	size_t strlen;
	wchar_t filepath[MAX_PATH];
	mbstowcs_s(&strlen, filepath, strnlen_s(pFilepath, MAX_PATH) + 1, pFilepath, _TRUNCATE);

	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	HRESULT	hr = D3DCompileFromFile(filepath, nullptr, nullptr, entryPoint, version, shaderFlags, 0, &m_bytecord, &errorBlob);


#if defined(DEBUG) || defined(_DEBUG)
	//	コンパイルが失敗した時
	if (FAILED(hr))
	{
		TRACE(errorBlob.Get() ? static_cast<const char*>(errorBlob.Get()->GetBufferPointer()) : "Not Found ShaderFile");
		return false;
	}
#endif

	return true;
}

/// <summary>
/// 動的シェーダーを有効にするリンクを作成
/// </summary>
/// <returns>成否</returns>
bool DX_Shader::CreateClassLinkage()
{
	if (DX_Debug::GetInstance()->IsFailedHresult(DX_System::GetInstance()->GetDevice()->CreateClassLinkage(&m_classLinkage))){
		TRACE("クラスリンクケージの作成に失敗しました。");
		return false;
	}

	return true;
}
