#include	"DX_Library.h"
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//	マクロ定義
//
//-----------------------------------------------------------------------------------------
//	3D描画用
#define DEFAULT_VERTEX_SHADER_3D		"Source\\ShaderFile\\VS_3DObject.hlsl"
#define DEFAULT_GEOMETRY_SHADER_3D		"Source\\ShaderFile\\GS_3DObject.hlsl"
#define DEFAULT_PIXEL_SHADER_3D			"Source\\ShaderFile\\PS_3DObject.hlsl"
#define DEFAULT_VERTEX_SHADER_SKIN_MESH	"Source\\ShaderFile\\VS_SkinMesh.hlsl"
#define DEFAULT_GEOMETRY_SHADER_RAYPICK	"Source\\ShaderFile\\GS_RayPick.hlsl"

//	インスタンス描画時の計算用
#define COMPUTE_SHADER_INSTANCE_3D			"Source\\ShaderFile\\CS_InstanceMesh.hlsl"
#define VARTEX_SHADER_INSTANCE_3D			"Source\\ShaderFile\\VS_InstanceMesh.hlsl"
#define COMPUTE_SHADER_INSTANCE_SKIN_MESH	"Source\\ShaderFile\\CS_"
#define COMPUTE_SHADER_INSTANCE_PARTICLE	"Source\\ShaderFile\\CS_"

//-----------------------------------------------------------------------------------------
//
//	staticメンバ変数
//
//-----------------------------------------------------------------------------------------
DX_ShaderManager* DX_ShaderManager::m_pInstance = nullptr;

//-----------------------------------------------------------------------------------------
//
//	初期化
//
//-----------------------------------------------------------------------------------------
DX_ShaderManager::DX_ShaderManager() : 
	m_bCanUsetoComputeShader(false),
	m_pInputLayout2D(nullptr),
	m_pInputLayout3D(nullptr),
	m_pInputLayoutInstanceMesh(nullptr),
	m_pInputLayoutSkinMesh(nullptr),
	m_pInputLayoutInstance2D(nullptr),
	m_shaderIndex(0)
{
	ZeroMemory(m_shaders, SHADER_NUM);
}

//-----------------------------------------------------------------------------------------
//
//	解放
//
//-----------------------------------------------------------------------------------------
DX_ShaderManager::~DX_ShaderManager()
{
	SAFE_RELEASE(m_pInputLayout2D);
	SAFE_RELEASE(m_pInputLayout3D);
	SAFE_RELEASE(m_pInputLayoutInstanceMesh);
	SAFE_RELEASE(m_pInputLayoutSkinMesh);
	SAFE_RELEASE(m_pInputLayoutInstance2D);

	for (int i = 0; i < SHADER_NUM; ++i) {
		DELETE_OBJ(m_shaders[i].pShader);
		m_shaders[i].path = nullptr;
	}
}


//-----------------------------------------------------------------------------------------
//
//	インスタンスを取得する
//
//-----------------------------------------------------------------------------------------
DX_ShaderManager* DX_ShaderManager::GetInstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance = new DX_ShaderManager();
	}
	return m_pInstance;
}

