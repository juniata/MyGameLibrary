#ifndef __DX_GRAPHICS_H_
#define __DX_GRAPHICS_H_

//****************************************************************************************************
//
//	Graphics
//
//****************************************************************************************************
class DX_Graphics
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		Graphicsを初期化する
	//
	//------------------------------------------------------------------------------
	static bool Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		描画開始
	//	@param[in]	pSwapChain DX_System::GetSwapChain()
	//
	//------------------------------------------------------------------------------
	static void BeginRender(IDXGISwapChain* pSwapChain);

	//------------------------------------------------------------------------------
	//
	//  @brief		描画終了
	//	@param[in]	pSwapChain DX_System::GetSwapChain()
	//
	//------------------------------------------------------------------------------
	static void EndRender(IDXGISwapChain* pSwapChain);

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
	static const char* GetVideoCardDescription();

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
	//	画面が開かれている時だけ描画を行うためのフラグ
	static bool m_standByMode;

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
};
#endif // !__DX_GRAPHICS_H_
