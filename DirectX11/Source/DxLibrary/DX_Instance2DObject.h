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
	//  @brief		メンバー変数を初期化
	//
	//------------------------------------------------------------------------------
	DX_Instance2DObject();

	//------------------------------------------------------------------------------
	//
	//  @brief		デストラクタ
	//
	//------------------------------------------------------------------------------
	~DX_Instance2DObject();

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバー変数を初期化し、インスタンス描画オブジェクトを作成する
	//	@param[in]	pFilepath	テクスチャのファイルパス
	//	@param[in]	num			インスタンス数
	//	@param[in]	renderSize	描画するサイズ
	//
	//------------------------------------------------------------------------------
	bool Initialize(const char* pFilepath, const UINT num, const DirectX::XMFLOAT2& renderSize);
	bool Initialize(const char* pFilepath, const UINT num, const DirectX::XMFLOAT2& renderSize, const DirectX::XMFLOAT2& mapChipSize);


	//------------------------------------------------------------------------------
	//
	//  @brief		描画する
	//
	//------------------------------------------------------------------------------
	bool Render();

	//------------------------------------------------------------------------------
	//
	//  @brief		インタンス描画を行うわないようにする
	//
	//------------------------------------------------------------------------------
	void Disable();

	//------------------------------------------------------------------------------
	//
	//  @brief		インタンス描画を行うようにする
	//
	//------------------------------------------------------------------------------
	void Enable();

	//------------------------------------------------------------------------------
	//
	//  @brief		指定したインスタンスが無効かどうか
	//	@return		無効かどうか
	//
	//------------------------------------------------------------------------------
	bool IsDisable() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		指定したインスタンスが有効かどうか
	//	@return		有効化どうか
	//
	//------------------------------------------------------------------------------
	bool IsEnable() const;

	inline bool IsEnalbe(const unsigned int index) { return m_pVertices[index].z == 0.0f; }
	inline bool IsDisable(const unsigned int index) { return m_pVertices[index].z != 0.0f; }
	inline void Enable(const unsigned int index) { m_pVertices[index].z = 0.0f; }
	inline void Disable(const unsigned int index) {	m_pVertices[index].z = 1.1f; }
	inline void SetPos(const unsigned int index, const DirectX::XMFLOAT2& pos) { m_pVertices[index].x = pos.x; m_pVertices[index].y = pos.y; m_changedPos = true; }
	inline DirectX::XMFLOAT2 GetPos(const unsigned int index) { return DirectX::XMFLOAT2(m_pVertices[index].x, m_pVertices[index].y); }
	inline void SetUV(const unsigned int index, const unsigned int uvIndexX, const unsigned int uvIndexY) { 
		m_pUvs[index].x = m_chipSize.x * CAST_F(uvIndexX);
		m_pUvs[index].y = m_chipSize.y * CAST_F(uvIndexY);
		m_changedUV = true; 
	}
	inline DirectX::XMFLOAT2 GetUV(const unsigned int index) { return m_pUvs[index]; }

private:
	//	テクスチャ情報
	ID3D11ShaderResourceView*		m_pShaderResourceView;

	// 頂点バッファ
	ID3D11Buffer* m_pVertexBuffer;

	// 頂点バッファ(座標)
	ID3D11Buffer* m_pPosBuffer;

	// 頂点バッファ(UV)
	ID3D11Buffer* m_pUVBuffer;

	// 頂点情報
	DirectX::XMFLOAT3* m_pVertices;
	DirectX::XMFLOAT2* m_pUvs;

	DirectX::XMFLOAT2 m_chipSize;

	// テクスチャのサイズ
	size_t m_width;
	size_t m_height;

	// インスタンス数
	UINT m_instanceNum;

	// 有効かどうか
	bool m_enabled;

	// 座標を更新したかどうか
	bool m_changedPos;

	// UV座標を更新したかどうか
	bool m_changedUV;

	//------------------------------------------------------------------------------
	//
	//  @brief		テクスチャを読み込む
	//	@param[in]	pFilepath	テクスチャのファイルパス
	//
	//------------------------------------------------------------------------------
	bool LoadTexture(const char* pFilepath);

	void BufferUpdate();
};
#endif // !__DX_INSTANCE_2DOBJECT_H_
