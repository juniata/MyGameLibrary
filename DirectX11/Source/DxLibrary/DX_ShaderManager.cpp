#include	"DX_Library.h"
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//	�V�F�[�_�[�̃t�@�C���p�X���`
//
//-----------------------------------------------------------------------------------------
const char* DEFAULT_2D_SHADER::VERTEX_SHADER			= "Source\\ShaderFile\\VS_2DObject.hlsl";
const char* DEFAULT_2D_SHADER::PIXEL_SHADER				= "Source\\ShaderFile\\PS_2DObject.hlsl";
const char* DEFAULT_2D_SHADER::INSTANCE_VERTEX_SHADER	= "Source\\ShaderFile\\VS_Instance2D.hlsl";

//-----------------------------------------------------------------------------------------
//
//	static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------
DX_ShaderManager* DX_ShaderManager::m_pInstance = nullptr;

//-----------------------------------------------------------------------------------------
//
//	������
//
//-----------------------------------------------------------------------------------------
DX_ShaderManager::DX_ShaderManager() : 
	m_bCanUsetoComputeShader(false),
	m_pDefaultVS2D(nullptr),
	m_pDefaultPS2D(nullptr),
	m_pDefaultInstanceVS2D(nullptr),
	m_pInputLayout2D(nullptr),
	m_pInputLayoutInstance2D(nullptr)
{}

//-----------------------------------------------------------------------------------------
//
//	���
//
//-----------------------------------------------------------------------------------------
DX_ShaderManager::~DX_ShaderManager()
{
	DELETE_OBJ(m_pDefaultVS2D);
	DELETE_OBJ(m_pDefaultPS2D);
	DELETE_OBJ(m_pDefaultInstanceVS2D);

	SAFE_RELEASE(m_pInputLayout2D);
	SAFE_RELEASE(m_pInputLayoutInstance2D);
}


//-----------------------------------------------------------------------------------------
//
//	�C���X�^���X���擾����
//
//-----------------------------------------------------------------------------------------
DX_ShaderManager* DX_ShaderManager::GetInstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance = new DX_ShaderManager();
	}
	return m_pInstance;
}

//-----------------------------------------------------------------------------------------
//
//	�����o�ϐ��̏�����(�V�F�[�_�[�̍쐬)
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::Initialize(ID3D11Device* pDevice)
{
	PROFILE("DX_ShaderManager::Initialize()");

	DX_ShaderManager* pShaderManager = GetInstance();

	//	�R���s���[�g�V�F�[�_�[���g���邩���`�F�b�N
	pShaderManager->UsedComputeShaderCheck(pDevice);
		
	//	2D�`��p�V�F�[�_�[���쐬
	pShaderManager ->m_pDefaultVS2D = pShaderManager->CreateShader(pDevice, DEFAULT_2D_SHADER::VERTEX_SHADER);
	pShaderManager ->m_pDefaultPS2D = pShaderManager->CreateShader(pDevice, DEFAULT_2D_SHADER::PIXEL_SHADER);
	pShaderManager ->m_pDefaultInstanceVS2D = pShaderManager->CreateShader(pDevice, DEFAULT_2D_SHADER::INSTANCE_VERTEX_SHADER);

	//	�C���v�b�g���C�A�E�g���쐬
	pShaderManager ->CreateInputLayout(pDevice);
}

//-----------------------------------------------------------------------------------------
//
//	�V�F�[�_�[�Ɋւ���ϐ������
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::Release()
{
	DELETE_OBJ(m_pInstance);
}

//-----------------------------------------------------------------------------------------
//
//	�f�t�H���g��2D���_�V�F�[�_�[���擾����
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetDefaultVertexShader2D() const
{
	return m_pDefaultVS2D;
}

//-----------------------------------------------------------------------------------------
//
//	�f�t�H���g��2D�s�N�Z���V�F�[�_�[���擾����
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetDefaultPixelShader2D() const
{
	return m_pDefaultPS2D;
}

//-----------------------------------------------------------------------------------------
//
//	�f�t�H���g�̃C���X�^���X2D���_�V�F�[�_�[���擾����
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetDefaultInstanceVertexShader2D() const
{
	return m_pDefaultInstanceVS2D;
}

//-----------------------------------------------------------------------------------------
//
//  �f�t�H���g��2D�`��pInputLayout���擾����
//
//-----------------------------------------------------------------------------------------
ID3D11InputLayout* DX_ShaderManager::GetDefaultInputLayout2D() const
{
	return m_pInputLayout2D;
}

//-----------------------------------------------------------------------------------------
//
//  �f�t�H���g�̃C���X�^���X2D�`��pInputLayout���擾����
//
//-----------------------------------------------------------------------------------------
ID3D11InputLayout* DX_ShaderManager::GetDefaultInputLayoutInstance2D() const
{
	return m_pInputLayoutInstance2D;
}

