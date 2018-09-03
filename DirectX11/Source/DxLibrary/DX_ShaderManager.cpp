#include	"DX_Library.h"
#include<stdio.h>
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//	staticメンバ変数
//
//-----------------------------------------------------------------------------------------
DX_ShaderManager* DX_ShaderManager::m_pInstance = nullptr;

const char*		DEFAULT_2D_SHADER::VERTEX_SHADER			= "VS_2DObject.hlsl";
const char*		DEFAULT_2D_SHADER::PIXEL_SHADER				= "PS_2DObject.hlsl";
const char*		DEFAULT_2D_SHADER::INSTANCE_VERTEX_SHADER	= "VS_Instance2D.hlsl";

const char*		DEFAULT_OBJECT_SHADER::VERTEX_SHADER	= "VS_Object.hlsl";
const char*		DEFAULT_OBJECT_SHADER::GEOMETRY_SHADER	= "GS_Object.hlsl";
const char*		DEFAULT_OBJECT_SHADER::PIXEL_SHADER		= "PS_Object.hlsl";

//-----------------------------------------------------------------------------------------
//
//	初期化
//
//-----------------------------------------------------------------------------------------
DX_ShaderManager::DX_ShaderManager() : 
	m_bCanUsetoComputeShader(false),
	m_pInputLayout2D(nullptr),
	m_pInputLayoutInstance2D(nullptr),
	m_pInputLayoutObject(nullptr)
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
	for (size_t i = 0; i < SHADER_NUM; ++i)
	{
		DELETE_OBJ(m_shaders[i].pShader);
	}

	SAFE_RELEASE(m_pInputLayout2D);
	SAFE_RELEASE(m_pInputLayoutInstance2D);
	SAFE_RELEASE(m_pInputLayoutObject);
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
	try{
		//	コンピュートシェーダーが使えるかをチェック
		UsedComputeShaderCheck();
		
		//	2D描画用シェーダーを作成
		CreateShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
		CreateShader(DEFAULT_2D_SHADER::PIXEL_SHADER);
		CreateShader(DEFAULT_2D_SHADER::INSTANCE_VERTEX_SHADER);

		// 
		CreateShader(DEFAULT_OBJECT_SHADER::VERTEX_SHADER);
		//CreateShader(DEFAULT_OBJECT_SHADER::GEOMETRY_SHADER);
		CreateShader(DEFAULT_OBJECT_SHADER::PIXEL_SHADER);

		//	インプットレイアウトを作成
		CreateInputLayout();
	}
	catch (char* pMessage){
		throw pMessage;
	}
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
//  デフォルトの2D描画用InputLayoutを取得する
//
//-----------------------------------------------------------------------------------------
ID3D11InputLayout* DX_ShaderManager::GetDefaultInputLayout2D()
{
	return m_pInputLayout2D;
}

//-----------------------------------------------------------------------------------------
//
//  デフォルトのインスタンス2D描画用InputLayoutを取得する
//
//-----------------------------------------------------------------------------------------
ID3D11InputLayout* DX_ShaderManager::GetDefaultInputLayoutInstance2D()
{
	return m_pInputLayoutInstance2D;
}

//-----------------------------------------------------------------------------------------
//
//  ワールド行列をシェーダーに送る
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetWorldMat(ID3D11Buffer* pBuffer, const DirectX::XMFLOAT4X4& worldMat, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE	shaderType)
{
	SetMatrix(pBuffer, 1, worldMat, pDeviceContext, shaderType);
}


