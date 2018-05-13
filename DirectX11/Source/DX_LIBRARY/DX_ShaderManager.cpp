#include	"DX_Library.h"
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//	�}�N����`
//
//-----------------------------------------------------------------------------------------
//	3D�`��p
#define DEFAULT_VERTEX_SHADER_3D		"ShaderFile\\VS_3DObject.hlsl"
#define DEFAULT_GEOMETRY_SHADER_3D		"ShaderFile\\GS_3DObject.hlsl"
#define DEFAULT_PIXEL_SHADER_3D			"ShaderFile\\PS_3DObject.hlsl"
#define DEFAULT_VERTEX_SHADER_SKIN_MESH	"ShaderFile\\VS_SkinMesh.hlsl"
#define DEFAULT_GEOMETRY_SHADER_RAYPICK	"ShaderFile\\GS_RayPick.hlsl"

//	�C���X�^���X�`�掞�̌v�Z�p
#define COMPUTE_SHADER_INSTANCE_3D			"ShaderFile\\CS_InstanceMesh.hlsl"
#define VARTEX_SHADER_INSTANCE_3D			"ShaderFile\\VS_InstanceMesh.hlsl"
#define COMPUTE_SHADER_INSTANCE_SKIN_MESH	"ShaderFile\\CS_"
#define COMPUTE_SHADER_INSTANCE_PARTICLE	"ShaderFile\\CS_"

//-----------------------------------------------------------------------------------------
//
//	static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------
std::map<const char*, DX_Shader*>	DX_ShaderManager::m_shaders;
ID3D11InputLayout*	DX_ShaderManager::m_pInputLayout2D			= nullptr;
ID3D11InputLayout*	DX_ShaderManager::m_pInputLayout3D			= nullptr;
ID3D11InputLayout*	DX_ShaderManager::m_pInputLayoutSkinMesh	= nullptr;
ID3D11InputLayout*	DX_ShaderManager::m_pInputLayoutInstanceMesh = nullptr;
bool				DX_ShaderManager::m_bCanUsetoComputeShader  = false;

//-----------------------------------------------------------------------------------------
//
//	�����o�ϐ��̏�����(�V�F�[�_�[�̍쐬)
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::Initialize()
{
	PROFILE("DX_ShaderManager::Initialize()");

	try{
		//	�R���s���[�g�V�F�[�_�[���g���邩���`�F�b�N
		UsedComputeShaderCheck();
		
		//	2D�`��p�V�F�[�_�[���쐬
		CreateShader(DEFAULT_2D_SHADER::VERTEX_SHADER);
		CreateShader(DEFAULT_2D_SHADER::PIXEL_SHADER);

		//	3D�`��p�V�F�[�_�[���쐬
		CreateShader(DEFAULT_VERTEX_SHADER_3D);
		CreateShader(DEFAULT_GEOMETRY_SHADER_3D);
		CreateShader(DEFAULT_PIXEL_SHADER_3D);
		CreateShader(DEFAULT_VERTEX_SHADER_SKIN_MESH);

		CreateShader(DEFAULT_GEOMETRY_SHADER_RAYPICK);

		//	3D�C���X�^���X�`��v�Z�p�V�F�[�_�[���쐬
		CreateShader(COMPUTE_SHADER_INSTANCE_3D);
		CreateShader(VARTEX_SHADER_INSTANCE_3D);


		//	�C���v�b�g���C�A�E�g���쐬
		CreateInputLayout();
	}
	catch (char* pMessage){
		throw pMessage;
	}

	D3D11_SO_DECLARATION_ENTRY l_declaration[] = {
		{ 0, "OUT_POS", 0, 0, 4 ,0 },
		{ 0, "OUT_NORMAL", 0, 0, 3, 0 }

	};

	unsigned int strides[] = { sizeof(DirectX::XMFLOAT4) + sizeof(DirectX::XMFLOAT3) };
	static_cast<DX_GeometryShader*>(m_shaders[DEFAULT_GEOMETRY_SHADER_RAYPICK])->CreateGeometryShaderWithStreamOutput(
		l_declaration,
		_countof(l_declaration),
		strides,
		_countof(strides)
		);
}

//-----------------------------------------------------------------------------------------
//
//	�V�F�[�_�[�Ɋւ���ϐ������
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::Release()
{
	for (auto itr = m_shaders.begin(); itr != m_shaders.end(); ++itr){
		DELETE_OBJ(itr->second);
	}
	m_shaders.clear();
}

