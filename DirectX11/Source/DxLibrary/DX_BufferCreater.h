#ifndef __DX_BUFFER_CREATER_H_
#define __DX_BUFFER_CREATER_H_


/// <summary>
/// バッファの作成を行う
/// </summary>
class DX_BufferCreater
{
public:
	
	/// <summary>
	/// 定数バッファを作成する
	/// </summary>
	/// <param name="bufferSize">バッファサイズ</param>
	/// <returns>バッファ(失敗したならnullptr)</returns>
	static ID3D11Buffer* ConstantBuffer(const UINT bufferSize);

	/// <summary>
	/// 頂点バッファを作成する(スキンバッファもこれで作成します)
	/// </summary>
	/// <param name="vertexDataSize">頂点データのサイズ</param>
	/// <param name="vertices">頂点情報</param>
	/// <returns>バッファ(失敗したならnullptr)</returns>
	static ID3D11Buffer* VertexBuffer(const UINT vertexDataSize, const void* vertices);

	/// <summary>
	/// インデックスバッファを作成する
	/// </summary>
	/// <param name="indexDataSize">インデックスデータのサイズ</param>
	/// <param name="indicies">インデックスデータ</param>
	/// <returns>バッファ(失敗したならnullptr)</returns>
	static ID3D11Buffer* IndexBuffer(const UINT indexDataSize, const void* indicies);

	/// <summary>
	/// 構造化バッファを作成する
	/// </summary>
	/// <param name="elementSize">配列の要素のデータサイズ</param>
	/// <param name="elementCount">配列の要素数</param>
	/// <param name="data">データ</param>
	/// <returns>バッファ(失敗したならnullptr)</returns>
	static ID3D11Buffer* StructuredBuffer(const UINT elementSize, const UINT elementCount, void* data);

	/// <summary>
	/// バイトアドレスバッファを作成する
	/// </summary>
	/// <param name="dataSize">配列のデータサイズ</param>
	/// <param name="data">データ</param>
	/// <param name="usedVertex ">頂点バッファでも使いたい場合はtrueにする</param>
	/// <returns>バッファ(失敗したならnullptr)</returns>
	static ID3D11Buffer* ByteAddressBuffer(const UINT dataSize, void* data, const bool usedVertex = false);

	/// <summary>
	/// ジオメトリストリーム出力用バッファを作成する
	/// </summary>
	/// <param name="bufferDateSize">ジオメトリから出力するデータサイズ</param>
	/// <returns>バッファ(失敗したならnullptr)</returns>
	static ID3D11Buffer* StreamOutputBuffer(const UINT bufferDateSize);

	/// <summary>
	/// CPUで書き込み可能なバッファを作成する
	/// </summary>
	/// <param name="bufferDateSize">書き込むデータサイズ</param>
	/// <returns>バッファ(失敗したならnullptr)</returns>
	static ID3D11Buffer* CPUWriteBuffer(const UINT bufferDateSize);

	/// <summary>
	/// CPUで読み込み可能なバッファを作成する
	/// </summary>
	/// <param name="bufferDateSize">読み込むデータサイズ</param>
	/// <param name="strctureByteStride">構造体のバイトサイズ</param>
	/// <returns>バッファ(失敗したならnullptr)</returns>
	static ID3D11Buffer* CPUReadBuffer(const UINT bufferDateSize, const UINT strctureByteStride = 0);

private:
	DX_BufferCreater(){}
	~DX_BufferCreater(){}

	/// <summary>
	/// バッファを作成する
	/// </summary>
	/// <param name="desc">作成するバッファの設定</param>
	/// <param name="subResource">作成するバッファに設定するデータ</param>
	/// <returns>バッファ(失敗したならnullptr)</returns>
	static ID3D11Buffer* Buffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* subResource = nullptr);
};
#endif // !__DX_BUFFER_H_
