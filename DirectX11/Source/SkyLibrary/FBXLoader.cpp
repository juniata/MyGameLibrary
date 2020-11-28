#include "SkyLibrary.h"
#include <string.h>

/// <summary>
/// メンバ変数の初期化を行う
/// </summary>
FBXLoader::FBXLoader() :
	m_pFbxManager(nullptr),
	m_pFbxIos(nullptr),
	m_pFbxScene(nullptr),
	m_pMeshList(nullptr),
	m_meshCount(0)
{}

/// <summary>
/// fbx、変数等の解放処理を行う
/// </summary>
FBXLoader::~FBXLoader()
{
	for (auto mLink : m_materialLinks) {
		DX_TextureManager::Release(mLink.second);
	}
	m_materialLinks.clear();

	for (int i = 0; i < m_meshCount; ++i) {
		DELETE_OBJ_ARRAY(m_pMeshList[i].pIndices);
		DELETE_OBJ_ARRAY(m_pMeshList[i].pVertices);
	}
	DELETE_OBJ_ARRAY(m_pMeshList);

	m_pFbxIos->Destroy();
	m_pFbxIos = nullptr;

	m_pFbxScene->Destroy();
	m_pFbxScene = nullptr;

	m_pFbxManager->Destroy();
	m_pFbxManager = nullptr;
}

/// <summary>
/// fbxファイルを読み込む。
/// </summary>
/// <param name="pFilename">読み込むfbxのファイル名(※.fbxも含まれます)</param>
/// <returns>読み込みの成否</returns>
bool FBXLoader::ReadObj(const char* pFilename)
{
	if (!Initialize()) {
		return false;
	}

	if (!LoadScene(pFilename)) {
		return false;
	}


	// メッシュに使われているマテリアル単位でメッシュを分割する
	FbxGeometryConverter converter(m_pFbxManager);
	converter.SplitMeshesPerMaterial(m_pFbxScene, true);
	
	int materialCount = m_pFbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (int i = 0; i < materialCount; ++i) {
		LoadMaterials(m_pFbxScene->GetSrcObject<FbxSurfaceMaterial>(i));
	}


	m_meshCount = m_pFbxScene->GetSrcObjectCount<FbxMesh>();
	m_pMeshList = new tagMeshData[m_meshCount];
	for (int i = 0; i < m_meshCount; ++i) {
		LoadMesh(m_pFbxScene->GetSrcObject<FbxMesh>(i), &m_pMeshList[i]);
		m_pMeshList[i].pVertexBuffer	= DX_BufferCreater::VertexBuffer(sizeof(m_pMeshList[i].pVertices[0]) * m_pMeshList[i].vertexCount, &m_pMeshList[i].pVertices[0]);
		m_pMeshList[i].pIndexBuffer		= DX_BufferCreater::IndexBuffer(sizeof(m_pMeshList[i].pIndices[0]) * m_pMeshList[i].indexCount, &m_pMeshList[i].pIndices[0]);
	}

	return true;
}

/// <summary>
/// 描画用メッシュデータを取得する
/// </summary>
/// <returns>描画用に整形したメッシュデータが返る</returns>
FBXLoader::tagMeshData* FBXLoader::GetMeshData()
{
	return m_pMeshList;
}

/// <summary>
/// マテリアル名とSRVの紐づけリストを取得する
/// </summary>
/// <returns>紐づけリスト</returns>
std::map<const char*, ID3D11ShaderResourceView*>& FBXLoader::GetMaterialLinks()
{
	return m_materialLinks;
}

/// <summary>
/// メッシュの数を取得する
/// </summary>
/// <returns>メッシュ数</returns>
int FBXLoader::GetMeshCount() const
{
	return m_meshCount;
}

/// <summary>
/// FBX SDKの初期化を行う
/// </summary>
/// <returns>初期化できたかどうか</returns>
bool FBXLoader::Initialize()
{
	m_pFbxManager = FbxManager::Create();
	if (!m_pFbxManager) {
		TRACE("FBXManagerの作成に失敗しました。")
		return false;
	}

	m_pFbxIos = FbxIOSettings::Create(m_pFbxManager, IOSROOT);
	m_pFbxManager->SetIOSettings(m_pFbxIos);

	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
	if (!m_pFbxScene) {
		TRACE("FBXSceneの作成に失敗しました")
		return false;
	}

	return true;
}

/// <summary>
/// FBXシーンの読み込みを行う
/// </summary>
/// <param name="pFilepath">読み込むfbxのファイル名(※.fbxも含まれます)</param>
/// <returns>シーンの読み込みができたかどうか</returns>
bool FBXLoader::LoadScene(const char* pFilepath)
{
	FbxImporter* pImpoter = FbxImporter::Create(m_pFbxManager, "");
	if (!pImpoter->Initialize(pFilepath, -1, m_pFbxManager->GetIOSettings())) {
		FbxString str = pImpoter->GetStatus().GetErrorString();
		TRACE(str.Buffer())
		return false;
	}
	
	if (!pImpoter->IsFBX()) {
		TRACE("読み込んだファイルがFBXではありません")
		pImpoter->Destroy();
		pImpoter = nullptr;
		return false;
	}

	pImpoter->Import(m_pFbxScene);
	pImpoter->Destroy();
	pImpoter = nullptr;

	return true;
}

