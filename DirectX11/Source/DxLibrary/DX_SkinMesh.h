//#ifndef __DX_SKIN_MESH_H_
//#define __DX_SKIN_MESH_H_
//
////----------------------------------------------------------
////	頂点が保持するskin情報
////----------------------------------------------------------
////	頂点が保持するｓｋｉｎ情報
//struct tagVertexSkinInfo
//{
//	unsigned int	boneNum;
//	float			weight[4];
//	tagVertexSkinInfo() : boneNum(0)
//	{
//		ZeroMemory(weight, sizeof(weight));
//	}
//};
//
//
////----------------------------------------------------------
////	IEM用のアニメーション情報
////----------------------------------------------------------
//struct tagIEMAnimation
//{
//	unsigned long	rotCount;
//	unsigned short* pRotFrame;
//	DirectX::XMFLOAT4*		pRot;
//
//	unsigned long	posCount;
//	unsigned short* pPosFrame;
//	DirectX::XMFLOAT3* pPos;
//
//};
//
////----------------------------------------------------------
////	KM_SkinMeshクラスで使うIEM用ボーン情報
////----------------------------------------------------------
//struct tagIEMBoneSkinMesh
//{
//	WORD	parent;			//	親
//	DirectX::XMFLOAT4X4	offsetMatrix;	//	基準化行列
//	DirectX::XMFLOAT4X4	boneMatrix;		//	計算結果を格納する行列
//
//	DirectX::XMFLOAT4	orgPose;	//	標準指定
//	DirectX::XMFLOAT3	orgPos;		//	標準位置
//
//	DirectX::XMFLOAT4	curPose;
//	DirectX::XMFLOAT3	curPos;
//
//	float*			pInfluence;			//	ウェイトの影響力
//	unsigned long*	pWeightIndex;		//	ウェイトのインデックス
//	unsigned short  weightIndexCount;	//	ウェイトインデックスの数
//
//};
//
//
//
////****************************************************************************************************
////
////	SkinMesh
////
////****************************************************************************************************
//class DX_SkinMesh : public DX_Mesh
//{
//public:
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		メンバ変数を初期化
//	//
//	//------------------------------------------------------------------------------
//	DX_SkinMesh();
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		メンバ変数を初期化し、モデルを読み込む
//	//	@param[in]	pFilepath	モデルのファイルパス
//	//
//	//------------------------------------------------------------------------------
//	DX_SkinMesh(const char* pFilepath);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		全データ解放
//	//
//	//------------------------------------------------------------------------------
//	~DX_SkinMesh();
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		オブジェクトを複製
//	//	@retrun		ポインタが返る
//	//
//	//------------------------------------------------------------------------------
//	DX_SkinMesh* Clone()const;
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		モデルを読み込む
//	//	@param[in]	pFilepath	モデルのファイルパス
//	//
//	//------------------------------------------------------------------------------
//	void LoadModel(const char* pFilepath);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		座標やメッシュを更新
//	//
//	//------------------------------------------------------------------------------
//	void Update();
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		スキンメッシュを描画
//	//
//	//------------------------------------------------------------------------------
//	void Render();
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		モデルをアニメーションさせる
//	//
//	//------------------------------------------------------------------------------
//	void Animation();
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		モーションを切り返る
//	//	@param		motinoNum	モーション番号	
//	//
//	//------------------------------------------------------------------------------
//	void SetMotion(const int motinoNum);
//
//private:
//	//	スキンバッファ
//	ID3D11Buffer*	m_pSkinBuffer;
//
//	tagIEMBoneSkinMesh*	m_pBoneInfo;		//	ボーン情報
//	tagIEMAnimation*	m_pAnimationInfo;	//	アニメーション情報
//
//
//	DirectX::XMFLOAT4X4*	m_pBoneMatrix;	//	パレット行列
//
//	int m_boneCount;	//	ボーン数
//
//	unsigned char	m_motion;		//	現在のモーション番号
//	unsigned short	m_frameCount;	//	フレーム数
//	unsigned short* m_pFrameFlag;	//	フレーム情報
//	unsigned short	m_offset[256];		//	モーション先頭フレーム
//	unsigned char	m_param[16];
//
//	int m_dwFrame;
//	int m_frame;
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		フレームによって姿勢を更新する
//	//	@param[in]	frame	フレーム
//	//
//	//------------------------------------------------------------------------------
//	void UpdateSkinMeshFrame(const float frame);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		フレームによってm_pBoneInfo->curPoseを更新する
//	//	@param[in]	frame	フレーム
//	//
//	//------------------------------------------------------------------------------
//	void UpdateBonePoseByFrame(const float frame, tagIEMBoneSkinMesh* pIEMBoneSkinMesh, tagIEMAnimation* pIEMAnimation);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		フレームによってm_pBoneInfo->curPosを更新する
//	//	@param[in]	frame	フレーム
//	//
//	//------------------------------------------------------------------------------
//	void UpdateBonePosByFrame(const float frame, tagIEMBoneSkinMesh* pIEMBoneSkinMesh, tagIEMAnimation* pIEMAnimation);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		現在のフレームと開始、終了時フレームから経過フレームを算出する
//	//	@param[in]	frame		m_dwFrame
//	//	@param[in]	curFrame	m_pAnimationInfo->pRotFrame[i]
//	//	@param[in]	nextFrame	m_pAnimationInfo->pRotFrame[i + 1]
//	//	@retrun		補間されたフレーム
//	//
//	//------------------------------------------------------------------------------
//	float GetElapsedFrame(const float frame, const WORD curFrame, const WORD argNextFrame);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		現在のフレームが、現在のモーションと次のモーションの間にあるかどうか。
//	//	@param[in]	frame		m_dwFrame
//	//	@param[in]	curFrame	m_pAnimationInfo->pRotFrame[i]
//	//	@param[in]	nextFrame	m_pAnimationInfo->pRotFrame[i + 1]
//	//	@retrun		ture:ある	false：ない
//	//
//	//------------------------------------------------------------------------------
//	bool IsCurrentFrameIsInTheMiddle(const float frame, const WORD argCurFrame, const WORD nextFrame);
//
//	//------------------------------------------------------------------------------
//	//
//	//  @brief		ボーンの行列を更新する
//	//
//	//------------------------------------------------------------------------------
//	void UpdateBoneMatrix();
//
//};
//#endif // !__DX_SKIN_MESH_H_
