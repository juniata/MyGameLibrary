#include "../DX_LIBRARY/DX_Library.h"
#include	<stdio.h>

#include	"IMOLoader.h"
using namespace DirectX;
//-----------------------------------------------------------------------------------------
//
//  imoファイルを読み込むための構造体群
//
//-----------------------------------------------------------------------------------------
namespace imo_object_read_from_file{

	//	IMO読み込み用 頂点情報
	struct tagIMOVertex {
		XMFLOAT3 pos;
		DWORD	color;
		XMFLOAT2 uv;
	};

	//	IMO読み込み用 マテリアル情報
	struct tagIMOMaterial{
		XMFLOAT4   diffuse;
		XMFLOAT4   ambient;
		XMFLOAT4   specular;
		XMFLOAT4   emissive;
		float      power;
	};

	//	IMO読み込み用 全情報
	struct tagIMOObject {
		DWORD		id;
		int			type;
		DWORD		vertexCount;
		tagIMOVertex* p_vertices;

		DWORD		faceCount;
		LPWORD		p_faces;
		LPDWORD		p_attribute;

		DWORD			materialCount;	//	マテリアル数
		tagIMOMaterial	material[32];	//	マテリアル
		char			textures[32][32];//	テクスチャファイル

	};
};

//-----------------------------------------------------------------------------------------
//
//  型名定義
//
//-----------------------------------------------------------------------------------------
typedef imo_object_read_from_file::tagIMOVertex IMO_OBJECT_VERTEX;
typedef imo_object_read_from_file::tagIMOObject IMO_OBJECT;


//-----------------------------------------------------------------------------------------
//
//  メンバ変数の初期化
//
//-----------------------------------------------------------------------------------------
IMOLoader::IMOLoader() : 
m_file(0), 
m_vertexCount(0),
m_indexCount(0), 
m_faceCount(0), 
m_materialCount(0),
m_pVertices(nullptr), m_pAttributes(nullptr), m_pIndices(nullptr)
{
	ZeroMemory(m_pTextures, sizeof(m_pTextures));
}


//-----------------------------------------------------------------------------------------
//
//  IMOファイルを読み込む
//
//-----------------------------------------------------------------------------------------
IMOLoader::IMOLoader(const char* pFilePath) : IMOLoader()
{
	IMO_OBJECT l_imoObj;

	//	ファイルを生成
	m_file = CreateFile(pFilePath, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	if (m_file == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, "ファイル名、またはファイルパスが間違っています", "class IMOLoader(char*) : CreateFile error", MB_OK);
		exit(EXIT_FAILURE);
	}

	//	IMO情報を読み込む
	ReadFile(&l_imoObj, sizeof(IMO_OBJECT));

	//	頂点数、インデックス数、面数、マテリアル数、材質数保存
	m_vertexCount	= l_imoObj.vertexCount;
	m_indexCount	= l_imoObj.faceCount * 3;
	m_faceCount		= l_imoObj.faceCount;
	m_materialCount = l_imoObj.materialCount;
	
	//	各種生成
	m_pVertices		= new IMOMESHVERTEX[m_vertexCount];
	m_pIndices		= new WORD[m_indexCount];
	m_pAttributes	= new DWORD[m_faceCount];
	
	//	IMOのversionに従ってロード方法を変える
	if (l_imoObj.id != '2OMI'){
		LoadVersion1();
	}
	else{
		LoadVersion2();
	}

	//	面が保有するマテリアル番号を取得
	ReadFile(m_pAttributes, sizeof(DWORD) * m_faceCount);

	//	パスを分割し、マテリアル数分テクスチャパスを取得する
	char	workpath[MAX_PATH] = { NULL };

	//	ファイルパスを分割する
	SplitThePath(workpath, pFilePath);

	//	m_pTexturesにテクスチャパスを入れていく
	BindPath(workpath, l_imoObj.textures);

	//	ファイルを閉じる
	CloseHandle(m_file);
}

//-----------------------------------------------------------------------------------------
//
//  メンバ変数を解放
//
//-----------------------------------------------------------------------------------------
IMOLoader::~IMOLoader()
{
	if (m_pVertices){	delete[] m_pVertices;	m_pVertices		= nullptr; }
	if (m_pAttributes){ delete[] m_pAttributes; m_pAttributes	= nullptr; }
	if (m_pIndices){	delete[] m_pIndices;	m_pIndices	= nullptr; }
}

