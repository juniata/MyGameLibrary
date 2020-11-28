#include	"DX_Library.h"


/// <summary>
/// メンバ変数を初期化
/// </summary>
DX_ComputeShader::DX_ComputeShader() : DX_Shader(SHADER_TYPE::COMPUTE_SHADER)
{}


/// <summary>
/// デストラクタ
/// </summary>
DX_ComputeShader::~DX_ComputeShader()
{
}

/// シェーダーを利用する
/// </summary>
/// <param name="classInstanceCount">クラスインスタンスの数</param>
void DX_ComputeShader::Begin(const unsigned int classInstanceCount)
{
	DX_System::GetInstance()->GetDeviceContext()->CSSetShader(m_conmputeShader.Get(), m_classInstance.GetAddressOf(), classInstanceCount);
}


/// <summary>
/// シェーダーオブジェクトを作成する
/// </summary>
/// <returns>成否</returns>
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
			TRACE("ComputeShaderオブジェクトの作成に失敗しました。");
			break;
		}

		succeed = true;
	} while (false);

	return succeed;
}

