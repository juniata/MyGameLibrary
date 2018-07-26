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
	//  @brief		�������
	//
	//------------------------------------------------------------------------------
	~DX_RenderState();

	//------------------------------------------------------------------------------
	//
	//  @brief		�C���X�^���X���擾����
	//
	//------------------------------------------------------------------------------
	static DX_RenderState* GetInstance();

	//------------------------------------------------------------------------------
	//
	//  @brief		�C���X�^���X���������
	//
	//------------------------------------------------------------------------------
	static void Release();

	//------------------------------------------------------------------------------
	//
	//  @brief		�����`��̐ݒ���s��
	//
	//------------------------------------------------------------------------------
	void Initialize();

private:
	ID3D11RasterizerState*		m_pRasterizerState;
	ID3D11BlendState*			m_pBlendState;
	ID3D11DepthStencilState*	m_pDepthStencilState;
	ID3D11SamplerState*			m_pSamplerState;

	static DX_RenderState*		m_pInstance;

	//------------------------------------------------------------------------------
	//
	//  @brief		����������
	//
	//------------------------------------------------------------------------------
	DX_RenderState();

	//------------------------------------------------------------------------------
	//
	//  @brief		���X�^���C�U�[�X�e�[�g���쐬����
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	void CreateRasterizerState(
		ID3D11Device* pDevice
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		�u�����h�X�e�[�g���쐬����
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	void CreateBlendState(
		ID3D11Device* pDevice
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		�[�x��X�e���V���X�e�[�g���쐬����
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	void CreateDepthStencilState(
		ID3D11Device* pDevice
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		�T���v���[�X�e�[�g���쐬����
	//	@param[in]	pDevice	DX_System::GetDevice()
	//
	//------------------------------------------------------------------------------
	void CreateSamplerState(
		ID3D11Device* pDevice
		);
};
#endif // !__DX_RENDER_STATE_H_