DX_Shader*	DX_ShaderManager::GetShader(const char* pShaderFileName)
{
	return m_shaders[pShaderFileName];
}

//-----------------------------------------------------------------------------------------
//
//  �f�t�H���g��3D�`��p���_�V�F�[�_�[���擾
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetDefaultVertexShader3D()
{
	return m_shaders[DEFAULT_VERTEX_SHADER_3D];
}


//-----------------------------------------------------------------------------------------
//
//  �f�t�H���g�̃X�L�����b�V���`��p���_�V�F�[�_�[���擾
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetDefaultVertexShaderSkinMesh()
{
	return m_shaders[DEFAULT_VERTEX_SHADER_SKIN_MESH];
}


//-----------------------------------------------------------------------------------------
//
//  �f�t�H���g�̃��C�s�b�N�����̓�����3D�`��p�W�I���g���V�F�[�_�[���擾
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetDefaultGeometryShader3D()
{
	return m_shaders[DEFAULT_GEOMETRY_SHADER_3D];
}


//-----------------------------------------------------------------------------------------
//
//  �f�t�H���g�̂RD�`��p�W�I���g���V�F�[�_�[���擾
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetDefaultGeometryShaderRayPick()
{
	return m_shaders[DEFAULT_GEOMETRY_SHADER_RAYPICK];

}

//-----------------------------------------------------------------------------------------
//
//  �f�t�H���g��3D�`��p�s�N�Z���V�F�[�_�[���擾
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetDefaultPixelShader3D()
{
	return m_shaders[DEFAULT_PIXEL_SHADER_3D];
}

//-----------------------------------------------------------------------------------------
//
//  �C���X�^���X���b�V���p�R���s���[�g�V�F�[�_�[���擾
//
//-----------------------------------------------------------------------------------------
DX_Shader* DX_ShaderManager::GetInstanceMeshComputeShader()
{
	return m_shaders[COMPUTE_SHADER_INSTANCE_3D];
}

//-----------------------------------------------------------------------------------------
//
//  �f�t�H���g��2D�`��pInputLayout���擾����
//
//-----------------------------------------------------------------------------------------
ID3D11InputLayout* DX_ShaderManager::GetDefaultInputLayout2D()
{
	return m_pInputLayout2D;
}

//-----------------------------------------------------------------------------------------
//
//  �f�t�H���g��3D�`��pInputLayout���擾����
//
//-----------------------------------------------------------------------------------------
ID3D11InputLayout* DX_ShaderManager::GetDefaultInputLayout3D()
{
	return m_pInputLayout3D;
}

//-----------------------------------------------------------------------------------------
//
//  �f�t�H���g�̃X�L�����b�V���`��pInputLayout���擾����
//
//-----------------------------------------------------------------------------------------
ID3D11InputLayout* DX_ShaderManager::GetDefaultInputLayoutSkinMesh()
{
	return m_pInputLayoutSkinMesh;
}

