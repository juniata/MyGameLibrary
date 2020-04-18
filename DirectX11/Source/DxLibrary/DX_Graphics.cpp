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
 bool						DX_Graphics::m_standByMode					= false;
 unsigned int				DX_Graphics::m_refreshRateN					= 0;
 unsigned int				DX_Graphics::m_refreshRateD					= 0;
 unsigned int				DX_Graphics::m_videoCardMemory				= 0;
 char						DX_Graphics::m_videoCardDescription[128]	= {"\0"};
 DXGI_MODE_SCANLINE_ORDER	DX_Graphics::m_scanlineOrder				= DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
 DXGI_MODE_SCALING			DX_Graphics::m_scaling						= DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
 DXGI_FORMAT				DX_Graphics::m_format						= DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;

//-----------------------------------------------------------------------------------------
//
//	初期化
//
//-----------------------------------------------------------------------------------------
bool DX_Graphics::Initialize()
{
	auto result = false;

	//	変数定義
	Microsoft::WRL::ComPtr<IDXGIFactory>	factory;
	Microsoft::WRL::ComPtr<IDXGIAdapter>	adapter;
	Microsoft::WRL::ComPtr<IDXGIOutput>		output;
	DXGI_MODE_DESC*		pDisplayModeList	= nullptr;
	DXGI_ADAPTER_DESC	adapterDesc			= { NULL };
	UINT				numModes			= 0;


	const char* pErrMsg = nullptr;

	do {
		//	factryを作成
		if (!DX_Debug::GetInstance()->CheckHresult(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory))) {
			pErrMsg = "failed to CreateDXGIFactory()";
			break;
		}

		// adaptersを作成
		if (!DX_Debug::GetInstance()->CheckHresult(factory->EnumAdapters(0, &adapter))) {
			pErrMsg = "failed to EnumAdapters()";
			break;
		}

		//	enumOutputを作成
		if (!DX_Debug::GetInstance()->CheckHresult(adapter->EnumOutputs(0, &output))) {
			pErrMsg = "failed to EnumOutputs()";
			break;
		}

		//	displayModeListの数を取得
		if (!DX_Debug::GetInstance()->CheckHresult(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, pDisplayModeList))) {
			pErrMsg = "failed to GetDisplayModeList()";
			break;
		}

		//	listの数だけ生成
		pDisplayModeList = new DXGI_MODE_DESC[numModes];

		//	listの内容を取得
		if (!DX_Debug::GetInstance()->CheckHresult(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, pDisplayModeList))) {
			pErrMsg = "failed to GetDisplayModeList()";
			break;
		}

		DX_System* pSystem = DX_System::GetInstance();
		unsigned int width = pSystem->GetScreenWidth();
		unsigned int height = pSystem->GetScreenHeight();


		//	ウィンドウサイズに一致したGPU情報を取得
		//	ウィンドウサイズに一致するリフレッシュシートを取得
		for (UINT i = 0; i < numModes; ++i) {
			DXGI_MODE_DESC displayMode = pDisplayModeList[i];

			//DX_Debug::Printf("Width = %d", displayMode.Width);
			//DX_Debug::Printf(" height = %d", displayMode.Height);
			//DX_Debug::Printf(" m_refreshRateD = %d", displayMode.RefreshRate.Denominator);
			//DX_Debug::Printf(" m_refreshRateN = %d", displayMode.RefreshRate.Numerator);
			//DX_Debug::Printf(" m_scaling = %d", displayMode.Scaling);
			//DX_Debug::Printf(" m_scanlineOrder = %d", displayMode.ScanlineOrdering);
			//DX_Debug::Printf(" m_format = %d", displayMode.Format);
			//DX_Debug::Printf("\n\n");
			//	スクリーンの幅が一致している場合
			if (displayMode.Width == width && displayMode.Height == height) {
				//	GPUの情報を取得
				m_refreshRateD = displayMode.RefreshRate.Denominator;
				m_refreshRateN = displayMode.RefreshRate.Numerator;
				m_scaling = displayMode.Scaling;
				m_scanlineOrder = displayMode.ScanlineOrdering;
				m_format = displayMode.Format;
			}
		}

		//	リフレッシュレートが取得できていない場合
		if (m_refreshRateD == 0 && m_refreshRateN == 0) {
			pErrMsg = "Get RefreshReate failed";
			break;
		}
		
		//	DXGI_ADAPTER_DESCを取得
		if (!DX_Debug::GetInstance()->CheckHresult(adapter->GetDesc(&adapterDesc))) {
			pErrMsg = "failed to GetDesc()";
			break;
		}

		//	byte→MegaByte
		m_videoCardMemory = DX::CAST::UI(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		//	文字列を変換
		size_t	l_stringLendth = 0;
		if (::wcstombs_s(&l_stringLendth, m_videoCardDescription, sizeof(m_videoCardDescription), adapterDesc.Description, sizeof(m_videoCardDescription)) != 0) {
			pErrMsg = "failed to wcstombs_s()";
			break;
		}

		result = true;

	} while (false);

	DELETE_OBJ_ARRAY(pDisplayModeList);

	return result;
}

//-----------------------------------------------------------------------------------------
//
//	描画開始
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::BeginRender(IDXGISwapChain* pSwapChain)
{
	if (m_standByMode){
		//	最小化などにされており、描画する必要が無い場合はreturn
		if (pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED){ 
			return; 
		}
		m_standByMode = false;
	}
}

//-----------------------------------------------------------------------------------------
//
//	描画終了
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::EndRender(IDXGISwapChain* pSwapChain)
{
	if (m_standByMode == false) {
		if (pSwapChain->Present(1, 0) == DXGI_STATUS_OCCLUDED) {
			m_standByMode = true; 
		}
	}

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
const char* DX_Graphics::GetVideoCardDescription()
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

