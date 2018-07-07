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

	try{
		//	factryを作成
		CreateDXGIFactory(l_pFactory);

		// adaptersを作成
		EnumAdapters(l_pFactory, l_pAdapter);

		//	enumOutputを作成
		EnumOutputs(l_pAdapter, l_pOutput);

		//	displayModeListの数を取得
		GetDisplayModeList(l_pOutput, l_numModes, l_pDisplayModeList);

		//	listの数だけ生成
		l_pDisplayModeList = new DXGI_MODE_DESC[l_numModes];

		//	listの内容を取得
		GetDisplayModeList(l_pOutput, l_numModes, l_pDisplayModeList);

		//	ウィンドウサイズに一致したGPU情報を取得
		GetRefreshRate(l_numModes, DX_System::GetWindowWidth(), DX_System::GetWindowHeight(), l_pDisplayModeList);

		//	DXGI_ADAPTER_DESCを取得
		GetDesc(l_pAdapter, &l_adpterDesc);

		//	byte→MegaByte
		m_videoCardMemory = unsigned int(l_adpterDesc.DedicatedVideoMemory / 1024 / 1024);

		//	文字列を変換
		wcstombs_s(l_adpterDesc.Description);

		//	解放処理
		if (l_pDisplayModeList){ delete[] l_pDisplayModeList; l_pDisplayModeList = nullptr; }
		if (l_pOutput){ l_pOutput->Release(); l_pOutput = nullptr; }
		if (l_pAdapter){ l_pAdapter->Release(); l_pAdapter = nullptr; }
		if (l_pFactory){ l_pFactory->Release(); l_pFactory = nullptr; }
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

//-----------------------------------------------------------------------------------------
//
//	CreateDXGIFactoryのラッパー
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::CreateDXGIFactory(IDXGIFactory*& pFactary)
{
	if (!DX_Debug::GetInstance()->IsHresultCheck(::CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactary))){
		throw "CreateDXGIFactory() : failed";
	}
}

//-----------------------------------------------------------------------------------------
//
//	IDXGIFactory::EnumAdaptersのラッパー
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::EnumAdapters(IDXGIFactory* pFactory, IDXGIAdapter*& pAapter)
{
	if (!DX_Debug::GetInstance()->IsHresultCheck(pFactory->EnumAdapters(0, &pAapter))){
		throw "IDXGIFactory::EnumAdapters() : failed";
	}
}

//-----------------------------------------------------------------------------------------
//
//	IDXGIAdapter::EnumOutputsのラッパー
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::EnumOutputs(IDXGIAdapter* pAdapter, IDXGIOutput*& pOutput)
{
	if (!DX_Debug::GetInstance()->IsHresultCheck(pAdapter->EnumOutputs(0, &pOutput))){
		throw "IDXGIAdapter::EnumOutputs() : failed";
	}
}

//-----------------------------------------------------------------------------------------
//
//	IDXGIOutput::GetDisplayModeListのラッパー
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::GetDisplayModeList(IDXGIOutput* pOutput, unsigned int& pNumModels, DXGI_MODE_DESC*& pModelList)
{
	if (!DX_Debug::GetInstance()->IsHresultCheck(pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &pNumModels, pModelList))){
		if (pModelList == nullptr){
			throw "IDXGIOutput::GetDisplayModeList() : DXGI_MODE_DESC element failed";
		}
		else{
			throw "IDXGIOutput::GetDisplayModeList() : numModels failed";
		}
	}
}

//-----------------------------------------------------------------------------------------
//
//	IDXGIAdapter::GetDescのラッパー
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::GetDesc(IDXGIAdapter* pAdapter, DXGI_ADAPTER_DESC* pAdapterDesc)
{
	if (!DX_Debug::GetInstance()->IsHresultCheck(pAdapter->GetDesc(pAdapterDesc))){
		throw "IDXGIAdapter::GetDesc(): failed";
	}
}

//-----------------------------------------------------------------------------------------
//
//	ウィンドウサイズに一致したリフレッシュレートを取得
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::GetRefreshRate(
	const unsigned int modelsCount,
	const unsigned int windowWidth,
	const unsigned int windowHeight,
	const DXGI_MODE_DESC* pDisplayModeList
	)
{
	//	ウィンドウサイズに一致するリフレッシュシートを取得
	for (unsigned int i = 0; i < modelsCount; ++i){

		//	スクリーンの幅が一致している場合
		if (pDisplayModeList[i].Width == windowWidth){

			//	スクリーンの高さが一致している場合
			if (pDisplayModeList[i].Height == windowHeight){

				//	GPUの情報を取得
				m_refreshRateD	= pDisplayModeList[i].RefreshRate.Denominator;
				m_refreshRateN	= pDisplayModeList[i].RefreshRate.Numerator;
				m_scaling		= pDisplayModeList[i].Scaling;
				m_scanlineOrder = pDisplayModeList[i].ScanlineOrdering;
				m_format		= pDisplayModeList[i].Format;
			}
		}
	}


	//	リフレッシュレートが取得できていない場合
	if (m_refreshRateD == 0 && m_refreshRateN == 0){
		throw "Get RefreshReate failed";
	}
}


//-----------------------------------------------------------------------------------------
//
//	文字列変換する
//
//-----------------------------------------------------------------------------------------
void DX_Graphics::wcstombs_s(
	const wchar_t * _Src
	)
{
	size_t	l_stringLendth = 0;

	if (::wcstombs_s(
		&l_stringLendth,
		m_videoCardDescription,
		sizeof(m_videoCardDescription),
		_Src,
		sizeof(m_videoCardDescription)
		) != 0)
	{
		throw "wcstombs_s() failed";
	}
}