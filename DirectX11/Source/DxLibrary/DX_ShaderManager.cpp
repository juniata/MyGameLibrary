#include	"DX_Library.h"
#include<stdio.h>

//-----------------------------------------------------------------------------------------
//
//	staticメンバ変数
//
//-----------------------------------------------------------------------------------------

const char*		SHADER_OBJECT_2D::VERTEX_SHADER			= "VS_Object2D.hlsl";
const char*		SHADER_OBJECT_2D::PIXEL_SHADER			= "PS_Object2D.hlsl";

const char*		SHADER_OBJECT_3D::VERTEX_SHADER		= "VS_Object3D.hlsl";
const char*		SHADER_OBJECT_3D::PIXEL_SHADER		= "PS_Object3D.hlsl";

const char*		SHADER_BOX::VERTEX_SHADER	= "VS_Box.hlsl";
const char*		SHADER_BOX::PIXEL_SHADER	= "PS_Box.hlsl";


/// <summary>
/// 変数の初期化を行う
/// </summary>
DX_ShaderManager::DX_ShaderManager()
{
	ZeroMemory(m_shaders, sizeof(m_shaders));
}


/// <summary>
/// メンバ変数の初期化及びシェーダーの作成を行う
/// </summary>
bool DX_ShaderManager::Initialize()
{
	auto succeed = false;
	
	m_constantBuffer = DX_BufferCreater::ConstantBuffer(sizeof(DirectX::XMFLOAT4X4));

	do
	{
		//	2D描画用シェーダーを作成
		if (CreateShader(SHADER_OBJECT_2D::VERTEX_SHADER) == false)
		{
			break;
		}
		if (CreateShader(SHADER_OBJECT_2D::PIXEL_SHADER) == false)
		{
			break;
		}
		// 3D描画用シェーダーを作成
		if (CreateShader(SHADER_OBJECT_3D::VERTEX_SHADER) == false)
		{
			break;
		}
		if (CreateShader(SHADER_OBJECT_3D::PIXEL_SHADER) == false)
		{
			break;
		}
		// Box描画用シェーダーを作成
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
/// シェーダーに関する変数を解放
/// </summary>
DX_ShaderManager::~DX_ShaderManager()
{
	for (size_t i = 0; i < SHADER_NUM; ++i)
	{
		DELETE_OBJ(m_shaders[i].shader);
	}
}

/// <summary>
/// 指定したシェーダーを取得する
/// </summary>
/// <param name="filepath">ファイルパス</param>
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
/// 2D描画用InputLayoutを取得する
/// </summary>
/// <returns>2D描画用InputLayout</returns>
ID3D11InputLayout* DX_ShaderManager::GetInputLayout2D()
{
	return m_inputLayout2D.Get();
}

/// <summary>
/// 3D描画用InputLayoutを取得する
/// </summary>
/// <returns>3D描画用InputLayout</returns>
ID3D11InputLayout* DX_ShaderManager::GetInputLayout3D()
{
	return m_inputLayout3D.Get();
}

/// <summary>
/// Box描画用InputLayoutを取得する
/// </summary>
/// <returns>Box描画用InputLayout</returns>
ID3D11InputLayout* DX_ShaderManager::GetInputLayoutBox()
{
	return m_inputLayoutBox.Get();
}

/// <summary>
/// ワールド行列を送る
/// レジスタ番号は1
/// </summary>
/// <param name="worldMat">ワールド行列</param>
/// <param name="shaderType">送るシェーダーのタイプ</param>
void DX_ShaderManager::SetWorldMat(const DirectX::XMFLOAT4X4& worldMat, SHADER_TYPE	shaderType)
{
	SetMatrix(1, worldMat, shaderType);
}

/// <summary>
/// intをシェーダーに送る
/// </summary>
/// <param name="registerNum">使用するレジスタ番号</param>
/// <param name="value">送る値</param>
/// <param name="shaderType">送るシェーダーのタイプ</param>
void DX_ShaderManager::SetInt(const unsigned int registerNum, const int value, SHADER_TYPE shaderType)
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = DX_BufferCreater::ConstantBuffer(sizeof(DirectX::XMINT4));

	ID3D11DeviceContext* pDeviceContext = DX_System::GetInstance()->GetDeviceContext();

	pDeviceContext->UpdateSubresource(buffer.Get(), 0, nullptr, &value, 0, 0);

	DX_ShaderManager::SetConstantBuffers(registerNum, 1, buffer.GetAddressOf(), shaderType);
}

/// <summary>
/// Vector2をシェーダーに送る
/// </summary>
/// <param name="registerNum">使用するレジスタ番号</param>
/// <param name="value">送る値</param>
/// <param name="shaderType">送るシェーダーのタイプ</param>
void DX_ShaderManager::SetVector2(const unsigned int registerNum, const DirectX::XMFLOAT2& vec2, SHADER_TYPE shaderType)
{
	SetVector4(registerNum, DirectX::XMFLOAT4(vec2.x, vec2.y, 0.0f, 0.0f), shaderType);
}

/// <summary>
/// Vector3をシェーダーに送る
/// </summary>
/// <param name="registerNum">使用するレジスタ番号</param>
/// <param name="value">送る値</param>
/// <param name="shaderType">送るシェーダーのタイプ</param>
void DX_ShaderManager::SetVector3(const unsigned int registerNum, const DirectX::XMFLOAT3& vec3, SHADER_TYPE shaderType)
{
	SetVector4(registerNum, DirectX::XMFLOAT4(vec3.x, vec3.y, vec3.z, 0.0f), shaderType);
}


/// <summary>
/// Vector4をシェーダーに送る
/// </summary>
/// <param name="registerNum">使用するレジスタ番号</param>
/// <param name="value">送る値</param>
/// <param name="shaderType">送るシェーダーのタイプ</param>
void DX_ShaderManager::SetVector4(const unsigned int registerNum, const DirectX::XMFLOAT4& value, SHADER_TYPE shaderType)
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = DX_BufferCreater::ConstantBuffer(sizeof(DirectX::XMFLOAT4));

	ID3D11DeviceContext* pDeviceContext = DX_System::GetInstance()->GetDeviceContext();

	pDeviceContext->UpdateSubresource(buffer.Get(), 0, nullptr, &value, 0, 0);

	DX_ShaderManager::SetConstantBuffers(registerNum, 1, buffer.GetAddressOf(), shaderType);
}