//-----------------------------------------------------------------------------------------
//
//  ワールド行列をシェーダーに送る
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetInt(ID3D11Buffer* pBuffer, const unsigned int registerNum, const int value, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE	shaderType)
{
	//	updateSubResource
	pDeviceContext->UpdateSubresource(pBuffer, 0, nullptr, &value, 0, 0);

	//	シェーダーステージを指定し、定数バッファを送る
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, registerNum, 1, &pBuffer, shaderType);
}
void DX_ShaderManager::SetInt(ID3D11Buffer* pBuffer, const unsigned int registerNum, const DirectX::XMINT2& value, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE	shaderType)
{
	//	updateSubResource
	pDeviceContext->UpdateSubresource(pBuffer, 0, nullptr, &value, 0, 0);

	//	シェーダーステージを指定し、定数バッファを送る
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, registerNum, 1, &pBuffer, shaderType);
}
void DX_ShaderManager::SetInt(ID3D11Buffer* pBuffer, const unsigned int registerNum, const DirectX::XMINT3& value, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE	shaderType)
{
	//	updateSubResource
	pDeviceContext->UpdateSubresource(pBuffer, 0, nullptr, &value, 0, 0);

	//	シェーダーステージを指定し、定数バッファを送る
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, registerNum, 1, &pBuffer, shaderType);
}
void DX_ShaderManager::SetInt(ID3D11Buffer* pBuffer, const unsigned int registerNum, const DirectX::XMINT4& value, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE	shaderType)
{
	//	updateSubResource
	pDeviceContext->UpdateSubresource(pBuffer, 0, nullptr, &value, 0, 0);

	//	シェーダーステージを指定し、定数バッファを送る
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, registerNum, 1, &pBuffer, shaderType);
}
void DX_ShaderManager::SetInt(ID3D11Buffer* pBuffer, const unsigned int registerNum, const unsigned int value, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE	shaderType)
{
	//	updateSubResource
	pDeviceContext->UpdateSubresource(pBuffer, 0, nullptr, &value, 0, 0);

	//	シェーダーステージを指定し、定数バッファを送る
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, registerNum, 1, &pBuffer, shaderType);
}
void DX_ShaderManager::SetInt(ID3D11Buffer* pBuffer, const unsigned int registerNum, const DirectX::XMUINT2& value, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE	shaderType)
{
	//	updateSubResource
	pDeviceContext->UpdateSubresource(pBuffer, 0, nullptr, &value, 0, 0);

	//	シェーダーステージを指定し、定数バッファを送る
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, registerNum, 1, &pBuffer, shaderType);
}
void DX_ShaderManager::SetInt(ID3D11Buffer* pBuffer, const unsigned int registerNum, const DirectX::XMUINT3& value, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE	shaderType)
{
	//	updateSubResource
	pDeviceContext->UpdateSubresource(pBuffer, 0, nullptr, &value, 0, 0);

	//	シェーダーステージを指定し、定数バッファを送る
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, registerNum, 1, &pBuffer, shaderType);
}
void DX_ShaderManager::SetInt(ID3D11Buffer* pBuffer, const unsigned int registerNum, const DirectX::XMUINT4& value, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE	shaderType)
{
	//	updateSubResource
	pDeviceContext->UpdateSubresource(pBuffer, 0, nullptr, &value, 0, 0);

	//	シェーダーステージを指定し、定数バッファを送る
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, registerNum, 1, &pBuffer, shaderType);
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
void DX_ShaderManager::SetMatrix(ID3D11Buffer* pBuffer, const unsigned int regiserNum, const XMFLOAT4X4& mat, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE	shaderType)
{
	//	updateSubResource
	pDeviceContext->UpdateSubresource(pBuffer, 0, nullptr, &mat, 0, 0);

	//	シェーダーステージを指定し、定数バッファを送る
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &pBuffer, shaderType);
}


//-----------------------------------------------------------------------------------------
//
//	行列を複数シェーダーに送る
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetMatrix(ID3D11Buffer* pBuffer, const unsigned int regiserNum, const XMFLOAT4X4* pMat, const unsigned int matCount, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE shaderType)
{
	//	updateSubResource
	pDeviceContext->UpdateSubresource(pBuffer, 0, nullptr, pMat, 0, 0);

	//	シェーダーステージを指定し、定数バッファを送る
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &pBuffer, shaderType);
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
	DX_Shader* pShader = nullptr;

	char path[_MAX_PATH] = { '\0' };

	sprintf_s(path, sizeof(path), "%s%s", "Source\\ShaderFile\\Default\\", pFilepath);

	//	文字列が無ければNULLが返る
	if (strstr(pFilepath, "VS_")){
		pShader = new  DX_VertexShader();
	}
	else if (strstr(pFilepath, "PS_")){
		pShader = new  DX_PixelShader();
	}
	else if (strstr(pFilepath, "GS_")){
		pShader  = new  DX_GeometryShader();
	}
	else if (strstr(pFilepath, "CS_")){
		if (m_bCanUsetoComputeShader == false){
			throw "ComputeShaderは使えません!";
		}
		pShader  = new  DX_ComputeShader();
	}
	else {
		throw "";
	}

	pShader->CreateShader(path);

	for (int i = 0; i < SHADER_NUM; ++i) {
		if (m_shaders[i].pFilepath == nullptr) {
			m_shaders[i].pShader = pShader;
			m_shaders[i].pFilepath = pFilepath;
			break;
		}
	}
}
DX_Shader* DX_ShaderManager::GetShader(const char* pFilepath)
{
	DX_Shader* pShader = nullptr;

	for (int i = 0; i < SHADER_NUM; ++i) {
		if (m_shaders[i].pFilepath == pFilepath) {
			pShader = m_shaders[i].pShader;
			break;
		}
	}

	return pShader;
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

	// BOX用レイアウト
	D3D11_INPUT_ELEMENT_DESC layoutBox[] = {
		{ "POSITION",		0,	DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",			0,	DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",			0,	DXGI_FORMAT_R32G32B32A32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	try {
		//	2D用レイアウトを作成
		CreateInputLayout(pDevice, layout2D, _countof(layout2D), GetShader(DEFAULT_2D_SHADER::VERTEX_SHADER)->GetByteCord(), &m_pInputLayout2D);

		// 2Dインスタンスレイアウトを作成
		CreateInputLayout(pDevice, layout2DInstance, _countof(layout2DInstance), GetShader(DEFAULT_2D_SHADER::INSTANCE_VERTEX_SHADER)->GetByteCord(), &m_pInputLayoutInstance2D);

		// BOX用レイアウトを作成
		CreateInputLayout(pDevice, layoutBox, _countof(layoutBox), GetShader(DEFAULT_OBJECT_SHADER::VERTEX_SHADER)->GetByteCord(), &m_pInputLayoutObject);
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

