#ifndef __DX_RENDER_STATE_H_
#define __DX_RENDER_STATE_H_



//****************************************************************************************************
//
//	RenderState
//
//****************************************************************************************************
class RenderState
{
private:
	static ID3D11RasterizerState*	m_pRasterizerState;
	static ID3D11BlendState*		m_pBlendState;
	static ID3D11DepthStencilState*	m_pDepthStencilState;

public:
	static ID3D11SamplerState*		m_pSamplerState;

	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	RenderState() = delete;

public:
	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	~RenderState(){}

	//------------------------------------------------------------------------------
	//
	//  @brief		初期描画の設定を行う
	//
	//------------------------------------------------------------------------------
	static void Initialize();

private:
	//------------------------------------------------------------------------------
	//
	//  @brief		ラスタライザーステートを作成する
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	static void CreateRasterizerState(
		ID3D11Device* pDevice
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		ブレンドステートを作成する
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	static void CreateBlendState(
		ID3D11Device* pDevice
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		深度･ステンシルステートを作成する
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	static void CreateDepthStencilState(
		ID3D11Device* pDevice
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		サンプラーステートを作成する
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	static void CreateSamplerState(
		ID3D11Device* pDevice
		);
};
#endif // !__DX_RENDER_STATE_H_
