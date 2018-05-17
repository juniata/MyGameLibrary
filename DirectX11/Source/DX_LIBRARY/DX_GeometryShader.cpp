#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  メンバ変数を初期化
//
//-----------------------------------------------------------------------------------------
DX_GeometryShader::DX_GeometryShader()
{
}

//-----------------------------------------------------------------------------------------
//
//  実体があれば解放
//
//-----------------------------------------------------------------------------------------
DX_GeometryShader::~DX_GeometryShader()
{
}

//-----------------------------------------------------------------------------------------
//
//  シェーダーを作成する
//
//-----------------------------------------------------------------------------------------
void DX_GeometryShader::CreateShader(const char* pFilepath)
{
	try{

		//	シェーダーファイルをコンパイルする
		CompileFromFile(pFilepath, GS_ENTRY_POINT, GS_VERSION);

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
void DX_GeometryShader::Begin(ID3D11DeviceContext* pDeviceContext, const unsigned int classInstanceCount)
{
	if (m_outputGeometryShader.Get()){
		pDeviceContext->GSSetShader(m_outputGeometryShader.Get(), &m_classInstance, classInstanceCount);
	}
	else{
		pDeviceContext->GSSetShader(m_geometryShader.Get(), &m_classInstance, classInstanceCount);
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
void DX_GeometryShader::CreateShaderObject()
{
	//	動的シェーダー　リンクを有効にするクラス
	CreateClassLinkage();

	//	シェーダーオブジェクトを作成
	HRESULT l_hr = DX_System::GetInstance()->GetDevice()->CreateGeometryShader(
		m_bytecord->GetBufferPointer(),
		m_bytecord->GetBufferSize(),
		m_classLinkage.Get(),
		&m_geometryShader
		);

	//	ShaderObjectの作成に失敗した場合､バイトコードを解放する
	if (!DX_Debug::IsHresultCheck(l_hr)){
		//SAFE_RELEASE(m_pBytecord);
		throw "GeometryShaderオブジェクトの作成に失敗しました";
	}
}


//-----------------------------------------------------------------------------------------
//
//  ジオメトリーシェーダーから出力したデータを取得
//
//-----------------------------------------------------------------------------------------
void DX_GeometryShader::CreateGeometryShaderWithStreamOutput(
	D3D11_SO_DECLARATION_ENTRY	decreation[],
	const UINT					decreationElementCount,
	unsigned int*				pBufferStrides,
	const UINT					stridesElementCount
	)
{
	HRESULT l_hr = DX_System::GetInstance()->GetDevice()->CreateGeometryShaderWithStreamOutput(
		m_bytecord->GetBufferPointer(),
		m_bytecord->GetBufferSize(),
		decreation,
		decreationElementCount,
		pBufferStrides,
		stridesElementCount,
		0,
		nullptr,
		&m_outputGeometryShader
		);

	//	ShaderObjectの作成に失敗した場合､バイトコードを解放する
	if (!DX_Debug::IsHresultCheck(l_hr)){
		//SAFE_RELEASE(m_pBytecord);
		throw "GeometryShaderWithStreamOutputオブジェクトの作成に失敗しました";
	}

}
