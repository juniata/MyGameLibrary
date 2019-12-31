#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  メンバ変数を初期化
//
//-----------------------------------------------------------------------------------------
DX_ComputeShader::DX_ComputeShader() :
	m_pComputeShader(nullptr)
{
}



//-----------------------------------------------------------------------------------------
//
//  実体があれば解放
//
//-----------------------------------------------------------------------------------------
DX_ComputeShader::~DX_ComputeShader()
{
	SAFE_RELEASE(m_pComputeShader);
}



//-----------------------------------------------------------------------------------------
//
//  シェーダーを作成する
//
//-----------------------------------------------------------------------------------------
void DX_ComputeShader::CreateShader(const char* pFilepath)
{
	try{

		//	シェーダーファイルをコンパイルする
		CompileFromFile(pFilepath, CS_ENTRY_POINT, CS_VERSION);

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
void DX_ComputeShader::Begin(ID3D11DeviceContext* pDeviceContext, const unsigned int classInstanceCount)
{
	pDeviceContext->CSSetShader(m_pComputeShader, &m_pClassInstance, classInstanceCount);
}

//-----------------------------------------------------------------------------------------
//
//  シェーダーの利用を終える
//
//-----------------------------------------------------------------------------------------
void DX_ComputeShader::End(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->CSSetShader(nullptr, nullptr, 0);
}

//-----------------------------------------------------------------------------------------
//
//  シェーダーオブジェクトを作成する
//
//-----------------------------------------------------------------------------------------
void DX_ComputeShader::CreateShaderObject()
{
	//	動的シェーダー　リンクを有効にするクラス
	CreateClassLinkage();

	//	シェーダーオブジェクトを作成
	HRESULT l_hr = DX_System::GetInstance()->GetDevice()->CreateComputeShader(
		m_pBytecord->GetBufferPointer(),
		m_pBytecord->GetBufferSize(),
		m_pClassLinkage,
		&m_pComputeShader
		);

	//	ShaderObjectの作成に失敗した場合､バイトコードを解放する
	if (!DX_Debug::GetInstance()->CheckHresult(l_hr)){
		throw "ComputeShaderオブジェクトの作成に失敗しました";
	}

}

