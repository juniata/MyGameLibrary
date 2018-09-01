#include	"DX_Library.h"

//-----------------------------------------------------------------------------------------
//
//  定数バッファを作成する
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateConstantBuffer(ID3D11Device*	pDevice, UINT bufferSize)
{
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= bufferSize;
	desc.Usage				= D3D11_USAGE_DEFAULT;
	desc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;

	return CreateBuffer(pDevice, &desc);
}

//-----------------------------------------------------------------------------------------
//
//  頂点バッファを作成する
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateVertexBuffer(ID3D11Device* pDevice, const size_t vertexDataSize, const void*	pVertex)
{
	//	bufferの設定
	D3D11_BUFFER_DESC		desc = { NULL };
	desc.ByteWidth		= static_cast<UINT>(vertexDataSize);
	desc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;

	//	subResourceDataの設定
	D3D11_SUBRESOURCE_DATA	subResource = { NULL };
	subResource.pSysMem = pVertex;

	return CreateBuffer(pDevice, &desc, &subResource);
}


//-----------------------------------------------------------------------------------------
//
//  構造化バッファを作成する
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateStructuredBuffer(ID3D11Device* pDevice, const size_t	elementSize, const size_t elementCount, void* pData)
{
	D3D11_BUFFER_DESC desc		= { NULL };
	desc.BindFlags				= D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth				= static_cast<UINT>(elementSize * elementCount);
	desc.MiscFlags				= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride	= static_cast<UINT>(elementSize);

	D3D11_SUBRESOURCE_DATA subResource = { NULL };
	subResource.pSysMem = pData;

	return CreateBuffer(pDevice, &desc, pData ? &subResource : nullptr);
}

//-----------------------------------------------------------------------------------------
//
//  バイトアドレスバッファを作成する
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateByteAddressBuffer(ID3D11Device* pDevice, const size_t dataSize, void* pData, const bool isVertex)
{
	D3D11_BUFFER_DESC desc		= { NULL };
	desc.BindFlags				= isVertex ? (D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_VERTEX_BUFFER) : (D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	desc.ByteWidth				= static_cast<UINT>(dataSize);
	desc.MiscFlags				= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	desc.StructureByteStride	= 0;

	D3D11_SUBRESOURCE_DATA subResource = { NULL };
	subResource.pSysMem = pData;

	return CreateBuffer(pDevice, &desc, pData ? &subResource : nullptr);
}

//-----------------------------------------------------------------------------------------
//
//  インデックスバッファを作成する
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateIndexBuffer(ID3D11Device* pDevice, const size_t indexDataSize, const void* pIndex)
{
	//	bufferの設定
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= static_cast<UINT>(indexDataSize);
	desc.BindFlags			= D3D11_BIND_INDEX_BUFFER;

	//	subResourceDataの設定
	D3D11_SUBRESOURCE_DATA	subResource = { NULL };
	subResource.pSysMem = pIndex;

	return CreateBuffer(pDevice, &desc, &subResource);
}

//-----------------------------------------------------------------------------------------
//
//  スキンバッファを作成する
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer*	DX_Buffer::CreateSkinBuffer(ID3D11Device* pDevice, const size_t skinDatasize, const void* pIndex)
{
	//	bufferの設定
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= static_cast<UINT>(skinDatasize);
	desc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;

	//	subResourceDataの設定
	D3D11_SUBRESOURCE_DATA	subResource = { NULL };
	subResource.pSysMem = pIndex;

	return CreateBuffer(pDevice, &desc, &subResource);
}

//-----------------------------------------------------------------------------------------
//
//  ジオメトリストリーム出力用バッファを作成する
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer*	DX_Buffer::CreateStreamOutputBuffer(ID3D11Device* pDevice, const size_t	bufferDateSize)
{
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= static_cast<UINT>(bufferDateSize);
	desc.Usage				= D3D11_USAGE_DEFAULT;
	desc.BindFlags			= D3D11_BIND_STREAM_OUTPUT;

	return CreateBuffer(pDevice, &desc);
}


//-----------------------------------------------------------------------------------------
//
//  CPUで書き込み可能なバッファを作成する
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer*	DX_Buffer::CPUWriteBuffer(ID3D11Device*	pDevice, const size_t bufferDateSize)
{
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= static_cast<UINT>(bufferDateSize);
	desc.Usage				= D3D11_USAGE_DYNAMIC;
	desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;

	return CreateBuffer(pDevice, &desc);
}


//-----------------------------------------------------------------------------------------
//
//  CPUで読み込み可能なバッファを作成する
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer*	DX_Buffer::CPUReadBuffer(ID3D11Device* pDevice, const size_t bufferDateSize, const size_t strctureByteStride)
{	
	D3D11_BUFFER_DESC desc		= { NULL };
	desc.ByteWidth				= static_cast<UINT>(bufferDateSize);
	desc.Usage					= D3D11_USAGE_STAGING;
	desc.BindFlags				= 0;
	desc.CPUAccessFlags			= D3D11_CPU_ACCESS_READ;
	desc.StructureByteStride	= static_cast<UINT>(strctureByteStride);

	return CreateBuffer(pDevice, &desc);
}
//-----------------------------------------------------------------------------------------
//
//  2Dを描画
//
//-----------------------------------------------------------------------------------------
bool DX_Buffer::Render2D(DX_ShaderManager* pShaderManager, ID3D11DeviceContext* pContext, ID3D11Buffer* pVertexBuffer, ID3D11ShaderResourceView* pShaderRerousceView)
{
	bool result = false;

	unsigned int l_stride = sizeof(tagVertex2D);
	unsigned int l_offset = 0;

	//	VertexBufferを送る
	pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &l_stride, &l_offset);

	//	InputLayoutの設定を送る
	pContext->IASetInputLayout(pShaderManager->GetDefaultInputLayout2D());

	//	Primitiveの設定を送る
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//	PixelShaderにテクスチャ情報を送る
	result = DX_ResourceManager::SetShaderResources(pContext, 0, 1, &pShaderRerousceView,DX_SHADER_TYPE::PIXEL_SHADER);

	//	描画
	pContext->Draw(4, 0);

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  バッファを作成する
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateBuffer(ID3D11Device* pDevice, D3D11_BUFFER_DESC* pDesc, D3D11_SUBRESOURCE_DATA* pSubResource)
{
	ID3D11Buffer* pBuffer = nullptr;

	//	bufferを作成する
	if (!DX_Debug::GetInstance()->IsHresultCheck(pDevice->CreateBuffer(pDesc, pSubResource, &pBuffer))) {
		SAFE_RELEASE(pBuffer);
	}
	
	return pBuffer;
}