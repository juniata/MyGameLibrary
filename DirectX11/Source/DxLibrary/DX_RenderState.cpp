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
void DX_RenderState::Initialize(ID3D11Device* pDevice)
{
	PROFILE("DX_RenderState::Initialize()");

	DX_RenderState* pRenderState = GetInstance();

	//	���X�^���C�U�[�X�e�[�g���쐬
	pRenderState->CreateRasterizerState(pDevice);

	//	�u�����h�X�e�[�g���쐬
	pRenderState->CreateBlendState(pDevice);

	//	�[�x��X�e���V���X�e�[�g���쐬
	pRenderState->CreateDepthStencilState(pDevice);
		
	//	�T���v���[�X�e�[�g���쐬
	pRenderState->CreateSamplerState(pDevice);
}

//-----------------------------------------------------------------------------------------
//
//  ���X�^���C�U�X�e�[�g���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11RasterizerState* DX_RenderState::GetDefaultRasterizerState() const
{
	return m_pRasterizerState;
}

//-----------------------------------------------------------------------------------------
//
//  �u�����h�X�e�[�g���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11BlendState* DX_RenderState::GetDefaultBlendState() const
{
	return m_pBlendState;
}

//-----------------------------------------------------------------------------------------
//
//  �[�x��X�e���V���X�e�[�g���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11DepthStencilState* DX_RenderState::GetDefaultDepthStencilState() const
{
	return m_pDepthStencilState;
}

//-----------------------------------------------------------------------------------------
//
//  �T���v���[�X�e�[�g���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11SamplerState* DX_RenderState::GetDefaultSamplerState() const
{
	return m_pSamplerState;
}

//-----------------------------------------------------------------------------------------
//
//  ���X�^���C�U�[�X�e�[�g���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::CreateRasterizerState(ID3D11Device* pDevice)
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
	DX_Debug::GetInstance()->ThrowIfFailed(pDevice->CreateRasterizerState(&desc, &m_pRasterizerState), "ID3D11Device::CreateRasterizerState() : failed");
}



//-----------------------------------------------------------------------------------------
//
//  �u�����h�X�e�[�g���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::CreateBlendState(ID3D11Device* pDevice)
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.AlphaToCoverageEnable		= TRUE;
	desc.IndependentBlendEnable		= FALSE; // RenderTarget0�̂�blende�ݒ���g�p����
	desc.RenderTarget[0].BlendEnable = FALSE;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // �S���̐F����������
	
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
	DX_Debug::GetInstance()->ThrowIfFailed(pDevice->CreateBlendState(&desc, &m_pBlendState), "ID3D11Device::CreateBlendState() : failed");
}


//-----------------------------------------------------------------------------------------
//
//  �[�x��X�e���V���X�e�[�g���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::CreateDepthStencilState(ID3D11Device* pDevice)
{
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	//	depth test parameters
	desc.DepthEnable	= TRUE;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc		= D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL; // z�l����O�ŕ`�悳�ꂽ���̂�菬�����ꍇ�A�`�悵�܂�

	// Stencil test parameters
	desc.StencilEnable = FALSE;
	
	//	depth stencil state���쐬����
	DX_Debug::GetInstance()->ThrowIfFailed(pDevice->CreateDepthStencilState(&desc, &m_pDepthStencilState), "ID3D11Device::CreateDepthStencilState() : failed");
}


//-----------------------------------------------------------------------------------------
//
//  �T���v���[�X�e�[�g���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_RenderState::CreateSamplerState(ID3D11Device* pDevice)
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = .0f;
	desc.MaxAnisotropy = 0;
	desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	desc.BorderColor[0] = .0f;
	desc.BorderColor[1] = .0f;
	desc.BorderColor[2] = .0f;
	desc.BorderColor[3] = .0f;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	DX_Debug::GetInstance()->ThrowIfFailed(pDevice->CreateSamplerState(&desc, &m_pSamplerState), "ID3D11Device::CreateSamplerState() : failed");
}
