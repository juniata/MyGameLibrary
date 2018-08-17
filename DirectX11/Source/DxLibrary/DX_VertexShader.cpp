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
void DX_VertexShader::CreateShader(const char* pFilepath)
{
	try{
		//	シェーダーファイルをコンパイルする
		CompileFromFile(pFilepath, VS_ENTRY_POINT, VS_VERSION);

		//	シェーダーオブジェクトを作成
		CreateShaderObject();
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
void DX_VertexShader::CreateShaderObject()
{
	//	動的シェーダー　リンクを有効にするクラス
	CreateClassLinkage();

	//	シェーダーオブジェクトを作成
	HRESULT l_hr = DX_System::GetInstance()->GetDevice()->CreateVertexShader(
		m_pBytecord->GetBufferPointer(),
		m_pBytecord->GetBufferSize(),
		m_pClassLinkage,
		&m_pVertexShader
		);

	//	ShaderObjectの作成に失敗した場合､バイトコードを解放する
	if (!DX_Debug::GetInstance()->IsHresultCheck(l_hr)){
		throw "VertexShaderオブジェクトの作成に失敗しました";
	}
	
}

