#include	"DX_Library.h"
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//	シェーダーのファイルパスを定義
//
//-----------------------------------------------------------------------------------------
const char* DEFAULT_2D_SHADER::VERTEX_SHADER			= "Source\\ShaderFile\\VS_2DObject.hlsl";
const char* DEFAULT_2D_SHADER::PIXEL_SHADER				= "Source\\ShaderFile\\PS_2DObject.hlsl";
const char* DEFAULT_2D_SHADER::INSTANCE_VERTEX_SHADER	= "Source\\ShaderFile\\VS_Instance2D.hlsl";

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
	m_pDefaultVS2D(nullptr),
	m_pDefaultPS2D(nullptr),
	m_pDefaultInstanceVS2D(nullptr),
	m_pInputLayout2D(nullptr),
	m_pInputLayoutInstance2D(nullptr)
{}

//-----------------------------------------------------------------------------------------
//
//	解放
//
//-----------------------------------------------------------------------------------------
DX_ShaderManager::~DX_ShaderManager()
{
	DELETE_OBJ(m_pDefaultVS2D);
	DELETE_OBJ(m_pDefaultPS2D);
	DELETE_OBJ(m_pDefaultInstanceVS2D);

	SAFE_RELEASE(m_pInputLayout2D);
	SAFE_RELEASE(m_pInputLayoutInstance2D);
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
void DX_ShaderManager::Initialize(ID3D11Device* pDevice)
{
	PROFILE("DX_ShaderManager::Initialize()");

	DX_ShaderManager* pShaderManager = GetInstance();

	//	コンピュートシェーダーが使えるかをチェック
	pShaderManager->UsedComputeShaderCheck(pDevice);
		
	//	2D描画用シェーダーを作成
	pShaderManager ->m_pDefaultVS2D = pShaderManager->CreateShader(pDevice, DEFAULT_2D_SHADER::VERTEX_SHADER);
	pShaderManager ->m_pDefaultPS2D = pShaderManager->CreateShader(pDevice, DEFAULT_2D_SHADER::PIXEL_SHADER);
	pShaderManager ->m_pDefaultInstanceVS2D = pShaderManager->CreateShader(pDevice, DEFAULT_2D_SHADER::INSTANCE_VERTEX_SHADER);

	//	インプットレイアウトを作成
	pShaderManager ->CreateInputLayout(pDevice);
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

//-----------------------------------------------------------------------------------------
//
//	デフォルトの2D頂点シェーダーを取得する
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetDefaultVertexShader2D() const
{
	return m_pDefaultVS2D;
}

//-----------------------------------------------------------------------------------------
//
//	デフォルトの2Dピクセルシェーダーを取得する
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetDefaultPixelShader2D() const
{
	return m_pDefaultPS2D;
}

//-----------------------------------------------------------------------------------------
//
//	デフォルトのインスタンス2D頂点シェーダーを取得する
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetDefaultInstanceVertexShader2D() const
{
	return m_pDefaultInstanceVS2D;
}

//-----------------------------------------------------------------------------------------
//
//  デフォルトの2D描画用InputLayoutを取得する
//
//-----------------------------------------------------------------------------------------
ID3D11InputLayout* DX_ShaderManager::GetDefaultInputLayout2D() const
{
	return m_pInputLayout2D;
}

//-----------------------------------------------------------------------------------------
//
//  デフォルトのインスタンス2D描画用InputLayoutを取得する
//
//-----------------------------------------------------------------------------------------
ID3D11InputLayout* DX_ShaderManager::GetDefaultInputLayoutInstance2D() const
{
	return m_pInputLayoutInstance2D;
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
void DX_ShaderManager::UsedComputeShaderCheck(ID3D11Device* pDevice)
{
	//	LEVEL_11_0ならサポートされているので、チェックしない
	//if (DX_System::GetFeatureLevel() == D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0){
	//	return;
	//}

	D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS l_hardwareOptions;
	
	DX_Debug::GetInstance()->ThrowIfFailed(pDevice->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &l_hardwareOptions, sizeof(l_hardwareOptions)), "CheckFeatureSupport() : failed");

	//	フラグを設定
	m_bCanUsetoComputeShader = (l_hardwareOptions.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x == TRUE) ? true : false;

}
//-----------------------------------------------------------------------------------------
//
//	シェーダーを作成する
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::CreateShader(ID3D11Device* pDevice, const char* pFilepath)
{
	DX_Shader* pShader = nullptr;

	//	文字列が無ければNULLが返る
	if (strstr(pFilepath, "VS_")){
		pShader = new  DX_VertexShader();
	}
	else if (strstr(pFilepath, "PS_")){
		pShader = new  DX_PixelShader();
	}
	else if (strstr(pFilepath, "GS_")){
		pShader = new  DX_GeometryShader();
	}
	else if (strstr(pFilepath, "CS_")){
		if (m_bCanUsetoComputeShader == false){
			MessageBox(NULL, "ComputeShaderは使えません!", "error", MB_OK);
			throw;
		}
		pShader  = new  DX_ComputeShader();
	}
	else {
		MessageBox(NULL, "指定してシェーダーは対応していません。", "error", MB_OK);
		throw;
	}

	pShader->CreateShader(pDevice, pFilepath);

	return pShader;
}

//-----------------------------------------------------------------------------------------
//
//  InputLauoutを作成する
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::CreateInputLayout(ID3D11Device* pDevice)
{
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
	
	//	2D用レイアウトを作成
	m_pInputLayout2D = CreateInputLayout(pDevice, layout2D, _countof(layout2D), m_pDefaultVS2D->GetByteCord());

	// 2Dインスタンスレイアウトを作成
	m_pInputLayoutInstance2D = CreateInputLayout(pDevice, layout2DInstance, _countof(layout2DInstance), m_pDefaultInstanceVS2D->GetByteCord());

}

//-----------------------------------------------------------------------------------------
//
//  InputLayoutを作成する
//
//-----------------------------------------------------------------------------------------
ID3D11InputLayout* DX_ShaderManager::CreateInputLayout(ID3D11Device* pDevice, D3D11_INPUT_ELEMENT_DESC*	pInputLayoutDesc, const unsigned int inputLayoutCount, ID3DBlob* pByteCord) const
{
	ID3D11InputLayout* pInputLayout = nullptr;
	DX_Debug::GetInstance()->ThrowIfFailed(pDevice->CreateInputLayout(pInputLayoutDesc, inputLayoutCount, pByteCord->GetBufferPointer(), pByteCord->GetBufferSize(), &pInputLayout), " InputLayout failed()");

	return pInputLayout;
}

