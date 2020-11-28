#ifndef __DX_GRAPHICS_H_
#define __DX_GRAPHICS_H_

/// <summary>
/// 描画の開始終了及び、モニターの詳細を管理しています
/// </summary>
class DX_Graphics : public DX_Singleton<DX_Graphics>
{
public:
	/// <summary>
	/// </summary>
	/// モニターに関する情報を取得を行う。
	/// <returns>成否</returns>
	bool Initialize();

	/// <summary>
	/// 描画開始
	/// </summary>
	void BeginRender();

	/// <summary>
	/// 描画終了
	/// </summary>
	void EndRender();

	/// <summary>
	/// リフレッシュシートの分母を取得
	/// </summary>
	/// <returns>リフレッシュシートの分母</returns>
	unsigned int GetRefreshRateN();

	/// <summary>
	/// リフレッシュシートの分子を取得
	/// </summary>
	/// <returns>リフレッシュシートの分子</returns>
	unsigned int GetRefreshRateD();

	/// <summary>
	/// ビデオカードのメモリ量の取得
	/// </summary>
	/// <returns>ビデオカードのメモリ量</returns>
	unsigned int GetVieoCardMemory();

	/// <summary>
	///　ビデオカードの名前を取得
	/// </summary>
	/// <returns>ビデオカードの名前</returns>
	const char* GetVideoCardDescription();

	/// <summary>
	/// スキャンラインオーダーを取得
	/// </summary>
	/// <returns>スキャンラインオーダー</returns>
	DXGI_MODE_SCANLINE_ORDER GetScanLineOrder();

	/// <summary>
	/// スケーリングを取得
	/// </summary>
	/// <returns>スケーリング</returns>
	DXGI_MODE_SCALING GetScaling();

	/// <summary>
	/// フォーマットを取得
	/// </summary>
	/// <returns>フォーマット</returns>
	DXGI_FORMAT	GetFortmat();

private:
	friend class DX_Singleton<DX_Graphics>;
	//	画面が開かれている時だけ描画を行うためのフラグ
	bool m_standByMode;

	//	リフレッシュシートの分母と分子
	unsigned int m_refreshRateN;	//	分子 nolecule
	unsigned int m_refreshRateD;	//	分母 denominator

	//	ビデオカードのメモリ量			
	unsigned int m_videoCardMemory;

	//	ビデオカードの名前
	char m_videoCardDescription[128];

	//	スキャンライン
	DXGI_MODE_SCANLINE_ORDER m_scanlineOrder;

	//	スケーリング
	DXGI_MODE_SCALING		m_scaling;

	//	フォーマット
	DXGI_FORMAT				m_format;

	/// <summary>
	/// メンバ変数の初期化を行う
	/// </summary>
		DX_Graphics();
};
#endif // !__DX_GRAPHICS_H_
