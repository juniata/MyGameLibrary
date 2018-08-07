//#include	"DX_Library.h"
//#include	"../ModelLoader/IEMLoader.h"
//using namespace DirectX;
//
////-----------------------------------------------------------------------------------------
////
////	メンバ変数を初期化
////
////-----------------------------------------------------------------------------------------
//DX_SkinMesh::DX_SkinMesh() :
//
////	ボーン情報を初期化
//m_pBoneInfo(nullptr),
//m_pAnimationInfo(nullptr),
//m_boneCount(0),
//
////	パレット行列を初期化
//m_pBoneMatrix(nullptr),
//
//m_motion(0),
//m_frameCount(0),
//m_pFrameFlag(nullptr),
//m_dwFrame(0),
//m_frame(0),
//
//m_pSkinBuffer(nullptr)
//{
//	ZeroMemory(m_param, sizeof(m_param));
//	ZeroMemory(m_offset, sizeof(m_offset));
//
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	メンバ変数を初期化し、モデルを読み込む
////
////-----------------------------------------------------------------------------------------
//DX_SkinMesh::DX_SkinMesh(const char* pFilepath) : DX_SkinMesh()
//{
//	//	モデルを読み込み
//	LoadModel(pFilepath);
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	全データ解放
////
////-----------------------------------------------------------------------------------------
//DX_SkinMesh::~DX_SkinMesh()
//{
//	//	自身がオリジナルなら、データを解放する
//	if (IsOriginal()){
//
//		for (int i = 0; i < m_boneCount; ++i){
//
//			//	ボーンデータを解放
//			DELETE_OBJ_ARRAY(m_pBoneInfo[i].pInfluence);
//			DELETE_OBJ_ARRAY(m_pBoneInfo[i].pWeightIndex);
//
//			//	アニメーションデータを解放
//			DELETE_OBJ_ARRAY(m_pAnimationInfo[i].pRotFrame);
//			DELETE_OBJ_ARRAY(m_pAnimationInfo[i].pRot);
//			DELETE_OBJ_ARRAY(m_pAnimationInfo[i].pPosFrame);
//			DELETE_OBJ_ARRAY(m_pAnimationInfo[i].pPos);
//		}
//
//		//	ボーンデータを解放
//		DELETE_OBJ_ARRAY(m_pBoneInfo);
//
//		//	アニメーションデータを解放
//		DELETE_OBJ_ARRAY(m_pAnimationInfo);
//
//		SAFE_RELEASE(m_pSkinBuffer);
//	}
//	else{
//		//	パレット行列だけは別なので、削除しておく
//		DELETE_OBJ_ARRAY(m_pBoneMatrix);
//	}
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	オブジェクトを複製する
////
////-----------------------------------------------------------------------------------------
//DX_SkinMesh* DX_SkinMesh::Clone()const
//{
//	DX_SkinMesh* l_pSkinMesh = new DX_SkinMesh(*this);
//	l_pSkinMesh->m_bClone = true;
//	
//	l_pSkinMesh->m_pBoneMatrix = new DirectX::XMFLOAT4X4[m_boneCount];
//	CopyMemory(l_pSkinMesh->m_pBoneMatrix, m_pBoneMatrix,sizeof(DirectX::XMFLOAT4X4)* m_boneCount);
//	return l_pSkinMesh;
//}
//
////-----------------------------------------------------------------------------------------
////
////	モデルを読み込む
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::LoadModel(const char* pFilepath)
//{
//	//	デバイスを取得する
//	auto l_pDevice = DX_System::GetInstance()->GetDevice();
//
//	//	IEMLoaderを生成
//	IEMLoader l_iemLoader(pFilepath);
//
//	//	IEM情報から各変数の最大数を取得
//	m_indexCount	= l_iemLoader.GetIndexCount();
//	m_materialCount = l_iemLoader.GetMaterialCount();
//	m_vertexCount	= l_iemLoader.GetVertexCount();
//	m_faceCount		= l_iemLoader.GetFaceCount();
//	m_frameCount = l_iemLoader.GetFrameCount();
//	m_boneCount = l_iemLoader.GetBoneCount();
//
//	//	サイズを一気に確保する
//	std::vector<tagMeshVertex>	l_vertexList(m_vertexCount);
//	std::vector<WORD>			l_indexList(m_indexCount);
//
//	//	グループ描画用構造体を生成
//	m_pGroupMesh = new tagGroupMesh[m_materialCount];
//
//	//	頂点データをコピーする
//	CopyMemory(&l_vertexList[0], l_iemLoader.GetVertices(), sizeof(tagIEMMeshVertex)* m_vertexCount);
//
//	//	インデックスをマテリアル順に直す
//	CreateIndexMaterialOrder(&l_indexList, l_iemLoader.GetIndices(), l_iemLoader.GetAttributes());
//
//	//	頂点バッファを作成
//	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(l_pDevice, sizeof(tagMeshVertex)* m_vertexCount, (tagMeshVertex*)&l_vertexList[0]);
//
//	//	インデックスバッファ
//	m_pIndexBuffer = DX_Buffer::CreateIndexBuffer(l_pDevice, sizeof(WORD)* l_indexList.size(), (LPWORD)&l_indexList[0]);
//
//	//	マテリアル情報を作成
//	CreateTextureInfo(l_iemLoader.GetTextures());
//
//	//	解放
//	l_vertexList.clear();
//	l_indexList.clear();
//
//	//	ボーン情報、アニメーション情報、頂点座標を各々分生成
//	m_pBoneInfo = new tagIEMBoneSkinMesh[m_boneCount];
//	m_pAnimationInfo = new tagIEMAnimation[m_boneCount];
//	ZeroMemory(m_pAnimationInfo, sizeof(tagIEMAnimation)* m_boneCount);
//
//	//	アニメーション情報
//	m_pFrameFlag = new WORD[m_frameCount];
//	CopyMemory(m_offset, l_iemLoader.GetOffSet(), sizeof(m_offset));
//	CopyMemory(m_pFrameFlag, l_iemLoader.GetFrameFlag(), sizeof(WORD)* m_frameCount);
//
//	tagIEMBoneSkinMesh* l_pIEMBoneSkineMeshInfo = nullptr;
//	tagIEMAnimation*	l_pIEMAnimationInfo = nullptr;
//	tagIEMBone*			l_pTempIEMBoneInfoData = nullptr;
//	tagIEMMotion*		l_pTempIEMMotionInfo = nullptr;
//
//	for (int i = 0; i < m_boneCount; ++i){
//
//		//	アドレスを取得
//		l_pIEMBoneSkineMeshInfo = &m_pBoneInfo[i];
//		l_pIEMAnimationInfo = &m_pAnimationInfo[i];
//		l_pTempIEMBoneInfoData = &l_iemLoader.GetBoneInfo()[i];
//		l_pTempIEMMotionInfo = &l_iemLoader.GetMotionInfo()[i];
//
//		//	データをコピー、及び生成
//		l_pIEMBoneSkineMeshInfo->parent = l_pTempIEMBoneInfoData->parent;			//	親
//		l_pIEMBoneSkineMeshInfo->offsetMatrix = l_pTempIEMBoneInfoData->boneMatrix;		//	基準化行列
//		l_pIEMBoneSkineMeshInfo->orgPos = l_pTempIEMBoneInfoData->orgPos;			//	標準位置
//		l_pIEMBoneSkineMeshInfo->orgPose = l_pTempIEMBoneInfoData->orgPose;			//	標準指定
//		l_pIEMBoneSkineMeshInfo->weightIndexCount = l_pTempIEMBoneInfoData->indexCount;		//	ウェイトインデックスの数
//		l_pIEMBoneSkineMeshInfo->pWeightIndex = new DWORD[l_pIEMBoneSkineMeshInfo->weightIndexCount];	// ウェイトのインデックス
//		l_pIEMBoneSkineMeshInfo->pInfluence = new float[l_pIEMBoneSkineMeshInfo->weightIndexCount];	//	ウェイトの影響力
//
//		//	データをコピー
//		CopyMemory(l_pIEMBoneSkineMeshInfo->pWeightIndex, l_pTempIEMBoneInfoData->pIndices, sizeof(DWORD)*	l_pIEMBoneSkineMeshInfo->weightIndexCount);
//		CopyMemory(l_pIEMBoneSkineMeshInfo->pInfluence, l_pTempIEMBoneInfoData->pInfluence, sizeof(float)*	l_pIEMBoneSkineMeshInfo->weightIndexCount);
//
//		//	アニメーション情報
//		l_pIEMAnimationInfo->rotCount = l_pTempIEMMotionInfo->rotateCount;
//		l_pIEMAnimationInfo->pRotFrame = new WORD[l_pIEMAnimationInfo->rotCount];
//		l_pIEMAnimationInfo->pRot = new DirectX::XMFLOAT4[l_pIEMAnimationInfo->rotCount];
//
//		//	クォータニオンコピー
//		for (DWORD j = 0; j < l_pIEMAnimationInfo->rotCount; ++j){
//			l_pIEMAnimationInfo->pRotFrame[j] = l_pTempIEMMotionInfo->pRotateFrame[j];
//			l_pIEMAnimationInfo->pRot[j] = l_pTempIEMMotionInfo->pRotate[j];
//		}
//
//		//	ポジションコピー
//		l_pIEMAnimationInfo->posCount = l_pTempIEMMotionInfo->posCount;
//
//		if (l_pIEMAnimationInfo->posCount > 0){
//			l_pIEMAnimationInfo->pPosFrame = new WORD[l_pIEMAnimationInfo->posCount];
//			l_pIEMAnimationInfo->pPos = new DirectX::XMFLOAT3[l_pIEMAnimationInfo->posCount];
//			CopyMemory(l_pIEMAnimationInfo->pPosFrame, l_pTempIEMMotionInfo->pPosFrame, sizeof(WORD)*		l_pIEMAnimationInfo->posCount);
//			CopyMemory(l_pIEMAnimationInfo->pPos, l_pTempIEMMotionInfo->pPos, sizeof(DirectX::XMFLOAT3)*	l_pIEMAnimationInfo->posCount);
//		}
//	}
//
//	//	ボーンのパレット行列
//	m_pBoneMatrix = new DirectX::XMFLOAT4X4[m_boneCount];
//	ZeroMemory(m_pBoneMatrix, sizeof(DirectX::XMFLOAT4X4)* m_boneCount);
//
//	//	頂点数確保
//	std::vector<tagVertexSkinInfo> l_vertexSkinList(m_vertexCount);
//
//	//	一つのポリゴンに影響するボーンは４つ
//	//	index == uint4   weight == float4
//	//	1つの頂点にインデックスとウェイトが４つある
//	tagIEMBoneSkinMesh* l_pTempSkinMesh = nullptr;
//	tagVertexSkinInfo*	l_pVertexSkinInfo = nullptr;
//	std::vector<int>	l_weightIndexCount(m_vertexCount, 0);
//
//	DWORD l_weinghtIndex = 0;
//
//	//	bone数だけ回す
//	for (int bone = 0; bone < m_boneCount; ++bone){
//
//		//	アドレスを取得
//		l_pTempSkinMesh = &m_pBoneInfo[bone];
//
//		//	ウェイトカウント分回す
//		for (int i = 0; i < l_pTempSkinMesh->weightIndexCount; ++i){
//
//			//	影響を与える頂点のインデックスを取得
//			l_weinghtIndex = l_pTempSkinMesh->pWeightIndex[i];
//
//			//	ウェイトインデックスでアクセス
//			l_pVertexSkinInfo = &l_vertexSkinList[l_weinghtIndex];
//
//			//	この頂点が影響を受けるボーンの番号を保持
//			//	ビット演算でuint4 ~ uint1にパック
//			l_pVertexSkinInfo->boneNum |= (bone << (l_weightIndexCount[l_weinghtIndex] * 8));
//
//			//	この頂点がboneから受ける影響力を設定
//			l_pVertexSkinInfo->weight[l_weightIndexCount[l_weinghtIndex]] = l_pTempSkinMesh->pInfluence[i];
//
//			//	インデックスをカウントする
//			l_weightIndexCount[l_weinghtIndex] += 1;
//		}
//	}
//
//	//	スキンバッファを作成する
//	m_pSkinBuffer = DX_Buffer::CreateSkinBuffer(l_pDevice, sizeof(tagVertexSkinInfo)* m_vertexCount, (tagVertexSkinInfo*)&l_vertexSkinList[0]);
//
//	//	スキン情報を削除
//	l_vertexSkinList.clear();
//	l_weightIndexCount.clear();
//}
////-----------------------------------------------------------------------------------------
////
////	座標やメッシュを更新
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::Update()
//{
//	//	スキンメッシュ更新
//	if (m_frame != m_dwFrame){
//	
//		//	フレームによって姿勢を更新する
//		UpdateSkinMeshFrame(CAST_F(m_dwFrame));
//
//		//	パレット行列を作成
//		UpdateBoneMatrix();
//	}
//
//	DX_Mesh::Update();
//
//	m_frame = m_dwFrame;
//
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	スキンメッシュを描画
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::Render()
//{
//	//	デバイスコンテキストを取得
//	auto l_pDeviceContext = DX_System::GetInstance()->GetDeviceContext();
//
//	//	stride
//	unsigned int l_stride[] = {
//		sizeof(tagMeshVertex),
//		sizeof(tagVertexSkinInfo)
//	};
//
//	//	offset
//	unsigned int l_offset = 0;
//
//	//	IAにverteBufferを設定
//	l_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &l_stride[0], &l_offset);
//
//	//	IAにSkinBufferを設定
//	l_pDeviceContext->IASetVertexBuffers(1, 1, &m_pSkinBuffer, &l_stride[1], &l_offset);
//
//	//	行優先から列優先に変更(float4x4 → flaot3x4)
//	XMMATRIX boneMat;
//	for (int i = 0; i < m_boneCount; ++i){
//		boneMat = XMLoadFloat4x4(&m_pBoneMatrix[i]);
//		XMStoreFloat4x4(&m_pBoneMatrix[i], XMMatrixTranspose(boneMat));
//	}
//
//	//	Bone行列を頂点シェーダーのregister(t1)に送る
//	DX_ShaderManager::GetInstance()->SetMatrixResoruce(1, m_pBoneMatrix, m_boneCount, l_pDeviceContext, DX_SHADER_TYPE::VERTEX_SHADER);
//
//	//	レイピックフラグが立っていれば、レイピック処理の入ったジオメトリシェーダーを取得
//	DX_Shader* l_pGeometryShader = ((m_bRayPick) ? DX_ShaderManager::GetInstance()->GetDefaultGeometryShaderRayPick() : DX_ShaderManager::GetInstance()->GetDefaultGeometryShader3D());
//
//	//	描画
//	DX_Mesh::Render(
//		l_pDeviceContext,
//		DX_ShaderManager::GetInstance()->GetDefaultVertexShaderSkinMesh(),
//		l_pGeometryShader,
//		DX_ShaderManager::GetInstance()->GetDefaultPixelShader3D(),
//		DX_ShaderManager::GetInstance()->GetDefaultInputLayoutSkinMesh()
//		);
//	
//	//	レイピック処理フラグをfalseにする
//	m_bRayPick = false;
//}
////-----------------------------------------------------------------------------------------
////
////	アニメーションフレームを更新
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::Animation()
//{
//	int l_param = m_pFrameFlag[m_dwFrame];
//	int l_work = m_dwFrame;
//
//	if (l_param & 0x4000) { l_param = 0xFFFF; }
//	if (l_param != 0xFFFF){
//
//		//	アニメーションジャンプ
//		if (l_param & 0x8000){
//			SetMotion(l_param & 0xFF);
//		}
//		else {
//			m_dwFrame = l_param;
//		}
//	}
//	else{
//		m_dwFrame++;
//		if (m_dwFrame >= m_frameCount){ m_dwFrame = 0; }
//	}
//
//	if (m_dwFrame != l_work){ m_bChanged = true; }
//
//	l_param = m_pFrameFlag[m_dwFrame];
//	if ((l_param != 0xFFFF) && (l_param & 0x4000)){
//		m_param[(l_param & 0x0F00) >> 8] = (unsigned char)(l_param & 0x00FF);
//	}
//
//}
//
////-----------------------------------------------------------------------------------------
////
////	モーションを設定する
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::SetMotion(const int motinoNum)
//{
//	int		l_param;
//
//	if (m_offset[motinoNum] == 65535){ return; }
//	m_motion 	= motinoNum;
//	m_dwFrame 	= m_offset[motinoNum];
//	m_bChanged 	= true;
//
//	l_param = m_pFrameFlag[m_dwFrame];
//	if ((l_param != 0xFFFF) && (l_param & 0x4000)) {
//		m_param[(l_param & 0x0F00) >> 8] = (unsigned char)(l_param & 0x00FF);
//	}
//}
//
////-----------------------------------------------------------------------------------------
////
////	フレームによって姿勢を更新する
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::UpdateSkinMeshFrame(const float frame)
//{
//	tagIEMAnimation*	l_pAnimation 	= nullptr;
//	tagIEMBoneSkinMesh*	l_pIEMBoneInfo 	= nullptr;
//
//	for (int i = 0; i < m_boneCount; ++i){
//
//		//	アドレス取得
//		l_pAnimation	= &m_pAnimationInfo[i];
//		l_pIEMBoneInfo	= &m_pBoneInfo[i];
//
//		//	ポーズ設定
//		UpdateBonePoseByFrame(frame, l_pIEMBoneInfo, l_pAnimation);
//
//		//	座標設定
//		UpdateBonePosByFrame(frame, l_pIEMBoneInfo, l_pAnimation);
//	}
//}
//
////-----------------------------------------------------------------------------------------
////
////	フレームによってm_pBoneInfo->curPoseを更新する
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::UpdateBonePoseByFrame(const float argFrame, tagIEMBoneSkinMesh* pIEMBoneSkinMesh, tagIEMAnimation* pIEMAnimation)
//{
//	float l_t;
//	DWORD j;
//
//	switch (pIEMAnimation->rotCount){
//
//		//	フレームが０の場合は初期姿勢を設定する
//	case 0:	pIEMBoneSkinMesh->curPose = pIEMBoneSkinMesh->orgPose;		break;
//
//		//	フレームが１の場合は、アニメーション１フレーム目の姿勢を設定する
//	case 1:	pIEMBoneSkinMesh->curPose = pIEMAnimation->pRot[0];			break;
//
//		//	それ以外の場合
//	default:
//	{
//		XMVECTOR q1;
//		XMVECTOR q2;
//		//	回転キー補間
//		for (j = 0; j < pIEMAnimation->rotCount - 1; ++j) {
//
//			//	現在位置検索
//			if (IsCurrentFrameIsInTheMiddle(argFrame, pIEMAnimation->pRotFrame[j], pIEMAnimation->pRotFrame[j + 1])) {
//
//				//	経過フレームを取得
//				l_t = GetElapsedFrame(argFrame, pIEMAnimation->pRotFrame[j], pIEMAnimation->pRotFrame[j + 1]);
//
//				//	補間
//				q1 = XMLoadFloat4(&pIEMAnimation->pRot[j]);
//				q2 = XMLoadFloat4(&pIEMAnimation->pRot[j + 1]);
//				XMStoreFloat4(&pIEMBoneSkinMesh->curPose, XMQuaternionSlerp(q1, q2, l_t));
//				break;
//			}
//		}
//
//		if (j == pIEMAnimation->rotCount - 1) {
//			pIEMBoneSkinMesh->curPose = pIEMAnimation->pRot[pIEMAnimation->rotCount - 1];
//		}
//		break;
//	}
//	} // switch (pIEMAnimation->rotCount){
//}
//
////-----------------------------------------------------------------------------------------
////
////	フレームによってm_pBoneInfo->curPosを更新する
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::UpdateBonePosByFrame(const float argFrame, tagIEMBoneSkinMesh* pIEMBoneSkinMesh, tagIEMAnimation* pIEMAnimation)
//{
//	float l_t;
//	DWORD j;
//
//	if (pIEMAnimation->posCount == 0){
//		pIEMBoneSkinMesh->curPos = pIEMBoneSkinMesh->orgPos;
//	}
//	else{
//
//		XMVECTOR currPos;
//		XMVECTOR nextPos;
//
//		//	位置補間
//		for (j = 0; j < pIEMAnimation->posCount - 1; ++j){
//
//			//	現在位置検索
//			if (IsCurrentFrameIsInTheMiddle(argFrame, pIEMAnimation->pPosFrame[j], pIEMAnimation->pPosFrame[j + 1])){
//
//				//	経過フレームを取得
//				l_t = GetElapsedFrame(argFrame, pIEMAnimation->pPosFrame[j], pIEMAnimation->pPosFrame[j + 1]);
//
//				//	補間
//				currPos = XMLoadFloat3(&pIEMAnimation->pPos[j]);
//				nextPos = XMLoadFloat3(&pIEMAnimation->pPos[j + 1]);
//
//				XMStoreFloat3(&pIEMBoneSkinMesh->curPos, currPos + (nextPos - currPos) * l_t);
//				//pIEMBoneSkinMesh->curPos = pIEMAnimation->pPos[j] + (pIEMAnimation->pPos[j + 1] - pIEMAnimation->pPos[j]) *l_t;
//				break;
//			}
//		}
//
//
//		if (j == pIEMAnimation->posCount - 1){
//			pIEMBoneSkinMesh->curPos = pIEMAnimation->pPos[pIEMAnimation->posCount - 1];
//		}
//	}
//}
//
////-----------------------------------------------------------------------------------------
////
////	現在のフレームと開始、終了時フレームから経過フレームを算出する
////
////-----------------------------------------------------------------------------------------
//float DX_SkinMesh::GetElapsedFrame(const float argFrame, const WORD argCurFrame, const WORD argNextFrame)
//{
//	//	(現在フレーム - 開始フレーム) / (終了フレーム - 開始フレーム)
//	return (CAST_F(argFrame - argCurFrame) / CAST_F(argNextFrame - argCurFrame));
//}
//
////-----------------------------------------------------------------------------------------
////
////	現在のフレームが、現在のモーションと次のモーションの間にあるかどうか。あればtrue
////
////-----------------------------------------------------------------------------------------
//bool DX_SkinMesh::IsCurrentFrameIsInTheMiddle(const float argFrame, const WORD argCurFrame, const WORD argNextFrame)
//{
//	return ((argFrame >= argCurFrame) && (argFrame < argNextFrame));
//}
//
////-----------------------------------------------------------------------------------------
////
////	ボーン行列を更新
////
////-----------------------------------------------------------------------------------------
//void DX_SkinMesh::UpdateBoneMatrix()
//{
//	tagIEMBoneSkinMesh*	l_pIEMBoneInfo = nullptr;
//	
//	XMMATRIX boneMat;
//	XMMATRIX parentBoneMat;
//	XMMATRIX offfsetMat;
//	XMVECTOR curPose;
//	XMVECTOR curPos;
//	for (int i = 0; i < m_boneCount; ++i){
//
//		//	アドレスを取得
//		l_pIEMBoneInfo = &m_pBoneInfo[i];
//
//		curPose = XMLoadFloat4(&l_pIEMBoneInfo->curPose);
//		curPos  = XMLoadFloat3(&l_pIEMBoneInfo->curPos);
//		boneMat = XMLoadFloat4x4(&l_pIEMBoneInfo->boneMatrix);
//
//		//	クォータニオンから回転行列を取得
//		boneMat = XMMatrixRotationQuaternion(curPose);
//		boneMat.r[3] = curPos;
//
//		//D3DXMatrixRotationQuaternion(
//		//	&l_pIEMBoneInfo->boneMatrix,
//		//	&l_pIEMBoneInfo->curPose
//		//	);
//		////	位置情報コピー
//		//l_pIEMBoneInfo->boneMatrix._41 = l_pIEMBoneInfo->curPos.x;
//		//l_pIEMBoneInfo->boneMatrix._42 = l_pIEMBoneInfo->curPos.y;
//		//l_pIEMBoneInfo->boneMatrix._43 = l_pIEMBoneInfo->curPos.z;
//
//		//	自身のボーンに親がいれば、親の行列をかける
//		if (l_pIEMBoneInfo->parent != 0xFFFF){
//			parentBoneMat = XMLoadFloat4x4(&m_pBoneInfo[l_pIEMBoneInfo->parent].boneMatrix);
//			boneMat *= parentBoneMat;
//			//l_pIEMBoneInfo->boneMatrix *= m_pBoneInfo[l_pIEMBoneInfo->parent].boneMatrix;
//		}
//
//		//	アニメーション行列  = 基本化行列 * bone行列
//		offfsetMat = XMLoadFloat4x4(&l_pIEMBoneInfo->offsetMatrix);
//		XMStoreFloat4x4(&m_pBoneMatrix[i], offfsetMat * boneMat);
//		//m_pBoneMatrix[i] = l_pIEMBoneInfo->offsetMatrix * l_pIEMBoneInfo->boneMatrix;
//	}
//}