//-----------------------------------------------------------------------------------------
//
//	メンバ変数の初期化(シェーダーの作成)
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::Initialize()
{
	PROFILE("DX_ShaderManager::Initialize()");

	try{
		//	コンピュートシェーダーが使えるかをチェック
		UsedComputeShaderCheck();
		
		//	2D描画用シェーダーを作成
		CreateShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
		CreateShader(DEFAULT_2D_SHADER::PIXEL_SHADER);
		CreateShader(DEFAULT_2D_SHADER::INSTANCE_VERTEX_SHADER);

		////	3D描画用シェーダーを作成
		//CreateShader(DEFAULT_VERTEX_SHADER_3D);
		//CreateShader(DEFAULT_GEOMETRY_SHADER_3D);
		//CreateShader(DEFAULT_PIXEL_SHADER_3D);
		//CreateShader(DEFAULT_VERTEX_SHADER_SKIN_MESH);
		//CreateShader(DEFAULT_GEOMETRY_SHADER_RAYPICK);

		////	3Dインスタンス描画計算用シェーダーを作成
		//CreateShader(COMPUTE_SHADER_INSTANCE_3D);
		//CreateShader(VARTEX_SHADER_INSTANCE_3D);

		//	インプットレイアウトを作成
		CreateInputLayout();
	}
	catch (char* pMessage){
		throw pMessage;
	}

	//D3D11_SO_DECLARATION_ENTRY l_declaration[] = {
	//	{ 0, "OUT_POS", 0, 0, 4 ,0 },
	//	{ 0, "OUT_NORMAL", 0, 0, 3, 0 }

	//};

	//unsigned int strides[] = { sizeof(DirectX::XMFLOAT4) + sizeof(DirectX::XMFLOAT3) };
	//static_cast<DX_GeometryShader*>(GetShader(DEFAULT_GEOMETRY_SHADER_RAYPICK))->CreateGeometryShaderWithStreamOutput(
	//	l_declaration,
	//	_countof(l_declaration),
	//	strides,
	//	_countof(strides)
	//	);
}

//-----------------------------------------------------------------------------------------
//
//	シェーダーに関する変数を解放
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::Release()
{
	DELETE_OBJ(m_pInstance);
}

DX_Shader*	DX_ShaderManager::GetShader(const char* pShaderFileName)
{
	DX_Shader* pShader = nullptr;
	for (int i = 0; i < SHADER_NUM; ++i)
	{
		// 先頭から順番にデータを格納しているため、nullが見つかった場合最後尾となるためそれ以上の検索は無駄となる
		if (m_shaders[i].path == nullptr) {
			break;
		}
		if (strcmp(m_shaders[i].path, pShaderFileName) == 0)
		{
			pShader = m_shaders[i].pShader;
		}
	}

	return pShader;
}

//-----------------------------------------------------------------------------------------
//
//  デフォルトの3D描画用頂点シェーダーを取得
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetDefaultVertexShader3D()
{
	return GetShader(DEFAULT_VERTEX_SHADER_3D);
}


//-----------------------------------------------------------------------------------------
//
//  デフォルトのスキンメッシュ描画用頂点シェーダーを取得
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetDefaultVertexShaderSkinMesh()
{
	return GetShader(DEFAULT_VERTEX_SHADER_SKIN_MESH);
}


//-----------------------------------------------------------------------------------------
//
//  デフォルトのレイピック処理の入った3D描画用ジオメトリシェーダーを取得
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetDefaultGeometryShader3D()
{
	return GetShader(DEFAULT_GEOMETRY_SHADER_3D);
}


//-----------------------------------------------------------------------------------------
//
//  デフォルトの３D描画用ジオメトリシェーダーを取得
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetDefaultGeometryShaderRayPick()
{
	return GetShader(DEFAULT_GEOMETRY_SHADER_RAYPICK);

}

//-----------------------------------------------------------------------------------------
//
//  デフォルトの3D描画用ピクセルシェーダーを取得
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetDefaultPixelShader3D()
{
	return GetShader(DEFAULT_PIXEL_SHADER_3D);
}

//-----------------------------------------------------------------------------------------
//
//  インスタンスメッシュ用コンピュートシェーダーを取得
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetInstanceMeshComputeShader()
{
	return GetShader(COMPUTE_SHADER_INSTANCE_3D);
}

//-----------------------------------------------------------------------------------------
//
//  デフォルトの2D描画用InputLayoutを取得する
//
//-----------------------------------------------------------------------------------------
ID3D11InputLayout* DX_ShaderManager::GetDefaultInputLayout2D()
{
	return m_pInputLayout2D;
}

//-----------------------------------------------------------------------------------------
//
//  デフォルトの3D描画用InputLayoutを取得する
//
//-----------------------------------------------------------------------------------------
ID3D11InputLayout* DX_ShaderManager::GetDefaultInputLayout3D()
{
	return m_pInputLayout3D;
}

