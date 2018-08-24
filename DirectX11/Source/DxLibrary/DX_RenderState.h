#ifndef __DX_RENDER_STATE_H_
#define __DX_RENDER_STATE_H_



//****************************************************************************************************
//
//	RenderState
//
//****************************************************************************************************
class DX_RenderState
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		解放する
	//
	//------------------------------------------------------------------------------
	~DX_RenderState();

	//------------------------------------------------------------------------------
	//
	//  @brief		インスタンスを取得する
	//
	//------------------------------------------------------------------------------
	static DX_RenderState* GetInstance();

	//------------------------------------------------------------------------------
	//
	//  @brief		インスタンスを解放する
	//
	//------------------------------------------------------------------------------
	static void Release();

	//------------------------------------------------------------------------------
	//
	//  @brief		初期描画の設定を行う
	//
	//------------------------------------------------------------------------------
	void Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		ラスタライザステートを取得する
	//	@return		ラスタライザステート
	//
	//------------------------------------------------------------------------------
	ID3D11RasterizerState* GetDefaultRasterizerState() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		ブレンドステートを取得する
	//	@return		ブレンドステート
	//
	//------------------------------------------------------------------------------
	ID3D11BlendState* GetDefaultBlendState() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		深度/ステンシルステートを取得する
	//	@return		深度/ステンシルステート
	//
	//------------------------------------------------------------------------------
	ID3D11DepthStencilState* GetDefaultDepthStencilState() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		サンプラステートを取得する
	//	@return		サンプラステート
	//
	//------------------------------------------------------------------------------
	ID3D11SamplerState* GetDefaultSamplerState() const;

#if defined(DEBUG) || defined(_DEBUG)
	//------------------------------------------------------------------------------
	//
	//  @brief		ラスタライザーステートを取得する(SwitchSolidRS() SwitchWireframeRS()でラスタライザが切り替わる)
	//	@return		ラスタライザーステート
	//
	//------------------------------------------------------------------------------
	ID3D11RasterizerState* GetSwitchRasterizer() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		ラスタライザをソリッド描画に切り替える
	//
	//------------------------------------------------------------------------------
	void SwitchSolidRS();

	//------------------------------------------------------------------------------
	//
	//  @brief		ラスタライザをワイヤーフレーム描画に切り替える
	//
	//------------------------------------------------------------------------------
	void SwitchWireframeRS();
#endif

private:
	ID3D11RasterizerState*		m_pRasterizerState;
	ID3D11BlendState*			m_pBlendState;
	ID3D11DepthStencilState*	m_pDepthStencilState;
	ID3D11SamplerState*			m_pSamplerState;

	static DX_RenderState*		m_pInstance;

	//------------------------------------------------------------------------------
	//
	//  @brief		初期化する
	//
	//------------------------------------------------------------------------------
	DX_RenderState();

	//------------------------------------------------------------------------------
	//
	//  @brief		ラスタライザーステートを作成する
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	void CreateRasterizerState(ID3D11Device* pDevice);

	//------------------------------------------------------------------------------
	//
	//  @brief		ブレンドステートを作成する
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	void CreateBlendState(ID3D11Device* pDevice);

	//------------------------------------------------------------------------------
	//
	//  @brief		深度･ステンシルステートを作成する
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	void CreateDepthStencilState(ID3D11Device* pDevice);

	//------------------------------------------------------------------------------
	//
	//  @brief		サンプラーステートを作成する
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	void CreateSamplerState(ID3D11Device* pDevice);

#if defined(DEBUG) || defined(_DEBUG)
	ID3D11RasterizerState*		m_pWireFrameRS;
	bool m_bUseSolidRS;
#endif
};
#endif // !__DX_RENDER_STATE_H_