//-----------------------------------------------------------------------------------------
//
//  ���[���h�s����V�F�[�_�[�ɑ���
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetWorldMat(
	const DirectX::XMFLOAT4X4&				worldMat,
	ID3D11DeviceContext*		pDeviceContext,
	DX_SHADER_TYPE	shaderType
	)
{
	SetMatrix(1, worldMat, pDeviceContext, shaderType);
}

void DX_ShaderManager::SetInt(const unsigned int			registerNum,
	const DirectX::XMINT4&				vec4,
	ID3D11Device*				pDevice,
	ID3D11DeviceContext*		pDeviceContext,
	DX_SHADER_TYPE	shaderType)
{
	//	���[�J���ϐ�
	ID3D11Buffer*	l_pBuffer = DX_Buffer::CreateConstantBuffer(pDevice, sizeof(XMINT4));

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_pBuffer, 0, nullptr, &vec4, 0, 0);

	//	�V�F�[�_�[�X�e�[�W���w�肵�A�萔�o�b�t�@�𑗂�
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, registerNum, 1, &l_pBuffer, shaderType);

	SAFE_RELEASE(l_pBuffer);
}
//-----------------------------------------------------------------------------------------
//
//  Vector4���V�F�[�_�[�ɑ���
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetVector(
	const unsigned int				regiserNum,
	const DirectX::XMFLOAT4&					vec4,
	ID3D11Device*					pDevice,
	ID3D11DeviceContext*			pDeviceContext,
	DX_SHADER_TYPE		shaderType
	)
{
	//	���[�J���ϐ�
	ID3D11Buffer*	l_pBuffer = DX_Buffer::CreateConstantBuffer(pDevice, sizeof(XMFLOAT4));

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_pBuffer, 0, nullptr, &vec4, 0, 0);

	//	�V�F�[�_�[�X�e�[�W���w�肵�A�萔�o�b�t�@�𑗂�
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &l_pBuffer, shaderType);

	SAFE_RELEASE(l_pBuffer);
}

//-----------------------------------------------------------------------------------------
//
//  Vector3���V�F�[�_�[�ɑ���
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetVector(
	const unsigned int				regiserNum,
	const DirectX::XMFLOAT3&					vec3,
	ID3D11Device*					pDevice,
	ID3D11DeviceContext*			pDeviceContext,
	DX_SHADER_TYPE		shaderType
	)
{
	//	���[�J���ϐ�
	ID3D11Buffer*	l_pBuffer = DX_Buffer::CreateConstantBuffer(pDevice, sizeof(DirectX::XMFLOAT4));

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_pBuffer, 0, nullptr, &DirectX::XMFLOAT4(vec3.x,vec3.y,vec3.z,0.0f), 0, 0);

	//	�V�F�[�_�[�X�e�[�W���w�肵�A�萔�o�b�t�@�𑗂�
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &l_pBuffer, shaderType);

	SAFE_RELEASE(l_pBuffer);
}

void DX_ShaderManager::SetVectorResource(const unsigned int registerNum, const DirectX::XMFLOAT4* pVec3, const unsigned int vecCount, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, DX_SHADER_TYPE shaderType)
{
	//	���[�J���ϐ�
	ID3D11Buffer*	l_pBuffer = DX_Buffer::CPUWriteBuffer(pDevice, sizeof(XMFLOAT4) * vecCount);
	ID3D11ShaderResourceView* l_pSrv = nullptr;

	D3D11_MAPPED_SUBRESOURCE	l_subResource = { NULL };
	D3D11_SHADER_RESOURCE_VIEW_DESC l_srvDesc;
	ZeroMemory(&l_srvDesc, sizeof(l_srvDesc));


	//	SRVDesc��ݒ�
	l_srvDesc.Buffer.ElementWidth = vecCount;		//	view���̗v�f��
	l_srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	l_srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

	//	SRV���쐬
	if (FAILED(pDevice->CreateShaderResourceView(l_pBuffer, &l_srvDesc, &l_pSrv))) {
		MessageBox(NULL, "l_pSRV", "error", MB_OK);
		exit(1);
	}

	//	map
	pDeviceContext->Map(l_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &l_subResource);

	//	�f�[�^����������
	CopyMemory(l_subResource.pData, pVec3, sizeof(XMFLOAT4) * vecCount);

	//	unmap
	pDeviceContext->Unmap(l_pBuffer, 0);

	DX_ResourceManager::SetShaderResources(pDeviceContext, registerNum, 1, &l_pSrv, shaderType);

	SAFE_RELEASE(l_pBuffer);
	SAFE_RELEASE(l_pSrv);
}