ID3D11InputLayout* DX_ShaderManager::GetDefaultInputLayoutInstance2D()
{
	return m_pInputLayoutInstance2D;
}
//-----------------------------------------------------------------------------------------
//
//  デフォルトのスキンメッシュ描画用InputLayoutを取得する
//
//-----------------------------------------------------------------------------------------
ID3D11InputLayout* DX_ShaderManager::GetDefaultInputLayoutSkinMesh()
{
	return m_pInputLayoutSkinMesh;
}

//-----------------------------------------------------------------------------------------
//
//  インスタンスメッシュ描画用InputLayoutを取得する
//
//-----------------------------------------------------------------------------------------
ID3D11InputLayout* DX_ShaderManager::GetInputLayoutInstanceMesh()
{
	return m_pInputLayoutInstanceMesh;
}
//-----------------------------------------------------------------------------------------
//
//  ワールド行列をシェーダーに送る
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetWorldMat(
	const DirectX::XMFLOAT4X4&				worldMat,
	ID3D11DeviceContext*		pDeviceContext,
	DX_SHADER_TYPE	shaderType
	)
{
	SetMatrix(1, worldMat, pDeviceContext, shaderType);
}

void DX_ShaderManager::SetInt(const unsigned int			registerNum,
	const DirectX::XMINT4&				vec4,
	ID3D11Device*				pDevice,
	ID3D11DeviceContext*		pDeviceContext,
	DX_SHADER_TYPE	shaderType)
{
	//	ローカル変数
	ID3D11Buffer*	l_pBuffer = DX_Buffer::CreateConstantBuffer(pDevice, sizeof(XMINT4));

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_pBuffer, 0, nullptr, &vec4, 0, 0);

	//	シェーダーステージを指定し、定数バッファを送る
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, registerNum, 1, &l_pBuffer, shaderType);

	SAFE_RELEASE(l_pBuffer);
}
//-----------------------------------------------------------------------------------------
//
//  Vector4をシェーダーに送る
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetVector(
	const unsigned int				regiserNum,
	const DirectX::XMFLOAT4&					vec4,
	ID3D11Device*					pDevice,
	ID3D11DeviceContext*			pDeviceContext,
	DX_SHADER_TYPE		shaderType
	)
{
	//	ローカル変数
	ID3D11Buffer*	l_pBuffer = DX_Buffer::CreateConstantBuffer(pDevice, sizeof(XMFLOAT4));

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_pBuffer, 0, nullptr, &vec4, 0, 0);

	//	シェーダーステージを指定し、定数バッファを送る
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &l_pBuffer, shaderType);

	SAFE_RELEASE(l_pBuffer);
}

//-----------------------------------------------------------------------------------------
//
//  Vector3をシェーダーに送る
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetVector(
	const unsigned int				regiserNum,
	const DirectX::XMFLOAT3&					vec3,
	ID3D11Device*					pDevice,
	ID3D11DeviceContext*			pDeviceContext,
	DX_SHADER_TYPE		shaderType
	)
{
	//	ローカル変数
	ID3D11Buffer*	l_pBuffer = DX_Buffer::CreateConstantBuffer(pDevice, sizeof(DirectX::XMFLOAT4));

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_pBuffer, 0, nullptr, &DirectX::XMFLOAT4(vec3.x,vec3.y,vec3.z,0.0f), 0, 0);

	//	シェーダーステージを指定し、定数バッファを送る
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &l_pBuffer, shaderType);

	SAFE_RELEASE(l_pBuffer);
}

