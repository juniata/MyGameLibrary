#include	"DX_Library.h"
#include<stdio.h>

//-----------------------------------------------------------------------------------------
//
//	static�����o�ϐ�
//
//-----------------------------------------------------------------------------------------

const char*		SHADER_OBJECT_2D::VERTEX_SHADER			= "VS_Object2D.hlsl";
const char*		SHADER_OBJECT_2D::PIXEL_SHADER			= "PS_Object2D.hlsl";

const char*		SHADER_OBJECT_3D::VERTEX_SHADER		= "VS_Object3D.hlsl";
const char*		SHADER_OBJECT_3D::PIXEL_SHADER		= "PS_Object3D.hlsl";

const char*		SHADER_BOX::VERTEX_SHADER	= "VS_Box.hlsl";
const char*		SHADER_BOX::PIXEL_SHADER	= "PS_Box.hlsl";


/// <summary>
/// �ϐ��̏��������s��
/// </summary>
DX_ShaderManager::DX_ShaderManager()
{
	ZeroMemory(m_shaders, sizeof(m_shaders));
}


/// <summary>
/// �����o�ϐ��̏������y�уV�F�[�_�[�̍쐬���s��
/// </summary>
bool DX_ShaderManager::Initialize()
{
	auto succeed = false;
	
	m_constantBuffer = DX_BufferCreater::ConstantBuffer(sizeof(DirectX::XMFLOAT4X4));

	do
	{
		//	2D�`��p�V�F�[�_�[���쐬
		if (CreateShader(SHADER_OBJECT_2D::VERTEX_SHADER) == false)
		{
			break;
		}
		if (CreateShader(SHADER_OBJECT_2D::PIXEL_SHADER) == false)
		{
			break;
		}
		// 3D�`��p�V�F�[�_�[���쐬
		if (CreateShader(SHADER_OBJECT_3D::VERTEX_SHADER) == false)
		{
			break;
		}
		if (CreateShader(SHADER_OBJECT_3D::PIXEL_SHADER) == false)
		{
			break;
		}
		// Box�`��p�V�F�[�_�[���쐬
		if (CreateShader(SHADER_BOX::VERTEX_SHADER) == false)
		{
			break;
		}
		if (CreateShader(SHADER_BOX::PIXEL_SHADER) == false)
		{
			break;
		}

		if (CreateInputLayout() == false)
		{
			break;
		}

		succeed = true;
	} while (false);
	

	return succeed;
}

/// <summary>
/// �V�F�[�_�[�Ɋւ���ϐ������
/// </summary>
DX_ShaderManager::~DX_ShaderManager()
{
	for (size_t i = 0; i < SHADER_NUM; ++i)
	{
		DELETE_OBJ(m_shaders[i].shader);
	}
}

/// <summary>
/// �w�肵���V�F�[�_�[���擾����
/// </summary>
/// <param name="filepath">�t�@�C���p�X</param>
/// <returns></returns>
DX_Shader* DX_ShaderManager::GetShader(const char* filepath)
{
	DX_Shader* shader = nullptr;

	for (int i = 0; i < SHADER_NUM; ++i)
	{
		if (m_shaders[i].filepath == filepath)
		{
			shader = m_shaders[i].shader;
			break;
		}
	}

	return shader;
}

/// <summary>
/// 2D�`��pInputLayout���擾����
/// </summary>
/// <returns>2D�`��pInputLayout</returns>
ID3D11InputLayout* DX_ShaderManager::GetInputLayout2D()
{
	return m_inputLayout2D.Get();
}

/// <summary>
/// 3D�`��pInputLayout���擾����
/// </summary>
/// <returns>3D�`��pInputLayout</returns>
ID3D11InputLayout* DX_ShaderManager::GetInputLayout3D()
{
	return m_inputLayout3D.Get();
}

/// <summary>
/// Box�`��pInputLayout���擾����
/// </summary>
/// <returns>Box�`��pInputLayout</returns>
ID3D11InputLayout* DX_ShaderManager::GetInputLayoutBox()
{
	return m_inputLayoutBox.Get();
}

