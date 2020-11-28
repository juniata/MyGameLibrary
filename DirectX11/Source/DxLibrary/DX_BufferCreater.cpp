#include	"DX_Library.h"

/// <summary>
/// 定数バッファを作成する
/// </summary>
/// <param name="bufferSize">バッファサイズ</param>
/// <returns>バッファ(失敗したならnullptr)</returns>
ID3D11Buffer* DX_BufferCreater::ConstantBuffer(const UINT bufferSize)
{
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= bufferSize;
	desc.Usage				= D3D11_USAGE_DEFAULT;
	desc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;

	return Buffer(&desc);
}

/// <summary>
/// 頂点バッファを作成する(スキンバッファもこれで作成します)
/// </summary>
/// <param name="vertexDataSize">頂点データのサイズ</param>
/// <param name="vertices">頂点情報</param>
/// <returns>バッファ(失敗したならnullptr)</returns>
ID3D11Buffer* DX_BufferCreater::VertexBuffer(const UINT vertexDataSize, const void*	vertices)
{
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= vertexDataSize;
	desc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA	subResource = { NULL };
	subResource.pSysMem = vertices;

	return Buffer(&desc, &subResource);
}

/// <summary>
/// インデックスバッファを作成する
/// </summary>
/// <param name="indexDataSize">インデックスデータのサイズ</param>
/// <param name="indicies">インデックスデータ</param>
/// <returns>バッファ(失敗したならnullptr)</returns>
ID3D11Buffer* DX_BufferCreater::IndexBuffer(const UINT indexDataSize, const void* indicies)
{
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= indexDataSize;
	desc.BindFlags			= D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA	subResource = { NULL };
	subResource.pSysMem = indicies;

	return Buffer(&desc, &subResource);
}

/// <summary>
/// 構造化バッファを作成する
/// </summary>
/// <param name="elementSize">配列の要素のデータサイズ</param>
/// <param name="elementCount">配列の要素数</param>
/// <param name="data">データ</param>
/// <returns>バッファ(失敗したならnullptr)</returns>
ID3D11Buffer* DX_BufferCreater::StructuredBuffer(const UINT	elementSize, const UINT elementCount, void* data)
{
	D3D11_BUFFER_DESC desc		= { NULL };
	desc.BindFlags				= D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth				= elementSize * elementCount;
	desc.MiscFlags				= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride	= elementSize;

	D3D11_SUBRESOURCE_DATA subResource = { NULL };
	subResource.pSysMem = data;

	return Buffer(&desc, data ? &subResource : nullptr);
}

/// <summary>
/// バイトアドレスバッファを作成する
/// </summary>
/// <param name="dataSize">配列のデータサイズ</param>
/// <param name="data">データ</param>
/// <param name="usedVertex ">頂点バッファでも使いたい場合はtrueにする</param>
/// <returns>バッファ(失敗したならnullptr)</returns>
ID3D11Buffer* DX_BufferCreater::ByteAddressBuffer(const UINT dataSize, void* data, const bool usedVertex)
{
	D3D11_BUFFER_DESC desc		= { NULL };
	desc.BindFlags				= usedVertex ? (D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_VERTEX_BUFFER) : (D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	desc.ByteWidth				= dataSize;
	desc.MiscFlags				= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	desc.StructureByteStride	= 0;

	D3D11_SUBRESOURCE_DATA subResource = { NULL };
	subResource.pSysMem = data;

	return Buffer(&desc, data ? &subResource : nullptr);
}

/// <summary>
/// ジオメトリストリーム出力用バッファを作成する
/// </summary>
/// <param name="bufferDateSize">ジオメトリから出力するデータサイズ</param>
/// <returns>バッファ(失敗したならnullptr)</returns>
ID3D11Buffer*	DX_BufferCreater::StreamOutputBuffer(const UINT	bufferDateSize)
{
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= bufferDateSize;
	desc.Usage				= D3D11_USAGE_DEFAULT;
	desc.BindFlags			= D3D11_BIND_STREAM_OUTPUT;

	return Buffer(&desc);
}


/// <summary>
/// CPUで書き込み可能なバッファを作成する
/// </summary>
/// <param name="bufferDateSize">書き込むデータサイズ</param>
/// <returns>バッファ(失敗したならnullptr)</returns>
ID3D11Buffer*	DX_BufferCreater::CPUWriteBuffer(const size_t bufferDateSize)
{
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= static_cast<UINT>(bufferDateSize);
	desc.Usage				= D3D11_USAGE_DYNAMIC;
	desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;

	return Buffer(&desc);
}

/// <summary>
/// CPUで読み込み可能なバッファを作成する
/// </summary>
/// <param name="bufferDateSize">読み込むデータサイズ</param>
/// <param name="strctureByteStride">構造体のバイトサイズ</param>
/// <returns>バッファ(失敗したならnullptr)</returns>
ID3D11Buffer*	DX_BufferCreater::CPUReadBuffer(const UINT bufferDateSize, const UINT strctureByteStride)
{	
	D3D11_BUFFER_DESC desc		= { NULL };
	desc.ByteWidth				= bufferDateSize;
	desc.Usage					= D3D11_USAGE_STAGING;
	desc.BindFlags				= 0;
	desc.CPUAccessFlags			= D3D11_CPU_ACCESS_READ;
	desc.StructureByteStride	= strctureByteStride;

	return Buffer(&desc);
}

/// <summary>
/// バッファを作成する
/// </summary>
/// <param name="desc">作成するバッファの設定</param>
/// <param name="subResource">作成するバッファに設定するデータ</param>
/// <returns>バッファ(失敗したならnullptr)</returns>
ID3D11Buffer* DX_BufferCreater::Buffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* subResource)
{
	ID3D11Device* device = DX_System::GetInstance()->GetDevice();
	ID3D11Buffer* buffer = nullptr;

	if (DX_Debug::GetInstance()->IsFailedHresult(device->CreateBuffer(desc, subResource, &buffer))) {
		buffer = nullptr;
	}
	
	return buffer;
}