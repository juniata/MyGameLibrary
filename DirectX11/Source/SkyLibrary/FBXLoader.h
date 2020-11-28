#pragma once
#include <vector>
#include "fbxsdk.h"

struct tagVertexInfo {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 uv;
};

/// <summary>
/// FBXファイルを読み込み、描画できる形にする。
/// 
/// いろいろな取得方法があるが、MeshはMeshで、MaterialはMaterialで取得する(Nodeを探ってはしない)
/// 理由としては、↑のほうが簡単にプログラムをかけるから。
/// 
/// </summary>
class FBXLoader
{
public:
	struct tagMeshData
	{
		ID3D11Buffer*	pVertexBuffer;
		ID3D11Buffer*	pIndexBuffer;

		tagVertexInfo* pVertices;
		unsigned int* pIndices;

		int vertexCount;
		int indexCount;

		const char* pMaterialName;
	};


	/// <summary>
	/// メンバ変数の初期化を行う
	/// </summary>
	FBXLoader();

	/// <summary>
	/// fbx、変数等の解放処理を行う
	/// </summary>
	~FBXLoader();

	/// <summary>
	/// fbxファイルを読み込む。
	/// </summary>
	/// <param name="pFilename">読み込むfbxのファイル名(※.fbxも含まれます)</param>
	/// <returns>読み込みの成否</returns>
	bool ReadObj(const char* pFilename);

	/// <summary>
	/// 描画用メッシュデータを取得する
	/// </summary>
	/// <returns>描画用に整形したメッシュデータが返る</returns>
	tagMeshData* GetMeshData();

	/// <summary>
	/// マテリアル名とSRVの紐づけリストを取得する
	/// </summary>
	/// <returns>紐づけリスト</returns>
	std::map<const char*, ID3D11ShaderResourceView*>& GetMaterialLinks();

	/// <summary>
	/// メッシュの数を取得する
	/// </summary>
	/// <returns>メッシュ数</returns>
	int GetMeshCount() const;

private:
	FbxManager*		m_pFbxManager;
	FbxIOSettings*	m_pFbxIos;
	FbxScene*		m_pFbxScene;


	tagMeshData* m_pMeshList;
	int m_meshCount;

	std::map<const char*, ID3D11ShaderResourceView*> m_materialLinks;

	/// <summary>
	/// FBX SDKの初期化を行う
	/// </summary>
	/// <returns>初期化できたかどうか</returns>
	bool Initialize();

	/// <summary>
	/// FBXシーンの読み込みを行う
	/// </summary>
	/// <param name="pFilepath">読み込むfbxのファイル名(※.fbxも含まれます)</param>
	/// <returns>シーンの読み込みができたかどうか</returns>
	bool LoadScene(const char* pFilepath);

	/// <summary>
	/// ノードに含まれているマテリアル情報を読み込む
	/// </summary>
	/// <param name="pMaterial">マテリアル情報</param>
	void LoadMaterials(FbxSurfaceMaterial* pMaterial);

	/// <summary>
	/// テクスチャ情報を読み込む
	/// </summary>
	/// <param name="pMesh">マテリアル情報</param>
	void LoadTextures(FbxSurfaceMaterial* pMaterial);


	/// <summary>
	/// メッシュ情報を読み込む
	/// <param name="pMesh">メッシュ情報</param>
	/// <param name="pMeshData">描画するためのメッシュ情報を格納する</param>
	/// </summary>
	void LoadMesh(FbxMesh* pMesh, tagMeshData* pMeshData);

	/// <summary>
	/// インデックスを読み込む
	/// </summary>
	/// <param name="pMesh">メッシュ情報</param>
	/// <param name="ppMeshData">インデックスデータを格納する</param>
	void LoadIndices(FbxMesh* pMesh, tagMeshData** ppMeshData);

	/// <summary>
	/// 頂点座標を読み込む
	/// </summary>
	/// <param name="pMesh">メッシュ情報</param>
	/// <param name="ppMeshData">頂点座標データを格納する</param>
	void LoadVertices(FbxMesh* pMesh, tagMeshData** ppMeshData);

	/// <summary>
	/// 頂点法線を読み込む
	/// </summary>
	/// <param name="pMesh">メッシュ情報</param>
	/// <param name="ppMeshData">頂点法線データを格納する</param>
	void LoadNormal(FbxMesh* pMesh, tagMeshData** ppMeshData);

	/// <summary>
	/// 頂点カラーを読み込む
	/// </summary>
	/// <param name="pMesh">メッシュ情報</param>
	/// <param name="ppMeshData">頂点カラーデータを格納する</param>
	void LoadVertexColors(FbxMesh* pMesh, tagMeshData** ppMeshData);

	/// <summary>
	/// 頂点UVを読み込む
	/// </summary>
	/// <param name="pMesh">メッシュ情報</param>
	/// <param name="ppMeshData">頂点UVデータを格納する</param>
	void LoadUVSetNames(FbxMesh* pMesh, tagMeshData** ppMeshData);

	/// <summary>
	/// メッシュにアタッチされているマテリアルの名前を読み込む
	/// </summary>
	/// <param name="pMesh">メッシュ情報</param>
	/// <param name="ppMeshData">マテリアルの名前を格納する</param>
	void LoadMaterialName(FbxMesh* pMesh, tagMeshData** ppMeshData);
};