/// <summary>
/// ���[���h�s��𑗂�
/// ���W�X�^�ԍ���1
/// </summary>
/// <param name="worldMat">���[���h�s��</param>
/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
void DX_ShaderManager::SetWorldMat(const DirectX::XMFLOAT4X4& worldMat, SHADER_TYPE	shaderType)
{
	SetMatrix(1, worldMat, shaderType);
}

/// <summary>
/// int���V�F�[�_�[�ɑ���
/// </summary>
/// <param name="registerNum">�g�p���郌�W�X�^�ԍ�</param>
/// <param name="value">����l</param>
/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
void DX_ShaderManager::SetInt(const unsigned int registerNum, const int value, SHADER_TYPE shaderType)
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = DX_BufferCreater::ConstantBuffer(sizeof(DirectX::XMINT4));

	ID3D11DeviceContext* pDeviceContext = DX_System::GetInstance()->GetDeviceContext();

	pDeviceContext->UpdateSubresource(buffer.Get(), 0, nullptr, &value, 0, 0);

	DX_ShaderManager::SetConstantBuffers(registerNum, 1, buffer.GetAddressOf(), shaderType);
}

/// <summary>
/// Vector2���V�F�[�_�[�ɑ���
/// </summary>
/// <param name="registerNum">�g�p���郌�W�X�^�ԍ�</param>
/// <param name="value">����l</param>
/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
void DX_ShaderManager::SetVector2(const unsigned int registerNum, const DirectX::XMFLOAT2& vec2, SHADER_TYPE shaderType)
{
	SetVector4(registerNum, DirectX::XMFLOAT4(vec2.x, vec2.y, 0.0f, 0.0f), shaderType);
}

/// <summary>
/// Vector3���V�F�[�_�[�ɑ���
/// </summary>
/// <param name="registerNum">�g�p���郌�W�X�^�ԍ�</param>
/// <param name="value">����l</param>
/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
void DX_ShaderManager::SetVector3(const unsigned int registerNum, const DirectX::XMFLOAT3& vec3, SHADER_TYPE shaderType)
{
	SetVector4(registerNum, DirectX::XMFLOAT4(vec3.x, vec3.y, vec3.z, 0.0f), shaderType);
}


/// <summary>
/// Vector4���V�F�[�_�[�ɑ���
/// </summary>
/// <param name="registerNum">�g�p���郌�W�X�^�ԍ�</param>
/// <param name="value">����l</param>
/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
void DX_ShaderManager::SetVector4(const unsigned int registerNum, const DirectX::XMFLOAT4& value, SHADER_TYPE shaderType)
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = DX_BufferCreater::ConstantBuffer(sizeof(DirectX::XMFLOAT4));

	ID3D11DeviceContext* pDeviceContext = DX_System::GetInstance()->GetDeviceContext();

	pDeviceContext->UpdateSubresource(buffer.Get(), 0, nullptr, &value, 0, 0);

	DX_ShaderManager::SetConstantBuffers(registerNum, 1, buffer.GetAddressOf(), shaderType);
}

/// <summary>
/// Vector2�����\�[�X�Ƃ��ăV�F�[�_�[�ɑ���
/// </summary>
/// <param name="registerNum">�g�p���郌�W�X�^�ԍ�</param>
/// <param name="vecList">����l�̃��X�g</param>
/// <param name="shader_type">����V�F�[�_�[�̃^�C�v</param>
void DX_ShaderManager::SetVector2Resource(const unsigned int registerNum, const DirectX::XMFLOAT2* vecList, const unsigned int vecCount, SHADER_TYPE shaderType)
{
	SetVectorResource(registerNum, vecList, vecCount, sizeof(DirectX::XMFLOAT2), shaderType);
}

