#ifndef __DX_GRAPHICS_H_
#define __DX_GRAPHICS_H_

//****************************************************************************************************
//
//	Graphics
//
//****************************************************************************************************
class Graphics
{
private:
	//	画面が開かれている時だけ描画を行うためのフラグ
	static bool m_stand_by_mode;

	//	リフレッシュシートの分母と分子
	static unsigned int m_refreshRateN;	//	分子 nolecule
	static unsigned int m_refreshRateD;	//	分母 denominator

	//	ビデオカードのメモリ量
	static unsigned int m_videoCardMemory;

	//	ビデオカードの名前
	static char m_videoCardDescription[128];

	//	スキャンライン
	static DXGI_MODE_SCANLINE_ORDER m_scanlineOrder;

	//	スケーリング
	static DXGI_MODE_SCALING		m_scaling;

	//	フォーマット
	static DXGI_FORMAT				m_format;


	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	Graphics(){}
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	~Graphics(){}

	//------------------------------------------------------------------------------
	//
	//  @brief		Graphicsを初期化する
	//
	//------------------------------------------------------------------------------
	static void Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		描画開始
	//	@param[in]	pSwapChain DX_System::GetSwapChain()
	//
	//------------------------------------------------------------------------------
	static void BeginRender(
		IDXGISwapChain* pSwapChain
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		描画終了
	//	@param[in]	pSwapChain DX_System::GetSwapChain()
	//
	//------------------------------------------------------------------------------
	static void EndRender(
		IDXGISwapChain* pSwapChain
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		リフレッシュシートの分母を取得
	//	@return		m_refreshRateN
	//
	//------------------------------------------------------------------------------
	static unsigned int GetRefreshRateN();

	//------------------------------------------------------------------------------
	//
	//  @brief		リフレッシュシートの分母を取得
	//	@return		m_refreshRateD
	//
	//------------------------------------------------------------------------------
	static unsigned int GetRefreshRateD();

	//------------------------------------------------------------------------------
	//
	//  @brief		ビデオカードのメモリ量の取得
	//	@return		m_videoCardMemory
	//
	//------------------------------------------------------------------------------
	static unsigned int GetVieoCardMemory();

	//------------------------------------------------------------------------------
	//
	//  @brief		ビデオカードのメモリ量の取得
	//	@return		m_videoCardDescription
	//
	//------------------------------------------------------------------------------
	static char* GetVideoCardDescription();

	//------------------------------------------------------------------------------
	//
	//  @brief		スキャンラインオーダーを取得
	//	@return		m_scanlineOrder
	//
	//------------------------------------------------------------------------------
	static DXGI_MODE_SCANLINE_ORDER GetScanLineOrder();

	//------------------------------------------------------------------------------
	//
	//  @brief		スケーリングを取得
	//	@return		m_scaling
	//
	//------------------------------------------------------------------------------
	static DXGI_MODE_SCALING GetScaling();

	//------------------------------------------------------------------------------
	//
	//  @brief		フォーマット取得
	//	@return		m_format
	//
	//------------------------------------------------------------------------------
	static DXGI_FORMAT	GetFortmat();

private:
	//------------------------------------------------------------------------------
	//
	//  @brief		CreateDXGIFactoryのラッパー
	//	@param[out]	l_pFactory	空のIDXGIFactoryポインタ変数
	//
	//------------------------------------------------------------------------------
	static void CreateDXGIFactory(
		IDXGIFactory*& pFactary
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		IDXGIFactory::EnumAdaptersのラッパー
	//	@param[in]	pFactory	CreateDXGIFactory()で作成したIDXGIFactoryポインタ変数
	//	@param[out]	pAdapter	空のIDXGIAdapterポインタ変数
	//
	//------------------------------------------------------------------------------
	static void EnumAdapters(
		IDXGIFactory*	pFactory, 
		IDXGIAdapter*&	pAapter
		);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		IDXGIAdapter::EnumOutputsのラッパー
	//	@param[in]	pAdapter	EnumAdapters()で作成したIDXGIAdapterポインタ変数
	//	@param[out]	pOutput		空のIDXGIOutputポインタ変数
	//
	//------------------------------------------------------------------------------
	static void EnumOutputs(
		IDXGIAdapter* pAdapter,
		IDXGIOutput*& pOutput
		);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		IDXGIOutput::GetDisplayModeListのラッパー
	//	@param[in]	pOutput					EnumOutputs()で作成したIDXGIOutputポインタ変数
	//	@param[out]	pNumModels				displayModeListの数を取得
	//	@param[in or out]	pModelList		DXGI_MODE_DESCポインタ変数
	//
	//------------------------------------------------------------------------------
	static void GetDisplayModeList(
		IDXGIOutput*		pOutput, 
		unsigned int&		pNumModels, 
		DXGI_MODE_DESC*&	pModelList
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		IDXGIAdapter::GetDescのラッパー
	//	@param[in]	pAdapter		EnumOutputs()で作成したIDXGIAdapterポインタ変数
	//	@param[out] pAdapterDesc	空のDXGI_ADAPTER_DESC変数
	//
	//------------------------------------------------------------------------------
	static void GetDesc(
		IDXGIAdapter*		pAdapter,
		DXGI_ADAPTER_DESC*	pAdapterDesc
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		ウィンドウサイズに一致したリフレッシュレートを取得
	//	@param[in]	modelsCount			GetDisplayModeList()で取得したdisplayModeListの数
	//	@param[in]	windowWidth			DX_System::GetWindowWidth()
	//	@param[in]	windowHeight		DX_System::GetWindowHeight()
	//	@param[in]	pDisplayModeList	displayModeListで配列生成したDXGI_MODE_DESCポインタ変数
	//
	//------------------------------------------------------------------------------
	static void GetRefreshRate(
		const unsigned int		modelsCount,
		const unsigned int		windowWidth,
		const unsigned int		windowHeight,
		const DXGI_MODE_DESC*	pDisplayModeList
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		wcstombs_sのラッパー
	//	@param[in]	_Src		文字列
	//
	//------------------------------------------------------------------------------
	static void wcstombs_s(
		const wchar_t* _Src
		);
};
#endif // !__DX_GRAPHICS_H_