//-----------------------------------------------------------------------------------------
//
//  頂点数を取得
//
//-----------------------------------------------------------------------------------------
DWORD IMOLoader::GetVertexCount()const
{
	return m_vertexCount;
}

//-----------------------------------------------------------------------------------------
//
//  インデックス数を取得
//
//-----------------------------------------------------------------------------------------
DWORD IMOLoader::GetIndexCount()const
{
	return m_indexCount; 
}

//-----------------------------------------------------------------------------------------
//
//  面数を取得
//
//-----------------------------------------------------------------------------------------
DWORD IMOLoader::GetFaceCount()const
{
	return m_faceCount;
}


//-----------------------------------------------------------------------------------------
//
//  マテリアル数を取得
//
//-----------------------------------------------------------------------------------------
DWORD IMOLoader::GetMaterialCount()const
{
	return m_materialCount;
}

//-----------------------------------------------------------------------------------------
//
//  属性データを取得
//
//-----------------------------------------------------------------------------------------
const LPDWORD IMOLoader::GetAttributes()const
{
	return m_pAttributes;
}

//-----------------------------------------------------------------------------------------
//
//  インデックスデータを取得
//
//-----------------------------------------------------------------------------------------
const LPWORD  IMOLoader::GetIndices()const
{
	return m_pIndices; 
}

//-----------------------------------------------------------------------------------------
//
//  頂点データを取得
//
//-----------------------------------------------------------------------------------------
const LPIMOMESHVERTEX IMOLoader::GetVertices()const
{
	return m_pVertices; 
}

//-----------------------------------------------------------------------------------------
//
//	テクスチャパスを取得
//
//-----------------------------------------------------------------------------------------
char (*IMOLoader::GetTextures())[MAX_PATH]
{
	return m_pTextures;
}