/// <summary>
/// Vector3�����\�[�X�Ƃ��ăV�F�[�_�[�ɑ���
/// </summary>
/// <param name="registerNum">�g�p���郌�W�X�^�ԍ�</param>
/// <param name="vecList">����l�̃��X�g</param>
/// <param name="shader_type">����V�F�[�_�[�̃^�C�v</param>
void DX_ShaderManager::SetVector3Resource(const unsigned int registerNum, const DirectX::XMFLOAT3* vecList, const unsigned int vecCount, SHADER_TYPE shaderType)
{
	SetVectorResource(registerNum, vecList, vecCount, sizeof(DirectX::XMFLOAT3), shaderType);
}

/// <summary>
/// Vector4�����\�[�X�Ƃ��ăV�F�[�_�[�ɑ���
/// </summary>
/// <param name="registerNum">�g�p���郌�W�X�^�ԍ�</param>
/// <param name="vecList">����l�̃��X�g</param>
/// <param name="shader_type">����V�F�[�_�[�̃^�C�v</param>
void DX_ShaderManager::SetVector4Resource(const unsigned int registerNum, const DirectX::XMFLOAT4* vecList, const unsigned int vecCount, SHADER_TYPE shaderType)
{
	SetVectorResource(registerNum, vecList, vecCount, sizeof(DirectX::XMFLOAT4), shaderType);
}


/// <summary>
/// �s����V�F�[�_�[�ɑ���
/// </summary>
/// <param name="regiserNum">�g�p���郌�W�X�^�ԍ�</param>
/// <param name="mat">����l</param>
/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
void DX_ShaderManager::SetMatrix(const unsigned int regiserNum, const DirectX::XMFLOAT4X4& mat, SHADER_TYPE	shaderType)
{
	ID3D11DeviceContext* deviceContext = DX_System::GetInstance()->GetDeviceContext();

	deviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &mat, 0, 0);

	SetConstantBuffers(regiserNum, 1, m_constantBuffer.GetAddressOf(), shaderType);
}

/// <summary>
/// �����̍s����V�F�[�_�[�ɑ���
/// </summary>
/// <param name="ppBuffer">�o�b�t�@</param>
/// <param name="regiserNum">�g�p���郌�W�X�^�ԍ�</param>
/// <param name="mat">����l</param>
/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
void DX_ShaderManager::SetMatrix(ID3D11Buffer* const* ppBuffer, const unsigned int regiserNum, const DirectX::XMFLOAT4X4* pMat, const unsigned int matCount, SHADER_TYPE shaderType)
{
	ID3D11DeviceContext* deviceContext = DX_System::GetInstance()->GetDeviceContext();

	deviceContext->UpdateSubresource(*ppBuffer, 0, nullptr, pMat, 0, 0);

	SetConstantBuffers(regiserNum, matCount, ppBuffer, shaderType);
}

/// <summary>
/// �����̍s����V�F�[�_�[�ɑ���
/// </summary>
/// <param name="regiserNum">�g�p���郌�W�X�^�̔ԍ�</param>
/// <param name="matList">�s��̃��X�g</param>
/// <param name="matCount">�s��̃��X�g�̐�</param>
/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
void DX_ShaderManager::SetMatrixResoruce(const unsigned int	registerNum, const DirectX::XMFLOAT4X4* matList, const unsigned int matCount, SHADER_TYPE shaderType)
{
	Microsoft::WRL::ComPtr<ID3D11Buffer>				buffer = DX_BufferCreater::CPUWriteBuffer(sizeof(DirectX::XMFLOAT4X4) * matCount);
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	srv;

	DX_System*				system			= DX_System::GetInstance();
	ID3D11Device*			device			= system->GetDevice();
	ID3D11DeviceContext*	deviceContext	= system->GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE	subResource = { NULL };
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	srvDesc.Buffer.ElementWidth = matCount * 4;
	srvDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension		= D3D11_SRV_DIMENSION_BUFFER;

	if (SUCCEEDED(device->CreateShaderResourceView(buffer.Get(), &srvDesc, srv.GetAddressOf())))
	{
		//	map
		deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

		//	�f�[�^����������
		CopyMemory(subResource.pData, matList, sizeof(DirectX::XMFLOAT4X4) * matCount);

		//	unmap
		deviceContext->Unmap(buffer.Get(), 0);

		SetShaderResources(registerNum, 1, srv.GetAddressOf(), shaderType);
	}
	else
	{
		TRACE("Failed to CreateShaderResourveVeiw()");
	}
}

