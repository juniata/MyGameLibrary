#ifndef __IEM_LOADER_H_
#define __IEM_LOADER_H_


//----------------------------------------------------------
//	IEM用ボーン情報
//----------------------------------------------------------
struct tagIEMBone{
	DirectX::XMFLOAT4X4		boneMatrix;			//	ボーン行列
	WORD			parent;				//	親ボーン

	DirectX::XMFLOAT4		orgPose;			//	基本姿勢
	DirectX::XMFLOAT3			orgPos;				//	基本座標

	WORD			indexCount;			//	影響頂点数
	DWORD*			pIndices;			//	影響頂点Index
	float*			pInfluence;			//	影響力
};
//----------------------------------------------------------
//	IEM用モーション情報
//----------------------------------------------------------
struct tagIEMMotion {
	WORD		rotateCount;		//	回転キーフレーム数
	WORD*		pRotateFrame;		//	回転キーフレーム
	DirectX::XMFLOAT4*	pRotate;			//	ボーンの状態クォータニオン

	WORD		posCount;			//	座標キーフレーム数
	WORD*		pPosFrame;			//	座標キーフレーム
	DirectX::XMFLOAT3*	pPos;				//	座標

};


//-----------------------------------------------------------------------------------------
//
//	IEM読み取り用構造体
//
//-----------------------------------------------------------------------------------------
struct tagIEMMeshVertex{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 color;
};

//****************************************************************************************************
//
//	IEMLoader
//
//****************************************************************************************************
class IEMLoader
{
private:
	HANDLE m_file;			//	CreateFile用変数


	tagIEMMeshVertex*	m_pVertices;	//	頂点情報
	LPDWORD				m_pAttributes;	//	面のマテリアル番号
	WORD*				m_pIndices;		//	インデックス
	tagIEMBone*			m_pBoneInfo;	//	ボーン情報
	tagIEMMotion*		m_pMotionInfo;	//	モーション情報

	int		m_version;					//	バージョン
	WORD	m_materialCount;			//	マテリアル数
	WORD	m_indexCount;				//	インデックス数
	WORD	m_vertexCount;				//	頂点数			
	WORD	m_frameCount;				//	フレーム数
	WORD	m_faceCount;				//	面数
	WORD	m_boneCount;				//	ボーン数
	WORD	m_motionCount;				//	モーション数

	char m_pTextures[32][MAX_PATH];	//	テクスチャ情報

	WORD	m_offset[256];
	WORD	m_frameFlag[65535];

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数の初期化
	//
	//------------------------------------------------------------------------------
	IEMLoader();
public:
	//------------------------------------------------------------------------------
	//
	//  @brief		IEMファイルを読み込む
	//	@param[in]	pFilePath	IEMファイルをパス
	//
	//------------------------------------------------------------------------------
	IEMLoader(const char* pFilePath);

	//------------------------------------------------------------------------------
	//
	//  @brief		メンバ変数を解放
	//
	//------------------------------------------------------------------------------
	~IEMLoader();

	//------------------------------------------------------------------------------
	//
	//  @brief		読み込み用ファイルを作成する
	//	@param[in]	pFilePath	モデルのファイルパス
	//
	//------------------------------------------------------------------------------
	void _CreateFile(const char* pFilePath);

	//------------------------------------------------------------------------------
	//
	//  @brief		IEMのバージョンを取得する
	//	@return		m_version
	//
	//------------------------------------------------------------------------------
	int GetVersion()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		マテリアル数を取得する
	//	@return		m_version
	//
	//------------------------------------------------------------------------------
	WORD GetMaterialCount()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		頂点数を取得する
	//	@return		m_vertexCount
	//
	//------------------------------------------------------------------------------
	WORD GetVertexCount()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		面数を取得する
	//	@return		m_faceCount
	//
	//------------------------------------------------------------------------------
	WORD GetFaceCount()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		フレーム数を取得する
	//	@return		m_frameCount
	//
	//------------------------------------------------------------------------------
	WORD GetFrameCount()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		ボーン数を取得する
	//	@return		m_boneCount
	//
	//------------------------------------------------------------------------------
	WORD GetBoneCount()const;
	
	//------------------------------------------------------------------------------
	//
	//  @brief		インデックス数を取得する
	//	@return		m_indexCount
	//
	//------------------------------------------------------------------------------
	WORD GetIndexCount()const;

