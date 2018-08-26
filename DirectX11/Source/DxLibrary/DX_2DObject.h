#ifndef __DX_2DOBJECT_H_
#define __DX_2DOBJECT_H_

//--------------------------------------------------
//	2D用頂点情報
//--------------------------------------------------
struct tagVertex2D{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

//****************************************************************************************************
//
//	Object2D
//
//****************************************************************************************************
class DX_2DObject : public DX_GameObject
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		メンバー変数の初期化
	//
	//------------------------------------------------------------------------------
	DX_2DObject();

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバー変数を初期化し、2DObjectを作成する
	//	@param[in]	pFilepath	テクスチャのファイルパス
	//
	//------------------------------------------------------------------------------
	DX_2DObject(
		const char* pFilepath
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		m_pShaderResourceViewを解放
	//
	//------------------------------------------------------------------------------
	~DX_2DObject();

	//------------------------------------------------------------------------------
	//
	//  @brief		テクスチャの高さを取得する
	//
	//------------------------------------------------------------------------------
	unsigned int GetHeight()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		テクスチャの幅を取得する
	//
	//------------------------------------------------------------------------------
	unsigned int GetWidth()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		全画面に描画
	//
	//------------------------------------------------------------------------------
	void Render();

	//------------------------------------------------------------------------------
	//
	//  @brief		指定した範囲に描画
	//	@param[in]	renderPos	描画する範囲
	//
	//------------------------------------------------------------------------------
	void Render(const tagRect& renderPos);
	
	//------------------------------------------------------------------------------
	//
	//  @brief		指定した範囲、指定した画像座標で描画
	//	@param[in]	renderPos	描画する範囲
	//	@param[in]	texturePos	描画する画像の範囲
	//
	//------------------------------------------------------------------------------
	void Render(const tagRect& renderPos,const tagRect& texturePos);

	//------------------------------------------------------------------------------
	//
	//  @brief		指定した範囲、指定した画像座標で描画
	//	@param[in]	renderPos	描画する範囲
	//	@param[in]	renderSize	描画する画像の範囲
	//
	//------------------------------------------------------------------------------
	void Render(DirectX::XMFLOAT2 renderPos, DirectX::XMFLOAT2 renderSize);

	//------------------------------------------------------------------------------
	//
	//  @brief		テクスチャを読み込む
	//	@param[in]	pFilepath	テクスチャのファイルパス
	//
	//------------------------------------------------------------------------------
	void LoadTexture(const char* pFilepath);

	//------------------------------------------------------------------------------
	//
	//  @brief		複製する
	//	@return		複製したオブジェクト
	//
	//------------------------------------------------------------------------------
	DX_2DObject* Clone();

	//------------------------------------------------------------------------------
	//
	//  @brief		オブジェクトが複製したものかどうか
	//	@return		オブジェクトが複製したものならtrue
	//
	//------------------------------------------------------------------------------
	bool IsClone() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		オブジェクトがオリジナルかどうか
	//	@return		オブジェクトがオリジナルならtrue
	//
	//------------------------------------------------------------------------------
	bool IsOriginal() const;
private:
	ID3D11Buffer* m_pVertexBuffer;

	ID3D11ShaderResourceView*		m_pShaderResourceView;

	unsigned int m_height;
	unsigned int m_width;

	bool m_bClone;
	//------------------------------------------------------------------------------
	//
	//  @brief		頂点情報を作成する
	//	@param[in]	pContext	コンテキスト
	//	@param[in]	renderPos	画面に描画する範囲	
	//	@param[in]	texturePos	描画する画像の範囲
	//
	//------------------------------------------------------------------------------
	void CreateVertex(ID3D11DeviceContext* pContext, const tagRect& renderPos, const tagRect& texturePos);

};

#endif // !__DX_2DOBJECT_H_
