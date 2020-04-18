#ifndef __DX_SYSTEM_H_
#define __DX_SYSTEM_H_
	
//****************************************************************************************************
//
//	System
//
//****************************************************************************************************
class DX_System : public DX_Singleton<DX_System>
{
	friend class DX_Singleton<DX_System>;
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		DirectXを初期化する
	//	@param[in]	hwnd	DX_Framework::GetHwnd()
	//
	//------------------------------------------------------------------------------
	bool Initialize(HWND hWnd, HINSTANCE hInst);

	//------------------------------------------------------------------------------
	//
	//  @brief		スクリーンサイズを設定
	//	@param[in]	width ウィンドウ幅
	//	@param[in]	height ウィンドウ高さ
	//
	//------------------------------------------------------------------------------
	void SetScreenSize(const unsigned int width, const unsigned int height);

	//------------------------------------------------------------------------------
	//
	//  @brief		スクリーンの高さを取得
	//	@return		m_windowHeight
	//
	//------------------------------------------------------------------------------
	unsigned int GetScreenHeight();

	//------------------------------------------------------------------------------
	//
	//  @brief		スクリーンの幅を取得
	//	@return		m_windowWidth
	//
	//------------------------------------------------------------------------------
	unsigned int GetScreenWidth();
	
	//------------------------------------------------------------------------------
	//
	//  @brief		デバイスを取得
	//	@return		m_pDevice
	//
	//------------------------------------------------------------------------------
	ID3D11Device*	GetDevice();

	//------------------------------------------------------------------------------
	//
	//  @brief		デバイスコンテキストを取得
	//	@return		m_pDeviceContext
	//
	//------------------------------------------------------------------------------
	ID3D11DeviceContext*	GetDeviceContext();

	//------------------------------------------------------------------------------
	//
	//  @brief		スワップチェインを取得
	//	@return		m_pSwapChain
	//
	//------------------------------------------------------------------------------
	IDXGISwapChain*	GetSwapChain();

	//------------------------------------------------------------------------------
	//
	//  @brief		FeatureLevelを取得
	//	@return		m_featureLevel
	//
	//------------------------------------------------------------------------------
	D3D_FEATURE_LEVEL GetFeatureLevel();
	
	//------------------------------------------------------------------------------
	//
	//  @brief		DriverTypeを取得
	//	@return		m_driverType
	//
	//------------------------------------------------------------------------------
	D3D_DRIVER_TYPE GetDriverType();

	//------------------------------------------------------------------------------
	//
	//  @brief		レンダーターゲットを取得
	//	@return		m_pRenderTargetView
	//
	//------------------------------------------------------------------------------
	ID3D11RenderTargetView*  GetRenderTargetView();

	//------------------------------------------------------------------------------
	//
	//  @brief		深度･ステンシルビュー取得
	//	@return		m_pDepthStencilview
	//
	//------------------------------------------------------------------------------
	ID3D11DepthStencilView* 	GetDepthStencilView();

	//------------------------------------------------------------------------------
	//
	//  @brief		深度･ステンシルバッファを取得
	//	@return		m_pDepthStencilbuffer
	//
	//------------------------------------------------------------------------------
	ID3D11Texture2D*		GetDepthStencilBuffer();

	//------------------------------------------------------------------------------
	//
	//  @brief		ウィンドウハンドルを取得
	//	@return		m_windowHandle
	//
	//------------------------------------------------------------------------------
	HWND	GetWindowHandle();

	 //------------------------------------------------------------------------------
	 //
	 //  @brief		バックバッファの初期化を行う
	 //
	 //------------------------------------------------------------------------------
	bool InitBuckBuffer();

	 //------------------------------------------------------------------------------
	 //
	 //  @brief		リサイズ処理
	 //
	 //------------------------------------------------------------------------------
	bool BufferResize(const WORD width, const WORD height);
private:
	Microsoft::WRL::ComPtr<ID3D11Device>			m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_rtv;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_dsv;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_dsb;
	D3D_FEATURE_LEVEL		m_featureLevel;
	D3D_DRIVER_TYPE			m_driverType;
	HWND					m_windowHandle;

	unsigned int m_screenWidth;
	unsigned int m_screenHeight;

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数を初期化する
	//
	//------------------------------------------------------------------------------
	DX_System();

	//------------------------------------------------------------------------------
	//
	//  @brief		DirectX関連　解放
	//
	//------------------------------------------------------------------------------
	~DX_System();

	//------------------------------------------------------------------------------
	//
	//  @brief		SwapChain,ID3D11Device,ID3D11DeviceContextを作成
	//	@param[in]	hWnd	DX_Framework::GetHwnd()
	//
	//------------------------------------------------------------------------------
	bool CreateDeviceAndSwapChain(const HWND& hWnd);

	//------------------------------------------------------------------------------
	//
	//  @brief		レンデーターゲットビューを作成
	//
	//------------------------------------------------------------------------------
	bool CreateRenderTargetView();

	//------------------------------------------------------------------------------
	//
	//  @brief		深度･ステンシルバッファを作成する
	//
	//------------------------------------------------------------------------------
	bool CreateDepthStencilBuffer();

	//------------------------------------------------------------------------------
	//
	//  @brief		深度･ステンシルビューを作成
	//
	//------------------------------------------------------------------------------
	bool CreateDepthStencilView();
};
#endif // !__DX_SYSTEM_H_
