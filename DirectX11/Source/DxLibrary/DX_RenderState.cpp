#include	"DX_Library.h"

//-----------------------------------------------------------------------------------------
//
//  static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------
DX_RenderState* DX_RenderState::m_pInstance = nullptr;


//-----------------------------------------------------------------------------------------
//
//  ������
//
//-----------------------------------------------------------------------------------------
DX_RenderState::DX_RenderState() :
	m_pRasterizerState(nullptr),
	m_pBlendState(nullptr),
	m_pDepthStencilState(nullptr),
	m_pSamplerState(nullptr)

#if defined(DEBUG) || defined(_DEBUG)
	,m_pWireFrameRS(nullptr)
#endif
{}

//-----------------------------------------------------------------------------------------
//
//  ���
//
//-----------------------------------------------------------------------------------------
DX_RenderState::~DX_RenderState()
{
	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pBlendState);
	SAFE_RELEASE(m_pDepthStencilState);
	SAFE_RELEASE(m_pSamplerState);

#if defined(DEBUG) || defined(_DEBUG)
	SAFE_RELEASE(m_pWireFrameRS);
#endif

}

//-----------------------------------------------------------------------------------------
//
//  �C���X�^���X���擾����
//
//-----------------------------------------------------------------------------------------
DX_RenderState* DX_RenderState::GetInstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance = new DX_RenderState();
	}
	
	return m_pInstance;
}

//-----------------------------------------------------------------------------------------
//
//  �C���X�^���X���������
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::Release()
{
	DELETE_OBJ(m_pInstance);
}

//-----------------------------------------------------------------------------------------
//
//  �����`��̐ݒ���s��
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::Initialize()
{
	DX_System*				pSystem		= DX_System::GetInstance();
	ID3D11Device*			pDevice		= pSystem->GetDevice();
	ID3D11DeviceContext*	pContext	= pSystem->GetDeviceContext();

	try{
		//	���X�^���C�U�[�X�e�[�g���쐬
		CreateRasterizerState(pDevice);

		//	�u�����h�X�e�[�g���쐬
		CreateBlendState(pDevice);

		//	�[�x��X�e���V���X�e�[�g���쐬
		CreateDepthStencilState(pDevice);
		
		//	�T���v���[�X�e�[�g���쐬
		CreateSamplerState(pDevice);
	}
	catch (char* pMessage){
		throw pMessage;
	}


	//	OM�ɕK�v����ݒ�
	float l_blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11RenderTargetView* const targets[1] = { pSystem->GetDefaultRenderTargetView() };
	pContext->OMSetRenderTargets(1, targets, pSystem->GetDefaultDepthStencilView());
	pContext->OMSetDepthStencilState(GetDefaultDepthStencilState(), 1);

	//	�T���v���[��ݒ肷��
	ID3D11SamplerState* const sampler[1] = { GetDefaultSamplerState() };
	pContext->PSSetSamplers(0, 1, sampler);

	//	�|���S���`��ݒ�
	pContext->RSSetState(GetDefaultRasterizerState());
}

//-----------------------------------------------------------------------------------------
//
//  ���X�^���C�U�X�e�[�g���擾����
//
//-----------------------------------------------------------------------------------------
ID3D11RasterizerState* DX_RenderState::GetDefaultRasterizerState() const
{
	return m_pRasterizerState;
}

//-----------------------------------------------------------------------------------------
//
//  �u�����h�X�e�[�g���擾����
//
//-----------------------------------------------------------------------------------------
ID3D11BlendState* DX_RenderState::GetDefaultBlendState() const
{
	return m_pBlendState;
}

//-----------------------------------------------------------------------------------------
//
//  �[�x/�X�e���V���X�e�[�g���擾����
//
//-----------------------------------------------------------------------------------------
ID3D11DepthStencilState* DX_RenderState::GetDefaultDepthStencilState() const
{
	return m_pDepthStencilState;
}

//-----------------------------------------------------------------------------------------
//
//  �T���v���X�e�[�g���擾����
//
//-----------------------------------------------------------------------------------------
ID3D11SamplerState* DX_RenderState::GetDefaultSamplerState() const
{
	return m_pSamplerState;
}

#if defined(DEBUG) || defined(_DEBUG)
//------------------------------------------------------------------------------
//
//  ���X�^���C�U���\���b�h�`��ɐ؂�ւ���
//
//------------------------------------------------------------------------------
void DX_RenderState::SwitchSolidRS()
{
	DX_System::GetInstance()->GetDeviceContext()->RSSetState(GetDefaultRasterizerState());
}

//------------------------------------------------------------------------------
//
//  ���X�^���C�U�����C���[�t���[���`��ɐ؂�ւ���
//
//------------------------------------------------------------------------------
void DX_RenderState::SwitchWireframeRS()
{
	DX_System::GetInstance()->GetDeviceContext()->RSSetState(m_pWireFrameRS);
}
#endif

