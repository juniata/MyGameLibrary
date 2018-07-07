#include	"DX_Library.h"

//-----------------------------------------------------------------------------------------
//
//  staticメンバ変数
//
//-----------------------------------------------------------------------------------------
DX_RenderState* DX_RenderState::m_pInstance = nullptr;

DX_RenderState::DX_RenderState()
{

}

DX_RenderState::~DX_RenderState()
{

}
DX_RenderState* DX_RenderState::GetInstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance = new DX_RenderState();
	}
	
	return m_pInstance;
}

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

	//	サンプラーを設定する
	l_pDeviceContext->PSSetSamplers(0, 1, &m_samplerState);

	//	ポリゴン描画設定
	l_pDeviceContext->RSSetState(m_rasterizerState.Get());

	//	OMに必要情報を設定
	float l_blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11RenderTargetView* const targets[1] = { DX_System::GetInstance()->GetDefaultRenderTargetView() };
	l_pDeviceContext->OMSetRenderTargets(1, targets, DX_System::GetInstance()->GetDefaultDepthStencilView());
	l_pDeviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);
	l_pDeviceContext->OMSetBlendState(m_blendState.Get(), l_blendFactor, 1);
	
}


//-----------------------------------------------------------------------------------------
//
//  ラスタライザーステートを作成する
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::CreateRasterizerState(ID3D11Device*	pDevice)
{
	D3D11_RASTERIZER_DESC l_rsDesc;
	ZeroMemory(&l_rsDesc, sizeof(l_rsDesc));

	l_rsDesc.FillMode				= D3D11_FILL_MODE::D3D11_FILL_SOLID;	//	普通に描画
	l_rsDesc.CullMode				= D3D11_CULL_MODE::D3D11_CULL_BACK;		//	裏面非描画
	l_rsDesc.FrontCounterClockwise	= FALSE;	//	時計周りが表面
	l_rsDesc.DepthBias				= 0;		//	深度バイアス｢0｣
	l_rsDesc.DepthBiasClamp			= 0;
	l_rsDesc.SlopeScaledDepthBias	= 0;
	l_rsDesc.DepthClipEnable		= TRUE;		//	深度クリッピング有り
	l_rsDesc.ScissorEnable			= FALSE;	//	シザー矩形無し
	l_rsDesc.MultisampleEnable		= FALSE;	//	マルチサンプリング無し
	l_rsDesc.AntialiasedLineEnable	= FALSE;	//	ライン･アンチエイリアシング無し
	ComPtr<ID3D11RasterizerState> com;
	//	rasterizer stateを作成する
	if (!DX_Debug::GetInstance()->IsHresultCheck(pDevice->CreateRasterizerState(&l_rsDesc, &com))){
		throw "ID3D11Device::CreateRasterizerState() : failed";
	}

	if (!DX_Debug::GetInstance()->IsHresultCheck(com.As(&m_rasterizerState))) {
		throw "ID3D11RasterizerState As failed";
	}
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
	l_blendDesc.IndependentBlendEnable		= FALSE;
	l_blendDesc.RenderTarget[0].BlendEnable = TRUE;

	//	どうブレンドするか
	l_blendDesc.RenderTarget[0].BlendOp			= D3D11_BLEND_OP_ADD;
	l_blendDesc.RenderTarget[0].BlendOpAlpha	= D3D11_BLEND_OP_ADD;

	//	Blendの対象はなんなのか
	l_blendDesc.RenderTarget[0].SrcBlend	= D3D11_BLEND_SRC_ALPHA;
	l_blendDesc.RenderTarget[0].DestBlend	= D3D11_BLEND_INV_SRC_ALPHA;

	//	Blend係数はいくらか
	l_blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	l_blendDesc.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;
	l_blendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	ComPtr<ID3D11BlendState> com;

	//	blendStateを作成する
	if (!DX_Debug::GetInstance()->IsHresultCheck(pDevice->CreateBlendState(&l_blendDesc, &com))){
		throw "ID3D11Device::CreateBlendState() : failed";
	}

	if (!DX_Debug::GetInstance()->IsHresultCheck(com.As(&m_blendState))) {
		throw "ID3D11BlendState As failed";
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
	l_dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	l_dsDesc.DepthFunc		= D3D11_COMPARISON_LESS;

	// Stencil test parameters
	l_dsDesc.StencilEnable		= TRUE;	
	l_dsDesc.StencilReadMask	= 0xFF;
	l_dsDesc.StencilWriteMask	= 0xFF;

	// Stencil operations if pixel is front-facing
	l_dsDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	l_dsDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	l_dsDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	l_dsDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	l_dsDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	l_dsDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	l_dsDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	l_dsDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	ComPtr<ID3D11DepthStencilState> com;

	//	depth stencil stateを作成する
	if (!DX_Debug::GetInstance()->IsHresultCheck(pDevice->CreateDepthStencilState(&l_dsDesc, &com))){
		throw "ID3D11Device::CreateDepthStencilState() : failed";
	}

	if (!DX_Debug::GetInstance()->IsHresultCheck(com.As(&m_depthStencilState))) {
		throw "ID3D11BlendState As failed";
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

	l_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	l_samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	l_samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	l_samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	l_samplerDesc.MipLODBias = .0f;
	l_samplerDesc.MaxAnisotropy = 0;
	l_samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	l_samplerDesc.BorderColor[0] = .0f;
	l_samplerDesc.BorderColor[1] = .0f;
	l_samplerDesc.BorderColor[2] = .0f;
	l_samplerDesc.BorderColor[3] = .0f;
	l_samplerDesc.MinLOD = 0;
	l_samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ComPtr<ID3D11SamplerState> com;

	if (!DX_Debug::GetInstance()->IsHresultCheck(pDevice->CreateSamplerState(&l_samplerDesc, &com))){
		throw "ID3D11Device::CreateSamplerState() : failed";
	}

	if (!DX_Debug::GetInstance()->IsHresultCheck(com.As(&m_samplerState))) {
		throw "ID3D11SamplerState As failed";
	}
}
ID3D11SamplerState* DX_RenderState::GetDefaultSamplerState()
{
	return m_samplerState.Get();
}