//-----------------------------------------------------------------------------------------
//
//  �C���X�^���X���b�V���`��pInputLayout���擾����
//
//-----------------------------------------------------------------------------------------
ID3D11InputLayout* DX_ShaderManager::GetInputLayoutInstanceMesh()
{
	return m_pInputLayoutInstanceMesh;
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
	ComPtr<ID3D11Buffer>	l_buffer = DX_Buffer::CreateConstantBuffer(pDevice, sizeof(XMFLOAT4));

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_buffer.Get(), 0, nullptr, &vec4, 0, 0);

	//	�V�F�[�_�[�X�e�[�W���w�肵�A�萔�o�b�t�@�𑗂�
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &l_buffer, shaderType);
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
	ComPtr<ID3D11Buffer>	l_buffer = DX_Buffer::CreateConstantBuffer(pDevice, sizeof(DirectX::XMFLOAT4));

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_buffer.Get(), 0, nullptr, &DirectX::XMFLOAT4(vec3.x,vec3.y,vec3.z,0.0f), 0, 0);

	//	�V�F�[�_�[�X�e�[�W���w�肵�A�萔�o�b�t�@�𑗂�
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &l_buffer, shaderType);

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
	ComPtr<ID3D11Buffer>	l_buffer = DX_Buffer::CreateConstantBuffer(pDevice, sizeof(XMFLOAT4));

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_buffer.Get(), 0, nullptr, &XMFLOAT4(vec2.x,vec2.y,0.0f,0.0f), 0, 0);

	//	�V�F�[�_�[�X�e�[�W���w�肵�A�萔�o�b�t�@�𑗂�
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &l_buffer, shaderType);
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
	ComPtr<ID3D11Buffer> l_buffer = DX_Buffer::CreateConstantBuffer(DX_System::GetInstance()->GetDevice(), sizeof(XMFLOAT4X4));

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_buffer.Get(), 0, nullptr, &mat, 0, 0);

	//	�V�F�[�_�[�X�e�[�W���w�肵�A�萔�o�b�t�@�𑗂�
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &l_buffer, shaderType);
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
	ComPtr<ID3D11Buffer> l_buffer = DX_Buffer::CreateConstantBuffer(DX_System::GetInstance()->GetDevice(), sizeof(XMFLOAT4X4)* matCount);

	//	updateSubResource
	pDeviceContext->UpdateSubresource(l_buffer.Get(), 0, nullptr, pMat, 0, 0);

	//	�V�F�[�_�[�X�e�[�W���w�肵�A�萔�o�b�t�@�𑗂�
	DX_ResourceManager::SetConstantbuffers(pDeviceContext, regiserNum, 1, &l_buffer, shaderType);

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
	ID3D11Device*				l_pDevice	= DX_System::GetInstance()->GetDevice();
	ComPtr<ID3D11Buffer>		l_buffer = DX_Buffer::CPUWriteBuffer(l_pDevice,sizeof(XMFLOAT4X4)* matCount);
	ComPtr<ID3D11ShaderResourceView>	l_srv;
	D3D11_MAPPED_SUBRESOURCE	l_subResource = { NULL };
	D3D11_SHADER_RESOURCE_VIEW_DESC l_srvDesc;
	ZeroMemory(&l_srvDesc, sizeof(l_srvDesc));


	//	SRVDesc��ݒ�
	l_srvDesc.Buffer.ElementWidth   = matCount * 4;		//	view���̗v�f��
	l_srvDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	l_srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFER;

	//	SRV���쐬
	if (FAILED(l_pDevice->CreateShaderResourceView(l_buffer.Get(), &l_srvDesc, &l_srv))){
		MessageBox(NULL, "l_pSRV", "error", MB_OK);
		exit(1);
	}

	//	map
	pDeviceContext->Map(l_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &l_subResource);

	//	�f�[�^����������
	CopyMemory(l_subResource.pData, pMat, sizeof(XMFLOAT4X4) * matCount);

	//	unmap
	pDeviceContext->Unmap(l_buffer.Get(), 0);

	DX_ResourceManager::SetShaderResources(pDeviceContext, regiserNum, 1, &l_srv, shaderType);
}
void DX_ShaderManager::SetMatrixResoruce(
	const unsigned int			regiserNum,
	ID3D11Buffer*				pBuffer,
	const unsigned int			matCount,
	ID3D11DeviceContext*		pDeviceContext,
	DX_SHADER_TYPE	shaderType
	)
{
	//	���[�J���ϐ�
	ID3D11Device*				l_pDevice = DX_System::GetInstance()->GetDevice();
	ComPtr<ID3D11ShaderResourceView>	l_srv;
	D3D11_SHADER_RESOURCE_VIEW_DESC l_srvDesc;
	ZeroMemory(&l_srvDesc, sizeof(l_srvDesc));


	//	SRVDesc��ݒ�
	l_srvDesc.Buffer.ElementWidth	= matCount * 4;		//	view���̗v�f��
	l_srvDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	l_srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFER;

	//	SRV���쐬
	if (FAILED(l_pDevice->CreateShaderResourceView(pBuffer, &l_srvDesc, &l_srv))){
		MessageBox(NULL, "l_pSRV", "error", MB_OK);
		exit(1);
	}

	DX_ResourceManager::SetShaderResources(pDeviceContext, regiserNum, 1, &l_srv, shaderType);
}
//-----------------------------------------------------------------------------------------
//
//	�R���s���[�g�V�F�[�_�[���g���邩�ǂ������`�F�b�N
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::UsedComputeShaderCheck()
{
	//	LEVEL_11_0�Ȃ�T�|�[�g����Ă���̂ŁA�`�F�b�N���Ȃ�
	//if (DX_System::GetFeatureLevel() == D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0){
	//	return;
	//}

	D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS l_hardwareOptions;


	DX_System::GetInstance()->GetDevice()->CheckFeatureSupport(
		D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS,
		&l_hardwareOptions,
		sizeof(l_hardwareOptions));

	//	�t���O��ݒ�
	m_bCanUsetoComputeShader = (l_hardwareOptions.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x == TRUE) ? true : false;

}
//-----------------------------------------------------------------------------------------
//
//	�V�F�[�_�[���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::CreateShader(const char* pFilepath)
{
	//	�����񂪖������NULL���Ԃ�
	if (strstr(pFilepath, "VS_")){
		m_shaders[pFilepath] = new  DX_VertexShader();
	}
	else if (strstr(pFilepath, "PS_")){
		m_shaders[pFilepath] = new  DX_PixelShader();
	}
	else if (strstr(pFilepath, "GS_")){
		m_shaders[pFilepath] = new DX_GeometryShader();
	}
	else if (strstr(pFilepath, "CS_")){
		if (m_bCanUsetoComputeShader == false){
			throw "ComputeShader�͎g���܂���!";
		}
		m_shaders[pFilepath] = new DX_ComputeShader();
	}

	m_shaders[pFilepath]->CreateShader(pFilepath);
}

