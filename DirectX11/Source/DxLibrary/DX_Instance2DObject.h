#ifndef __DX_INSTANCE_2DOBJECT_H_
#define __DX_INSTANCE_2DOBJECT_H_

//------------------------------------------------------------------------------
//
// 2DObjectをインスタンス描画する
// 画像サイズが大きいほど描画負荷がかかるので、そこには気を付けて下さい
// 今は座標のz値で描画するかしないかを決めてる
//
//------------------------------------------------------------------------------
class DX_Instance2DObject
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		メンバー変数を初期化し、2DObjectを作成する
	//	@param[in]	pFilepath	テクスチャのファイルパス
	//	@param[in]	num			インスタンス数
	//	@param[in]	renderSize	描画するサイズ
	//
	//------------------------------------------------------------------------------
	DX_Instance2DObject(const char* pFilepath, const UINT num, const DirectX::XMFLOAT2& renderSize);

	//------------------------------------------------------------------------------
	//
	//  @brief		デストラクタ
	//
	//------------------------------------------------------------------------------
	~DX_Instance2DObject();

	//------------------------------------------------------------------------------
	//
	//  @brief		座標一覧を取得する
	//	@return		座標一覧
	//
	//------------------------------------------------------------------------------
	DirectX::XMFLOAT3* GetPosList();

	//------------------------------------------------------------------------------
	//
	//  @brief		座標一覧を取得する
	//	@param[in]	index 指定した箇所からの座標一覧を取得する
	//	@return		座標一覧
	//
	//------------------------------------------------------------------------------
	DirectX::XMFLOAT3* GetPosList(const unsigned int index);

	//------------------------------------------------------------------------------
	//
	//  @brief		描画する
	//
	//------------------------------------------------------------------------------
	void Render();

	//------------------------------------------------------------------------------
	//
	//  @brief		指定した番号のオブジェクトの無効化する（描画されなくなる）
	//
	//------------------------------------------------------------------------------
	void Disable(const size_t index);

	//------------------------------------------------------------------------------
	//
	//  @brief		指定した番号のオブジェクトの有効化する（描画されるようになる）
	//
	//------------------------------------------------------------------------------
	void Enable(const size_t index);

	//------------------------------------------------------------------------------
	//
	//  @brief		指定したインスタンスが有効かどうか
	//	@return		無効かどうか
	//
	//------------------------------------------------------------------------------
	bool IsDisable(const size_t index) const;

	//------------------------------------------------------------------------------
	//
	//  @brief		指定したインスタンスが無効かどうか
	//	@return		有効化どうか
	//
	//------------------------------------------------------------------------------
	bool IsEnable(const size_t index) const;
private:
	//	テクスチャ情報
	ID3D11ShaderResourceView*		m_pShaderResourceView;
	// 頂点情報
	ID3D11Buffer* m_pVertexBuffer;

	// テクスチャのサイズ
	size_t m_width;
	size_t m_height;

	// インスタンス数
	UINT m_instanceNum;

	// インスタンスの座標リスト
	DirectX::XMFLOAT3* m_pPosList;

	//------------------------------------------------------------------------------
	//
	//  @brief		デフォルトコンストラクタ
	//
	//------------------------------------------------------------------------------
	DX_Instance2DObject() {};

	//------------------------------------------------------------------------------
	//
	//  @brief		テクスチャを読み込む
	//	@param[in]	pFilepath	テクスチャのファイルパス
	//
	//------------------------------------------------------------------------------
	void LoadTexture(const char* pFilepath);
};
#endif // !__DX_INSTANCE_2DOBJECT_H_
