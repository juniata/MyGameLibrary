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

	//------------------------------------------------------------------------------
	//
	//  @brief		インスタンスリストを取得する
	//	@return		インスタンスリスト
	//
	//------------------------------------------------------------------------------
	DX::tagInstance2D* GetInstanceList();

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
private:
	//	テクスチャ情報
	ID3D11ShaderResourceView*		m_pShaderResourceView;

	// 頂点情報
	ID3D11Buffer* m_pVertexBuffer;

	// インスタンスの座標リスト
	DX::tagInstance2D* m_pInstance2DList;

	// テクスチャのサイズ
	size_t m_width;
	size_t m_height;

	// インスタンス数
	UINT m_instanceNum;

	// 有効かどうか
	bool m_enabled;

	//------------------------------------------------------------------------------
	//
	//  @brief		テクスチャを読み込む
	//	@param[in]	pFilepath	テクスチャのファイルパス
	//
	//------------------------------------------------------------------------------
	bool LoadTexture(const char* pFilepath);
};
#endif // !__DX_INSTANCE_2DOBJECT_H_