//-----------------------------------------------------------------------------------------
//
//  InputLauout���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::CreateInputLayout()
{
	//	�f�o�C�X���擾
	ID3D11Device* l_pDevice = DX_System::GetInstance()->GetDevice();

	// 2D�p���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC l_layout2D[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//	3D�p���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC l_layout3D[] = {
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	
	//	3D�X�L�����b�V���p���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC l_layoutSkinMesh[] = {
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "SKIN_INDEX", 0, DXGI_FORMAT_R32_UINT,			1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SKIN_WEIGHT",0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//	�C���X�^���X���b�V���`��p���C�A�E�g���쐬
	D3D11_INPUT_ELEMENT_DESC l_insntanceMesh[] = {
		{ "POSITION",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",		0,	DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",			0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",			0,	DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "WORLD_MAT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD_MAT", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD_MAT", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD_MAT", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	
	try {
		//	2D�p���C�A�E�g���쐬
		CreateInputLayout(l_pDevice, l_layout2D, _countof(l_layout2D), m_shaders[DEFAULT_2D_SHADER::VERTEX_SHADER]->GetByteCord(), &m_pInputLayout2D);

		//	3D�p���C�A�E�g���쐬
		CreateInputLayout(l_pDevice, l_layout3D, _countof(l_layout3D), m_shaders[DEFAULT_VERTEX_SHADER_3D]->GetByteCord(), &m_pInputLayout3D);

		//	�X�L�����b�V���p���C�A�E�g���쐬
		CreateInputLayout(l_pDevice, l_layoutSkinMesh, _countof(l_layoutSkinMesh), m_shaders[DEFAULT_VERTEX_SHADER_SKIN_MESH]->GetByteCord(), &m_pInputLayoutSkinMesh);

		//	�C���X�^���X�X�L�����b�V���`��p���C�A�E�g���쐬
		CreateInputLayout(l_pDevice, l_insntanceMesh, _countof(l_insntanceMesh), m_shaders[VARTEX_SHADER_INSTANCE_3D]->GetByteCord(), &m_pInputLayoutInstanceMesh);
	}
	catch (char* pMessage){
		throw pMessage;
	}
}

//-----------------------------------------------------------------------------------------
//
//  InputLayout���쐬����
//
//-----------------------------------------------------------------------------------------
void DX_ShaderManager::CreateInputLayout(
	ID3D11Device*				pDevice,
	D3D11_INPUT_ELEMENT_DESC*	pInputLayoutDesc,
	const unsigned int			inputLayoutCount,
	ID3DBlob*					pByteCord,
	ID3D11InputLayout**			pInputLayout
	)
{
	HRESULT l_hr = pDevice->CreateInputLayout(pInputLayoutDesc, inputLayoutCount, pByteCord->GetBufferPointer(), pByteCord->GetBufferSize(), pInputLayout);

	//	�߂�l�`�F�b�N
	if (!DX_Debug::IsHresultCheck(l_hr)){
		throw "create InputLayout failed()";
	}

}