/// <summary>
/// ノードに含まれているマテリアル情報を読み込む
/// </summary>
/// <param name="pMaterial">マテリアル情報</param>
void FBXLoader::LoadMaterials(FbxSurfaceMaterial* pMaterial)
{
	// FbxSurfacePhongの場合、最終的な継承先のため、FbxSurfaceLambertの値も扱えるが、
	// FbxSurfaceLambertの場合FbxSuracePhoneの値は扱えないため、ここでプロパティを取得しておき、
	// FbxSurfaceLambertの場合のときだけプロパティを二タブ取得する
	FbxDouble3 colors[2];
	FbxDouble factors[2];

	// TODO:Material取得処理作成中
	//FbxProperty prop = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbient);
	//
	//if (pMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
	//	const char* pElementCheckList[] = {
	//		FbxSurfaceMaterial::sAmbient,
	//		FbxSurfaceMaterial::sDiffuse
	//	};

	//	const char* pFactorCheckList[] = {
	//		FbxSurfaceMaterial::sAmbientFactor,
	//		FbxSurfaceMaterial::sDiffuseFactor
	//	};

	//	for (int i = 0; i < 2; ++i) {
	//		prop = pMaterial->FindProperty(pElementCheckList[i]);
	//		colors[i] = prop.IsValid() ? prop.Get<FbxDouble3>() : FbxDouble3(1.0, 1.0, 1.0);

	//		prop = pMaterial->FindProperty(pFactorCheckList[i]);
	//		factors[i] = prop.IsValid() ? prop.Get<FbxDouble>() : 1.0;
	//	}
	//}


	LoadTextures(pMaterial);
}

/// <summary>
/// テクスチャ情報を読み込む
/// </summary>
/// <param name="pMesh">マテリアル情報</param>
void FBXLoader::LoadTextures(FbxSurfaceMaterial* pMaterial)
{
	FbxProperty prop = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	FbxFileTexture* pTexture = nullptr;

	// TODO:一つのテクスチャにしか対応していません。
	int textureCount = prop.GetSrcObjectCount<FbxFileTexture>();
	if (textureCount > 0) {
		pTexture = prop.GetSrcObject<FbxFileTexture>(0);
	}
	else {
		int layerCount = prop.GetSrcObjectCount<FbxLayeredTexture>();
		if (layerCount > 0) {
			pTexture = prop.GetSrcObject<FbxFileTexture>(0);
		}
	}

	if (pTexture == nullptr) {
		return;
	}

	const char* pName = pMaterial->GetName();
	const char* pFilepath = pTexture->GetFileName();
	m_materialLinks[pName] = DX_TextureManager::GetTexture(pFilepath);
}

/// <summary>
/// メッシュ情報を取得
/// <param name="pNode">メッシュ情報</param>
/// <param name="ppMeshData">描画するためのメッシュ情報を格納する</param>
/// </summary>
void FBXLoader::LoadMesh(FbxMesh* pMesh, tagMeshData* pMeshData)
{
	LoadVertices(pMesh, &pMeshData);
	LoadIndices(pMesh, &pMeshData);
	LoadNormal(pMesh, &pMeshData);
	LoadVertexColors(pMesh, &pMeshData);
	LoadUVSetNames(pMesh, &pMeshData);
	LoadMaterialName(pMesh, &pMeshData);
}

/// <summary>
/// インデックスを読み込む
/// </summary>
/// <param name="pMesh">メッシュ情報</param>
/// <param name="ppMeshData">インデックスデータを格納する</param>
void FBXLoader::LoadIndices(FbxMesh* pMesh, FBXLoader::tagMeshData** ppMeshData)
{
	tagMeshData* pMeshData = *ppMeshData;

	int polygonCount = pMesh->GetPolygonCount();
	pMeshData->pIndices = new unsigned int[polygonCount * 3];
	pMeshData->indexCount = polygonCount * 3;

	for (int i = 0; i < polygonCount; ++i) {
		pMeshData->pIndices[i * 3]		= i * 3 + 2;
		pMeshData->pIndices[i * 3 + 1]	= i * 3 + 1;
		pMeshData->pIndices[i * 3 + 2]	= i * 3;
	}
}