//-----------------------------------------------------------------------------------------
//
//  Vector3���V�F�[�_�[�ɑ���
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetVector(
	const unsigned int				regiserNum,
	const XMFLOAT2&					vec2,
	ID3D11Device*					pDevice,
	ID3D11DeviceContext*			pDeviceContext,
	DX_SHADER_TYPE		shaderType
	)
{
	//	���[�J���ϐ�
	ID3D11Buffer*	l_pBuffer = DX_Buffer::CreateConstantBuffer(pDevice, sizeof(XMFLOAT4));

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_pBuffer, 0, nullptr, &XMFLOAT4(vec2.x,vec2.y,0.0f,0.0f), 0, 0);

	//	�V�F�[�_�[�X�e�[�W���w�肵�A�萔�o�b�t�@�𑗂�
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &l_pBuffer, shaderType);

	SAFE_RELEASE(l_pBuffer);
}


//-----------------------------------------------------------------------------------------
//
//	�s����V�F�[�_�[�ɑ���
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetMatrix(
	const unsigned int			regiserNum,
	const XMFLOAT4X4&				mat,
	ID3D11DeviceContext*		pDeviceContext,
	DX_SHADER_TYPE	shaderType
	)
{
	//	���[�J���ϐ�
	ID3D11Buffer* l_pBuffer = DX_Buffer::CreateConstantBuffer(DX_System::GetInstance()->GetDevice(), sizeof(XMFLOAT4X4));

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_pBuffer, 0, nullptr, &mat, 0, 0);

	//	�V�F�[�_�[�X�e�[�W���w�肵�A�萔�o�b�t�@�𑗂�
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &l_pBuffer, shaderType);

	SAFE_RELEASE(l_pBuffer);
}


//-----------------------------------------------------------------------------------------
//
//	�s��𕡐��V�F�[�_�[�ɑ���
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetMatrix(
	const unsigned int			regiserNum,
	const XMFLOAT4X4*				pMat,
	const unsigned int			matCount,
	ID3D11DeviceContext*		pDeviceContext,
	DX_SHADER_TYPE shaderType
	)
{
	//	���[�J���ϐ�
	ID3D11Buffer* l_pBuffer = DX_Buffer::CreateConstantBuffer(DX_System::GetInstance()->GetDevice(), sizeof(XMFLOAT4X4)* matCount);

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_pBuffer, 0, nullptr, pMat, 0, 0);

	//	�V�F�[�_�[�X�e�[�W���w�肵�A�萔�o�b�t�@�𑗂�
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &l_pBuffer, shaderType);

	SAFE_RELEASE(l_pBuffer);

}
//-----------------------------------------------------------------------------------------
//
//	�����̍s���SRV�Ƃ��ăV�F�[�_�[�ɑ���
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetMatrixResoruce(
	const unsigned int			regiserNum,
	const XMFLOAT4X4*				pMat,
	const unsigned int			matCount,
	ID3D11DeviceContext*		pDeviceContext,
	DX_SHADER_TYPE	shaderType
	)
{
	//	���[�J���ϐ�
	ID3D11Device*	l_pDevice = DX_System::GetInstance()->GetDevice();
	ID3D11Buffer*	l_pBuffer = DX_Buffer::CPUWriteBuffer(l_pDevice,sizeof(XMFLOAT4X4)* matCount);
	ID3D11ShaderResourceView* l_pSrv = nullptr;

	D3D11_MAPPED_SUBRESOURCE	l_subResource = { NULL };
	D3D11_SHADER_RESOURCE_VIEW_DESC l_srvDesc;
	ZeroMemory(&l_srvDesc, sizeof(l_srvDesc));


	//	SRVDesc��ݒ�
	l_srvDesc.Buffer.ElementWidth   = matCount * 4;		//	view���̗v�f��
	l_srvDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	l_srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFER;

	//	SRV���쐬
	if (FAILED(l_pDevice->CreateShaderResourceView(l_pBuffer, &l_srvDesc, &l_pSrv))){
		MessageBox(NULL, "l_pSRV", "error", MB_OK);
		exit(1);
	}

	//	map
	pDeviceContext->Map(l_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &l_subResource);

	//	�f�[�^����������
	CopyMemory(l_subResource.pData, pMat, sizeof(XMFLOAT4X4) * matCount);

	//	unmap
	pDeviceContext->Unmap(l_pBuffer, 0);

	DX_ResourceManager::SetShaderResources(pDeviceContext, regiserNum, 1, &l_pSrv, shaderType);

	SAFE_RELEASE(l_pBuffer);
	SAFE_RELEASE(l_pSrv);
}