/// <summary>
/// Vector2をリソースとしてシェーダーに送る
/// </summary>
/// <param name="registerNum">使用するレジスタ番号</param>
/// <param name="vecList">送る値のリスト</param>
/// <param name="shader_type">送るシェーダーのタイプ</param>
void DX_ShaderManager::SetVector2Resource(const unsigned int registerNum, const DirectX::XMFLOAT2* vecList, const unsigned int vecCount, SHADER_TYPE shaderType)
{
	SetVectorResource(registerNum, vecList, vecCount, sizeof(DirectX::XMFLOAT2), shaderType);
}

/// <summary>
/// Vector3をリソースとしてシェーダーに送る
/// </summary>
/// <param name="registerNum">使用するレジスタ番号</param>
/// <param name="vecList">送る値のリスト</param>
/// <param name="shader_type">送るシェーダーのタイプ</param>
void DX_ShaderManager::SetVector3Resource(const unsigned int registerNum, const DirectX::XMFLOAT3* vecList, const unsigned int vecCount, SHADER_TYPE shaderType)
{
	SetVectorResource(registerNum, vecList, vecCount, sizeof(DirectX::XMFLOAT3), shaderType);
}

/// <summary>
/// Vector4をリソースとしてシェーダーに送る
/// </summary>
/// <param name="registerNum">使用するレジスタ番号</param>
/// <param name="vecList">送る値のリスト</param>
/// <param name="shader_type">送るシェーダーのタイプ</param>
void DX_ShaderManager::SetVector4Resource(const unsigned int registerNum, const DirectX::XMFLOAT4* vecList, const unsigned int vecCount, SHADER_TYPE shaderType)
{
	SetVectorResource(registerNum, vecList, vecCount, sizeof(DirectX::XMFLOAT4), shaderType);
}


/// <summary>
/// 行列をシェーダーに送る
/// </summary>
/// <param name="regiserNum">使用するレジスタ番号</param>
/// <param name="mat">送る値</param>
/// <param name="shaderType">送るシェーダーのタイプ</param>
void DX_ShaderManager::SetMatrix(const unsigned int regiserNum, const DirectX::XMFLOAT4X4& mat, SHADER_TYPE	shaderType)
{
	ID3D11DeviceContext* deviceContext = DX_System::GetInstance()->GetDeviceContext();

	deviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &mat, 0, 0);

	SetConstantBuffers(regiserNum, 1, m_constantBuffer.GetAddressOf(), shaderType);
}

/// <summary>
/// 複数の行列をシェーダーに送る
/// </summary>
/// <param name="ppBuffer">バッファ</param>
/// <param name="regiserNum">使用するレジスタ番号</param>
/// <param name="mat">送る値</param>
/// <param name="shaderType">送るシェーダーのタイプ</param>
void DX_ShaderManager::SetMatrix(ID3D11Buffer* const* ppBuffer, const unsigned int regiserNum, const DirectX::XMFLOAT4X4* pMat, const unsigned int matCount, SHADER_TYPE shaderType)
{
	ID3D11DeviceContext* deviceContext = DX_System::GetInstance()->GetDeviceContext();

	deviceContext->UpdateSubresource(*ppBuffer, 0, nullptr, pMat, 0, 0);

	SetConstantBuffers(regiserNum, matCount, ppBuffer, shaderType);
}

