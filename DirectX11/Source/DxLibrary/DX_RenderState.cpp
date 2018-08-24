#include	"DX_Library.h"

//-----------------------------------------------------------------------------------------
//
//  staticメンバ変数
//
//-----------------------------------------------------------------------------------------
DX_RenderState* DX_RenderState::m_pInstance = nullptr;


//-----------------------------------------------------------------------------------------
//
//  初期化
//
//-----------------------------------------------------------------------------------------
DX_RenderState::DX_RenderState() :
	m_pRasterizerState(nullptr),
	m_pBlendState(nullptr),
	m_pDepthStencilState(nullptr),
	m_pSamplerState(nullptr)

#if defined(DEBUG) || defined(_DEBUG)
	,m_pWireFrameRS(nullptr),
	m_bUseSolidRS(true)
#endif
{}

//-----------------------------------------------------------------------------------------
//
//  解放
//
//-----------------------------------------------------------------------------------------
DX_RenderState::~DX_RenderState()
{
	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pBlendState);
	SAFE_RELEASE(m_pDepthStencilState);
	SAFE_RELEASE(m_pSamplerState);

#if defined(DEBUG) || defined(_DEBUG)
	SAFE_RELEASE(m_pWireFrameRS);
#endif

}

//-----------------------------------------------------------------------------------------
//
//  インスタンスを取得する
//
//-----------------------------------------------------------------------------------------
DX_RenderState* DX_RenderState::GetInstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance = new DX_RenderState();
	}
	
	return m_pInstance;
}

//-----------------------------------------------------------------------------------------
//
//  インスタンスを解放する
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::Release()
{
	DELETE_OBJ(m_pInstance);
}

//-----------------------------------------------------------------------------------------
//
//  初期描画の設定を行う
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::Initialize()
{
	PROFILE("DX_RenderState::Initialize()");

	//	デバイスを取得
	ID3D11Device*	l_pDevice = DX_System::GetInstance()->GetDevice();

	//	デバイスコンテキストを取得
	ID3D11DeviceContext* l_pDeviceContext = DX_System::GetInstance()->GetDeviceContext();

	try{
		//	ラスタライザーステートを作成
		CreateRasterizerState(l_pDevice);

		//	ブレンドステートを作成
		CreateBlendState(l_pDevice);

		//	深度･ステンシルステートを作成
		CreateDepthStencilState(l_pDevice);
		
		//	サンプラーステートを作成
		CreateSamplerState(l_pDevice);
	}
	catch (char* pMessage){
		throw pMessage;
	}
}

//-----------------------------------------------------------------------------------------
//
//  ラスタライザステートを取得する
//
//-----------------------------------------------------------------------------------------
ID3D11RasterizerState* DX_RenderState::GetDefaultRasterizerState() const
{
	return m_pRasterizerState;
}

//-----------------------------------------------------------------------------------------
//
//  ブレンドステートを取得する
//
//-----------------------------------------------------------------------------------------
ID3D11BlendState* DX_RenderState::GetDefaultBlendState() const
{
	return m_pBlendState;
}

//-----------------------------------------------------------------------------------------
//
//  深度/ステンシルステートを取得する
//
//-----------------------------------------------------------------------------------------
ID3D11DepthStencilState* DX_RenderState::GetDefaultDepthStencilState() const
{
	return m_pDepthStencilState;
}

//-----------------------------------------------------------------------------------------
//
//  サンプラステートを取得する
//
//-----------------------------------------------------------------------------------------
ID3D11SamplerState* DX_RenderState::GetDefaultSamplerState() const
{
	return m_pSamplerState;
}

#if defined(DEBUG) || defined(_DEBUG)
//------------------------------------------------------------------------------
//
//  ラスタライザーステートを取得する(SwitchSolidRS() SwitchWireframeRS()でラスタライザが切り替わる)
//
//------------------------------------------------------------------------------
ID3D11RasterizerState* DX_RenderState::GetSwitchRasterizer() const
{
	return m_bUseSolidRS ? m_pRasterizerState : m_pWireFrameRS;
}

//------------------------------------------------------------------------------
//
//  ラスタライザをソリッド描画に切り替える
//
//------------------------------------------------------------------------------
void DX_RenderState::SwitchSolidRS()
{
	m_bUseSolidRS = true;
}

//------------------------------------------------------------------------------
//
//  ラスタライザをワイヤーフレーム描画に切り替える
//
//------------------------------------------------------------------------------
void DX_RenderState::SwitchWireframeRS()
{
	m_bUseSolidRS = false;
}
#endif

//-----------------------------------------------------------------------------------------
//
//  ラスタライザーステートを作成する
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::CreateRasterizerState(ID3D11Device*	pDevice)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.FillMode				= D3D11_FILL_MODE::D3D11_FILL_SOLID;	//	普通に描画
	desc.CullMode				= D3D11_CULL_MODE::D3D11_CULL_BACK;		//	裏面非描画
	desc.FrontCounterClockwise	= FALSE;	//	時計周りが表面
	desc.DepthBias				= 0;		//	深度バイアス｢0｣
	desc.DepthBiasClamp			= 0;
	desc.SlopeScaledDepthBias	= 0;
	desc.DepthClipEnable		= TRUE;		//	深度クリッピング有り
	desc.ScissorEnable			= FALSE;	//	シザー矩形無し
	desc.MultisampleEnable		= FALSE;	//	マルチサンプリング無し
	desc.AntialiasedLineEnable	= FALSE;	//	ライン･アンチエイリアシング無し

	//	rasterizer stateを作成する
	if (!DX_Debug::GetInstance()->IsHresultCheck(pDevice->CreateRasterizerState(&desc, &m_pRasterizerState))){
		throw "ID3D11Device::CreateRasterizerState() : failed";
	}

