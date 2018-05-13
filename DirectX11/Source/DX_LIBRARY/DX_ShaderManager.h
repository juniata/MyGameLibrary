#ifndef __DX_SHADER_MANAGER_H_
#define __DX_SHADER_MANAGER_H_

//	使用するシェーダー(描画パイプラインの順番に設定してある)
enum class SHADER_TYPE : int{
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
	}_2D;

	typedef struct _MESH{
		static const char* VERTEX_SHADER;
		static const char* GEOMETRY_SHADER;
		static const char* PIXEL_SHADER;
	}_MESH;

	typedef struct _SKIN_MESH{
		static const char* VERTEX_SHADER;
		static const char* GEOMETRY_SHADER;
		static const char* PIXEL_SHADER;
	}_SKIN_MESH;

}DEFAULT_SHADER;

typedef DEFAULT_SHADER::_2D			DEFAULT_2D_SHADER;
typedef DEFAULT_SHADER::_MESH		DEFAULT_MESH_SHADER;
typedef DEFAULT_SHADER::_SKIN_MESH	DEFAULT_SKIN_MESH_SHADER;

//	2D
__declspec(selectany) const char* DEFAULT_2D_SHADER::VERTEX_SHADER	= "ShaderFile\\VS_2DObject.hlsl";
__declspec(selectany) const char* DEFAULT_2D_SHADER::PIXEL_SHADER	= "ShaderFile\\PS_2DObject.hlsl";

//	Mesh
__declspec(selectany) const char* DEFAULT_MESH_SHADER::VERTEX_SHADER	= "ShaderFile\\VS_3DObject.hlsl";
__declspec(selectany) const char* DEFAULT_MESH_SHADER::GEOMETRY_SHADER	= "ShaderFile\\GS_3DObject.hlsl";
__declspec(selectany) const char* DEFAULT_MESH_SHADER::PIXEL_SHADER		= "ShaderFile\\PS_3DObject.hlsl";

//	SkinMesh
__declspec(selectany) const char* DEFAULT_SKIN_MESH_SHADER::VERTEX_SHADER	= "ShaderFile\\VS_SkinMesh.hlsl";
__declspec(selectany) const char* DEFAULT_SKIN_MESH_SHADER::GEOMETRY_SHADER = "ShaderFile\\GS_3DObject.hlsl";
__declspec(selectany) const char* DEFAULT_SKIN_MESH_SHADER::PIXEL_SHADER	= "ShaderFile\\PS_3DObject.hlsl";


//****************************************************************************************************
//
//	ShaderManager
//
//****************************************************************************************************
class ShaderManager
{
private:
	static std::map<const char*, dx_library::Shader*>		m_shaders;

	static ID3D11InputLayout*	m_pInputLayout2D;
	static ID3D11InputLayout*	m_pInputLayout3D;
	static ID3D11InputLayout*	m_pInputLayoutSkinMesh;
	static ID3D11InputLayout*	m_pInputLayoutInstanceMesh;

	//	コンピュートシェーダーが使えるかどうか
	static bool m_bCanUsetoComputeShader;

	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	ShaderManager() = delete;
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	~ShaderManager(){}

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数の初期化(シェーダーの作成)
	//
	//------------------------------------------------------------------------------
	static void Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーに関する変数を解放
	//
	//------------------------------------------------------------------------------
	static void Release();

	static dx_library::Shader*	GetShader(const char* pShaderFileName);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトの3D描画用頂点シェーダーを取得
	//	@return		DX_VertexShaderが返る
	//
	//------------------------------------------------------------------------------
	static dx_library::Shader* GetDefaultVertexShader3D();

	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトのスキンメッシュ描画用頂点シェーダーを取得
	//	@return		DX_VertexShaderが返る
	//
	//------------------------------------------------------------------------------
	static dx_library::Shader* GetDefaultVertexShaderSkinMesh();

	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトの3D描画用ジオメトリシェーダーを取得
	//	@return		DX_GeometryShaderが返る
	//
	//------------------------------------------------------------------------------
	static dx_library::Shader*	GetDefaultGeometryShader3D();

	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトのレイピック処理の入った3D描画用ジオメトリシェーダーを取得
	//	@return		DX_GeometryShaderが返る
	//
	//------------------------------------------------------------------------------
	static dx_library::Shader*	GetDefaultGeometryShaderRayPick();

	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトの3D描画用ピクセルシェーダーを取得
	//	@return		DX_PixelShaderが返る
	//
	//------------------------------------------------------------------------------
	static dx_library::Shader* GetDefaultPixelShader3D();

