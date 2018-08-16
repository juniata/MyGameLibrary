#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  メンバ変数を初期化
//
//-----------------------------------------------------------------------------------------
DX_VertexShader::DX_VertexShader() :
	m_pVertexShader(nullptr)
{
}



//-----------------------------------------------------------------------------------------
//
//  実体があれば解放
//
//-----------------------------------------------------------------------------------------
DX_VertexShader::~DX_VertexShader()
{
	SAFE_RELEASE(m_pVertexShader);
}

//-----------------------------------------------------------------------------------------
//
//  シェーダーを作成する
//
//-----------------------------------------------------------------------------------------
void DX_VertexShader::CreateShader(ID3D11Device* pDevice, const char* pFilepath)
{
	try{
		//	シェーダーファイルをコンパイルする
		CompileFromFile(pFilepath, VS_ENTRY_POINT, VS_VERSION);

		//	シェーダーオブジェクトを作成
		CreateShaderObject(pDevice);
	}
	catch (char* pMessage){
		throw pMessage;
	}

}

//-----------------------------------------------------------------------------------------
//
// シェーダーの使用を開始
//
//-----------------------------------------------------------------------------------------
void DX_VertexShader::Begin(ID3D11DeviceContext* pDeviceContext, const unsigned int classInstanceCount)
{
	pDeviceContext->VSSetShader(m_pVertexShader, &m_pClassInstance, classInstanceCount);
}

//-----------------------------------------------------------------------------------------
//
//  シェーダーの利用を終える
//
//-----------------------------------------------------------------------------------------
void DX_VertexShader::End(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->VSSetShader(nullptr, nullptr, 0);
}

//-----------------------------------------------------------------------------------------
//
//  シェーダーオブジェクトを作成する
//
//-----------------------------------------------------------------------------------------
void DX_VertexShader::CreateShaderObject(ID3D11Device* pDevice)
{
	//	動的シェーダー　リンクを有効にするクラス
	CreateClassLinkage(pDevice);

	//	シェーダーオブジェクトを作成
	HRESULT hr = pDevice->CreateVertexShader(m_pBytecord->GetBufferPointer(), m_pBytecord->GetBufferSize(), m_pClassLinkage, &m_pVertexShader);

	//	ShaderObjectの作成に失敗した場合､バイトコードを解放する
	DX_Debug::GetInstance()->ThrowIfFailed(hr, "VertexShaderオブジェクトの作成に失敗しました");
}

