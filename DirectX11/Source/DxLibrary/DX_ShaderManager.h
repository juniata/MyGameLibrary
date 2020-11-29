#ifndef __DX_SHADER_MANAGER_H_
#define __DX_SHADER_MANAGER_H_

struct SHADER {
	struct OBJECT2D {
		static const char* VERTEX_SHADER;
		static const char* PIXEL_SHADER;
	};

	struct OBJECT3D {
		static const char* VERTEX_SHADER;
		static const char* PIXEL_SHADER;
	};

	struct BOX {
		static const char* VERTEX_SHADER;
		static const char* PIXEL_SHADER;
	};
};

typedef SHADER::OBJECT2D	SHADER_OBJECT_2D;
typedef SHADER::OBJECT3D	SHADER_OBJECT_3D;
typedef SHADER::BOX			SHADER_BOX;

//****************************************************************************************************
//
//	ShaderManager
//
//****************************************************************************************************
/// <summary>
///	シェーダーの各クラスをファイル毎に管理する。
/// また、シェーダーにデータを送る
/// </summary>
class DX_ShaderManager : public DX_Singleton<DX_ShaderManager>
{
public:
	/// <summary>
	/// メンバ変数の初期化及びシェーダーの作成を行う
	/// </summary>
	bool Initialize();

	/// <summary>
	/// シェーダーに関する変数を解放
	/// </summary>
	~DX_ShaderManager();

	/// <summary>
	/// 指定したシェーダーを取得する
	/// </summary>
	/// <param name="filepath">ファイルパス</param>
	/// <returns></returns>
	DX_Shader* GetShader(const char* filepath);

	/// <summary>
	/// 2D描画用InputLayoutを取得する
	/// </summary>
	/// <returns>2D描画用InputLayout</returns>
	ID3D11InputLayout* GetInputLayout2D();


	/// <summary>
	/// 3D描画用InputLayoutを取得する
	/// </summary>
	/// <returns>3D描画用InputLayout</returns>
	ID3D11InputLayout* GetInputLayout3D();

	/// <summary>
	/// Box描画用InputLayoutを取得する
	/// </summary>
	/// <returns>Box描画用InputLayout</returns>
	ID3D11InputLayout* GetInputLayoutBox();

	/// <summary>
	/// ワールド行列を送る
	/// レジスタ番号は1
	/// </summary>
	/// <param name="worldMat">ワールド行列</param>
	/// <param name="shaderType">送るシェーダーのタイプ</param>
	void SetWorldMat(const DirectX::XMFLOAT4X4& worldMat, SHADER_TYPE shaderType);

	/// <summary>
	/// intをシェーダーに送る
	/// </summary>
	/// <param name="registerNum">使用するレジスタ番号</param>
	/// <param name="value">送る値</param>
	/// <param name="shaderType">送るシェーダーのタイプ</param>
	void SetInt(const unsigned int registerNum, const int value, SHADER_TYPE shaderType);
	
	/// <summary>
	/// Vector2をシェーダーに送る
	/// </summary>
	/// <param name="registerNum">使用するレジスタ番号</param>
	/// <param name="value">送る値</param>
	/// <param name="shaderType">送るシェーダーのタイプ</param>
	void SetVector2(const unsigned int registerNum, const DirectX::XMFLOAT2& value, SHADER_TYPE shaderType);

	/// <summary>
	/// Vector3をシェーダーに送る
	/// </summary>
	/// <param name="registerNum">使用するレジスタ番号</param>
	/// <param name="value">送る値</param>
	/// <param name="shaderType">送るシェーダーのタイプ</param>
	void SetVector3(const unsigned int registerNum, const DirectX::XMFLOAT3& value, SHADER_TYPE shaderType);

	/// <summary>
	/// Vector4をシェーダーに送る
	/// </summary>
	/// <param name="registerNum">使用するレジスタ番号</param>
	/// <param name="value">送る値</param>
	/// <param name="shaderType">送るシェーダーのタイプ</param>
	void SetVector4(const unsigned int registerNum, const DirectX::XMFLOAT4& value, SHADER_TYPE shaderType);
	
	/// <summary>
	/// Vector2をリソースとしてシェーダーに送る
	/// </summary>
	/// <param name="registerNum">使用するレジスタ番号</param>
	/// <param name="vecList">送る値のリスト</param>
	/// <param name="vecCount">リストの要素の数</param>
	/// <param name="shader_type">送るシェーダーのタイプ</param>
	void SetVector2Resource(const unsigned int registerNum, const DirectX::XMFLOAT2* vecList, const unsigned int vecCount, SHADER_TYPE shader_type);

	/// <summary>
	/// Vector3をリソースとしてシェーダーに送る
	/// </summary>
	/// <param name="registerNum">使用するレジスタ番号</param>
	/// <param name="vecList">送る値のリスト</param>
	/// <param name="vecCount">リストの要素の数</param>
	/// <param name="shader_type">送るシェーダーのタイプ</param>
	void SetVector3Resource(const unsigned int registerNum, const DirectX::XMFLOAT3* vecList, const unsigned int vecCount, SHADER_TYPE shader_type);

	/// <summary>
	/// Vector4をリソースとしてシェーダーに送る
	/// </summary>
	/// <param name="registerNum">使用するレジスタ番号</param>
	/// <param name="vecList">送る値のリスト</param>
	/// <param name="vecCount">リストの要素の数</param>
	/// <param name="shader_type">送るシェーダーのタイプ</param>
	void SetVector4Resource(const unsigned int registerNum, const DirectX::XMFLOAT4* vecList, const unsigned int vecCount, SHADER_TYPE shader_type);