/// <summary>
/// �����̍s����o�b�t�@�ŃV�F�[�_�[�ɑ���
/// </summary>
/// <param name="regiserNum">�g�p���郌�W�X�^�̔ԍ�</param>
/// <param name="matList">�s��̃��X�g</param>
/// <param name="matCount">�s��̃��X�g�̐�</param>
/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
void DX_ShaderManager::SetMatrixResoruce(const unsigned int registerNum, ID3D11Buffer* buffer, const unsigned int matCount, SHADER_TYPE shaderType)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	srv;

	DX_System*				system			= DX_System::GetInstance();
	ID3D11Device*			device			= system->GetDevice();

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	
	srvDesc.Buffer.ElementWidth	= matCount * 4;
	srvDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension		= D3D11_SRV_DIMENSION_BUFFER;

	//	SRV���쐬
	if (SUCCEEDED(device->CreateShaderResourceView(buffer, &srvDesc, srv.GetAddressOf())))
	{
		SetShaderResources(registerNum, 1, srv.GetAddressOf(), shaderType);
	}
	else
	{
		TRACE("Failed to CreateShaderResourveVeiw()");
	}
}

/// <summary>
/// �R���s���[�g�V�F�[�_�[���g���邩�ǂ�����
/// </summary>
/// <returns>�g�p�ł��邩�ǂ���</returns>
bool DX_ShaderManager::UsedComputeShader()
{
	D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS hardwareOptions;
	DX_System::GetInstance()->GetDevice()->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &hardwareOptions, sizeof(hardwareOptions));

	return (hardwareOptions.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x == TRUE) ? true : false;

}

/// <summary>
/// �V�F�[�_�[���쐬����
/// </summary>
/// <param name="filepath">�V�F�[�_�[�̃t�@�C���p�X</param>
/// <returns>����</returns>
bool DX_ShaderManager::CreateShader(const char* filepath)
{
	auto succeed = false;

	do
	{
		DX_Shader* shader = nullptr;

		if (strstr(filepath, "VS_"))
		{
			shader = new DX_VertexShader();
		}
		else if (strstr(filepath, "PS_"))
		{
			shader = new  DX_PixelShader();
		}
		else if (strstr(filepath, "GS_"))
		{
			shader = new  DX_GeometryShader();
		}
		else if (strstr(filepath, "CS_"))
		{
			// TODO:�t�@�C�����`�F�b�N����i�K�ł͂Ȃ��A�V�X�e�����������Ƀ`�F�b�N����ׂ����̂ł́H
			if (UsedComputeShader() == false)
			{
				TRACE("ComputerShader�͎g���܂���I");
				break;
			}
			shader = new  DX_ComputeShader();
		}
		else 
		{
			TRACE("�V�F�[�_�[�̃t�@�C�������Ԉ���Ă��邩�A�Ή����Ă��Ȃ��V�F�[�_�[�t�@�C����ǂݍ������Ƃ��Ă��܂��B");
			break;
		}

		char path[_MAX_PATH] = { '\0' };
		sprintf_s(path, sizeof(path), "%s%s", "Source\\ShaderFile\\", filepath);

		if (shader->CreateShader(path) == false)
		{
			break;
		}

		// TODO:�V�F�[�_�[��������ɍ쐬�ł��邩�ǂ����̃`�F�b�N���K�v
		for (int i = 0; i < SHADER_NUM; ++i)
		{
			if (m_shaders[i].filepath == nullptr)
			{
				m_shaders[i].shader		= shader;
				m_shaders[i].filepath	= filepath;
				break;
			}
		}

		succeed = true;
	} while (false);

	return succeed;
}

