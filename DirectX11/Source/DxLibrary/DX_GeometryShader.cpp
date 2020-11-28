#include	"DX_Library.h"

/// <summary>
/// メンバ変数を初期化
/// </summary>
DX_GeometryShader::DX_GeometryShader() : DX_Shader(SHADER_TYPE::GEOMETRY_SHADER)
{}

/// <summary>
/// デストラクタ
/// </summary>
DX_GeometryShader::~DX_GeometryShader()
{}

/// シェーダーを利用する
/// </summary>
/// <param name="classInstanceCount">クラスインスタンスの数</param>
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
/// シェーダーオブジェクトを作成する
/// </summary>
/// <returns>成否</returns>
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
			TRACE("GeometryShaderオブジェクトの作成に失敗しました。");
			break;
		}

		succeed = true;

	} while (false);
	
	return succeed;
}


/// <summary>
/// ジオメトリシェーダーから出力したデータを取得
/// </summary>
/// <param name="decreation">ジオメトリと同じセマンティクスのものにデータを出力</param>
/// <param name="decreationElementCount">decreationの要素数</param>
/// <param name="bufferStrides">データを分割する線引き</param>
/// <param name="stridesElementCount">bufferStridesの要素数</param>
void DX_GeometryShader::CreateGeometryShaderWithStreamOutput(D3D11_SO_DECLARATION_ENTRY decreation[], const UINT decreationElementCount, unsigned int* pBufferStrides, const UINT stridesElementCount)
{
	HRESULT hr = DX_System::GetInstance()->GetDevice()->CreateGeometryShaderWithStreamOutput(m_bytecord->GetBufferPointer(), m_bytecord->GetBufferSize(), decreation, decreationElementCount, pBufferStrides, stridesElementCount, 0, nullptr, &m_outputGeometryShader);

	//	ShaderObjectの作成に失敗した場合､バイトコードを解放する
	if (DX_Debug::GetInstance()->IsFailedHresult(hr)){
		TRACE("GeometryShaderWithStreamOutputオブジェクトの作成に失敗しました");
	}

}
