#include	"DX_Library.h"
#include	<wrl/client.h>
using namespace Microsoft::WRL;

//-----------------------------------------------------------------------------------------
//
//	マクロ定義
//
//-----------------------------------------------------------------------------------------
#define FPS_60 1
#define FPS_30 2

//-----------------------------------------------------------------------------------------
//
//	staticメンバ変数
//
//-----------------------------------------------------------------------------------------
DX_Graphics* DX_Graphics::m_pInstance = nullptr;

//-----------------------------------------------------------------------------------------
//
//	初期化
//
//-----------------------------------------------------------------------------------------
DX_Graphics::DX_Graphics() :
	m_stand_by_mode(false),
	m_refreshRateD(0),
	m_refreshRateN(0),
	m_videoCardMemory(0),
	m_scaling(DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED),
	m_format(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
{
	ZeroMemory(m_videoCardDescription, sizeof(m_videoCardDescription));
}

//-----------------------------------------------------------------------------------------
//
//	解放
//
//-----------------------------------------------------------------------------------------
DX_Graphics::~DX_Graphics()
{}

//-----------------------------------------------------------------------------------------
//
//	インスタンスを取得する
//
//-----------------------------------------------------------------------------------------
DX_Graphics* DX_Graphics::GetInstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance = new DX_Graphics();
	}

	return m_pInstance;
}
//-----------------------------------------------------------------------------------------
//
//	初期化
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::Initialize()
{
	PROFILE("DX_Graphics::Initialize()");

	DX_Graphics* pGraphics = DX_Graphics::GetInstance();

	//	変数定義
	ComPtr<IDXGIFactory>	factory;
	ComPtr<IDXGIAdapter>	adapter;
	ComPtr<IDXGIOutput>		output;
	DXGI_MODE_DESC*		pDisplayModeList  = nullptr;
	DXGI_ADAPTER_DESC	adpterDesc = { NULL };
	unsigned int		numModes		= 0;

	DX_Debug* pDebugInstance = DX_Debug::GetInstance();
	
	//	factryを作成
	pDebugInstance->ThrowIfFailed(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory), "CreateDXGIFactory() : failed");

	// adaptersを作成
	pDebugInstance->ThrowIfFailed(factory->EnumAdapters(0, &adapter), "IDXGIFactory::EnumAdapters() : failed");

	//	enumOutputを作成
	pDebugInstance->ThrowIfFailed(adapter->EnumOutputs(0, &output), "IDXGIAdapter::EnumOutputs() : failed");

	//	displayModeListの数を取得
	pDebugInstance->ThrowIfFailed(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, pDisplayModeList), "IDXGIOutput::GetDisplayModeList() : DXGI_MODE_DESC element faile");

	//	listの数だけ生成
	pDisplayModeList = new DXGI_MODE_DESC[numModes];

	//	listの内容を取得
	pDebugInstance->ThrowIfFailed(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, pDisplayModeList), "IDXGIOutput::GetDisplayModeList() : numModels failed");

	unsigned int width	= DX_System::GetWindowWidth();
	unsigned int height = DX_System::GetWindowHeight();

	//	ウィンドウサイズに一致したGPU情報を取得
	//	ウィンドウサイズに一致するリフレッシュシートを取得
	for (unsigned int i = 0; i < numModes; ++i) {
		DXGI_MODE_DESC displayMode = pDisplayModeList[i];

		//	スクリーンの幅が一致している場合
		if (displayMode.Width == width && displayMode.Height == height) {
			//	GPUの情報を取得
			pGraphics->m_refreshRateD	= displayMode.RefreshRate.Denominator;
			pGraphics->m_refreshRateN	= displayMode.RefreshRate.Numerator;
			pGraphics->m_scaling		= displayMode.Scaling;
			pGraphics->m_scanlineOrder = displayMode.ScanlineOrdering;
			pGraphics->m_format		= displayMode.Format;
		}
	}

	//	解放処理
	DELETE_OBJ_ARRAY(pDisplayModeList);

	//	リフレッシュレートが取得できていない場合
	if (pGraphics->m_refreshRateD == 0 && pGraphics->m_refreshRateN == 0) {
		MessageBox(NULL, "Get RefreshReate failed", "error", MB_OK);
		throw;
	}

	//	DXGI_ADAPTER_DESCを取得
	if (!DX_Debug::GetInstance()->IsHresultCheck(adapter->GetDesc(&adpterDesc))) {
		MessageBox(NULL, "IDXGIAdapter::GetDesc(): failed", "error", MB_OK);
		throw;
	}

	//	byte→MegaByte
	pGraphics->m_videoCardMemory = unsigned int(adpterDesc.DedicatedVideoMemory / 1024 / 1024);

	//	文字列を変換
	size_t	l_stringLendth = 0;
	if (::wcstombs_s(&l_stringLendth, pGraphics->m_videoCardDescription, sizeof(pGraphics->m_videoCardDescription), adpterDesc.Description, sizeof(pGraphics->m_videoCardDescription)) != 0) {
		MessageBox(NULL, "wcstombs_s() failed", "error", MB_OK);
		throw;
	}

	
}

