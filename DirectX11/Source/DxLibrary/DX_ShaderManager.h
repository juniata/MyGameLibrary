#ifndef __DX_SHADER_MANAGER_H_
#define __DX_SHADER_MANAGER_H_

//	使用するシェーダー(描画パイプラインの順番に設定してある)
enum class DX_SHADER_TYPE : int{
	VERTEX_SHADER,
	HULL_SHADER,
	DOMAIN_SHADER,
	GEOMETRY_SHADER,
	PIXEL_SHADER,
	COMPUTE_SHADER
};


typedef struct DEFAULT_SHADER{

	typedef struct _2D{
		static const char* VERTEX_SHADER;
		static const char* PIXEL_SHADER;
		static const char* INSTANCE_VERTEX_SHADER;
	}_2D;

	typedef struct _OBJECT {
		static const char* VERTEX_SHADER;
		static const char* GEOMETRY_SHADER;
		static const char* PIXEL_SHADER;
	}_OBJECT;

}DEFAULT_SHADER;

typedef DEFAULT_SHADER::_2D			DEFAULT_2D_SHADER;
typedef DEFAULT_SHADER::_OBJECT		DEFAULT_OBJECT_SHADER;


//****************************************************************************************************
//
//	ShaderManager
//
//****************************************************************************************************
class DX_ShaderManager
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		インスタンスを取得する
	//
	//------------------------------------------------------------------------------
	static DX_ShaderManager* GetInstance();

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数の初期化(シェーダーの作成)
	//
	//------------------------------------------------------------------------------
	void Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーに関する変数を解放
	//
	//------------------------------------------------------------------------------
	static void Release();

	//------------------------------------------------------------------------------
	//
	//  @brief		指定したシェーダーを取得する
	//	@return		DX_Shader*
	//
	//------------------------------------------------------------------------------
	DX_Shader* GetShader(const char* pFilepath);

	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトの2D描画用InputLayoutを取得する
	//	@return		m_pInplutLayout2Dが返る
	//
	//------------------------------------------------------------------------------
	ID3D11InputLayout* GetDefaultInputLayout2D();

	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトのインスタンス2D描画用InputLayoutを取得する
	//	@return		m_pInplutLayout3Dが返る
	//
	//------------------------------------------------------------------------------
	ID3D11InputLayout* GetDefaultInputLayoutInstance2D();

	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトのインスタンス2D描画用InputLayoutを取得する
	//	@return		m_pInplutLayout3Dが返る
	//
	//------------------------------------------------------------------------------
	ID3D11InputLayout* GetDefaultInputLayoutObject() {
		return m_pInputLayoutObject
			;
	}

	//------------------------------------------------------------------------------
	//
	//  @brief		ワールド行列を送る
	//	@param[in]	worldMat			ワールド行列
	//	@param[n]	l_pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType			SHADER_TYPE
	//	@note		レジスタ番号は1
	//
	//------------------------------------------------------------------------------
	void SetWorldMat(ID3D11Buffer* pBuffer, const DirectX::XMFLOAT4X4& worldMat, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE shaderType);

	//------------------------------------------------------------------------------
	//
	//  @brief		intをシェーダーに送る
	//	@return		m_pInplutLayout3Dが返る
	//
	//------------------------------------------------------------------------------
	void SetInt(ID3D11Buffer* pBuffer, const unsigned int registerNum, const int value, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE	shaderType);
	//------------------------------------------------------------------------------
	//
	//  @brief		Vector4をシェーダーに送る
	//	@param[in]	registerNum		シェーダーのレジスタ番号
	//	@param[in]	vec4			Vector4
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType		SHADER_TYPE
	//	
	//------------------------------------------------------------------------------
	void SetVector(
		const unsigned int			registerNum,
		const DirectX::XMFLOAT4&				vec4,
		ID3D11Device*				pDevice,
		ID3D11DeviceContext*		pDeviceContext,
		DX_SHADER_TYPE	shaderType
	);

	//------------------------------------------------------------------------------
	//
	//  @brief		Vector3をシェーダーに送る
	//	@param[in]	registerNum		シェーダーのレジスタ番号
	//	@param[in]	vec4			Vector4
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType		SHADER_TYPE
	//	
	//------------------------------------------------------------------------------
	void SetVector(
		const unsigned int			registerNum,
		const DirectX::XMFLOAT3&				vec3,
		ID3D11Device*				pDevice,
		ID3D11DeviceContext*		pDeviceContext,
		DX_SHADER_TYPE	shaderType
	);

	void SetVectorResource(const unsigned int registerNum, const DirectX::XMFLOAT4* pVec3,
		const unsigned int			vecCount,
		ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE shaderType);
	//------------------------------------------------------------------------------
	//
	//  @brief		Vector2をシェーダーに送る
	//	@param[in]	registerNum		シェーダーのレジスタ番号
	//	@param[in]	vec4			Vector4
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType		SHADER_TYPE
	//	
	//------------------------------------------------------------------------------
	void SetVector(
		const unsigned int			registerNum,
		const DirectX::XMFLOAT2&				vec2,
		ID3D11Device*				pDevice,
		ID3D11DeviceContext*		pDeviceContext,
		DX_SHADER_TYPE	shaderType
	);

	//------------------------------------------------------------------------------
	//
	//  @brief		行列をシェーダーに送る
	//	@param[in]	regiserNum			シェーダーのレジスタ番号
	//	@param[in]	mat					Matrix
	//	@param[in]	l_pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType			SHADER_TYPE
	//	
	//------------------------------------------------------------------------------
	void SetMatrix(ID3D11Buffer* pBuffer, const unsigned int regiserNum, const DirectX::XMFLOAT4X4& mat, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE shaderType);

	//------------------------------------------------------------------------------
	//
	//  @brief		複数の行列をシェーダーに送る
	//	@param[in]	regiserNum			シェーダーのレジスタ番号
	//	@param[in]	pMat				行列を複数送る
	//	@param[in]	matCount			送る行列数
	//	@param[in]	l_pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType			SHADER_TYPE
	//
	//------------------------------------------------------------------------------
	void SetMatrix(ID3D11Buffer* pBuffer, const unsigned int regiserNum, const DirectX::XMFLOAT4X4*	pMat, const unsigned int matCount, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE	shaderType);

	//------------------------------------------------------------------------------
	//
	//  @brief		複数の行列をSRVとしてシェーダーに送る
	//	@param[in]	regiserNum			シェーダーのレジスタ番号
	//	@param[in]	pMat				行列を複数送る
	//	@param[in]	matCount			送る行列数
	//	@param[in]	l_pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType			SHADER_TYPE
	//
	//------------------------------------------------------------------------------
	void SetMatrixResoruce(
		const unsigned int			regiserNum,
		const DirectX::XMFLOAT4X4*				pMat,
		const unsigned int			matCount,
		ID3D11DeviceContext*		pDeviceContext,
		DX_SHADER_TYPE	shaderType
	);

	void SetMatrixResoruce(
		const unsigned int			regiserNum,
		ID3D11Buffer*			pBuffer,
		const unsigned int			matCount,
		ID3D11DeviceContext*		pDeviceContext,
		DX_SHADER_TYPE	shaderType
	);

