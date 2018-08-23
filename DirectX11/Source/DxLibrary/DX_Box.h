#ifndef __DX_BOX_H_
#define __DX_BOX_H_

class DX_Box
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		変数の初期化とバッファの作成
	//
	//------------------------------------------------------------------------------
	DX_Box();

	//------------------------------------------------------------------------------
	//
	//  @brief		解放処理
	//
	//------------------------------------------------------------------------------
	~DX_Box();

	//------------------------------------------------------------------------------
	//
	//  @brief		座標を設定
	//	@param[in]	pos	座標
	//
	//------------------------------------------------------------------------------
	void SetPos(const DirectX::XMFLOAT3& pos);

	//------------------------------------------------------------------------------
	//
	//  @brief		大きさを設定
	//	@param[in]	scale 大きさ
	//
	//------------------------------------------------------------------------------
	void SetScale(const DirectX::XMFLOAT3& scale);

	//------------------------------------------------------------------------------
	//
	//  @brief		向きを設定
	//	@param[in]	angle 向き
	//
	//------------------------------------------------------------------------------
	void SetAngle(const DirectX::XMFLOAT3& angle);

	//------------------------------------------------------------------------------
	//
	//  @brief		座標を取得
	//	@return		座標
	//
	//------------------------------------------------------------------------------
	DirectX::XMFLOAT3 GetPos() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		向きを取得
	//	@param[in]	向き
	//
	//------------------------------------------------------------------------------
	DirectX::XMFLOAT3 GetAngle() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		大きさを取得設定
	//	@param[in]	大きさ
	//
	//------------------------------------------------------------------------------
	DirectX::XMFLOAT3 GetScale() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		座標等を更新する
	//
	//------------------------------------------------------------------------------
	void Update();

	//------------------------------------------------------------------------------
	//
	//  @brief		描画する
	//
	//------------------------------------------------------------------------------
	void Render();

private:
	ID3D11Buffer*	m_pVertexBuffer;
	ID3D11Buffer*	m_pIndexBuffer;

	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_angle;
	DirectX::XMFLOAT4X4 m_worldMat;

	bool m_bChanged;

};
#endif // !__DX_BOX_H_
