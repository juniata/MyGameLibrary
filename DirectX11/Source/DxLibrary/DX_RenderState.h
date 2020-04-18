#ifndef __DX_RENDER_STATE_H_
#define __DX_RENDER_STATE_H_

//****************************************************************************************************
//
//	RenderState
//
//****************************************************************************************************
class DX_RenderState : public DX_Singleton<DX_RenderState>
{
	friend class DX_Singleton<DX_RenderState>;
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		�����`��̐ݒ���s��
	//
	//------------------------------------------------------------------------------
	bool Initialize();

	//------------------------------------------------------------------------------
	//
	//  @brief		���X�^���C�U�X�e�[�g���擾����
	//	@return		���X�^���C�U�X�e�[�g
	//
	//------------------------------------------------------------------------------
	ID3D11RasterizerState* GetDefaultRasterizerState();

	//------------------------------------------------------------------------------
	//
	//  @brief		�u�����h�X�e�[�g���擾����
	//	@return		�u�����h�X�e�[�g
	//
	//------------------------------------------------------------------------------
	ID3D11BlendState* GetDefaultBlendState();

	//------------------------------------------------------------------------------
	//
	//  @brief		�[�x/�X�e���V���X�e�[�g���擾����
	//	@return		�[�x/�X�e���V���X�e�[�g
	//
	//------------------------------------------------------------------------------
	ID3D11DepthStencilState* GetDefaultDepthStencilState();

	//------------------------------------------------------------------------------
	//
	//  @brief		�T���v���X�e�[�g���擾����
	//	@return		�T���v���X�e�[�g
	//
	//------------------------------------------------------------------------------
	ID3D11SamplerState* GetDefaultSamplerState();

#if defined(DEBUG) || defined(_DEBUG)
	//------------------------------------------------------------------------------
	//
	//  @brief		���X�^���C�U���\���b�h�`��ɐ؂�ւ���
	//
	//------------------------------------------------------------------------------
	void SwitchSolidRS();

	//------------------------------------------------------------------------------
	//
	//  @brief		���X�^���C�U�����C���[�t���[���`��ɐ؂�ւ���
	//
	//------------------------------------------------------------------------------
	void SwitchWireframeRS();
#endif

private:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11BlendState>		m_blendState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		m_samplerState;


	//------------------------------------------------------------------------------
	//
	//  @brief		���X�^���C�U�[�X�e�[�g���쐬����
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	bool CreateRasterizerState(ID3D11Device* pDevice);

	//------------------------------------------------------------------------------
	//
	//  @brief		�u�����h�X�e�[�g���쐬����
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	 bool CreateBlendState(ID3D11Device* pDevice);

	//------------------------------------------------------------------------------
	//
	//  @brief		�[�x��X�e���V���X�e�[�g���쐬����
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	bool CreateDepthStencilState(ID3D11Device* pDevice);

	//------------------------------------------------------------------------------
	//
	//  @brief		�T���v���[�X�e�[�g���쐬����
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	bool CreateSamplerState(ID3D11Device* pDevice);
#if defined(DEBUG) || defined(_DEBUG)
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_wireFrameRS;
#endif
};
#endif // !__DX_RENDER_STATE_H_
