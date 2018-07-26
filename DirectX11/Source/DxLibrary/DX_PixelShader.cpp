#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  メンバ変数を初期化
//
//-----------------------------------------------------------------------------------------
DX_PixelShader::DX_PixelShader() :
	m_pPixelShader(nullptr)
{
}



//-----------------------------------------------------------------------------------------
//
//  実体があれば解放
//
//-----------------------------------------------------------------------------------------
DX_PixelShader::~DX_PixelShader()
{
	SAFE_RELEASE(m_pPixelShader);
}



//-----------------------------------------------------------------------------------------
//
//  シェーダーを作成する
//
//-----------------------------------------------------------------------------------------
void DX_PixelShader::CreateShader(const char* pFilepath)
{
	try{

		//	シェーダーファイルをコンパイルする
		CompileFromFile(pFilepath, PS_ENTRY_POINT, PS_VERSION);

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
void DX_PixelShader::Begin(ID3D11DeviceContext* pDeviceContext, const unsigned int classInstanceCount)
{
	pDeviceContext->PSSetShader(m_pPixelShader, &m_pClassInstance, classInstanceCount);
}

//-----------------------------------------------------------------------------------------
//
//  シェーダーの利用を終える
//
//-----------------------------------------------------------------------------------------
void DX_PixelShader::End(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->PSSetShader(nullptr, nullptr, 0);
}

//-----------------------------------------------------------------------------------------
//
//  シェーダーオブジェクトを作成する
//
//-----------------------------------------------------------------------------------------
void DX_PixelShader::CreateShaderObject()
{
	//	動的シェーダー　リンクを有効にするクラス
	CreateClassLinkage();

	//	シェーダーオブジェクトを作成
	HRESULT l_hr = DX_System::GetInstance()->GetDevice()->CreatePixelShader(
		m_pBytecord->GetBufferPointer(),
		m_pBytecord->GetBufferSize(),
		m_pClassLinkage,
		&m_pPixelShader
		);

	//	ShaderObjectの作成に失敗した場合､バイトコードを解放する
	if (!DX_Debug::GetInstance()->IsHresultCheck(l_hr)){
		throw "PixelShaderオブジェクトの作成に失敗しました";
	}

}

