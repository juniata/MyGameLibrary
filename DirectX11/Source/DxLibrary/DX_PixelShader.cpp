#include	"DX_Library.h"


/// <summary>
/// メンバ変数を初期化
/// </summary>
DX_PixelShader::DX_PixelShader() : DX_Shader(SHADER_TYPE::PIXEL_SHADER)
{}

/// <summary>
/// デストラクタ
/// </summary>
DX_PixelShader::~DX_PixelShader() {}


/// シェーダーを利用する
/// </summary>
/// <param name="classInstanceCount">クラスインスタンスの数</param>
void DX_PixelShader::Begin(const unsigned int classInstanceCount)
{
	DX_System::GetInstance()->GetDeviceContext()->PSSetShader(m_pixelShader.Get(), m_classInstance.GetAddressOf(), classInstanceCount);
}

/// <summary>
/// シェーダーオブジェクトを作成する
/// </summary>
/// <returns>成否</returns>
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
			TRACE("PixelShaderオブジェクトの作成に失敗しました");
			break;
		}

		succeed = true;
	} while (false);

	return succeed;
}