#if defined(DEBUG) || defined(_DEBUG)
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(wireframeDesc));

	wireframeDesc.FillMode				= D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;	//	普通に描画
	wireframeDesc.CullMode				= D3D11_CULL_MODE::D3D11_CULL_BACK;		//	裏面非描画
	wireframeDesc.FrontCounterClockwise = FALSE;	//	時計周りが表面
	wireframeDesc.DepthBias				= 0;		//	深度バイアス｢0｣
	wireframeDesc.DepthBiasClamp		= 0;
	wireframeDesc.SlopeScaledDepthBias	= 0;
	wireframeDesc.DepthClipEnable		= TRUE;		//	深度クリッピング有り
	wireframeDesc.ScissorEnable			= FALSE;	//	シザー矩形無し
	wireframeDesc.MultisampleEnable		= FALSE;	//	マルチサンプリング無し
	wireframeDesc.AntialiasedLineEnable = FALSE;	//	ライン･アンチエイリアシング無し
	
	//	rasterizer stateを作成する
	if (!DX_Debug::GetInstance()->IsHresultCheck(pDevice->CreateRasterizerState(&wireframeDesc, &m_pWireFrameRS))) {
		throw "ID3D11Device::CreateRasterizerState() : failed";
	}

#endif
}



//-----------------------------------------------------------------------------------------
//
//  ブレンドステートを作成する
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::CreateBlendState(ID3D11Device* pDevice)
{
	D3D11_BLEND_DESC l_blendDesc;
	ZeroMemory(&l_blendDesc, sizeof(l_blendDesc));

	l_blendDesc.AlphaToCoverageEnable		= TRUE;
	l_blendDesc.IndependentBlendEnable		= FALSE; // RenderTarget0のみblende設定を使用する
	l_blendDesc.RenderTarget[0].BlendEnable = FALSE;
	l_blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 全部の色を書き込む
	
	////	どうブレンドするか
	//l_blendDesc.RenderTarget[0].BlendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	//l_blendDesc.RenderTarget[0].BlendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

	////	Blendの対象はなんなのか
	//l_blendDesc.RenderTarget[0].SrcBlend	= D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	//l_blendDesc.RenderTarget[0].DestBlend	= D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;

	////	Blend係数はいくらか
	//l_blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND::D3D11_BLEND_ONE;
	//l_blendDesc.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND::D3D11_BLEND_ZERO;
	//l_blendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;


	//	blendStateを作成する
	if (!DX_Debug::GetInstance()->IsHresultCheck(pDevice->CreateBlendState(&l_blendDesc, &m_pBlendState))){
		throw "ID3D11Device::CreateBlendState() : failed";
	}
}


//-----------------------------------------------------------------------------------------
//
//  深度･ステンシルステートを作成する
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::CreateDepthStencilState(ID3D11Device* pDevice)
{
	D3D11_DEPTH_STENCIL_DESC l_dsDesc;
	ZeroMemory(&l_dsDesc, sizeof(l_dsDesc));

	//	depth test parameters
	l_dsDesc.DepthEnable	= TRUE;
	l_dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	l_dsDesc.DepthFunc		= D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL; // z値が手前で描画されたものより小さい場合、描画します

	// Stencil test parameters
	l_dsDesc.StencilEnable = FALSE;
	
	//	depth stencil stateを作成する
	if (!DX_Debug::GetInstance()->IsHresultCheck(pDevice->CreateDepthStencilState(&l_dsDesc, &m_pDepthStencilState))){
		throw "ID3D11Device::CreateDepthStencilState() : failed";
	}
}


//-----------------------------------------------------------------------------------------
//
//  サンプラーステートを作成する
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::CreateSamplerState(ID3D11Device* pDevice)
{
	D3D11_SAMPLER_DESC l_samplerDesc;
	ZeroMemory(&l_samplerDesc, sizeof(l_samplerDesc));

	l_samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	l_samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	l_samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	l_samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	l_samplerDesc.MipLODBias = .0f;
	l_samplerDesc.MaxAnisotropy = 0;
	l_samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	l_samplerDesc.BorderColor[0] = .0f;
	l_samplerDesc.BorderColor[1] = .0f;
	l_samplerDesc.BorderColor[2] = .0f;
	l_samplerDesc.BorderColor[3] = .0f;
	l_samplerDesc.MinLOD = 0;
	l_samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (!DX_Debug::GetInstance()->IsHresultCheck(pDevice->CreateSamplerState(&l_samplerDesc, &m_pSamplerState))){
		throw "ID3D11Device::CreateSamplerState() : failed";
	}
}