void DX_ShaderManager::SetVectorResource(const unsigned int registerNum, const DirectX::XMFLOAT4* pVec3, const unsigned int vecCount, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE shaderType)
{
	//	ローカル変数
	ID3D11Buffer*	l_pBuffer = DX_Buffer::CPUWriteBuffer(pDevice, sizeof(XMFLOAT4) * vecCount);
	ID3D11ShaderResourceView* l_pSrv = nullptr;

	D3D11_MAPPED_SUBRESOURCE	l_subResource = { NULL };
	D3D11_SHADER_RESOURCE_VIEW_DESC l_srvDesc;
	ZeroMemory(&l_srvDesc, sizeof(l_srvDesc));


	//	SRVDescを設定
	l_srvDesc.Buffer.ElementWidth = vecCount;		//	view内の要素数
	l_srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	l_srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

	//	SRVを作成
	if (FAILED(pDevice->CreateShaderResourceView(l_pBuffer, &l_srvDesc, &l_pSrv))) {
		MessageBox(NULL, "l_pSRV", "error", MB_OK);
		exit(1);
	}

	//	map
	pDeviceContext->Map(l_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &l_subResource);

	//	データを書き込む
	CopyMemory(l_subResource.pData, pVec3, sizeof(XMFLOAT4) * vecCount);

	//	unmap
	pDeviceContext->Unmap(l_pBuffer, 0);

	DX_ResourceManager::SetShaderResources(pDeviceContext, registerNum, 1, &l_pSrv, shaderType);

	SAFE_RELEASE(l_pBuffer);
	SAFE_RELEASE(l_pSrv);
}

//-----------------------------------------------------------------------------------------
//
//  Vector3をシェーダーに送る
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetVector(
	const unsigned int				regiserNum,
	const XMFLOAT2&					vec2,
	ID3D11Device*					pDevice,
	ID3D11DeviceContext*			pDeviceContext,
	DX_SHADER_TYPE		shaderType
	)
{
	//	ローカル変数
	ID3D11Buffer*	l_pBuffer = DX_Buffer::CreateConstantBuffer(pDevice, sizeof(XMFLOAT4));

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_pBuffer, 0, nullptr, &XMFLOAT4(vec2.x,vec2.y,0.0f,0.0f), 0, 0);

	//	シェーダーステージを指定し、定数バッファを送る
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &l_pBuffer, shaderType);

	SAFE_RELEASE(l_pBuffer);
}


//-----------------------------------------------------------------------------------------
//
//	行列をシェーダーに送る
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetMatrix(
	const unsigned int			regiserNum,
	const XMFLOAT4X4&				mat,
	ID3D11DeviceContext*		pDeviceContext,
	DX_SHADER_TYPE	shaderType
	)
{
	//	ローカル変数
	ID3D11Buffer* l_pBuffer = DX_Buffer::CreateConstantBuffer(DX_System::GetInstance()->GetDevice(), sizeof(XMFLOAT4X4));

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_pBuffer, 0, nullptr, &mat, 0, 0);

	//	シェーダーステージを指定し、定数バッファを送る
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &l_pBuffer, shaderType);

	SAFE_RELEASE(l_pBuffer);
}


