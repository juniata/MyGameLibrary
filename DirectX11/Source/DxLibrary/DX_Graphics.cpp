#include	"DX_Library.h"

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
bool						DX_Graphics::m_stand_by_mode = false;
unsigned int				DX_Graphics::m_refreshRateD = 0;
unsigned int				DX_Graphics::m_refreshRateN = 0;
unsigned int				DX_Graphics::m_videoCardMemory = 0;
char						DX_Graphics::m_videoCardDescription[128];
DXGI_MODE_SCANLINE_ORDER	DX_Graphics::m_scanlineOrder;
DXGI_MODE_SCALING			DX_Graphics::m_scaling;
DXGI_FORMAT					DX_Graphics::m_format;


//-----------------------------------------------------------------------------------------
//
//	初期化
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::Initialize()
{
	PROFILE("DX_Graphics::Initialize()");

	//	変数定義
	IDXGIFactory*		l_pFactory			= nullptr;
	IDXGIAdapter*		l_pAdapter			= nullptr;
	IDXGIOutput*		l_pOutput			= nullptr;
	DXGI_MODE_DESC*		l_pDisplayModeList  = nullptr;
	DXGI_ADAPTER_DESC	l_adpterDesc = { NULL };
	unsigned int		l_numModes		= 0;

	try {
		//	factryを作成
		if (!DX_Debug::GetInstance()->IsHresultCheck(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&l_pFactory))) {
			throw "CreateDXGIFactory() : failed";
		}

		// adaptersを作成
		if (!DX_Debug::GetInstance()->IsHresultCheck(l_pFactory->EnumAdapters(0, &l_pAdapter))) {
			throw "IDXGIFactory::EnumAdapters() : failed";
		}

		//	enumOutputを作成
		if (!DX_Debug::GetInstance()->IsHresultCheck(l_pAdapter->EnumOutputs(0, &l_pOutput))) {
			throw "IDXGIAdapter::EnumOutputs() : failed";
		}

		//	displayModeListの数を取得
		if (!DX_Debug::GetInstance()->IsHresultCheck(l_pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &l_numModes, l_pDisplayModeList))) {
				throw "IDXGIOutput::GetDisplayModeList() : DXGI_MODE_DESC element failed";
		}

		//	listの数だけ生成
		l_pDisplayModeList = new DXGI_MODE_DESC[l_numModes];

		//	listの内容を取得
		if (!DX_Debug::GetInstance()->IsHresultCheck(l_pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &l_numModes, l_pDisplayModeList))) {
			throw "IDXGIOutput::GetDisplayModeList() : numModels failed";
		}

		unsigned int width	= DX_System::GetWindowWidth();
		unsigned int height = DX_System::GetWindowHeight();

		//	ウィンドウサイズに一致したGPU情報を取得
		//	ウィンドウサイズに一致するリフレッシュシートを取得
		for (unsigned int i = 0; i < l_numModes; ++i) {
			DXGI_MODE_DESC displayMode = l_pDisplayModeList[i];

			//DX_Debug::GetInstance()->Printf("Width = %d", displayMode.Width);
			//DX_Debug::GetInstance()->Printf(" height = %d", displayMode.Height);
			//DX_Debug::GetInstance()->Printf(" m_refreshRateD = %d", displayMode.RefreshRate.Denominator);
			//DX_Debug::GetInstance()->Printf(" m_refreshRateN = %d", displayMode.RefreshRate.Numerator);
			//DX_Debug::GetInstance()->Printf(" m_scaling = %d", displayMode.Scaling);
			//DX_Debug::GetInstance()->Printf(" m_scanlineOrder = %d", displayMode.ScanlineOrdering);
			//DX_Debug::GetInstance()->Printf(" m_format = %d", displayMode.Format);
			//DX_Debug::GetInstance()->Printf("\n\n");
			//	スクリーンの幅が一致している場合
			if (displayMode.Width == width && displayMode.Height == height) {
				//	GPUの情報を取得
				m_refreshRateD	= displayMode.RefreshRate.Denominator;
				m_refreshRateN	= displayMode.RefreshRate.Numerator;
				m_scaling		= displayMode.Scaling;
				m_scanlineOrder = displayMode.ScanlineOrdering;
				m_format		= displayMode.Format;
			}
		}
		
		//	リフレッシュレートが取得できていない場合
		if (m_refreshRateD == 0 && m_refreshRateN == 0) {
			throw "Get RefreshReate failed";
		}

		//	DXGI_ADAPTER_DESCを取得
		if (!DX_Debug::GetInstance()->IsHresultCheck(l_pAdapter->GetDesc(&l_adpterDesc))) {
			throw "IDXGIAdapter::GetDesc(): failed";
		}

		//	byte→MegaByte
		m_videoCardMemory = unsigned int(l_adpterDesc.DedicatedVideoMemory / 1024 / 1024);

		//	文字列を変換
		size_t	l_stringLendth = 0;
		if (::wcstombs_s(&l_stringLendth, m_videoCardDescription, sizeof(m_videoCardDescription), l_adpterDesc.Description, sizeof(m_videoCardDescription)) != 0) {
			throw "wcstombs_s() failed";
		}

		//	解放処理
		DELETE_OBJ_ARRAY(l_pDisplayModeList);
		SAFE_RELEASE(l_pFactory);
		SAFE_RELEASE(l_pAdapter);
		SAFE_RELEASE(l_pOutput);
	}
	catch (char* pErrorMsg){
		throw  pErrorMsg;
	}
}


//-----------------------------------------------------------------------------------------
//
//	描画開始
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::BeginRender(IDXGISwapChain* pSwapChain)
{
	if (m_stand_by_mode){
		//	最小化などにされており、描画する必要が無い場合はreturn
		if (pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED){ return; }
		m_stand_by_mode = false;
	}


	DX_View::Clear();

	DX_System* pSystem = DX_System::GetInstance();
	ID3D11DeviceContext* l_pDeviceContext = pSystem->GetDeviceContext();
	DX_RenderState* pRenderState = DX_RenderState::GetInstance();
	
	//	サンプラーを設定する
	ID3D11SamplerState* const sampler[1] = { pRenderState->GetDefaultSamplerState() };
	l_pDeviceContext->PSSetSamplers(0, 1, sampler);
	
#if defined(DEBUG) || defined(_DEBUG)
	//	ポリゴン描画設定
	l_pDeviceContext->RSSetState(pRenderState->GetSwitchRasterizer());
#else 
	//	ポリゴン描画設定
	l_pDeviceContext->RSSetState(pRenderState->GetDefaultRasterizerState());
#endif
	//	RSにビューポートを設定
	l_pDeviceContext->RSSetViewports(1, DX_View::GetViewPort());

	//	OMに必要情報を設定
	float l_blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11RenderTargetView* const targets[1] = { pSystem->GetDefaultRenderTargetView() };
	l_pDeviceContext->OMSetRenderTargets(1, targets, pSystem->GetDefaultDepthStencilView());
	l_pDeviceContext->OMSetDepthStencilState(pRenderState->GetDefaultDepthStencilState(), 1);
	l_pDeviceContext->OMSetBlendState(pRenderState->GetDefaultBlendState(), l_blendFactor, 0xffffffff);
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

