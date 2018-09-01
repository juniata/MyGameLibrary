#ifndef __DX_TEXTURE_H_
#define __DX_TEXTURE_H_


//****************************************************************************************************
//
//	DX_TextureManager
//
//****************************************************************************************************
class DX_TextureManager
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		テクスチャをロードする
	//	@param[in]	pFilepath	テクスチャのファイルパス
	//	@return		テクスチャ情報
	//
	//------------------------------------------------------------------------------
	static ID3D11ShaderResourceView* GetTexture(const char* pFilepath);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		テクスチャを解放
	//	@param[in]	pTexture 使用したテクスチャ
	//
	//------------------------------------------------------------------------------
	static void Release(ID3D11ShaderResourceView* pTexture);

private:
	struct TextureInfo {
		const char * pFilepath;
		ID3D11ShaderResourceView* pSrv;
	};
	static const int TEXTURE_NUM = 100;
	//	テクスチャ情報を格納
	static TextureInfo m_textureList[TEXTURE_NUM];

	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	DX_TextureManager() {};

	//------------------------------------------------------------------------------
	//
	//  @brief		何もしない
	//
	//------------------------------------------------------------------------------
	~DX_TextureManager() {}

	//------------------------------------------------------------------------------
	//
	//  @brief		テクスチャを使用しているかを調べる
	//	@param[in]	pfilepath エクスチャのファイルパス
	//	@return		発見:ポインタ	未発見:nullptr
	//
	//------------------------------------------------------------------------------
	static ID3D11ShaderResourceView* SearchTexture(const char* pfilepath);
};
#endif // !__DX_TEXTURE_H_