//-----------------------------------------------------------------------------------------
//
//	行列を複数シェーダーに送る
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetMatrix(
	const unsigned int			regiserNum,
	const XMFLOAT4X4*				pMat,
	const unsigned int			matCount,
	ID3D11DeviceContext*		pDeviceContext,
	DX_SHADER_TYPE shaderType
	)
{
	//	ローカル変数
	ID3D11Buffer* l_pBuffer = DX_Buffer::CreateConstantBuffer(DX_System::GetInstance()->GetDevice(), sizeof(XMFLOAT4X4)* matCount);

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_pBuffer, 0, nullptr, pMat, 0, 0);

	//	シェーダーステージを指定し、定数バッファを送る
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &l_pBuffer, shaderType);

	SAFE_RELEASE(l_pBuffer);

}
//-----------------------------------------------------------------------------------------
//
//	複数の行列をSRVとしてシェーダーに送る
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetMatrixResoruce(
	const unsigned int			regiserNum,
	const XMFLOAT4X4*				pMat,
	const unsigned int			matCount,
	ID3D11DeviceContext*		pDeviceContext,
	DX_SHADER_TYPE	shaderType
	)
{
	//	ローカル変数
	ID3D11Device*	l_pDevice = DX_System::GetInstance()->GetDevice();
	ID3D11Buffer*	l_pBuffer = DX_Buffer::CPUWriteBuffer(l_pDevice,sizeof(XMFLOAT4X4)* matCount);
	ID3D11ShaderResourceView* l_pSrv = nullptr;

	D3D11_MAPPED_SUBRESOURCE	l_subResource = { NULL };
	D3D11_SHADER_RESOURCE_VIEW_DESC l_srvDesc;
	ZeroMemory(&l_srvDesc, sizeof(l_srvDesc));


	//	SRVDescを設定
	l_srvDesc.Buffer.ElementWidth   = matCount * 4;		//	view内の要素数
	l_srvDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	l_srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFER;

	//	SRVを作成
	if (FAILED(l_pDevice->CreateShaderResourceView(l_pBuffer, &l_srvDesc, &l_pSrv))){
		MessageBox(NULL, "l_pSRV", "error", MB_OK);
		exit(1);
	}

	//	map
	pDeviceContext->Map(l_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &l_subResource);

	//	データを書き込む
	CopyMemory(l_subResource.pData, pMat, sizeof(XMFLOAT4X4) * matCount);

	//	unmap
	pDeviceContext->Unmap(l_pBuffer, 0);

	DX_ResourceManager::SetShaderResources(pDeviceContext, regiserNum, 1, &l_pSrv, shaderType);

	SAFE_RELEASE(l_pBuffer);
	SAFE_RELEASE(l_pSrv);
}

//-----------------------------------------------------------------------------------------
//
//	をSRVとしてシェーダーに送る
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetMatrixResoruce(
	const unsigned int			regiserNum,
	ID3D11Buffer*				pBuffer,
	const unsigned int			matCount,
	ID3D11DeviceContext*		pDeviceContext,
	DX_SHADER_TYPE	shaderType
	)
{
	//	ローカル変数
	ID3D11Device* l_pDevice = DX_System::GetInstance()->GetDevice();
	ID3D11ShaderResourceView*	l_pSrv = nullptr;
	D3D11_SHADER_RESOURCE_VIEW_DESC l_srvDesc;
	ZeroMemory(&l_srvDesc, sizeof(l_srvDesc));


	//	SRVDescを設定
	l_srvDesc.Buffer.ElementWidth	= matCount * 4;		//	view内の要素数
	l_srvDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	l_srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFER;

	//	SRVを作成
	if (FAILED(l_pDevice->CreateShaderResourceView(pBuffer, &l_srvDesc, &l_pSrv))){
		MessageBox(NULL, "l_pSRV", "error", MB_OK);
		exit(1);
	}

	DX_ResourceManager::SetShaderResources(pDeviceContext, regiserNum, 1, &l_pSrv, shaderType);

	SAFE_RELEASE(l_pSrv);
}
//-----------------------------------------------------------------------------------------
//
//	コンピュートシェーダーが使えるかどうかをチェック
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::UsedComputeShaderCheck()
{
	//	LEVEL_11_0ならサポートされているので、チェックしない
	//if (DX_System::GetFeatureLevel() == D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0){
	//	return;
	//}

	D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS l_hardwareOptions;


	DX_System::GetInstance()->GetDevice()->CheckFeatureSupport(
		D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS,
		&l_hardwareOptions,
		sizeof(l_hardwareOptions));

	//	フラグを設定
	m_bCanUsetoComputeShader = (l_hardwareOptions.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x == TRUE) ? true : false;

}
//-----------------------------------------------------------------------------------------
//
//	シェーダーを作成する
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::CreateShader(const char* pFilepath)
{
	//	文字列が無ければNULLが返る
	if (strstr(pFilepath, "VS_")){
		m_shaders[m_shaderIndex].pShader = new  DX_VertexShader();
	}
	else if (strstr(pFilepath, "PS_")){
		m_shaders[m_shaderIndex].pShader = new  DX_PixelShader();
	}
	else if (strstr(pFilepath, "GS_")){
		m_shaders[m_shaderIndex].pShader = new  DX_GeometryShader();
	}
	else if (strstr(pFilepath, "CS_")){
		if (m_bCanUsetoComputeShader == false){
			throw "ComputeShaderは使えません!";
		}
		m_shaders[m_shaderIndex].pShader = new  DX_ComputeShader();
	}

	m_shaders[m_shaderIndex].path = pFilepath;
	m_shaders[m_shaderIndex].pShader->CreateShader(pFilepath);
	++m_shaderIndex;
}