/// <summary>
/// 複数の行列をシェーダーに送る
/// </summary>
/// <param name="regiserNum">使用するレジスタの番号</param>
/// <param name="matList">行列のリスト</param>
/// <param name="matCount">行列のリストの数</param>
/// <param name="shaderType">送るシェーダーのタイプ</param>
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

		//	データを書き込む
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
/// 複数の行列をバッファでシェーダーに送る
/// </summary>
/// <param name="regiserNum">使用するレジスタの番号</param>
/// <param name="matList">行列のリスト</param>
/// <param name="matCount">行列のリストの数</param>
/// <param name="shaderType">送るシェーダーのタイプ</param>
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

	//	SRVを作成
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
/// コンピュートシェーダーが使えるかどうかを
/// </summary>
/// <returns>使用できるかどうか</returns>
bool DX_ShaderManager::UsedComputeShader()
{
	D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS hardwareOptions;
	DX_System::GetInstance()->GetDevice()->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &hardwareOptions, sizeof(hardwareOptions));

	return (hardwareOptions.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x == TRUE) ? true : false;

}

/// <summary>
/// シェーダーを作成する
/// </summary>
/// <param name="filepath">シェーダーのファイルパス</param>
/// <returns>成否</returns>
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
			// TODO:ファイルをチェックする段階ではなく、システム初期化時にチェックするべきものでは？
			if (UsedComputeShader() == false)
			{
				TRACE("ComputerShaderは使えません！");
				break;
			}
			shader = new  DX_ComputeShader();
		}
		else 
		{
			TRACE("シェーダーのファイル名が間違っているか、対応していないシェーダーファイルを読み込もうとしています。");
			break;
		}

		char path[_MAX_PATH] = { '\0' };
		sprintf_s(path, sizeof(path), "%s%s", "Source\\ShaderFile\\", filepath);

		if (shader->CreateShader(path) == false)
		{
			break;
		}

		// TODO:シェーダーをあたらに作成できるかどうかのチェックが必要
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
/// インプットレイアウトを作成する
/// </summary>
bool DX_ShaderManager::CreateInputLayout()
{
	auto succeed = false;

	// 2D用レイアウト
	D3D11_INPUT_ELEMENT_DESC layout2D[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// 2D用インスタンスレイアウト
	//D3D11_INPUT_ELEMENT_DESC layout2DInstance[] = {
	//	{ "POSITION",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD",		0,	DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "INSTANCE_POS",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	1, 0,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	//	{ "INSTANCE_UV",	0,	DXGI_FORMAT_R32G32_FLOAT,		2, 0,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	//};

	// 3D用レイアウト
	D3D11_INPUT_ELEMENT_DESC layout3D[] =
	{
		{ "POSITION",		0,	DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",			0,	DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",			0,	DXGI_FORMAT_R32G32B32A32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",		0,	DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 }

	};

	// BOX用レイアウト
	D3D11_INPUT_ELEMENT_DESC layoutBox[] =
	{
		{ "POSITION",		0,	DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,								D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",			0,	DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",			0,	DXGI_FORMAT_R32G32B32A32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	do
	{
		DX_ShaderManager* shaderManager = DX_ShaderManager::GetInstance();

		//	2D用レイアウトを作成
		if (CreateInputLayout(layout2D, _countof(layout2D), shaderManager->GetShader(SHADER_OBJECT_2D::VERTEX_SHADER)->GetByteCord(), &m_inputLayout2D) == false)
		{
			TRACE("failed to CreateInputLayout(), layout2D")
			break;
		}

		// 3D用レイアウトを作成
		if (CreateInputLayout(layout3D, _countof(layout3D), shaderManager->GetShader(SHADER_OBJECT_3D::VERTEX_SHADER)->GetByteCord(), &m_inputLayout3D) == false)
		{
			TRACE("failed to CreateInputLayout(), layout3D")
			break;
		}

		// Box用レイアウトを作成
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
/// 指定したインプットレイアウトを作成する
/// </summary>
/// <param name="inputLayoutDesc">作成するインプットレイアウトの設定一覧</param>
/// <param name="inputLayoutCount">作成するインプットレイアウトの設定数</param>
/// <param name="byteCord">作成するインプットレイアウトを使用するシェーダーのバイトコード</param>
/// <param name="inputLayout">作成したインプットレイアウト</param>
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
/// 定数バッファをシェーダーに送る
/// </summary>
/// <param name="registerNum">使用するレジスタの番号</param>
/// <param name="numBuffers">バッファの数</param>
/// <param name="constantBuffers">シェーダーに送るバッファのリスト</param>
/// <param name="shaderType">使用するシェーダーのタイプ</param>
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
/// リソースをシェーダーに送る
/// </summary>
/// <param name="registerNum">使用するレジスタの番号</param>
/// <param name="numBuffers">使用するリソースの数</param>
/// <param name="shaderResourceView">シェーダーに送るリソースのリスト</param>
/// <param name="shaderType">使用するシェーダーのタイプ</param>
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
/// Vector2,3,4をリソースとしてシェーダーに送る
/// </summary>
/// <param name="registerNum">使用するレジスタ番号</param>
/// <param name="vecList">送る値のリスト</param>
/// <param name="vecCount">リストの要素の数</param>
/// <param name="elementSize">リストの要素のサイズ</param>
/// <param name="shaderType">送るシェーダーのタイプ</param>
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

	//	SRVDescを設定
	srvDesc.Buffer.ElementWidth = vecCount;
	srvDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension		= D3D11_SRV_DIMENSION_BUFFER;

	//	SRVを作成
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