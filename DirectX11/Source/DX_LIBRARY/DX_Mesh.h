#ifndef __DX_MESH_H_
#define __DX_MESH_H_

//--------------------------------------------------
//	マテリアル順描画用情報
//--------------------------------------------------
struct tagGroupMesh
{
	//	描画するインデックス数
	unsigned int indexCount;

	//	インデックスバッファからGUPで読み取る最初のインデックスの場所
	unsigned int startIndexLocation;

	//	頂点バッファーから頂点を読み取る前に、各インデックスに加算する値です
	int baseVertexLocation;

	//	テクスチャ
	ID3D11ShaderResourceView*	pTextureDecal;			//	通常テクスチャ
	ID3D11ShaderResourceView*	pTextureNormal;			//	法線テクスチャ
	ID3D11ShaderResourceView*	pTextureSpecular;		//	スペキュラテクスチャ
	ID3D11ShaderResourceView*	pTextureHeight;			//	高さテクスチャ
};

//--------------------------------------------------
//	Mesh用頂点情報
//--------------------------------------------------
struct tagMeshVertex{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 color;
};

//--------------------------------------------------
//	面情報
//--------------------------------------------------
struct tagFase{
	tagMeshVertex	vertex[3];		//	頂点情報
	DirectX::XMFLOAT3			normal;			//	面法線
	DWORD			materialNum;	//	マテリアル番号
	WORD			index[3];		//	インデックス番号
};

//****************************************************************************************************
//
//	Mesh
//
//****************************************************************************************************
class Mesh : public dx_library::GameObject
{
protected:
	ComPtr<ID3D11Buffer>	m_vertexBuffer;	//	頂点バッファ
	ComPtr<ID3D11Buffer>	m_indexBuffer;		//	インデックスバッファ
	ComPtr<ID3D11Buffer>	m_rayPickBuffer;	//	レイピック用バッファ

	//	マテリアル順描画用構造体ポインタ変数
	dx_library::tagGroupMesh*	m_pGroupMesh;

	DWORD  m_indexCount;
	DWORD  m_materialCount;
	DWORD  m_vertexCount;
	DWORD  m_faceCount;

	DirectX::XMFLOAT4X4  m_worldMatrix;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_angle;

	bool m_bChanged;
	bool m_bClone;
	bool m_bRayPick;

	//	コピーコンスラクタと代入を禁止にする
//	dx_library::Mesh& operator=(const dx_library::Mesh& copy);
//	dx_library::Mesh(const dx_library::Mesh& copy);
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		メンバー変数の初期化
	//
	//------------------------------------------------------------------------------
	Mesh();

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数を解放
	//
	//------------------------------------------------------------------------------
	virtual ~Mesh();

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数を初期化し、モデルを読み込む
	//	@param[in]	pFilepath	モデルのファイルパス
	//
	//------------------------------------------------------------------------------
	Mesh(const char* pFilepath);

	//------------------------------------------------------------------------------
	//
	//  @brief		オブジェクトを複製
	//	@retrun		ポインタが返る
	//
	//------------------------------------------------------------------------------
	Mesh* Clone()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		座標等の更新を行う
	//
	//------------------------------------------------------------------------------
	virtual void Update();

	//------------------------------------------------------------------------------
	//
	//  @brief		モデルを描画
	//
	//------------------------------------------------------------------------------
	virtual void Render();

	//------------------------------------------------------------------------------
	//
	//  @brief		モデルの座標を設定
	//	@param[in]	pos	座標
	//
	//------------------------------------------------------------------------------
	void SetPos(const DirectX::XMFLOAT3& pos);

	//------------------------------------------------------------------------------
	//
	//  @brief		モデルの向きを設定
	//	@param[in]	angle	向き
	//
	//------------------------------------------------------------------------------
	void SetAngle(const DirectX::XMFLOAT3& angle);

	//------------------------------------------------------------------------------
	//
	//  @brief		モデルの大きさを設定
	//	@param[in]	scale	大きさ
	//
	//------------------------------------------------------------------------------
	void SetScale(const DirectX::XMFLOAT3& scale);
	void SetScale(const float scale);

	//------------------------------------------------------------------------------
	//
	//  @brief		モデルの座標、向き、大きさを設定
	//	@param[in]	pos		座標
	//	@param[in]	angle	向き
	//	@param[in]	scale	大きさ
	//
	//------------------------------------------------------------------------------
	void SetTransform(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& angle, const DirectX::XMFLOAT3& scale);

