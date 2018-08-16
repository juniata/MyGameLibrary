#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  メンバ変数を初期化
//
//-----------------------------------------------------------------------------------------
DX_GeometryShader::DX_GeometryShader() :
	m_pOutputGeometryShader(nullptr),
	m_pGeometryShader(nullptr)
{
}

//-----------------------------------------------------------------------------------------
//
//  実体があれば解放
//
//-----------------------------------------------------------------------------------------
DX_GeometryShader::~DX_GeometryShader()
{
	SAFE_RELEASE(m_pGeometryShader);
	SAFE_RELEASE(m_pOutputGeometryShader);
}

//-----------------------------------------------------------------------------------------
//
//  シェーダーを作成する
//
//-----------------------------------------------------------------------------------------
void DX_GeometryShader::CreateShader(ID3D11Device* pDevice, const char* pFilepath)
{
	try{

		//	シェーダーファイルをコンパイルする
		CompileFromFile(pFilepath, GS_ENTRY_POINT, GS_VERSION);

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
void DX_GeometryShader::Begin(ID3D11DeviceContext* pDeviceContext, const unsigned int classInstanceCount)
{
	if (m_pOutputGeometryShader){
		pDeviceContext->GSSetShader(m_pOutputGeometryShader, &m_pClassInstance, classInstanceCount);
	}
	else{
		pDeviceContext->GSSetShader(m_pGeometryShader, &m_pClassInstance, classInstanceCount);
	}
}

//-----------------------------------------------------------------------------------------
//
//  シェーダーの利用を終える
//
//-----------------------------------------------------------------------------------------
void DX_GeometryShader::End(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->GSSetShader(nullptr, nullptr, 0);
}

//-----------------------------------------------------------------------------------------
//
//  シェーダーオブジェクトを作成する
//
//-----------------------------------------------------------------------------------------
void DX_GeometryShader::CreateShaderObject(ID3D11Device* pDevice)
{
	//	動的シェーダー　リンクを有効にするクラス
	CreateClassLinkage(pDevice);

	//	シェーダーオブジェクトを作成
	HRESULT hr = pDevice->CreateGeometryShader(m_pBytecord->GetBufferPointer(), m_pBytecord->GetBufferSize(), m_pClassLinkage, &m_pGeometryShader);

	//	ShaderObjectの作成に失敗した場合､バイトコードを解放する
	DX_Debug::GetInstance()->ThrowIfFailed(hr, "GeometryShaderオブジェクトの作成に失敗しました");
}


//-----------------------------------------------------------------------------------------
//
//  ジオメトリーシェーダーから出力したデータを取得
//
//-----------------------------------------------------------------------------------------
void DX_GeometryShader::CreateGeometryShaderWithStreamOutput(D3D11_SO_DECLARATION_ENTRY	decreation[], const UINT decreationElementCount, unsigned int* pBufferStrides, const UINT stridesElementCount)
{
	HRESULT hr = DX_System::GetInstance()->GetDevice()->CreateGeometryShaderWithStreamOutput(
		m_pBytecord->GetBufferPointer(),
		m_pBytecord->GetBufferSize(),
		decreation,
		decreationElementCount,
		pBufferStrides,
		stridesElementCount,
		0,
		nullptr,
		&m_pOutputGeometryShader
		);

	//	ShaderObjectの作成に失敗した場合､バイトコードを解放する
	DX_Debug::GetInstance()->ThrowIfFailed(hr, "GeometryShaderWithStreamOutputオブジェクトの作成に失敗しました");
}
