#ifndef __DX_RENDER_STATE_H_
#define __DX_RENDER_STATE_H_



//****************************************************************************************************
//
//	RenderState
//
//****************************************************************************************************
class DX_RenderState
{
private:
	ComPtr<ID3D11RasterizerState>	m_rasterizerState;
	ComPtr<ID3D11BlendState>		m_blendState;
	ComPtr<ID3D11DepthStencilState>	m_depthStencilState;
	ComPtr<ID3D11SamplerState>		m_samplerState;

	static DX_RenderState* m_pInstance;
public:

	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	DX_RenderState();

public:
	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	~DX_RenderState();

	static DX_RenderState* GetInstance();

	static void Release();
	//------------------------------------------------------------------------------
	//
	//  @brief		初期描画の設定を行う
	//
	//------------------------------------------------------------------------------
	void Initialize();

	ID3D11SamplerState* GetDefaultSamplerState();
private:
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