	//------------------------------------------------------------------------------
	//
	//  @brief		インスタンスメッシュ用コンピュートシェーダーを取得
	//	@return		DX_ComputeShaderが返る
	//
	//------------------------------------------------------------------------------
	static dx_library::Shader* GetInstanceMeshComputeShader();

	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトの2D描画用InputLayoutを取得する
	//	@return		m_pInplutLayout2Dが返る
	//
	//------------------------------------------------------------------------------
	static ID3D11InputLayout* GetDefaultInputLayout2D();

	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトの3D描画用InputLayoutを取得する
	//	@return		m_pInplutLayout3Dが返る
	//
	//------------------------------------------------------------------------------
	static ID3D11InputLayout* GetDefaultInputLayout3D();

	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトのスキンメッシュ描画用InputLayoutを取得する
	//	@return		m_pInputLayoutSkinMeshが返る
	//
	//------------------------------------------------------------------------------
	static ID3D11InputLayout* GetDefaultInputLayoutSkinMesh();

	//------------------------------------------------------------------------------
	//
	//  @brief		インスタンスメッシュ描画用InputLayoutを取得する
	//	@return		m_pInputLayoutInstanceMeshが返る
	//
	//------------------------------------------------------------------------------
	static ID3D11InputLayout* GetInputLayoutInstanceMesh();

	//------------------------------------------------------------------------------
	//
	//  @brief		ワールド行列を送る
	//	@param[in]	worldMat			ワールド行列
	//	@param[n]	l_pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	shaderType			SHADER_TYPE
	//	@note		レジスタ番号は1
	//
	//------------------------------------------------------------------------------
	static void SetWorldMat(
		const DirectX::XMFLOAT4X4&				worldMat,
		ID3D11DeviceContext*		pDeviceContext,
		dx_library::SHADER_TYPE	shaderType
		);

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
	static void SetVector(
		const unsigned int			registerNum,
		const DirectX::XMFLOAT4&				vec4,
		ID3D11Device*				pDevice,
		ID3D11DeviceContext*		pDeviceContext,
		dx_library::SHADER_TYPE	shaderType
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
	static void SetVector(
		const unsigned int			registerNum,
		const DirectX::XMFLOAT3&				vec3,
		ID3D11Device*				pDevice,
		ID3D11DeviceContext*		pDeviceContext,
		dx_library::SHADER_TYPE	shaderType
		);

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
	static void SetVector(
		const unsigned int			registerNum,
		const DirectX::XMFLOAT2&				vec2,
		ID3D11Device*				pDevice,
		ID3D11DeviceContext*		pDeviceContext,
		dx_library::SHADER_TYPE	shaderType
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
	static void SetMatrix(
		const unsigned int			regiserNum,
		const DirectX::XMFLOAT4X4&				mat,
		ID3D11DeviceContext*		pDeviceContext,
		dx_library::SHADER_TYPE	shaderType
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
	static void SetMatrix(
		const unsigned int			regiserNum,
		const DirectX::XMFLOAT4X4*				pMat,
		const unsigned int			matCount,
		ID3D11DeviceContext*		pDeviceContext,
		dx_library::SHADER_TYPE	shaderType
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
	static void SetMatrixResoruce(
		const unsigned int			regiserNum,
		const DirectX::XMFLOAT4X4*				pMat,
		const unsigned int			matCount,
		ID3D11DeviceContext*		pDeviceContext,
		dx_library::SHADER_TYPE	shaderType
		);

	static void SetMatrixResoruce(
		const unsigned int			regiserNum,
		ID3D11Buffer*			pBuffer,
		const unsigned int			matCount,
		ID3D11DeviceContext*		pDeviceContext,
		dx_library::SHADER_TYPE	shaderType
		);


private:
	//------------------------------------------------------------------------------
	//
	//  @brief		コンピュートシェーダーが使えるかどうかをチェック
	//
	//------------------------------------------------------------------------------
	static void UsedComputeShaderCheck();

	//------------------------------------------------------------------------------
	//
	//  @brief		シェーダーの作成
	//	@param[in]	pFilepath	シェーダーファイルのパス
	//
	//------------------------------------------------------------------------------
	static void CreateShader(
		const char* pFilepath
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数にあるInputLauoutを作成する
	//
	//------------------------------------------------------------------------------
	static void CreateInputLayout();

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
	static void CreateInputLayout(
		ID3D11Device*				pDevice,
		D3D11_INPUT_ELEMENT_DESC*	pInputLayoutDesc,
		const unsigned int			inputLayoutCount,
		ID3DBlob*					pByteCord,
		ID3D11InputLayout**			pInputLayout
		);


};
#endif // !__DX_SHADER_MANAGER_H_