	/// <summary>
	/// 行列をシェーダーに送る
	/// </summary>
	/// <param name="regiserNum">使用するレジスタ番号</param>
	/// <param name="mat">送る値</param>
	/// <param name="shaderType">送るシェーダーのタイプ</param>
	void SetMatrix(const unsigned int regiserNum, const DirectX::XMFLOAT4X4& mat, SHADER_TYPE shaderType);

	/// <summary>
	/// 複数の行列をシェーダーに送る
	/// </summary>
	/// <param name="ppBuffer">バッファ</param>
	/// <param name="regiserNum">使用するレジスタ番号</param>
	/// <param name="mat">送る値</param>
	/// <param name="shaderType">送るシェーダーのタイプ</param>
	void SetMatrix(ID3D11Buffer* const* ppBuffer, const unsigned int regiserNum, const DirectX::XMFLOAT4X4*	pMat, const unsigned int matCount, SHADER_TYPE shaderType);


	/// <summary>
	/// 複数の行列をシェーダーに送る
	/// </summary>
	/// <param name="regiserNum">使用するレジスタの番号</param>
	/// <param name="matList">行列のリスト</param>
	/// <param name="matCount">行列のリストの数</param>
	/// <param name="shaderType">送るシェーダーのタイプ</param>
	void SetMatrixResoruce(const unsigned int regiserNum, const DirectX::XMFLOAT4X4* matList, const unsigned int matCount, SHADER_TYPE shaderType);
	
	/// <summary>
	/// 複数の行列をバッファでシェーダーに送る
	/// </summary>
	/// <param name="regiserNum">使用するレジスタの番号</param>
	/// <param name="matList">行列のリスト</param>
	/// <param name="matCount">行列のリストの数</param>
	/// <param name="shaderType">送るシェーダーのタイプ</param>
	void SetMatrixResoruce(const unsigned int regiserNum, ID3D11Buffer* buffer, const unsigned int matCount, SHADER_TYPE shaderType);
	
	/// <summary>
	/// 定数バッファをシェーダーに送る
	/// </summary>
	/// <param name="registerNum">使用するレジスタの番号</param>
	/// <param name="numBuffers">バッファの数</param>
	/// <param name="constantBuffers">シェーダーに送るバッファのリスト</param>
	/// <param name="shaderType">使用するシェーダーのタイプ</param>
	void SetConstantBuffers(UINT registerNum, UINT numBuffers, ID3D11Buffer* const* constantBuffers, SHADER_TYPE shaderType);
	
	/// <summary>
	/// リソースをシェーダーに送る
	/// </summary>
	/// <param name="registerNum">使用するレジスタの番号</param>
	/// <param name="numBuffers">使用するリソースの数</param>
	/// <param name="shaderResourceView">シェーダーに送るリソースのリスト</param>
	/// <param name="shaderType">使用するシェーダーのタイプ</param>
	void SetShaderResources(const unsigned int registerNum, const unsigned int numBuffers, ID3D11ShaderResourceView* const* shaderResourceView, SHADER_TYPE shaderType);

private:
	friend class DX_Singleton<DX_ShaderManager>;

	static const size_t SHADER_NUM = 10;
	struct Shader {
		DX_Shader* shader;
		const char* filepath;
	};
	Shader m_shaders[SHADER_NUM];

	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout2D;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout3D;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayoutBox;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_constantBuffer;

	/// <summary>
	/// 変数の初期化を行う
	/// </summary>
	DX_ShaderManager();

	/// <summary>
	/// コンピュートシェーダーが使えるかどうかを
	/// </summary>
	/// <returns>使用できるかどうか</returns>
	bool UsedComputeShader();

	/// <summary>
	/// シェーダーを作成する
	/// </summary>
	/// <param name="filepath">シェーダーのファイルパス</param>
	/// <returns>成否</returns>
	bool CreateShader(const char* filepath);

	/// <summary>
	/// インプットレイアウトを作成する
	/// </summary>
	/// <returns>作成できたかどうか</returns>
	bool CreateInputLayout();

	/// <summary>
	/// 指定したインプットレイアウトを作成する
	/// </summary>
	/// <param name="inputLayoutDesc">作成するインプットレイアウトの設定一覧</param>
	/// <param name="inputLayoutCount">作成するインプットレイアウトの設定数</param>
	/// <param name="byteCord">作成するインプットレイアウトを使用するシェーダーのバイトコード</param>
	/// <param name="inputLayout">作成したインプットレイアウト</param>
	/// <returns>作成できたかどうか</returns>
	bool CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc, const unsigned int inputLayoutCount, ID3DBlob* byteCord, ID3D11InputLayout** inputLayout);


	/// <summary>
	/// Vector2,3,4をリソースとしてシェーダーに送る
	/// </summary>
	/// <param name="registerNum">使用するレジスタ番号</param>
	/// <param name="vecList">送る値のリスト</param>
	/// <param name="vecCount">リストの要素の数</param>
	/// <param name="elementSize">リストの要素のサイズ</param>
	/// <param name="shaderType">送るシェーダーのタイプ</param>
	void SetVectorResource(const unsigned int registerNum, const void* vecList, const unsigned int vecCount, const unsigned int elementSize, SHADER_TYPE shaderType);
};

#endif // !__DX_SHADER_MANAGER_H_