	//------------------------------------------------------------------------------
	//
	//  @brief		モデルの座標を取得
	//	@return		m_pos
	//
	//------------------------------------------------------------------------------
	DirectX::XMFLOAT3 GetPos()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		モデルの向きを取得
	//	@return		m_angle
	//
	//------------------------------------------------------------------------------
	DirectX::XMFLOAT3 GetAngle()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		モデルの大きさを取得
	//	@param[in]	m_scale
	//
	//------------------------------------------------------------------------------
	DirectX::XMFLOAT3 GetScale()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		モデルのワールド行列を取得
	//	@param[in]	&m_worldMat
	//
	//------------------------------------------------------------------------------
	DirectX::XMFLOAT4X4*	GetWorldMatrix();

	//------------------------------------------------------------------------------
	//
	//  @brief		モデルがオリジナルかどうかを判定
	//	@return		true:クローン	false:オリジナル
	//
	//------------------------------------------------------------------------------
	bool IsClone()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		モデルがオリジナルかどうかを判定
	//	@return		true:オリジナル	false:クローン
	//
	//------------------------------------------------------------------------------
	bool IsOriginal()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		モデルを読み込む
	//	@param[in]	pFilepath	モデルのファイルパス
	//
	//------------------------------------------------------------------------------
	virtual void LoadModel(const char* pFilepath);

	//------------------------------------------------------------------------------
	//
	//  @brief		レイピック
	//	@param[out]	pHitPos			レイのあたった面座標
	//	@param[out]	pHitFaceNormal	レイのあたった面法線
	//	@param[in]	pos		自身の座標
	//	@param[in]	vec		レイの向き
	//	@param[in]	dist	レイの長さ
	//	@return		true:ヒット	false:ヒットしてなす
	//
	//------------------------------------------------------------------------------
	bool RayPick(
		DirectX::XMFLOAT3*		pHitPos,
		DirectX::XMFLOAT3*		pHitFaceNormal,
		const DirectX::XMFLOAT3&	pos,
		const DirectX::XMFLOAT3&	vec,
		const float		dist
		);

private:
	//------------------------------------------------------------------------------
	//
	//  @brief			頂点データをコピー
	//	@param[in out]	pVertexList		pIMOVertexのデータをこっちにコピー
	//	@param[in]		pIMOVertex	IMOLoader::GetVertex()
	//
	//------------------------------------------------------------------------------
	void VertexCopy(std::vector<dx_library::tagMeshVertex>* pVertexList,void* pIMOVertex);

protected:
	//------------------------------------------------------------------------------
	//
	//  @brief			マテリアル順にインデックスを作成
	//	@param[in out]	pIndicesList		pIMOVertexのデータをこっちにコピー
	//	@param[in]		pIMOVertex	IMOLoader::GetVertex()
	//
	//------------------------------------------------------------------------------
	void CreateIndexMaterialOrder(std::vector<WORD>* pIndicesList, LPWORD pIndices, LPDWORD pAttributes);

	//------------------------------------------------------------------------------
	//
	//  @brief		テクスチャ情報を作成する
	//	@param[in]	pTextures	IMOLoader::GetTextures()
	//
	//------------------------------------------------------------------------------
	void CreateTextureInfo(char pTextures[32][MAX_PATH]);

	//------------------------------------------------------------------------------
	//
	//  @brief		MeshとSkinMeshの共通描画関数
	//	@param[in]	pDeviceContext	DX_System::GetDeviceContext()
	//	@param[in]	pVertexShader	DX_ShaderManager::頂点シェーダー
	//	@param[in]	pGeometryShader	DX_ShaderManager::GetDefaultGeometryShader3D()
	//	@param[in]	pPixelShader	DX_ShaderManager::GetDefaultPixelShader3D()
	//	@param[in]	pInputLayout	DX_ShaderManager::インプットレイアウト
	//
	//------------------------------------------------------------------------------
	void Render(
		ID3D11DeviceContext*	pDeviceContext,
		dx_library::Shader*		pVertexShader,
		dx_library::Shader*		pGeometryShader,
		dx_library::Shader*		pPixelShader,
		ID3D11InputLayout*		pInputLayout
		);

};
#endif // !__DX_MESH_H_
