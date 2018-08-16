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


// シェーダーのファイルパス定義
struct DEFAULT_SHADER{

	struct _2D {
		static const char* VERTEX_SHADER;
		static const char* PIXEL_SHADER;
		static const char* INSTANCE_VERTEX_SHADER;
	};

};

typedef DEFAULT_SHADER::_2D			DEFAULT_2D_SHADER;
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
	//	@param[in]	pDevice	DirectX11のデバイス
	//
	//------------------------------------------------------------------------------
	static void Initialize(ID3D11Device* pDevice);

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーに関する変数を解放
	//
	//------------------------------------------------------------------------------
	static void Release();

	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトのインスタンス2D頂点シェーダーを取得する
	//	@return		DX_VertexShaderのポインタ
	//
	//------------------------------------------------------------------------------
	DX_Shader* GetDefaultVertexShader2D() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトの2Dピクセルシェーダーを取得する
	//	@return		DX_PixelShaderのポインタ
	//
	//------------------------------------------------------------------------------
	DX_Shader* GetDefaultPixelShader2D() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトの2D頂点シェーダーを取得する
	//	@return		DX_VertexShaderのポインタ
	//
	//------------------------------------------------------------------------------
	DX_Shader* GetDefaultInstanceVertexShader2D() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトの2D描画用InputLayoutを取得する
	//	@return		インプットレイアウトのポインタ
	//
	//------------------------------------------------------------------------------
	ID3D11InputLayout* GetDefaultInputLayout2D() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトのインスタンス2D描画用InputLayoutを取得する
	//	@return		インプットレイアウトのポインタ
	//
	//------------------------------------------------------------------------------
	ID3D11InputLayout* GetDefaultInputLayoutInstance2D() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		ワールド行列を送る
	//	@param[in]	worldMat			ワールド行列
	//	@param[n]	l_pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType			SHADER_TYPE
	//	@note		レジスタ番号は1
	//
	//------------------------------------------------------------------------------
	void SetWorldMat(
		const DirectX::XMFLOAT4X4&				worldMat,
		ID3D11DeviceContext*		pDeviceContext,
		DX_SHADER_TYPE	shaderType
		);

	void SetInt(const unsigned int			registerNum,
		const DirectX::XMINT4&				vec4,
		ID3D11Device*				pDevice,
		ID3D11DeviceContext*		pDeviceContext,
		DX_SHADER_TYPE	shaderType);
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
	void SetMatrix(
		const unsigned int			regiserNum,
		const DirectX::XMFLOAT4X4&				mat,
		ID3D11DeviceContext*		pDeviceContext,
		DX_SHADER_TYPE	shaderType
		);

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
	void SetMatrix(
		const unsigned int			regiserNum,
		const DirectX::XMFLOAT4X4*				pMat,
		const unsigned int			matCount,
		ID3D11DeviceContext*		pDeviceContext,
		DX_SHADER_TYPE	shaderType
		);

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
	static DX_ShaderManager* m_pInstance;

	DX_Shader* m_pDefaultVS2D;
	DX_Shader* m_pDefaultPS2D;
	DX_Shader* m_pDefaultInstanceVS2D;

	ID3D11InputLayout*	m_pInputLayout2D;
	ID3D11InputLayout*	m_pInputLayoutInstance2D;

	//	コンピュートシェーダーが使えるかどうか
	bool m_bCanUsetoComputeShader;

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
	void UsedComputeShaderCheck(ID3D11Device* pDevice);

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーの作成
	//	@param[in]	pDevice		DirectX11のデバイス
	//	@param[in]	pFilepath	シェーダーファイルのパス
	//
	//------------------------------------------------------------------------------
	DX_Shader* CreateShader(ID3D11Device* pDevice, const char* pFilepath);

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数にあるInputLauoutを作成する
	//
	//------------------------------------------------------------------------------
	void CreateInputLayout(ID3D11Device* pDevice);

	//------------------------------------------------------------------------------
	//
	//  @brief		InputLauoutを作成する
	//	@param[in]	pDevice				DX_System::GetDevice()
	//	@param[in]	pInputLayoutDesc	作成したD3D11_INPUT_ELEMENT_DESC
	//	@param[in]	inputLayoutCount	作成したD3D11_INPUT_ELEMENT_DESCの要素数
	//	@param[in]	pByteCord			シェーダーをコンパイルしたときのバイトコード
	//
	//------------------------------------------------------------------------------
	ID3D11InputLayout* CreateInputLayout(ID3D11Device* pDevice, D3D11_INPUT_ELEMENT_DESC* pInputLayoutDesc, const unsigned int inputLayoutCount, ID3DBlob* pByteCord) const;


};
#endif // !__DX_SHADER_MANAGER_H_