	//------------------------------------------------------------------------------
	//
	//  @brief		テクスチャのファイルパスを取得する
	//	@return		m_pTextures
	//
	//------------------------------------------------------------------------------
	char (*GetTextures())[MAX_PATH];

	//------------------------------------------------------------------------------
	//
	//  @brief		頂点データを取得する
	//	@return		m_pVertices
	//
	//------------------------------------------------------------------------------
	tagIEMMeshVertex*	GetVertices();

	//------------------------------------------------------------------------------
	//
	//  @brief		属性データを取得する
	//	@return		m_pAttributes
	//
	//------------------------------------------------------------------------------
	LPDWORD GetAttributes();

	//------------------------------------------------------------------------------
	//
	//  @brief		インデックスデータを取得する
	//	@return		m_pIndices
	//
	//------------------------------------------------------------------------------
	WORD* GetIndices();

	//------------------------------------------------------------------------------
	//
	//  @brief		ボーンデータを取得する
	//	@return		m_pBoneInfo
	//
	//------------------------------------------------------------------------------
	tagIEMBone* GetBoneInfo();

	//------------------------------------------------------------------------------
	//
	//  @brief		モーションデータを取得する
	//	@return		m_pMotionInfo
	//
	//------------------------------------------------------------------------------
	tagIEMMotion* GetMotionInfo();

	//------------------------------------------------------------------------------
	//
	//  @brief		オフセットを取得する
	//	@return		m_offset
	//
	//------------------------------------------------------------------------------
	WORD* GetOffSet();

	//------------------------------------------------------------------------------
	//
	//  @brief		フレームフラグを取得する
	//	@return		m_frameFlag
	//
	//------------------------------------------------------------------------------
	WORD* GetFrameFlag();

private:
	//------------------------------------------------------------------------------
	//
	//  @brief		IEMのバージョンを判断する
	//
	//------------------------------------------------------------------------------
	void VersionJuage();

	//------------------------------------------------------------------------------
	//
	//  @brief		頂点情報を読み込む
	//	@param[in]	pEIMFILE	IEMFILE
	//
	//------------------------------------------------------------------------------
	void ReadVertexInfo(void* pEIMFILE);

	//------------------------------------------------------------------------------
	//
	//  @brief		旧IEMバージョンを読み込む
	//	@param[in]	pEIMFILE	IEMFILE
	//
	//------------------------------------------------------------------------------
	void ReadVertexInfoIEMVersion1(void* pEIMFILE);

	//------------------------------------------------------------------------------
	//
	//  @brief		新IEMバージョンを読み込む
	//	@param[in]	pEIMFILE	IEMFILE
	//
	//------------------------------------------------------------------------------
	void ReadVertexInfoIEMVersion2(void* pEIMFILE);

	//------------------------------------------------------------------------------
	//
	//  @brief		ポリゴン情報を読み込む
	//	@param[in]	pEIMFILE	IEMFILE
	//
	//------------------------------------------------------------------------------
	void ReadPolygonInfo(void* pEIMFILE);

	//------------------------------------------------------------------------------
	//
	//  @brief		マテリアル情報を読み込む
	//	@param[in]	pEIMFILE	IEMFILE
	//
	//------------------------------------------------------------------------------
	void ReadMaterialInfo(void* pEIMFILE);

	//------------------------------------------------------------------------------
	//
	//  @brief		ボーン情報を読み込む
	//	@param[in]	pEIMFILE	IEMFILE
	//
	//------------------------------------------------------------------------------
	void ReadBoneInfo(void* pEIMFILE);

	//------------------------------------------------------------------------------
	//
	//  @brief		モーション情報を読み込む
	//	@param[in]	pEIMFILE	IEMFILE
	//
	//------------------------------------------------------------------------------
	void ReadMotionInfo(void* pEIMFILE);

	//------------------------------------------------------------------------------
	//
	//  @brief		ディレクトリを考慮したテクスチャのファイルパスを作成する
	//	@param[in]	pFilePath	モデルのファイルパス
	//	@param[in]	pEIMFILE	IEMFILE
	//
	//------------------------------------------------------------------------------
	void CreateTextureFilepath(const char* pFilePath, void* pEIMFILE);

	//------------------------------------------------------------------------------
	//
	//  @brief		ReadFileのラッパー関数
	//	@param[in]	pBuffer		データを取得
	//	@param[in]	dataSize	取得するデータサイズ
	//
	//------------------------------------------------------------------------------
	void ReadFile(LPVOID pBuffer, DWORD dataSize);


};
#endif // !__IEM_LOADER_H_
