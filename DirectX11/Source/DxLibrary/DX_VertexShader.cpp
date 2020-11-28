#include	"DX_Library.h"


/// <summary>
/// メンバ変数を初期化
/// </summary>
DX_VertexShader::DX_VertexShader() : DX_Shader(SHADER_TYPE::VERTEX_SHADER)
{
}

/// <summary>
/// デストラクタ
/// </summary>
DX_VertexShader::~DX_VertexShader() {}

/// <summary>
/// シェーダーを利用する
/// </summary>
/// <param name="classInstanceCount">クラスインスタンスの数</param>
void DX_VertexShader::Begin(const unsigned int classInstanceCount)
{
	DX_System::GetInstance()->GetDeviceContext()->VSSetShader(m_vertexShader.Get(), m_classInstance.GetAddressOf(), classInstanceCount);
}

/// <summary>
/// シェーダーの利用を終える
/// </summary>
void DX_VertexShader::End()
{
	DX_System::GetInstance()->GetDeviceContext()->VSSetShader(nullptr, nullptr, 0);
}

/// <summary>
/// シェーダーオブジェクトを作成する
/// </summary>
/// <returns>成否</returns>
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
			TRACE("VertexShaderオブジェクトの作成に失敗しました");
			break;
		}

		succeed = true;
	} while (false);

	
	return succeed;
}