//-----------------------------------------------------------------------------------------
//
//	��SRV�Ƃ��ăV�F�[�_�[�ɑ���
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::SetMatrixResoruce(
	const unsigned int			regiserNum,
	ID3D11Buffer*				pBuffer,
	const unsigned int			matCount,
	ID3D11DeviceContext*		pDeviceContext,
	DX_SHADER_TYPE	shaderType
	)
{
	//	���[�J���ϐ�
	ID3D11Device* l_pDevice = DX_System::GetInstance()->GetDevice();
	ID3D11ShaderResourceView*	l_pSrv = nullptr;
	D3D11_SHADER_RESOURCE_VIEW_DESC l_srvDesc;
	ZeroMemory(&l_srvDesc, sizeof(l_srvDesc));


	//	SRVDesc��ݒ�
	l_srvDesc.Buffer.ElementWidth	= matCount * 4;		//	view���̗v�f��
	l_srvDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	l_srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFER;

	//	SRV���쐬
	if (FAILED(l_pDevice->CreateShaderResourceView(pBuffer, &l_srvDesc, &l_pSrv))){
		MessageBox(NULL, "l_pSRV", "error", MB_OK);
		exit(1);
	}

	DX_ResourceManager::SetShaderResources(pDeviceContext, regiserNum, 1, &l_pSrv, shaderType);

	SAFE_RELEASE(l_pSrv);
}
//-----------------------------------------------------------------------------------------
//
//	�R���s���[�g�V�F�[�_�[���g���邩�ǂ������`�F�b�N
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::UsedComputeShaderCheck(ID3D11Device* pDevice)
{
	//	LEVEL_11_0�Ȃ�T�|�[�g����Ă���̂ŁA�`�F�b�N���Ȃ�
	//if (DX_System::GetFeatureLevel() == D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0){
	//	return;
	//}

	D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS l_hardwareOptions;
	
	DX_Debug::GetInstance()->ThrowIfFailed(pDevice->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &l_hardwareOptions, sizeof(l_hardwareOptions)), "CheckFeatureSupport() : failed");

	//	�t���O��ݒ�
	m_bCanUsetoComputeShader = (l_hardwareOptions.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x == TRUE) ? true : false;

}
//-----------------------------------------------------------------------------------------
//
//	�V�F�[�_�[���쐬����
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::CreateShader(ID3D11Device* pDevice, const char* pFilepath)
{
	DX_Shader* pShader = nullptr;

	//	�����񂪖������NULL���Ԃ�
	if (strstr(pFilepath, "VS_")){
		pShader = new  DX_VertexShader();
	}
	else if (strstr(pFilepath, "PS_")){
		pShader = new  DX_PixelShader();
	}
	else if (strstr(pFilepath, "GS_")){
		pShader = new  DX_GeometryShader();
	}
	else if (strstr(pFilepath, "CS_")){
		if (m_bCanUsetoComputeShader == false){
			MessageBox(NULL, "ComputeShader�͎g���܂���!", "error", MB_OK);
			throw;
		}
		pShader  = new  DX_ComputeShader();
	}
	else {
		MessageBox(NULL, "�w�肵�ăV�F�[�_�[�͑Ή����Ă��܂���B", "error", MB_OK);
		throw;
	}

	pShader->CreateShader(pDevice, pFilepath);

	return pShader;
}

//-----------------------------------------------------------------------------------------
//
//  InputLauout���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::CreateInputLayout(ID3D11Device* pDevice)
{
	// 2D�p���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout2D[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// 2D�p�C���X�^���X���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout2DInstance[] = {
		{ "POSITION",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",		0,	DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCE_POS",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	1, 0,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },

	};
	
	//	2D�p���C�A�E�g���쐬
	m_pInputLayout2D = CreateInputLayout(pDevice, layout2D, _countof(layout2D), m_pDefaultVS2D->GetByteCord());

	// 2D�C���X�^���X���C�A�E�g���쐬
	m_pInputLayoutInstance2D = CreateInputLayout(pDevice, layout2DInstance, _countof(layout2DInstance), m_pDefaultInstanceVS2D->GetByteCord());

}

//-----------------------------------------------------------------------------------------
//
//  InputLayout���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11InputLayout* DX_ShaderManager::CreateInputLayout(ID3D11Device* pDevice, D3D11_INPUT_ELEMENT_DESC*	pInputLayoutDesc, const unsigned int inputLayoutCount, ID3DBlob* pByteCord) const
{
	ID3D11InputLayout* pInputLayout = nullptr;
	DX_Debug::GetInstance()->ThrowIfFailed(pDevice->CreateInputLayout(pInputLayoutDesc, inputLayoutCount, pByteCord->GetBufferPointer(), pByteCord->GetBufferSize(), &pInputLayout), " InputLayout failed()");

	return pInputLayout;
}