/// <summary>
/// 頂点座標を読み込む
/// </summary>
/// <param name="pMesh">メッシュ情報</param>
/// <param name="ppMeshData">頂点座標データを格納する</param>
void FBXLoader::LoadVertices(FbxMesh* pMesh, FBXLoader::tagMeshData** ppMeshData)
{
	tagMeshData* pMeshData = *ppMeshData;

	FbxVector4* pVertices	= pMesh->GetControlPoints();
	int polygonVertexCount	= pMesh->GetPolygonVertexCount();
	int* pIndices			= pMesh->GetPolygonVertices();

	pMeshData->pVertices = new tagVertexInfo[polygonVertexCount];
	
	int index = 0;
	for (int i = 0; i < polygonVertexCount; ++i) {
		index = pIndices[i];
		pMeshData->pVertices[i].pos.x = -static_cast<float>(pVertices[index][0]);
		pMeshData->pVertices[i].pos.y = static_cast<float>(pVertices[index][1]);
		pMeshData->pVertices[i].pos.z = static_cast<float>(pVertices[index][2]);

		pMeshData->pVertices[i].color.x = 0.5f;
		pMeshData->pVertices[i].color.y = 0.5f;
		pMeshData->pVertices[i].color.z = 0.5f;
		pMeshData->pVertices[i].color.w = 1.0f;
	}
	
	pMeshData->vertexCount = polygonVertexCount;
}

/// <summary>
/// 頂点法線を取得
/// </summary>
/// <param name="pMesh">メッシュ情報</param>
/// <param name="ppMeshData">頂点座標データを格納する</param>
void FBXLoader::LoadNormal(FbxMesh* pMesh, FBXLoader::tagMeshData** ppMeshData)
{
	tagMeshData* pMeshData = *ppMeshData;

	FbxArray<FbxVector4> normals;
	pMesh->GetPolygonVertexNormals(normals);

	for (int i = 0; i < normals.Size(); ++i) {
		// 右手系から左手系に変換
		pMeshData->pVertices[i].normal.x = -static_cast<float>(normals[i][0]);
		pMeshData->pVertices[i].normal.y = static_cast<float>(normals[i][1]);
		pMeshData->pVertices[i].normal.z = static_cast<float>(normals[i][2]);
	}
}

/// <summary>
/// 頂点カラーを読み込む
/// </summary>
/// <param name="pMesh">メッシュ情報</param>
/// <param name="ppMeshData">頂点カラーデータを格納する</param>
void FBXLoader::LoadVertexColors(FbxMesh* pMesh, tagMeshData** ppMeshData)
{
	//tagMeshData* pMeshData = *ppMeshData;

	int colorCount = pMesh->GetElementVertexColorCount();
	if (colorCount == 0) {
		return;
	}

	FbxGeometryElementVertexColor* color = pMesh->GetElementVertexColor(0);
	if (color == nullptr) {
		return;
	}

	//FbxLayerElement::EMappingMode	mappingMode =  color->GetMappingMode();
	//FbxLayerElement::EReferenceMode refMode		= color->GetReferenceMode();
	// TODO:頂点色取得作成中

}

/// <summary>
/// 頂点UVを読み込む
/// </summary>
/// <param name="pMesh">メッシュ情報</param>
/// <param name="ppMeshData">頂点UVデータを格納する</param>
void FBXLoader::LoadUVSetNames(FbxMesh* pMesh, tagMeshData** ppMeshData)
{
	tagMeshData* pMeshData = *ppMeshData;

	FbxStringList uvsetName;
	pMesh->GetUVSetNames(uvsetName);


	// TODO:UVsetが複数あった場合は後々作る
	/*int uvsetCount = uvsetName.GetCount();
	FbxArray<FbxVector2> uvBuffer;
	FbxVector2 uv;
	int bufferSize = 0;
	for (int i = 0; i < uvsetCount; ++i) {
		pMesh->GetPolygonVertexUVs(uvsetName.GetStringAt(i), uvBuffer);
		bufferSize = uvBuffer.Size();

		for (int j = 0; j < bufferSize; ++j) {
			uv = uvBuffer[j];

			static_cast<float>(uv[0]);
			static_cast<float>(1.0 - uv[1]);
;		}
	}*/
	FbxArray<FbxVector2> uvBuffer;
	pMesh->GetPolygonVertexUVs(uvsetName.GetStringAt(0), uvBuffer);
	int bufferSize = uvBuffer.Size();

	FbxVector2 uv;
	for (int j = 0; j < bufferSize; ++j) {
		uv = uvBuffer[j];
		
		pMeshData->pVertices[j].uv.x = static_cast<float>(uv[0]);
		pMeshData->pVertices[j].uv.y = static_cast<float>(1.0 - uv[1]);
	}
}


/// <summary>
/// メッシュにアタッチされているマテリアルの名前を読み込む
/// </summary>
/// <param name="pMesh">メッシュ情報</param>
/// <param name="ppMeshData">マテリアルの名前を格納する</param>
void FBXLoader::LoadMaterialName(FbxMesh* pMesh, tagMeshData** ppMeshData)
{
	tagMeshData* pMeshData = *ppMeshData;

	int materialCount = pMesh->GetElementMaterialCount();
	if (materialCount == 0) {
		pMeshData->pMaterialName = "";
		return;
	}

	FbxLayerElementMaterial* pMaterial = pMesh->GetElementMaterial(0);
	int index = pMaterial->GetIndexArray().GetAt(0);

	FbxSurfaceMaterial* pSurfaceMaterial = pMesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);
	pMeshData->pMaterialName = pSurfaceMaterial ? pSurfaceMaterial->GetName() : "";
}