//-----------------------------------------------------------------------------------------
//
//	インスタンスを解放
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::Release()
{
	DELETE_OBJ(m_pInstance);
}

//-----------------------------------------------------------------------------------------
//
//	描画開始
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::BeginRender(ID3D11DeviceContext* pContext, IDXGISwapChain* pSwapChain)
{
	if (m_stand_by_mode){
		//	最小化などにされており、描画する必要が無い場合はreturn
		if (pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED){ return; }
		m_stand_by_mode = false;
	}


	DX_View::Clear(pContext);

	DX_System* pSystem = DX_System::GetInstance();
	DX_RenderState* pRenderState = DX_RenderState::GetInstance();
	
	//	サンプラーを設定する
	ID3D11SamplerState* const sampler[1] = { pRenderState->GetDefaultSamplerState() };
	pContext->PSSetSamplers(0, 1, sampler);

	//	ポリゴン描画設定
	pContext->RSSetState(pRenderState->GetDefaultRasterizerState());

	//	RSにビューポートを設定
	pContext->RSSetViewports(1, DX_View::GetViewPort());

	//	OMに必要情報を設定
	float l_blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11RenderTargetView* const targets[1] = { pSystem->GetDefaultRenderTargetView() };
	pContext->OMSetRenderTargets(1, targets, pSystem->GetDefaultDepthStencilView());
	pContext->OMSetDepthStencilState(pRenderState->GetDefaultDepthStencilState(), 1);
	pContext->OMSetBlendState(pRenderState->GetDefaultBlendState(), l_blendFactor, 0xffffffff);
}

//-----------------------------------------------------------------------------------------
//
//	描画終了
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::EndRender(IDXGISwapChain* pSwapChain)
{
	//	スタンバイモードの場合return
	if (m_stand_by_mode){ return; }
	if (pSwapChain->Present(FPS_60, 0) == DXGI_STATUS_OCCLUDED){ m_stand_by_mode = true; }

}

//-----------------------------------------------------------------------------------------
//
//	リフレッシュシートの分母を取得
//
//-----------------------------------------------------------------------------------------
unsigned int DX_Graphics::GetRefreshRateN()
{
	return m_refreshRateN;
}

//-----------------------------------------------------------------------------------------
//
//	リフレッシュシートの分子を取得
//
//-----------------------------------------------------------------------------------------
unsigned int DX_Graphics::GetRefreshRateD()
{
	return m_refreshRateD;
}

//-----------------------------------------------------------------------------------------
//
//	ビデオカードのメモリ量の取得
//
//-----------------------------------------------------------------------------------------
unsigned int DX_Graphics::GetVieoCardMemory()
{
	return m_videoCardMemory;
}
//-----------------------------------------------------------------------------------------
//
//	ビデオカードの名前を取得
//
//-----------------------------------------------------------------------------------------
char* DX_Graphics::GetVideoCardDescription()
{
	return m_videoCardDescription;
}

//-----------------------------------------------------------------------------------------
//
//	スキャンラインオーダーを取得
//
//-----------------------------------------------------------------------------------------
DXGI_MODE_SCANLINE_ORDER DX_Graphics::GetScanLineOrder()
{
	return m_scanlineOrder;
}

//-----------------------------------------------------------------------------------------
//
//	スケーリングを取得
//
//-----------------------------------------------------------------------------------------
DXGI_MODE_SCALING DX_Graphics::GetScaling()
{
	return m_scaling;
}

//-----------------------------------------------------------------------------------------
//
//	フォーマット取得
//
//-----------------------------------------------------------------------------------------
DXGI_FORMAT	DX_Graphics::GetFortmat()
{
	return m_format;
}

