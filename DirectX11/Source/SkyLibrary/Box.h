#pragma once


//****************************************************************************************************
//
//	ボックス
//	
//****************************************************************************************************
class Box
{
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		変数の初期化
	//
	//------------------------------------------------------------------------------
	Box();

	//------------------------------------------------------------------------------
	//
	//  @brief		解放処理
	//
	//------------------------------------------------------------------------------
	~Box();

	//------------------------------------------------------------------------------
	//
	//  @brief		バッファの作成を行う
	//
	//------------------------------------------------------------------------------
	bool Initialize();

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

	//------------------------------------------------------------------------------
	//
	//  @brief		複製する
	//	@rereturn	複製したBox
	//
	//------------------------------------------------------------------------------
	Box* Clone();

	//------------------------------------------------------------------------------
	//
	//  @brief		オブジェクトが複製したものかどうか
	//	@rereturn	複製したものならtrue 
	//
	//------------------------------------------------------------------------------
	bool IsClone() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		オブジェクトがオリジナルかどうか
	//	@rereturn	オリジナルならtrue
	//
	//------------------------------------------------------------------------------
	bool IsOriginal() const;

	//------------------------------------------------------------------------------
	//
	//  @brief		レイキャスト判定
	//
	//------------------------------------------------------------------------------
	bool RayCast(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& vec, const float distance);
private:
	//------------------------------------------------------------------------------
	//
	//  @brief		頂点バッファ等の作成を行う
	//	@preturn	バッファの作成の成否
	//
	//------------------------------------------------------------------------------
	bool CreateBuffer();

	//------------------------------------------------------------------------------
	//
	//  @brief		法線を作成する
	//
	//------------------------------------------------------------------------------
	void CreateNormal(DX::tagObjectVertext* pVertex);

	//------------------------------------------------------------------------------
	//
	//  @brief		三角形の面法線を算出する
	//	@param[ref]	faceV 算出した面法線が格納される
	//	@param[in]	p0 頂点0の座標
	//	@param[in]	p1 頂点1の座標
	//	@param[in]	p2 頂点2の座標
	//
	//------------------------------------------------------------------------------
	void CalcFaceNormal(DirectX::XMFLOAT3& faceV, const DirectX::XMVECTOR& p0, const DirectX::XMVECTOR& p1, const DirectX::XMVECTOR& p2);

	//------------------------------------------------------------------------------
	//
	//  @brief		算出した面法線を、面を構成している頂点の法線に加算していく
	//	@param[ref]	outVf	面法線が格納される
	//	@param[in]	vN		頂点法線
	//	@param[in]	faceN	面法線
	//
	//------------------------------------------------------------------------------
	void AddFaceNormal(DirectX::XMFLOAT3& outVf, const DirectX::XMFLOAT3& vN, const DirectX::XMFLOAT3& faceN);

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_angle;
	DirectX::XMFLOAT4X4 m_worldMat;

	int m_pastRaypickIndexk;

	bool m_isChanged;
	bool m_isCloned;

};