/// <summary>
/// �C���v�b�g���C�A�E�g���쐬����
/// </summary>
bool DX_ShaderManager::CreateInputLayout()
{
	auto succeed = false;

	// 2D�p���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout2D[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// 2D�p�C���X�^���X���C�A�E�g
	//D3D11_INPUT_ELEMENT_DESC layout2DInstance[] = {
	//	{ "POSITION",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD",		0,	DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "INSTANCE_POS",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	1, 0,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	//	{ "INSTANCE_UV",	0,	DXGI_FORMAT_R32G32_FLOAT,		2, 0,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	//};

	// 3D�p���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout3D[] =
	{
		{ "POSITION",		0,	DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",			0,	DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",			0,	DXGI_FORMAT_R32G32B32A32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",		0,	DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 }

	};

	// BOX�p���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layoutBox[] =
	{
		{ "POSITION",		0,	DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",			0,	DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",			0,	DXGI_FORMAT_R32G32B32A32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	do
	{
		DX_ShaderManager* shaderManager = DX_ShaderManager::GetInstance();

		//	2D�p���C�A�E�g���쐬
		if (CreateInputLayout(layout2D, _countof(layout2D), shaderManager->GetShader(SHADER_OBJECT_2D::VERTEX_SHADER)->GetByteCord(), &m_inputLayout2D) == false)
		{
			TRACE("failed to CreateInputLayout(), layout2D")
			break;
		}

		// 3D�p���C�A�E�g���쐬
		if (CreateInputLayout(layout3D, _countof(layout3D), shaderManager->GetShader(SHADER_OBJECT_3D::VERTEX_SHADER)->GetByteCord(), &m_inputLayout3D) == false)
		{
			TRACE("failed to CreateInputLayout(), layout3D")
			break;
		}

		// Box�p���C�A�E�g���쐬
		if (CreateInputLayout(layoutBox, _countof(layoutBox), shaderManager->GetShader(SHADER_BOX::VERTEX_SHADER)->GetByteCord(), &m_inputLayoutBox) == false)
		{
			TRACE("failed to CreateInputLayout(), layoutBox")
			break;
		}
		
		succeed = true;
	} while (false);

	return succeed;
}

/// <summary>
/// �w�肵���C���v�b�g���C�A�E�g���쐬����
/// </summary>
/// <param name="inputLayoutDesc">�쐬����C���v�b�g���C�A�E�g�̐ݒ�ꗗ</param>
/// <param name="inputLayoutCount">�쐬����C���v�b�g���C�A�E�g�̐ݒ萔</param>
/// <param name="byteCord">�쐬����C���v�b�g���C�A�E�g���g�p����V�F�[�_�[�̃o�C�g�R�[�h</param>
/// <param name="inputLayout">�쐬�����C���v�b�g���C�A�E�g</param>
bool DX_ShaderManager::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC*	inputLayoutDesc, const unsigned int inputLayoutCount, ID3DBlob* byteCord, ID3D11InputLayout** inputLayout)
{
	ID3D11Device* device = DX_System::GetInstance()->GetDevice();

	HRESULT hr = device->CreateInputLayout(inputLayoutDesc, inputLayoutCount, byteCord->GetBufferPointer(), byteCord->GetBufferSize(), inputLayout);

	if (DX_Debug::GetInstance()->IsFailedHresult(hr))
	{
		return false;
	}

	return true;
}

/// <summary>
/// �萔�o�b�t�@���V�F�[�_�[�ɑ���
/// </summary>
/// <param name="registerNum">�g�p���郌�W�X�^�̔ԍ�</param>
/// <param name="numBuffers">�o�b�t�@�̐�</param>
/// <param name="constantBuffers">�V�F�[�_�[�ɑ���o�b�t�@�̃��X�g</param>
/// <param name="shaderType">�g�p����V�F�[�_�[�̃^�C�v</param>
void DX_ShaderManager::SetConstantBuffers(UINT registerNum, UINT numBuffers, ID3D11Buffer* const* constantBuffers, SHADER_TYPE shaderType)
{
	ID3D11DeviceContext* deviceContext = DX_System::GetInstance()->GetDeviceContext();

	switch (shaderType) {
	case SHADER_TYPE::VERTEX_SHADER:
		deviceContext->VSSetConstantBuffers(registerNum, numBuffers, constantBuffers);
		break;

	case SHADER_TYPE::PIXEL_SHADER:
		deviceContext->PSSetConstantBuffers(registerNum, numBuffers, constantBuffers);
		break;

	case SHADER_TYPE::COMPUTE_SHADER:
		deviceContext->CSSetConstantBuffers(registerNum, numBuffers, constantBuffers);
		break;

	case SHADER_TYPE::GEOMETRY_SHADER:
		deviceContext->GSSetConstantBuffers(registerNum, numBuffers, constantBuffers);
		break;
	}
}

/// <summary>
/// ���\�[�X���V�F�[�_�[�ɑ���
/// </summary>
/// <param name="registerNum">�g�p���郌�W�X�^�̔ԍ�</param>
/// <param name="numBuffers">�g�p���郊�\�[�X�̐�</param>
/// <param name="shaderResourceView">�V�F�[�_�[�ɑ��郊�\�[�X�̃��X�g</param>
/// <param name="shaderType">�g�p����V�F�[�_�[�̃^�C�v</param>
void DX_ShaderManager::SetShaderResources(const unsigned int registerNum, const unsigned int numBuffers, ID3D11ShaderResourceView* const* shaderResourceView, SHADER_TYPE shaderType)
{
	ID3D11DeviceContext* deviceContext = DX_System::GetInstance()->GetDeviceContext();

	switch (shaderType) {
	case SHADER_TYPE::VERTEX_SHADER:
		deviceContext->VSSetShaderResources(registerNum, numBuffers, shaderResourceView);
		break;

	case SHADER_TYPE::PIXEL_SHADER:
		deviceContext->PSSetShaderResources(registerNum, numBuffers, shaderResourceView);
		break;

	case SHADER_TYPE::COMPUTE_SHADER:
		deviceContext->CSSetShaderResources(registerNum, numBuffers, shaderResourceView);
		break;

	case SHADER_TYPE::GEOMETRY_SHADER:
		deviceContext->GSSetShaderResources(registerNum, numBuffers, shaderResourceView);
		break;
	}
}


/// <summary>
/// Vector2,3,4�����\�[�X�Ƃ��ăV�F�[�_�[�ɑ���
/// </summary>
/// <param name="registerNum">�g�p���郌�W�X�^�ԍ�</param>
/// <param name="vecList">����l�̃��X�g</param>
/// <param name="vecCount">���X�g�̗v�f�̐�</param>
/// <param name="elementSize">���X�g�̗v�f�̃T�C�Y</param>
/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
void DX_ShaderManager::SetVectorResource(const unsigned int registerNum, const void* vecList, const unsigned int vecCount, const unsigned int elementSize, SHADER_TYPE shaderType)
{
	const unsigned int listSize = vecCount * elementSize;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				buffer = DX_BufferCreater::CPUWriteBuffer(listSize);
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	srv;

	DX_System*		system = DX_System::GetInstance();
	ID3D11Device*	device = system->GetDevice();
	ID3D11DeviceContext* deviceContext = system->GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE	subResource = { NULL };
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	//	SRVDesc��ݒ�
	srvDesc.Buffer.ElementWidth = vecCount;
	srvDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension		= D3D11_SRV_DIMENSION_BUFFER;

	//	SRV���쐬
	if (SUCCEEDED(device->CreateShaderResourceView(buffer.Get(), &srvDesc, &srv)))
	{
		deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

		CopyMemory(subResource.pData, vecList, listSize);

		deviceContext->Unmap(buffer.Get(), 0);

		DX_ShaderManager::GetInstance()->SetShaderResources(registerNum, 1, srv.GetAddressOf(), shaderType);
	}
	else
	{
		TRACE("Failed to CreateShaderResourceView()");
	}
}