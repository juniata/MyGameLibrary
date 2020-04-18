#ifndef __DX_BUFFER_H_
#define __DX_BUFFER_H_



//****************************************************************************************************
//
//	DX_Buffer
//
//****************************************************************************************************
class DX_Buffer
{
public:
	
	//------------------------------------------------------------------------------
	//
	//  @brief		定数バッファを作成する
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	bufferSize		バッファサイズ
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer* CreateConstantBuffer(ID3D11Device*	pDevice, UINT bufferSize);

	//------------------------------------------------------------------------------
	//
	//  @brief		頂点バッファを作成する
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	vertexDataSize	頂点データのサイズ
	//	@param[in]	pVertex			頂点情報
	//	@return		作成した頂点バッファ
	//	@note		作成に失敗した場合、nullptrが返る
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer* CreateVertexBuffer(ID3D11Device* pDevice, const size_t	vertexDataSize, const void* pVertex);

	//------------------------------------------------------------------------------
	//
	//  @brief		構造化バッファを作成する
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	elementSize		配列の要素のデータサイズ
	//	@param[in]	elementCount	配列の要素数
	//	@param[in]	pData			データ
	//	@return		作成した構造化バッファ
	//	@note		作成に失敗した場合、nullptrが返る
	//				pDataはバッファにデータを書き込みたい時に使う
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer* CreateStructuredBuffer(ID3D11Device* pDevice, const size_t	elementSize, const size_t elementCount, void* pData);

	//------------------------------------------------------------------------------
	//
	//  @brief		バイトアドレスバッファを作成する
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	dataSize		配列のデータサイズ
	//	@param[in]	pData			データ
	//	@param[in]	isVertex		頂点バッファでも使いたい場合はtrueにする
	//	@return		作成したバイトアドレスバッファ
	//	@note		作成に失敗した場合、nullptrが返る
	//				pDataはバッファにデータを書き込みたい時に使う
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer* CreateByteAddressBuffer(ID3D11Device* pDevice, const size_t dataSize, void* pData, const bool isVertex = false);

	//------------------------------------------------------------------------------
	//
	//  @brief		インデックスバッファを作成する
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	indexDataSize	インデックスデータのサイズ
	//	@param[in]	pIndex			インデックス情報
	//	@return		作成した頂点バッファ
	//	@note		作成に失敗した場合、nullptrが返る
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer* CreateIndexBuffer(ID3D11Device* pDevice, const size_t indexDataSize, const void* pIndex);

	//------------------------------------------------------------------------------
	//
	//  @brief		スキンバッファを作成する
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	indexDataSize	インデックスデータのサイズ
	//	@param[in]	pIndex			インデックス情報
	//	@return		作成した頂点バッファ
	//	@note		作成に失敗した場合、nullptrが返る
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer*	CreateSkinBuffer(ID3D11Device* pDevice, const size_t skinDatasize, const void* pIndex);

	//------------------------------------------------------------------------------
	//
	//  @brief		ジオメトリストリーム出力用バッファを作成する
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	bufferDateSize	ジオメトリから出力するデータサイズ
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer*	CreateStreamOutputBuffer(ID3D11Device* pDevice, const size_t bufferDateSize);

	//------------------------------------------------------------------------------
	//
	//  @brief		CPUで書き込み可能なバッファを作成する
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	bufferDateSize	書き込むデータサイズ
	//	@note		CPUで読み込むことは不可能
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer*	CPUWriteBuffer(ID3D11Device* pDevice, const size_t bufferDateSize);

	//------------------------------------------------------------------------------
	//
	//  @brief		CPUで読み込み可能なバッファを作成する
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	bufferDateSize	読み込むデータサイズ
	//	@note		CPUで読み込むことは不可能
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer*	CPUReadBuffer(ID3D11Device* pDevice, const size_t bufferDateSize, const size_t strctureByteStride = 0);

private:

	//------------------------------------------------------------------------------
	//
	//  @brief		バッファを作成する
	//	@param[in]	pDevice			DXデバイス
	//	@param[in]	pDesc			作成するバッファ設定
	//	@param[in]	pSubResource	作成するバッファに設定するデータ
	//	@return		バッファのポインタ　作成に失敗した場合はnullptr
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer* CreateBuffer(ID3D11Device* pDevice, D3D11_BUFFER_DESC* pDesc, D3D11_SUBRESOURCE_DATA* pSubResource = nullptr);
};
#endif // !__DX_BUFFER_H_