//-----------------------------------------------------------------------------------------
//
//  InputLauoutを作成する
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::CreateInputLayout()
{
	//	デバイスを取得
	ID3D11Device* pDevice = DX_System::GetInstance()->GetDevice();

	// 2D用レイアウト
	D3D11_INPUT_ELEMENT_DESC layout2D[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// 2D用インスタンスレイアウト
	D3D11_INPUT_ELEMENT_DESC layout2DInstance[] = {
		{ "POSITION",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",		0,	DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCE_POS",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	1, 0,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },

	};

	////	3D用レイアウト
	//D3D11_INPUT_ELEMENT_DESC l_layout3D[] = {
	//	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//};
	//
	////	3Dスキンメッシュ用レイアウト
	//D3D11_INPUT_ELEMENT_DESC l_layoutSkinMesh[] = {
	//	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },

	//	{ "SKIN_INDEX", 0, DXGI_FORMAT_R32_UINT,			1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "SKIN_WEIGHT",0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//};


	////	インスタンスメッシュ描画用レイアウトを作成
	//D3D11_INPUT_ELEMENT_DESC l_insntanceMesh[] = {
	//	{ "POSITION",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD",		0,	DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "NORMAL",			0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "COLOR",			0,	DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },

	//	{ "WORLD_MAT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	//	{ "WORLD_MAT", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	//	{ "WORLD_MAT", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	//	{ "WORLD_MAT", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	//};

	
	try {
		//	2D用レイアウトを作成
		CreateInputLayout(pDevice, layout2D, _countof(layout2D), GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER)->GetByteCord(), &m_pInputLayout2D);

		// 2Dインスタンスレイアウトを作成
		CreateInputLayout(pDevice, layout2DInstance, _countof(layout2DInstance), GetShader(DEFAULT_2D_SHADER::INSTANCE_VERTEX_SHADER)->GetByteCord(), &m_pInputLayoutInstance2D);

		////	3D用レイアウトを作成
		//CreateInputLayout(pDevice, l_layout3D, _countof(l_layout3D), GetShader(DEFAULT_VERTEX_SHADER_3D)->GetByteCord(), &m_pInputLayout3D);

		////	スキンメッシュ用レイアウトを作成
		//CreateInputLayout(pDevice, l_layoutSkinMesh, _countof(l_layoutSkinMesh), GetShader(DEFAULT_VERTEX_SHADER_SKIN_MESH)->GetByteCord(), &m_pInputLayoutInstanceMesh);

		////	インスタンススキンメッシュ描画用レイアウトを作成
		//CreateInputLayout(pDevice, l_insntanceMesh, _countof(l_insntanceMesh), GetShader(VARTEX_SHADER_INSTANCE_3D)->GetByteCord(), &m_pInputLayoutSkinMesh);
	}
	catch (char* pMessage){
		throw pMessage;
	}
}

//-----------------------------------------------------------------------------------------
//
//  InputLayoutを作成する
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::CreateInputLayout(
	ID3D11Device*				pDevice,
	D3D11_INPUT_ELEMENT_DESC*	pInputLayoutDesc,
	const unsigned int			inputLayoutCount,
	ID3DBlob*					pByteCord,
	ID3D11InputLayout**			pInputLayout
	)
{
	HRESULT l_hr = pDevice->CreateInputLayout(pInputLayoutDesc, inputLayoutCount, pByteCord->GetBufferPointer(), pByteCord->GetBufferSize(), pInputLayout);

	//	戻り値チェック
	if (!DX_Debug::GetInstance()->IsHresultCheck(l_hr)){
		throw "create InputLayout failed()";
	}

}

