#pragma once

/// <summary>
/// テクスチャを表示する機能を持ったクラス
/// </summary>
class Object2D : public GameObject
{
public:
	/// <summary>
	/// メンバ変数等を初期化
	/// </summary>
	Object2D();

	/// <summary>
	/// メンバ変数等を解放
	/// </summary>
	virtual ~Object2D();

	/// <summary>
	/// テクスチャの読み込みと頂点バッファの作成を行う 
	/// </summary>
	/// <param name="filepath">テクスチャのファイルパス(/Resource/以下から)</param>
	/// <returns>成否</returns>
	bool Initialize(const char* filepath);

	/// <summary>
	/// 指定したサイズでテクスチャ及び頂点バッファを作成する
	/// </summary>
	/// <param name="filepath">テクスチャのファイルパス(/Resource/以下から)</param>
	/// <param name="rectPos">描画するサイズ</param>
	/// <returns>成否</returns>
	bool Initialize(const char* filepath, const DX::tagRect& rectPos);

	/// <summary>
	///  指定したサイズとUVでテクスチャ及び頂点バッファを作成する
	/// </summary>
	/// <param name="filepath">テクスチャのファイルパス(/Resource/以下から)</param>
	/// <param name="rectPos">描画するサイズ</param>
	/// <param name="uv">描画するテクスチャ座標</param>
	/// <returns>成否</returns>
	bool Initialize(const char* filepath, const DX::tagRect& rectPos, const DX::tagRect& uv);

	/// <summary>
	/// テクスチャの高さを取得する
	/// </summary>
	/// <returns>テクスチャの高さ</returns>
	unsigned int GetHeight()const;

	/// <summary>
	/// テクスチャの幅を取得する
	/// </summary>
	/// <returns>テクスチャの幅</returns>
	unsigned int GetWidth()const;

	/// <summary>
	/// 描画する
	/// </summary>
	/// <returns>成否</returns>
	bool Render();

	/// <summary>
	/// テクスチャを複製する
	/// </summary>
	/// <returns>複製したオブジェクト</returns>
	Object2D* Clone();

	/// <summary>
	/// オブジェクトが複製したものかどうかを取得する
	/// </summary>
	/// <returns>複製したものならtrue</returns>
	bool IsClone() const;

	/// <summary>
	/// オブジェクトがオリジナルかどうかを取得する
	/// </summary>
	/// <returns>オリジナルならtrue</returns>
	bool IsOriginal() const;

	/// <summary>
	/// スクリーン内に描画されているかどうかを取得する
	/// </summary>
	/// <returns>描画されているならtrue</returns>
	bool IsInScreen() const;

	/// <summary>
	/// 描画するオブジェクトのサイズを設定する
	/// </summary>
	/// <param name="rect">描画座標</param>
	void SetRect(const DX::tagRect& rect);

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(const DirectX::XMFLOAT3& pos);
	
	/// <summary>
	/// 大きさを設定する(描画するオブジェクトの大きさに対して掛け算する)
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const DirectX::XMFLOAT2& scale);

	/// <summary>
	/// 画像の向きの設定する
	/// </summary>
	/// <param name="val">向きを設定</param>
	/// <param name="isAngle">falseならラジアン、trueなら度数で設定</param>
	void SetAngle(const float val, bool isAngle = false);

	/// <summary>
	/// UV座標を設定する
	/// </summary>
	/// <param name="uv">UV座標</param>
	void SetUV(const DX::tagRect& uv);

	/// <summary>
	/// 描画座標を取得する
	/// </summary>
	/// <returns>描画座標</returns>
	const DX::tagRect& GetRectPos();

	/// <summary>
	/// UV座標を取得する
	/// </summary>
	/// <returns>UV座標</returns>
	const DX::tagRect& GetUV();

	/// <summary>
	/// 座標等を更新する
	/// </summary>
	void Update();

protected:
	DX::tagRect m_rect;
	DX::tagRect m_uv;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT2 m_scale;
	float m_angle;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_srv;
	unsigned int m_height;
	unsigned int m_width;
	bool m_isCloned;
	bool m_isChanged;

	/// <summary>
	/// テクスチャの読み込みと頂点バッファの作成を行う 
	/// </summary>
	/// <param name="filepath">テクスチャのファイルパス(/Resource/以下から)</param>
	/// <returns>成否</returns>
	bool CommonInitialize(const char* filepath);

	/// <summary>
	/// テクスチャを読み込む
	/// </summary>
	/// <param name="filepath"></param>
	/// <returns>成否</returns>
	bool LoadTexture(const char* filepath);

	/// <summary>
	/// 頂点バッファを更新する
	/// </summary>
	void UpdateVertexBuffer();
};
