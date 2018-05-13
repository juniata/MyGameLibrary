#include	"DX_Library.h"
#include <locale.h>

//-----------------------------------------------------------------------------------------
//
//	メンバ変数を初期化
//
//-----------------------------------------------------------------------------------------
DX_Shader::DX_Shader()
{}


//-----------------------------------------------------------------------------------------
//
//	実体があれば解放
//
//-----------------------------------------------------------------------------------------
DX_Shader::~DX_Shader()
{
}

//-----------------------------------------------------------------------------------------
//
//	コンパイルした時のバイトコードを取得する
//
//-----------------------------------------------------------------------------------------
ID3DBlob* DX_Shader::GetByteCord()
{
	return m_bytecord.Get();
}

//-----------------------------------------------------------------------------------------
//
//	シェーダーファイルをコンパイルする
//
//-----------------------------------------------------------------------------------------
void DX_Shader::CompileFromFile(
	const char* pFilepath,
	const char* pEntryPoint,
	const char* pShaderVersion
	)
{
	
	//	列優先シェーダー
#if defined(DEBUG) || defined(_DEBUG)
	DWORD l_dwShaderFlags =
		D3D10_SHADER_ENABLE_STRICTNESS | 
		D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR |
		D3D10_SHADER_DEBUG;
#else
	DWORD l_dwShaderFlags =
		D3D10_SHADER_ENABLE_STRICTNESS | 
		D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;
#endif
	
	//ロケール指定
	setlocale(LC_ALL, "japanese");

	size_t strlen;
	wchar_t filepath[MAX_PATH];
	mbstowcs_s(&strlen, filepath, strnlen_s(pFilepath,MAX_PATH)+1, pFilepath, _TRUNCATE
	);

	ComPtr<ID3DBlob>	l_errorBlob;
	HRESULT		l_hr =
		D3DCompileFromFile(
		filepath,			//	ファイル名
		nullptr,			//	マクロ定義なし
		nullptr,			//	インクルードファイル無し
		pEntryPoint,		//	エントリーポイント
		pShaderVersion,		//	シェーダーバージョン
		l_dwShaderFlags,	//	コンパイルオプション
		0,					//	コンパイルオプション無し
		&m_bytecord,		//	コンパイルされたバイトコード
		&l_errorBlob		//	エラーメッセージ
	);
	//l_hr = D3DX11CompileFromFile(
	//	pFilepath,			//	ファイル名
	//	nullptr,			//	マクロ定義なし
	//	nullptr,			//	インクルードファイル無し
	//	pEntryPoint,		//	エントリーポイント
	//	pShaderVersion,		//	シェーダーバージョン
	//	l_dwShaderFlags,	//	コンパイルオプション
	//	0,					//	コンパイルオプション無し
	//	nullptr,			//	すぐにコンパイルしてから関数を抜ける
	//	&m_pBytecord,		//	コンパイルされたバイトコード
	//	&l_pErrorBlob,		//	エラーメッセージ
	//	nullptr				//	戻り値
	//	);

	//	シェーダーファイルのソースコードをチェック
	DX_Debug::CheckSourceCordOfShaderFile(l_hr, l_errorBlob.Get());
}

//-----------------------------------------------------------------------------------------
//
//	動的シェーダーを有効にするリンクを作成
//
//-----------------------------------------------------------------------------------------
void DX_Shader::CreateClassLinkage()
{
	if (!DX_Debug::IsHresultCheck(DX_System::GetInstance()->GetDevice()->CreateClassLinkage(&m_classLinkage))){
		//SAFE_RELEASE(m_pClassLinkage);
	}
}