//-----------------------------------------------------------------------------------------
//
//	旧バージョンのIMOファイルを読み込む
//
//-----------------------------------------------------------------------------------------
void IMOLoader::LoadVersion1()
{
	//	頂点数分生成
	IMO_OBJECT_VERTEX* l_pReadIMOVertex = new IMO_OBJECT_VERTEX[m_vertexCount];
	
	//	頂点読み込み
	ReadFile(l_pReadIMOVertex, sizeof(IMO_OBJECT_VERTEX)* m_vertexCount);

	//	インデックス読み込み
	ReadFile(m_pIndices, sizeof(WORD) * m_indexCount);

	const float l_par = 1.0f / 255.0f;

	LPIMOMESHVERTEX l_pIMOMeshVertex = nullptr;
	IMO_OBJECT_VERTEX*	l_pIMOVertex = nullptr;
	//	頂点情報をメンバーにコピー
	for (DWORD i = 0; i < m_vertexCount; ++i){

		//	アドレスを取得
		l_pIMOMeshVertex = &m_pVertices[i];
		l_pIMOVertex = &l_pReadIMOVertex[i];

		l_pIMOMeshVertex->pos		= l_pIMOVertex->pos;
		l_pIMOMeshVertex->uv		= l_pIMOVertex->uv;
		l_pIMOMeshVertex->color		= l_pIMOVertex->color;
		l_pIMOMeshVertex->normal	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	XMFLOAT3 l_v1;
	XMFLOAT3 l_v2;
	XMFLOAT3 l_n;

	float l_d;

	WORD l_index1;
	WORD l_index2;
	WORD l_index3;

	IMO_OBJECT_VERTEX* l_pVertex1 = nullptr;
	IMO_OBJECT_VERTEX* l_pVertex2 = nullptr;
	IMO_OBJECT_VERTEX* l_pVertex3 = nullptr;

	LPIMOMESHVERTEX l_pIMOMeshVertex1 = nullptr;
	LPIMOMESHVERTEX l_pIMOMeshVertex2 = nullptr;
	LPIMOMESHVERTEX l_pIMOMeshVertex3 = nullptr;

	//	頂点法線を作成
	XMVECTOR pos1, pos2,pos3;
	XMVECTOR nor1, nor2, nor3;
	XMVECTOR temp;
	for (DWORD i = 0; i < m_faceCount; ++i){

		
		//	インデックスを取得
		l_index1 = m_pIndices[i * 3 + 0];
		l_index2 = m_pIndices[i * 3 + 1];
		l_index3 = m_pIndices[i * 3 + 2];

		//	頂点情報を取得
		l_pVertex1 = &l_pIMOVertex[l_index1];
		l_pVertex2 = &l_pIMOVertex[l_index2];
		l_pVertex3 = &l_pIMOVertex[l_index3];

		//	面法線計算
		pos1 = XMLoadFloat3(&l_pVertex1->pos);
		pos2 = XMLoadFloat3(&l_pVertex2->pos);
		pos3 = XMLoadFloat3(&l_pVertex3->pos);

		XMStoreFloat3(&l_v1, pos1 - pos2);
		XMStoreFloat3(&l_v2, pos2 - pos3);

		//	外積
		l_n.x = (l_v1.y * l_v2.z - l_v1.z * l_v2.y);
		l_n.y = (l_v1.z * l_v2.x - l_v1.x * l_v2.z);
		l_n.z = (l_v1.x * l_v2.y - l_v1.y * l_v2.x);

		//	内積
		l_d = sqrtf(l_n.x * l_n.x + l_n.y * l_n.y + l_n.z * l_n.z);

		l_d = 1.0f / l_d;
		l_n.x *= l_d;
		l_n.y *= l_d;
		l_n.z *= l_d;

		//	頂点情報を取得
		l_pIMOMeshVertex1 = &m_pVertices[l_index1];
		l_pIMOMeshVertex2 = &m_pVertices[l_index2];
		l_pIMOMeshVertex3 = &m_pVertices[l_index3];

		//	頂点法線を作成
		temp = XMLoadFloat3(&l_n);

		nor1 = XMLoadFloat3(&l_pIMOMeshVertex1->normal);
		nor2 = XMLoadFloat3(&l_pIMOMeshVertex2->normal);
		nor3 = XMLoadFloat3(&l_pIMOMeshVertex3->normal);

		nor1 += temp;
		nor2 += temp;
		nor3 += temp;

		XMStoreFloat3(&l_pIMOMeshVertex1->normal, nor1);
		XMStoreFloat3(&l_pIMOMeshVertex2->normal, nor2);
		XMStoreFloat3(&l_pIMOMeshVertex3->normal, nor3);
	}

	//	頂点法線を正規化
	for (DWORD i = 0; i < m_vertexCount; ++i){
		XMStoreFloat3(&m_pVertices[i].normal, XMVector3Normalize(XMLoadFloat3(&m_pVertices[i].normal)));
	}

	//	解放
	if (l_pReadIMOVertex){ delete[]	l_pReadIMOVertex; l_pReadIMOVertex = nullptr; }
}

//-----------------------------------------------------------------------------------------
//
//	新バージョンのIMOファイルを読み込む
//
//-----------------------------------------------------------------------------------------
void IMOLoader::LoadVersion2()
{
	//	頂点読み込み
	ReadFile(m_pVertices, sizeof(tagIMOMeshVertex)* m_vertexCount);

	//	インデックス読み込み
	ReadFile(m_pIndices, sizeof(WORD)*m_indexCount);

}

//-----------------------------------------------------------------------------------------
//
//	ReadFileのラッパー関数
//
//-----------------------------------------------------------------------------------------
void IMOLoader::ReadFile(LPVOID pBuffer, DWORD dataSize)
{
	DWORD l_readByteSize = 0;

	if (::ReadFile(m_file, pBuffer, dataSize, &l_readByteSize, nullptr) == -1){
		MessageBox(NULL, "IMOLoader::ReadFile() : failed", "error", MB_OK);
		exit(EXIT_FAILURE);
	}
}

//-----------------------------------------------------------------------------------------
//
//	パスを分割する
//
//-----------------------------------------------------------------------------------------
void IMOLoader::SplitThePath(char pSplitFilePath[MAX_PATH], const char* pFilepath)
{
	CopyMemory(pSplitFilePath, pFilepath, lstrlen(pFilepath) + 1);
	for (int i = lstrlen(pFilepath); i>0; i--){
		if (IsDBCSLeadByte(pSplitFilePath[i - 2])){
			i--;
			continue;
		}
		if (pSplitFilePath[i - 1] == '\\' || pSplitFilePath[i - 1] == '/'){
			pSplitFilePath[i] = '\0';
			break;
		}
	}
}

//------------------------------------------------------------------------------
//
//	パスをm_pTexturesに登録する
//
//------------------------------------------------------------------------------
void IMOLoader::BindPath(const char pSplitFilePath[MAX_PATH], const char imoTextures[32][32])
{
	char l_pTemp[MAX_PATH];
	for (DWORD i = 0; i < m_materialCount; ++i){
		sprintf_s(l_pTemp, "%s%s", pSplitFilePath, imoTextures[i]);
		strcpy_s(m_pTextures[i], MAX_PATH, l_pTemp);
	}
}

