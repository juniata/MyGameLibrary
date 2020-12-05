#pragma once


/// <summary>
/// 正方形のモデルを扱う
/// </summary>
class Box : public GameObject
{
public:
	/// <summary>
	/// 変数等を初期化する
	/// </summary>
	Box();

	/// <summary>
	/// 解放処理
	/// </summary>
	~Box();

	/// <summary>
	/// バッファ等の作成を行う
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	/// <summary>
	/// 座標を設定する
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(const DirectX::XMFLOAT3& pos);

	/// <summary>
	/// 大きさを設定する
	/// </summary>
	/// <param name="scale">大きさ</param>
	void SetScale(const DirectX::XMFLOAT3& scale);

	/// <summary>
	/// 向きを設定する
	/// </summary>
	/// <param name="angle">向き</param>
	void SetAngle(const DirectX::XMFLOAT3& angle);

	/// <summary>
	/// 座標を取得する
	/// </summary>
	/// <returns>座標</returns>
	DirectX::XMFLOAT3 GetPos() const;

	/// <summary>
	/// 向きを取得する
	/// </summary>
	/// <returns>向き</returns>
	DirectX::XMFLOAT3 GetAngle() const;

	/// <summary>
	/// 大きさを取得する
	/// </summary>
	/// <returns>大きさ</returns>
	DirectX::XMFLOAT3 GetScale() const;

	/// <summary>
	/// 座標等を更新する
	/// </summary>
	void Update();

	/// <summary>
	/// 描画する
	/// </summary>
	void Render();

	/// <summary>
	/// 複製する
	/// </summary>
	/// <returns>複製したオブジェクト</returns>
	Box* Clone();

	/// <summary>
	/// オブジェクトが複製したものかどうかを取得する
	/// </summary>
	/// <returns>複製したものかどうか</returns>
	bool IsClone() const;

	/// <summary>
	/// オブジェクトがオリジナルかどうかを取得する
	/// </summary>
	/// <returns>オリジナルかどうか</returns>
	bool IsOriginal() const;

	/// <summary>
	/// レイキャスト判定
	/// </summary>
	/// <param name="pos">判定を取りたいオブジェクトの座標</param>
	/// <param name="vec">判定を取りたいオブジェクトのベクトル</param>
	/// <param name="distance">レイの長さ</param>
	/// <returns>あたったかどうか</returns>
	bool RayCast(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& vec, const float distance);
private:
	static const int VERTEX_COUNT = 12;
	static const int INDEX_COUNT = 38;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_angle;
	DirectX::XMFLOAT4X4 m_worldMat;
	
	int m_pastRaypickIndex;
	unsigned short m_indices[INDEX_COUNT];
	bool m_isChanged;
	bool m_isCloned;

	/// <summary>
	/// バッファを作成する
	/// </summary>
	/// <returns>作成できたかどうか</returns>
	bool CreateBuffer();

	/// <summary>
	/// 法線を作成する
	/// </summary>
	/// <param name="pVertices">頂点情報</param>
	void CreateNormal(DX::tagObjectVertext* pVertices);

	/// <summary>
	/// 面法線を算出する
	/// </summary>
	/// <param name="faceV">算出した面法線が格納される</param>
	/// <param name="p0">頂点0の座標</param>
	/// <param name="p1">頂点1の座標</param>
	/// <param name="p2">頂点2の座標</param>
	void CalcFaceNormal(DirectX::XMFLOAT3& faceV, const DirectX::XMVECTOR& p0, const DirectX::XMVECTOR& p1, const DirectX::XMVECTOR& p2);

	/// <summary>
	/// 算出した面法線を、頂点の法線に加算していく
	/// </summary>
	/// <param name="outVf">面法線が格納される</param>
	/// <param name="vN">頂点法線</param>
	/// <param name="faceN">面法線</param>
	void AddFaceNormal(DirectX::XMFLOAT3& outVf, const DirectX::XMFLOAT3& vN, const DirectX::XMFLOAT3& faceN);


};