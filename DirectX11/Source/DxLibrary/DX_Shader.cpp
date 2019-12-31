#include	"DX_Library.h"
#include <locale.h>

//-----------------------------------------------------------------------------------------
//
//	メンバ変数を初期化
//
//-----------------------------------------------------------------------------------------
DX_Shader::DX_Shader() :
	m_pBytecord(nullptr),
	m_pClassInstance(nullptr),
	m_pClassLinkage(nullptr),
	m_pInputLayout(nullptr)
{}


//-----------------------------------------------------------------------------------------
//
//	実体があれば解放
//
//-----------------------------------------------------------------------------------------
DX_Shader::~DX_Shader()
{
	SAFE_RELEASE(m_pBytecord);
	SAFE_RELEASE(m_pClassInstance);
	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pClassLinkage);
}

//-----------------------------------------------------------------------------------------
//
//	コンパイルした時のバイトコードを取得する
//
//-----------------------------------------------------------------------------------------
ID3DBlob* DX_Shader::GetByteCord()
{
	return m_pBytecord;
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

	ID3DBlob*	l_pErrorBlob = nullptr;
	HRESULT		l_hr =
		D3DCompileFromFile(
		filepath,			//	ファイル名
		nullptr,			//	マクロ定義なし
		nullptr,			//	インクルードファイル無し
		pEntryPoint,		//	エントリーポイント
		pShaderVersion,		//	シェーダーバージョン
		l_dwShaderFlags,	//	コンパイルオプション
		0,					//	コンパイルオプション無し
		&m_pBytecord,		//	コンパイルされたバイトコード
		&l_pErrorBlob		//	エラーメッセージ
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
	DX_Debug::GetInstance()->CheckSourceCordOfShaderFile(l_hr, l_pErrorBlob);
	SAFE_RELEASE(l_pErrorBlob);
}

//-----------------------------------------------------------------------------------------
//
//	動的シェーダーを有効にするリンクを作成
//
//-----------------------------------------------------------------------------------------
void DX_Shader::CreateClassLinkage()
{
	if (!DX_Debug::GetInstance()->CheckHresult(DX_System::GetInstance()->GetDevice()->CreateClassLinkage(&m_pClassLinkage))){
		SAFE_RELEASE(m_pClassLinkage);
	}
}