//-----------------------------------------------------------------------------------------
//
//  ���X�^���C�U�[�X�e�[�g���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::CreateRasterizerState(ID3D11Device*	pDevice)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.FillMode				= D3D11_FILL_MODE::D3D11_FILL_SOLID;	//	���ʂɕ`��
	desc.CullMode				= D3D11_CULL_MODE::D3D11_CULL_BACK;		//	���ʔ�`��
	desc.FrontCounterClockwise	= FALSE;	//	���v���肪�\��
	desc.DepthBias				= 0;		//	�[�x�o�C�A�X�0�
	desc.DepthBiasClamp			= 0;
	desc.SlopeScaledDepthBias	= 0;
	desc.DepthClipEnable		= TRUE;		//	�[�x�N���b�s���O�L��
	desc.ScissorEnable			= FALSE;	//	�V�U�[��`����
	desc.MultisampleEnable		= FALSE;	//	�}���`�T���v�����O����
	desc.AntialiasedLineEnable	= FALSE;	//	���C����A���`�G�C���A�V���O����

	//	rasterizer state���쐬����
	if (!DX_Debug::GetInstance()->IsHresultCheck(pDevice->CreateRasterizerState(&desc, &m_pRasterizerState))){
		throw "ID3D11Device::CreateRasterizerState() : failed";
	}

#if defined(DEBUG) || defined(_DEBUG)
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(wireframeDesc));

	wireframeDesc.FillMode				= D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;	//	���ʂɕ`��
	wireframeDesc.CullMode				= D3D11_CULL_MODE::D3D11_CULL_BACK;		//	���ʔ�`��
	wireframeDesc.FrontCounterClockwise = FALSE;	//	���v���肪�\��
	wireframeDesc.DepthBias				= 0;		//	�[�x�o�C�A�X�0�
	wireframeDesc.DepthBiasClamp		= 0;
	wireframeDesc.SlopeScaledDepthBias	= 0;
	wireframeDesc.DepthClipEnable		= TRUE;		//	�[�x�N���b�s���O�L��
	wireframeDesc.ScissorEnable			= FALSE;	//	�V�U�[��`����
	wireframeDesc.MultisampleEnable		= FALSE;	//	�}���`�T���v�����O����
	wireframeDesc.AntialiasedLineEnable = FALSE;	//	���C����A���`�G�C���A�V���O����
	
	//	rasterizer state���쐬����
	if (!DX_Debug::GetInstance()->IsHresultCheck(pDevice->CreateRasterizerState(&wireframeDesc, &m_pWireFrameRS))) {
		throw "ID3D11Device::CreateRasterizerState() : failed";
	}

#endif
}



//-----------------------------------------------------------------------------------------
//
//  �u�����h�X�e�[�g���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::CreateBlendState(ID3D11Device* pDevice)
{
	D3D11_BLEND_DESC l_blendDesc;
	ZeroMemory(&l_blendDesc, sizeof(l_blendDesc));

	l_blendDesc.AlphaToCoverageEnable		= TRUE;
	l_blendDesc.IndependentBlendEnable		= FALSE; // RenderTarget0�̂�blende�ݒ���g�p����
	l_blendDesc.RenderTarget[0].BlendEnable = FALSE;
	l_blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // �S���̐F����������
	
	////	�ǂ��u�����h���邩
	//l_blendDesc.RenderTarget[0].BlendOp			= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	//l_blendDesc.RenderTarget[0].BlendOpAlpha	= D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

	////	Blend�̑Ώۂ͂Ȃ�Ȃ̂�
	//l_blendDesc.RenderTarget[0].SrcBlend	= D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	//l_blendDesc.RenderTarget[0].DestBlend	= D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;

	////	Blend�W���͂����炩
	//l_blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND::D3D11_BLEND_ONE;
	//l_blendDesc.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND::D3D11_BLEND_ZERO;
	//l_blendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;


	//	blendState���쐬����
	if (!DX_Debug::GetInstance()->IsHresultCheck(pDevice->CreateBlendState(&l_blendDesc, &m_pBlendState))){
		throw "ID3D11Device::CreateBlendState() : failed";
	}
}


//-----------------------------------------------------------------------------------------
//
//  �[�x��X�e���V���X�e�[�g���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::CreateDepthStencilState(ID3D11Device* pDevice)
{
	D3D11_DEPTH_STENCIL_DESC l_dsDesc;
	ZeroMemory(&l_dsDesc, sizeof(l_dsDesc));

	//	depth test parameters
	l_dsDesc.DepthEnable	= TRUE;
	l_dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	l_dsDesc.DepthFunc		= D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL; // z�l����O�ŕ`�悳�ꂽ���̂�菬�����ꍇ�A�`�悵�܂�

	// Stencil test parameters
	l_dsDesc.StencilEnable = FALSE;
	
	//	depth stencil state���쐬����
	if (!DX_Debug::GetInstance()->IsHresultCheck(pDevice->CreateDepthStencilState(&l_dsDesc, &m_pDepthStencilState))){
		throw "ID3D11Device::CreateDepthStencilState() : failed";
	}
}


//-----------------------------------------------------------------------------------------
//
//  �T���v���[�X�e�[�g���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::CreateSamplerState(ID3D11Device* pDevice)
{
	D3D11_SAMPLER_DESC l_samplerDesc;
	ZeroMemory(&l_samplerDesc, sizeof(l_samplerDesc));

	l_samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	l_samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	l_samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	l_samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	l_samplerDesc.MipLODBias = .0f;
	l_samplerDesc.MaxAnisotropy = 0;
	l_samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	l_samplerDesc.BorderColor[0] = .0f;
	l_samplerDesc.BorderColor[1] = .0f;
	l_samplerDesc.BorderColor[2] = .0f;
	l_samplerDesc.BorderColor[3] = .0f;
	l_samplerDesc.MinLOD = 0;
	l_samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (!DX_Debug::GetInstance()->IsHresultCheck(pDevice->CreateSamplerState(&l_samplerDesc, &m_pSamplerState))){
		throw "ID3D11Device::CreateSamplerState() : failed";
	}
}
