#ifndef __DX_SHADER_H_
#define __DX_SHADER_H_

enum class SHADER_TYPE {
	VERTEX_SHADER,
	GEOMETRY_SHADER,
	PIXEL_SHADER,
	COMPUTE_SHADER,
};

//****************************************************************************************************
//
//	Shader
//
//****************************************************************************************************
class DX_Shader
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DX_Shader();

	/// <summary>
	/// 引数付きコンストラクタ
	/// </summary>
	/// <param name="type">シェーダータイプ</param>
	DX_Shader(SHADER_TYPE type);

	/// <summary>
	///	仮想デストラクタ
	/// </summary>
	virtual ~DX_Shader();

	/// <summary>
	/// シェーダーを作成する
	/// </summary>
	/// <param name="filepath">シェーダーのファイルパス</param>
	/// <returns>成否</returns>
	bool CreateShader(const char* filepath);

	/// <summary>
	/// シェーダーを利用する
	/// </summary>
	/// <param name="classInstanceCount">クラスインスタンスの数</param>
	virtual void Begin(const unsigned int classInstanceCount = 0) = 0;

	/// <summary>
	/// シェーダーの利用を終える
	/// </summary>
	void End();

	/// <summary>
	/// シェーダーファイルをコンパイルした時のバイトコードを取得
	/// </summary>
	/// <returns>バイトコード</returns>
	ID3DBlob* GetByteCord();

protected:
	Microsoft::WRL::ComPtr<ID3D11ClassLinkage>	m_classLinkage;
	Microsoft::WRL::ComPtr<ID3D11ClassInstance>	m_classInstance;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
	Microsoft::WRL::ComPtr<ID3DBlob>			m_bytecord;

	SHADER_TYPE m_shaderType;

	/// <summary>
	/// シェーダーファイルをコンパイルする
	/// </summary>
	/// <param name="filepath">ファイルパス</param>
	/// <returns>成否</returns>
	bool Compile(const char* filepath);

	/// <summary>
	/// シェーダーオブジェクトを作成する
	/// </summary>
	/// <returns>成否</returns>
	virtual bool CreateShaderObject() = 0;

	/// <summary>
	/// 動的シェーダーを有効にするリンクを作成
	/// </summary>
	/// <returns>成否</returns>
	bool CreateClassLinkage();

private:
	//	各シェーダーのエントリーポイント
	static const char* VS_ENTRY_POINT;
	static const char* HS_ENTRY_POINT;
	static const char* DS_ENTRY_POINT;
	static const char* GS_ENTRY_POINT;
	static const char* PS_ENTRY_POINT;
	static const char* CS_ENTRY_POINT;

	//	各シェーダーのバージョン
	static const char* VS_VERSION;
	static const char* HS_VERSION;
	static const char* DS_VERSION;
	static const char* GS_VERSION;
	static const char* PS_VERSION;
	static const char* CS_VERSION;
};

//	各シェーダーヘッダーをインクルード
#include	"DX_VertexShader.h"
#include	"DX_GeometryShader.h"
#include	"DX_PixelShader.h"
#include	"DX_ComputeShader.h"
#endif // !__DX_SHADER_H_
