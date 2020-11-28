#ifndef __DX_SYSTEM_H_
#define __DX_SYSTEM_H_
	
/*
	DirectXデバイス等を管理するクラスです。
*/
class DX_System : public DX_Singleton<DX_System>
{
public:
	/// <summary>
	/// DirectXのデバイス等を初期化する
	/// </summary>
	/// <param name="hWnd">ウィンドウのインスタンスハンドル</param>
	/// <param name="hInst">ウィンドウのインスタンス</param>
	/// <param name="width">スクリーンの幅</param>
	/// <param name="height">スクリーンの高さ</param>
	/// <returns>成否</returns>
	bool Initialize(HWND hWnd, HINSTANCE hInst, const UINT width, const UINT height);

	/// <summary>
	/// スクリーンの高さを取得する
	/// </summary>
	/// <returns>スクリーンの高さ</returns>
	UINT GetScreenHeight();

	/// <summary>
	/// スクリーンの幅を取得する
	/// </summary>
	/// <returns>スクリーンの幅</returns>
	UINT GetScreenWidth();
	
	/// <summary>
	/// DirectXのデバイスを取得する
	/// </summary>
	/// <returns>デバイス</returns>
	ID3D11Device*	GetDevice();

	/// <summary>
	/// DIrectXのデバイスコンテキストを取得する
	/// </summary>
	/// <returns>デバイスコンテキスト</returns>
	ID3D11DeviceContext*	GetDeviceContext();

	/// <summary>
	/// スワップチェインを取得する
	/// </summary>
	/// <returns>スワップチェイン</returns>
	IDXGISwapChain*	GetSwapChain();

	/// <summary>
	/// FeatureLevelを取得する
	/// </summary>
	/// <returns>FeatureLevel</returns>
	D3D_FEATURE_LEVEL GetFeatureLevel();

	/// <summary>
	/// DriverTypeを取得する
	/// </summary>
	/// <returns>DriverType</returns>
	D3D_DRIVER_TYPE GetDriverType();

	/// <summary>
	/// レンダーターゲットを取得する
	/// </summary>
	/// <returns>レンダーターゲット</returns>
	ID3D11RenderTargetView*  GetRenderTargetView();

	/// <summary>
	/// 深度・ステンシルビューを取得する
	/// </summary>
	/// <returns>深度・ステンシルビュー</returns>
	ID3D11DepthStencilView* 	GetDepthStencilView();

	/// <summary>
	/// 深度・ステンシルバッファを取得する
	/// </summary>
	/// <returns>深度・ステンシルバッファ</returns>
	ID3D11Texture2D*		GetDepthStencilBuffer();

	/// <summary>
	/// ウィンドウのインスタンスハンドルを取得する
	/// </summary>
	/// <returns>ウィンドウのインスタンスハンドル</returns>
	HWND	GetWindowHandle();

	/// <summary>
	/// バックバッファを初期化する
	/// </summary>
	/// <returns>成否</returns>
	bool InitBuckBuffer();

	/// <summary>
	/// 画面変更に伴うバッファのリサイズを行う
	/// </summary>
	/// <param name="width">スクリーンの幅</param>
	/// <param name="height">スクリーンの高さ</param>
	/// <returns>成否</returns>
	bool BufferResize(const WORD width, const WORD height);
private:
	friend class DX_Singleton<DX_System>;

	Microsoft::WRL::ComPtr<ID3D11Device>			m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_rtv;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_dsv;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_dsb;
	D3D_FEATURE_LEVEL		m_featureLevel;
	D3D_DRIVER_TYPE			m_driverType;
	HWND					m_windowHandle;

	UINT m_screenWidth;
	UINT m_screenHeight;

	/// <summary>
	/// メンバ変数を初期化する
	/// </summary>
	DX_System();

	/// <summary>
	/// DXにかかわるすべてのデバイス等の解放を行う
	/// </summary>
	~DX_System();

	/// <summary>
	/// DirectXのデバイス及びデバイスコンテキスト並びにスワップチェインを作成する
	/// </summary>
	/// <param name="hWnd">ウィンドウのインスタンスハンドル</param>
	/// <returns>成否</returns>
	bool CreateDeviceAndSwapChain(const HWND hWnd);

	/// <summary>
	/// レンダーターゲットビューを作成する
	/// </summary>
	/// <returns>成否</returns>
	bool CreateRenderTargetView();

	/// <summary>
	/// 深度・ステンシルバッファを作成する
	/// </summary>
	/// <returns>成否<</returns>
	bool CreateDepthStencilBuffer();

	/// <summary>
	/// 深度・ステンシルビューを作成
	/// </summary>
	/// <returns>成否</returns>
	bool CreateDepthStencilView();
};
#endif // !__DX_SYSTEM_H_
