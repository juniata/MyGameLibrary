//#include	"DX_Library.h"
//#include	"../ModelLoader/IMOLoader.h"
//using namespace DirectX;
//
////-----------------------------------------------------------------------------------------
////
////	マクロ定義
////
////-----------------------------------------------------------------------------------------
////	DWORDからVECTOR4に変換
//#define VECTOR4_FROM_DWORD(dword) XMFLOAT4( (float((dword & 0xFF000000) >> 24) / 255.0f),(float((dword & 0x00FF0000) >> 16) / 255.0f),(float((dword & 0x0000FF00) >> 8) / 255.0f) ,(float((dword & 0x000000FF)) / 255.0f))
//
//#define TEXTURE_DECAL_RESOURCE_NUM		0
//#define TEXTURE_NORMAL_RESOURCE_NUM		1
//#define TEXTURE_SPECULAR_RESOURCE_NUM	2
//#define TEXTURE_HEIGHT_RESOURCE_NUM		3
//
////-----------------------------------------------------------------------------------------
////
////	メンバー変数の初期化
////
////-----------------------------------------------------------------------------------------
//DX_Mesh::DX_Mesh() :
//
////	グループメッシュを初期化
//m_pGroupMesh(nullptr),
//
////	モデルの頂点数などを初期化
//m_indexCount(0),
//m_materialCount(0),
//m_vertexCount(0),
//m_faceCount(0),
//
////	座標等を初期化
//m_pos(XMFLOAT3(0.0f,0.0f,0.0f)),
//m_scale(XMFLOAT3(1.0f,1.0f,1.0f)),
//m_angle(XMFLOAT3(0.0f,0.0f,0.0f)),
//
////	変更フラグとクローンフラグとレイピックフラグを初期化
//m_bChanged(true),
//m_bClone(false),
//m_bRayPick(false),
//
//m_pIndexBuffer(nullptr),
//m_pRayPickBuffer(nullptr),
//m_pVertexBuffer(nullptr)
//{
//	//	行列更新を行う
//	Update();
//}
//
////-----------------------------------------------------------------------------------------
////
////	メンバ変数を解放
////
////-----------------------------------------------------------------------------------------
//DX_Mesh::~DX_Mesh()
//{
//	//	自身がオリジナルデータなら、解放処理を行う
//	if (IsOriginal()){
//
//		//	テクスチャを解放
//		for (DWORD i = 0; i < m_materialCount; ++i){
//			DX_TextureManager::Release(m_pGroupMesh[i].pTextureDecal);
//			DX_TextureManager::Release(m_pGroupMesh[i].pTextureNormal);
//			DX_TextureManager::Release(m_pGroupMesh[i].pTextureSpecular);
//			DX_TextureManager::Release(m_pGroupMesh[i].pTextureHeight);
//		}
//
//		//	groupMeshを解放
//		DELETE_OBJ_ARRAY(m_pGroupMesh);
//
//		SAFE_RELEASE(m_pIndexBuffer);
//		SAFE_RELEASE(m_pRayPickBuffer);
//		SAFE_RELEASE(m_pVertexBuffer);
//	}
//}
//
////-----------------------------------------------------------------------------------------
////
////	メンバ変数を初期化し、モデルを読み込む
////
////-----------------------------------------------------------------------------------------
//DX_Mesh::DX_Mesh(const char* pFilepath) : DX_Mesh()
//{
//	//	モデルを読み込む
//	LoadModel(pFilepath);
//}
//
////-----------------------------------------------------------------------------------------
////
////	オブジェクトを複製
////
////-----------------------------------------------------------------------------------------
//DX_Mesh* DX_Mesh::Clone()const
//{
//	//	デフォルトコピーコンストラクタで生成する
//	DX_Mesh*	l_pClone = new DX_Mesh(*this);
//
//	//	クローンフラグを立てる
//	l_pClone->m_bClone = true;
//
//	return l_pClone;
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	座標等をお越しん
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::Update()
//{
//	//	Set関数で座標等を変更された場合のみ、行列計算を行う
//	if (m_bChanged){
//
//		//	拡大行列 * 回転行列 * 平行行列
//		XMMATRIX scale = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
//		XMMATRIX angle = XMMatrixRotationRollPitchYaw(m_angle.x, m_angle.y, m_angle.z);
//		XMMATRIX translation = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
//		
//		XMStoreFloat4x4(&m_worldMatrix, angle * scale * translation);
//
//		m_bChanged = false;
//	}
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	モデルを描画
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::Render()
//{
//	//	デバイスコンテキストを取得
//	ID3D11DeviceContext*	l_pDeviceContext = DX_System::GetDeviceContext();
//
//	//	buffer stride and offset
//	unsigned int l_stride = sizeof(tagMeshVertex);
//	unsigned int l_offset = 0;
//
//	//	VertexBufferを送る
//	l_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &l_stride, &l_offset);
//
//	//	レイピックフラグが立っていれば、レイピック処理の入ったジオメトリシェーダーを取得
//	DX_Shader* l_pGeometryShader =  ((m_bRayPick) ? DX_ShaderManager::GetInstance()->GetDefaultGeometryShaderRayPick() : DX_ShaderManager::GetInstance()->GetDefaultGeometryShader3D());
//
//	if (m_bRayPick){
//		unsigned int l_offset[] = { 0 };
//		l_pDeviceContext->SOSetTargets(1, &m_pRayPickBuffer, l_offset);
//	}
//	//	描画
//	Render(
//		l_pDeviceContext,
//		DX_ShaderManager::GetInstance()->GetDefaultVertexShader3D(),
//		l_pGeometryShader,
//		DX_ShaderManager::GetInstance()->GetDefaultPixelShader3D(),
//		DX_ShaderManager::GetInstance()->GetDefaultInputLayout3D()
//		);
//
//	if (m_bRayPick){
//		unsigned int l_offset[] = { 0 };
//		ID3D11Buffer* l_pNullBuffer = nullptr;
//		l_pDeviceContext->SOSetTargets(1, &l_pNullBuffer, l_offset);
//
//	}
//	//	レイピック処理フラグをfalseにする
//	m_bRayPick = false;
//}
//
////-----------------------------------------------------------------------------------------
////
////	モデルを読み込む
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::LoadModel(const char* pFilepath)
//{
//	//	デバイスを取得する
//	ID3D11Device*	l_pDevice = DX_System::GetDevice();
//
//	//	IMOLoaderを生成する
//	IMOLoader l_imoLoader(pFilepath);
//
//	//	IMO情報から各変数の最大数を取得
//	m_indexCount	= l_imoLoader.GetIndexCount();
//	m_materialCount = l_imoLoader.GetMaterialCount();
//	m_vertexCount	= l_imoLoader.GetVertexCount();
//	m_faceCount		= l_imoLoader.GetFaceCount();
//	
//	//	サイズを一気に確保する
//	std::vector<tagMeshVertex>	l_vertexList(m_vertexCount);
//	std::vector<WORD>			l_indexList(m_indexCount);
//
//	//	グループ描画用構造体を生成
//	m_pGroupMesh = new tagGroupMesh[m_materialCount];
//
//	//	頂点データをコピーする
//	VertexCopy(&l_vertexList, l_imoLoader.GetVertices());
//
//	//	インデックスをマテリアル順に直す
//	CreateIndexMaterialOrder(&l_indexList, l_imoLoader.GetIndices(), l_imoLoader.GetAttributes());
//	
//	//	頂点バッファを作成
//	m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(l_pDevice,sizeof(tagMeshVertex) * m_vertexCount, (tagMeshVertex*)&l_vertexList[0]);
//
//	//	インデックスバッファ
//	m_pIndexBuffer = DX_Buffer::CreateIndexBuffer(l_pDevice, sizeof(WORD)* l_indexList.size(), (LPWORD)&l_indexList[0]);
//
//	//	レイピックバッファを咲く瀬い
//	m_pRayPickBuffer = DX_Buffer::CreateStreamOutputBuffer(l_pDevice, (sizeof(XMFLOAT4)+sizeof(XMFLOAT3)) * m_faceCount * 3);
//
//	//	マテリアル情報を作成
//	CreateTextureInfo(l_imoLoader.GetTextures());
//
//	//	解放
//	l_vertexList.clear();
//	l_indexList.clear();
//
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	レイピック
////
////-----------------------------------------------------------------------------------------
//bool DX_Mesh::RayPick(
//	XMFLOAT3*		pHitPos,
//	XMFLOAT3*		pHitFaceNormal,
//	const XMFLOAT3& pos,
//	const XMFLOAT3& vec,
//	const float dist
//	)
//{
//	bool l_bHit = false;
//	struct tagRayPick{
//		XMFLOAT4 pos;
//		XMFLOAT3 vec;
//		float dist;
//	}l_rayPick;
//
//	l_rayPick.pos.x = pos.x;
//	l_rayPick.pos.y = pos.y;
//	l_rayPick.pos.z = pos.z;
//	l_rayPick.pos.w = 1.0f;
//	l_rayPick.vec	= vec;
//	l_rayPick.dist	= dist;
//
//	//	ローカル変数
//	ID3D11Buffer*				l_pBuffer			= nullptr;
//	D3D11_BUFFER_DESC			l_bufferDesc		= { NULL };
//	D3D11_MAPPED_SUBRESOURCE	l_subResource		= { NULL };
//	ID3D11DeviceContext*		l_pDeviceContext	= DX_System::GetDeviceContext();
//
//	//	定数バッファを作成
//	l_bufferDesc.ByteWidth		= sizeof(tagRayPick);
//	l_bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
//	l_bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
//	l_bufferDesc.CPUAccessFlags = 0;
//
//	//	bufferを作成
//	DX_System::GetDevice()->CreateBuffer(&l_bufferDesc, nullptr, &l_pBuffer);
//
//	//	updateSubResource
//	l_pDeviceContext->UpdateSubresource(l_pBuffer, 0, nullptr, &l_rayPick, 0, 0);
//	
//	//	GSに送る
//	DX_ResourceManager::SetConstantbuffers(l_pDeviceContext, 0, 1, &l_pBuffer, DX_SHADER_TYPE::GEOMETRY_SHADER);
//
//	//	ジオメトリから出力させるレイピック情報
//	struct tagRaypickOutput{
//		XMFLOAT4 pos;
//		XMFLOAT3 normal;
//	};
//
//	//	CPUで値を読めるバッファを作成
//	ID3D11Buffer* l_pCpuReadBuffer = DX_Buffer::CPUReadBuffer(DX_System::GetDevice(), (sizeof(XMFLOAT4)+sizeof(XMFLOAT4)) * m_faceCount * 3);
//
//	//	値チェック
//	DEBUG_VALUE_CHECK((l_pCpuReadBuffer == nullptr) ? false : true, "DX_Buffer::CPUReadBuffer() : failed");
//
//	//	レイピック用バッファをCPUReadBufferにコピー
//	l_pDeviceContext->CopyResource(l_pCpuReadBuffer, m_pRayPickBuffer);
//
//	//	値を読み込む
//	D3D11_MAPPED_SUBRESOURCE l_map;
//	l_pDeviceContext->Map(l_pCpuReadBuffer, 0, D3D11_MAP::D3D11_MAP_READ, 0, &l_map);
//
//	tagRaypickOutput* l_pRayPick = (tagRaypickOutput*)l_map.pData;
//
//	for (DWORD i = 0; i < m_faceCount * 3; ++i){
//
//		//	wが1の場合は、ヒットしたことになる
//		if (l_pRayPick[i].pos.w == 1){
//
//			//	ヒットした面座標
//			pHitPos->x = l_pRayPick[i].pos.x;
//			pHitPos->y = l_pRayPick[i].pos.y;
//			pHitPos->z = l_pRayPick[i].pos.z;
//
//			//	ヒットした面法線
//			*pHitFaceNormal = l_pRayPick[i].normal;
//
//			//	ヒットフラグをtrueにする
//			l_bHit = true;
//
//			//	for分を抜ける
//			break;
//		}
//
//	}
//
//	//	読み込み終了
//	l_pDeviceContext->Unmap(l_pCpuReadBuffer, 0);
//
//	//	レイピック処理フラグをtrueにする
//	m_bRayPick = true;
//
//	SAFE_RELEASE(l_pBuffer);
//	SAFE_RELEASE(l_pCpuReadBuffer);
//
//	//	ヒットフラグを返す
//	return l_bHit;
//}
////-----------------------------------------------------------------------------------------
////
////  モデルの座標を設定
////
////------------------------------------------------------------------------------
//void DX_Mesh::SetPos(const XMFLOAT3& pos)
//{
//	m_pos		= pos;
//	m_bChanged	= true;
//}
//
////-----------------------------------------------------------------------------------------
////
////  モデルの向きを設定
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::SetAngle(const XMFLOAT3& angle)
//{
//	m_angle		= angle;
//	m_bChanged	= true;
//}
//
////-----------------------------------------------------------------------------------------
////
////  モデルの大きさを設定
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::SetScale(const XMFLOAT3& scale)
//{
//	m_scale		= scale;
//	m_bChanged	= true;
//
//}
//void DX_Mesh::SetScale(const float scale)
//{
//	m_scale.x = m_scale.y = m_scale.z = scale;
//	m_bChanged = true;
//}
//
////-----------------------------------------------------------------------------------------
////
////  モデルの座標、向き、大きさを設定
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::SetTransform(const XMFLOAT3& pos, const XMFLOAT3& angle, const XMFLOAT3& scale)
//{
//	m_pos		= pos;
//	m_angle		= angle;
//	m_scale		= scale;
//	m_bChanged	= true;
//}
//
////-----------------------------------------------------------------------------------------
////
////  モデルの座標を取得
////
////-----------------------------------------------------------------------------------------
//XMFLOAT3 DX_Mesh::GetPos()const
//{
//	return m_pos;
//}
//
////-----------------------------------------------------------------------------------------
////
////  モデルの向きを取得
////
////-----------------------------------------------------------------------------------------
//XMFLOAT3 DX_Mesh::GetAngle()const
//{
//	return m_angle;
//}
//
////-----------------------------------------------------------------------------------------
////
////  モデルの大きさを取得
////
////-----------------------------------------------------------------------------------------
//XMFLOAT3 DX_Mesh::GetScale()const
//{
//	return m_scale;
//}
//
////-----------------------------------------------------------------------------------------
////
////  モデルのワールド行列を取得
////
////-----------------------------------------------------------------------------------------
//XMFLOAT4X4*	DX_Mesh::GetWorldMatrix()
//{
//	return &m_worldMatrix;
//}
//
////-----------------------------------------------------------------------------------------
////
////	モデルがオリジナルかどうかを判定
////
////-----------------------------------------------------------------------------------------
//bool DX_Mesh::IsClone()const
//{
//	return m_bClone;
//}
//
////-----------------------------------------------------------------------------------------
////
////	モデルがオリジナルかどうかを判定
////
////-----------------------------------------------------------------------------------------
//bool DX_Mesh::IsOriginal()const
//{
//	return !m_bClone;
//}
//
////-----------------------------------------------------------------------------------------
////
////	頂点データをコピー
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::VertexCopy(std::vector<tagMeshVertex>* pVertexList, void* pIMOVertex)
//{
//	LPIMOMESHVERTEX	l_pIMOVertex	= (LPIMOMESHVERTEX)pIMOVertex;
//	LPIMOMESHVERTEX l_pTempVertex	= nullptr;
//	tagMeshVertex*	l_pVertexLlist	= nullptr;
//
//	for (DWORD i = 0; i < m_vertexCount; ++i){
//
//		//	アドレスを取得
//		l_pTempVertex = &l_pIMOVertex[i];
//		l_pVertexLlist = &(*pVertexList)[i];
//
//		//	データをコピー
//		l_pVertexLlist->pos		= l_pTempVertex->pos;
//		l_pVertexLlist->uv		= l_pTempVertex->uv;
//		l_pVertexLlist->normal	= l_pTempVertex->normal;
//		l_pVertexLlist->color	= VECTOR4_FROM_DWORD(l_pTempVertex->color);
//	}
//}
//
////-----------------------------------------------------------------------------------------
////
////	マテリアル順にインデックスを作成
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::CreateIndexMaterialOrder(std::vector<WORD>* pIndicesList,LPWORD pIndices, LPDWORD pAttributes)
//{
//	//	添字アクセス用変数
//	int l_vectorIndexCount = 0;
//
//	//	startIndexLocationをカウントする変数
//	int l_startIndexCount = 0;
//
//	//	マテリアル順に頂点を組み替える
//	for (DWORD m = 0; m < m_materialCount; ++m){
//
//		//	m_pGropuMeshを初期化
//		m_pGroupMesh[m].startIndexLocation	= 0;
//		m_pGroupMesh[m].indexCount			= 0;
//		m_pGroupMesh[m].baseVertexLocation	= 0;
//
//		//	インデックスカウント
//		m_pGroupMesh[m].startIndexLocation = l_startIndexCount;
//
//		//	属性数と面数は同じ
//		for (DWORD i = 0; i < m_faceCount; ++i){
//
//			//	もしmateriauNumと取得したマテリアル番号が違うければ、continue
//			if (m != pAttributes[i]){ continue; }
//
//			//	index == m_faceCount * 3
//			for (int j = 0; j < 3; ++j){
//				++l_startIndexCount;
//
//				//	頂点インデックスを追加
//				(*pIndicesList)[l_vectorIndexCount++] = pIndices[i * 3 + j];
//
//				//	インデックスをカウントする
//				m_pGroupMesh[m].indexCount++;
//			}
//		}
//	}
//}
//
////-----------------------------------------------------------------------------------------
////
////	テクスチャ情報を作成する
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::CreateTextureInfo(char pTextures[32][MAX_PATH])
//{
//	char l_pTemp[32][MAX_PATH] = { NULL };
//
//	//	マテリアル分テクスチャを読み込む
//	for (WORD i = 0; i < m_materialCount; ++i){
//
//		//	通常テクスチャ
//		m_pGroupMesh[i].pTextureDecal = DX_TextureManager::GetTexture(pTextures[i]);
//
//		//	法線テクスチャ
//		sprintf_s(l_pTemp[i], MAX_PATH, "N%s", pTextures[i]);
//		m_pGroupMesh[i].pTextureNormal = DX_TextureManager::GetTexture(l_pTemp[i]);
//
//		//	スペキュラテクスチャ
//		sprintf_s(l_pTemp[i], MAX_PATH, "S%s", pTextures[i]);
//		m_pGroupMesh[i].pTextureSpecular = DX_TextureManager::GetTexture(l_pTemp[i]);
//
//		//	高さテクスチャ
//		sprintf_s(l_pTemp[i], MAX_PATH, "H%s", pTextures[i]);
//		m_pGroupMesh[i].pTextureHeight = DX_TextureManager::GetTexture(l_pTemp[i]);
//	}
//}
//
//
////-----------------------------------------------------------------------------------------
////
////	モデルを描画(共通の中身)
////
////-----------------------------------------------------------------------------------------
//void DX_Mesh::Render(
//	ID3D11DeviceContext*	pDeviceContext,
//	DX_Shader*				pVertexShader,
//	DX_Shader*				pGeometryShader,
//	DX_Shader*				pPixelShader,
//	ID3D11InputLayout*		pInputLayout
//	)
//{
//
//	//	IndexBufferを送る
//	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
//
//	//	InputLayoutの設定を送る
//	pDeviceContext->IASetInputLayout(pInputLayout);
//
//	//	Primitiveの設定を送る
//	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	//	ワールド行列を送る
//	DX_ShaderManager::GetInstance()->SetWorldMat(m_worldMatrix, pDeviceContext, DX_SHADER_TYPE::VERTEX_SHADER);
//
//	//	ビューに関する行列を送る
//	DX_View::SetMatrixForTheView();
//
//	//	シェーダー利用を開始
//	pVertexShader->Begin(pDeviceContext);
//	pGeometryShader->Begin(pDeviceContext);
//	pPixelShader->Begin(pDeviceContext);
//
//	//	マテリアルグループ描画
//	tagGroupMesh* l_pGroupMesh = nullptr;
//
//	//	テクスチャ情報を纏めて送る
//	ID3D11ShaderResourceView*	l_pSRV[4];
//
//	//	マテリアル順に描画
//	for (DWORD i = 0; i < m_materialCount; ++i){
//
//		//	アドレスを取得
//		l_pGroupMesh = &m_pGroupMesh[i];
//
//		//	テクスチャを設定
//		l_pSRV[TEXTURE_DECAL_RESOURCE_NUM]		= l_pGroupMesh->pTextureDecal;
//		l_pSRV[TEXTURE_NORMAL_RESOURCE_NUM]		= l_pGroupMesh->pTextureNormal;
//		l_pSRV[TEXTURE_SPECULAR_RESOURCE_NUM]	= l_pGroupMesh->pTextureSpecular;
//		l_pSRV[TEXTURE_HEIGHT_RESOURCE_NUM]		= l_pGroupMesh->pTextureHeight;
//
//		//	register(t0)に送る
//		DX_ResourceManager::SetShaderResources(pDeviceContext, 0, 4, l_pSRV,DX_SHADER_TYPE::PIXEL_SHADER);
//		
//		//	インデックス描画
//		pDeviceContext->DrawIndexed(
//			l_pGroupMesh->indexCount,
//			l_pGroupMesh->startIndexLocation,
//			l_pGroupMesh->baseVertexLocation
//			);
//	}
//
//	//	シェーダー利用を終了
//	pVertexShader->End(pDeviceContext);
//	pGeometryShader->End(pDeviceContext);
//	pPixelShader->End(pDeviceContext);
//
//
//}