private:
	static const size_t SHADER_NUM = 10;
	struct Shader {
		DX_Shader* pShader;
		const char* pFilepath;
	};
	Shader m_shaders[SHADER_NUM];

	ID3D11InputLayout*	m_pInputLayout2D;
	ID3D11InputLayout*	m_pInputLayoutInstance2D;
	ID3D11InputLayout*	m_pInputLayoutObject;

	//	コンピュートシェーダーが使えるかどうか
	bool m_bCanUsetoComputeShader;

	static DX_ShaderManager* m_pInstance;
	//------------------------------------------------------------------------------
	//
	//  @brief		初期化
	//
	//------------------------------------------------------------------------------
	DX_ShaderManager();

	//------------------------------------------------------------------------------
	//
	//  @brief		解放
	//
	//------------------------------------------------------------------------------
	~DX_ShaderManager();
	
	//------------------------------------------------------------------------------
	//
	//  @brief		コンピュートシェーダーが使えるかどうかをチェック
	//
	//------------------------------------------------------------------------------
	void UsedComputeShaderCheck();

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーの作成
	//	@param[in]	pFilepath	シェーダーファイルのパス
	//
	//------------------------------------------------------------------------------
	void CreateShader(const char* pFilepath);

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数にあるInputLauoutを作成する
	//
	//------------------------------------------------------------------------------
	void CreateInputLayout();

	//------------------------------------------------------------------------------
	//
	//  @brief		InputLauoutを作成する
	//	@param[in]	pDevice				DX_System::GetDevice()
	//	@param[in]	pInputLayoutDesc	作成したD3D11_INPUT_ELEMENT_DESC
	//	@param[in]	inputLayoutCount	作成したD3D11_INPUT_ELEMENT_DESCの要素数
	//	@param[in]	pByteCord			シェーダーをコンパイルしたときのバイトコード
	//	@param[in]	pInputLayout		作成したpInputLayout
	//
	//------------------------------------------------------------------------------
	void CreateInputLayout(
		ID3D11Device*				pDevice,
		D3D11_INPUT_ELEMENT_DESC*	pInputLayoutDesc,
		const unsigned int			inputLayoutCount,
		ID3DBlob*					pByteCord,
		ID3D11InputLayout**			pInputLayout
		);


};
#endif // !__DX_SHADER_MANAGER_H_
