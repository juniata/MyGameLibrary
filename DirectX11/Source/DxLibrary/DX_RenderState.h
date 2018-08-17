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

	ID3D11RasterizerState* GetDefaultRasterizerState() const;
	ID3D11BlendState* GetDefaultBlendState() const;
	ID3D11DepthStencilState* GetDefaultDepthStencilState() const;
	ID3D11SamplerState* GetDefaultSamplerState() const;

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
	void CreateRasterizerState(
		ID3D11Device* pDevice
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		ブレンドステートを作成する
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	void CreateBlendState(
		ID3D11Device* pDevice
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		深度･ステンシルステートを作成する
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	void CreateDepthStencilState(
		ID3D11Device* pDevice
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		サンプラーステートを作成する
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	void CreateSamplerState(
		ID3D11Device* pDevice
		);
};
#endif // !__DX_RENDER_STATE_H_
