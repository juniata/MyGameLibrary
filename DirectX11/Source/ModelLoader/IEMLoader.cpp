//#include	<D3DX10math.h>
#include	<Windows.h>
#include	<DirectXMath.h>
#include	<stdio.h>
#define GET_IME_FILE_POINTER(x) static_cast<IEMFILE*>(x)


#include	"IEMLoader.h"

//-----------------------------------------------------------------------------------------
//
//  マクロ定義
//
//-----------------------------------------------------------------------------------------
#define IEM_TEXTURE_STRING_MAX 64
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//  iemファイルを読み込むための構造体群
//
//-----------------------------------------------------------------------------------------
namespace iem_object_read_from_file{

	//	IEM用頂点情報(バージョン別)
	struct tagIEMVertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};

	//	IEM用頂点情報(バージョン別)
	struct tagIEMVertex2
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT4 color;
		XMFLOAT2 uv;
	};

	//	IEM用マテリアル情報
	struct tagIEMMaterial{
		XMFLOAT4 diffuse;
		XMFLOAT4 ambient;
		XMFLOAT4 specular;
		XMFLOAT4 emissive;
		float power;
	};

	//	IEM情報
	struct tagIEMFILE
	{
		int				version;			//	バージョン	

		//	メッシュ情報
		unsigned short	vertexCount;		//	頂点数
		void*			pVertex;			//	頂点バッファ

		unsigned short	faceCount;			//	ポリゴン数
		unsigned short*	pFace;				//	ポリゴンインデックス
		unsigned long*	pAtr;				//	ポリゴン材質

		unsigned short	materialCount;		//	マテリアル数
		tagIEMMaterial	material[32];		//	マテリアル
		char			texture[32][IEM_TEXTURE_STRING_MAX];	//	テクスチャファイル

		//	ボーン情報
		unsigned short  boneCount;
		tagIEMBone*		pBone;

		//	モーション情報
		unsigned short	frameCount;
		unsigned short	motionCount;
		unsigned short	offset[256];
		unsigned short	frameFlag[65535];
		tagIEMMotion*	pMotion;
	};
};


//-----------------------------------------------------------------------------------------
//
//  型名定義
//
//-----------------------------------------------------------------------------------------
typedef iem_object_read_from_file::tagIEMFILE		IEMFILE;
typedef iem_object_read_from_file::tagIEMVertex		IEMVERTEX;
typedef iem_object_read_from_file::tagIEMVertex2	IEMVERTEX2;
typedef iem_object_read_from_file::tagIEMMaterial	IEMMATERIAL;

//-----------------------------------------------------------------------------------------
//
//  メンバ変数初期化
//
//-----------------------------------------------------------------------------------------
IEMLoader::IEMLoader() :
m_file(nullptr),
m_pVertices(nullptr), 
m_pAttributes(nullptr),
m_pIndices(nullptr),
m_pBoneInfo(nullptr),
m_pMotionInfo(nullptr),
m_materialCount(0),
m_indexCount(0),
m_vertexCount(0),
m_frameCount(0),
m_faceCount(0),
m_boneCount(0),
m_motionCount(0),
m_version(-1)
{
	ZeroMemory(m_pTextures, sizeof(m_pTextures));
	ZeroMemory(m_offset, sizeof(m_offset));
	ZeroMemory(m_frameFlag, sizeof(m_frameFlag));
}

//-----------------------------------------------------------------------------------------
//
//  IEMファイルを読み込む
//
//-----------------------------------------------------------------------------------------
IEMLoader::IEMLoader(const char* argpFilapath) : IEMLoader()
{
	//	IEMファイル情報
	IEMFILE l_iemFile;

	//	読み込み用ファイルを作成
	_CreateFile(argpFilapath);
	
	//	バージョンを判断する
	VersionJuage();

	//	頂点情報を読み込む
	ReadVertexInfo(&l_iemFile);
	
	//	ポリゴン情報を読み込む
	ReadPolygonInfo(&l_iemFile);
	
	//	マテリアル情報を読み込む
	ReadMaterialInfo(&l_iemFile);

	//	ボーン情報を読み込む
	ReadBoneInfo(&l_iemFile);

	//	モーション情報を読み込む
	ReadMotionInfo(&l_iemFile);

	//	ハンドルを閉じる
	CloseHandle(m_file);

	//	ディレクトリを考慮したテクスチャのファイルパスを作成する
	CreateTextureFilepath(argpFilapath, &l_iemFile);
}

//-----------------------------------------------------------------------------------------
//
//  メンバ変数を解放
//
//-----------------------------------------------------------------------------------------
IEMLoader::~IEMLoader()
{
	delete[] m_pVertices;	m_pVertices		= nullptr; 
	delete[] m_pIndices;	m_pIndices		= nullptr; 
	delete[] m_pAttributes; m_pAttributes	= nullptr; 

	for (int i = 0; i < m_boneCount; ++i){

		//	モーションデータを解放
		delete[] m_pMotionInfo[i].pPos;			m_pMotionInfo[i].pPos			= nullptr;
		delete[] m_pMotionInfo[i].pPosFrame;	m_pMotionInfo[i].pPosFrame		= nullptr;
		delete[] m_pMotionInfo[i].pRotate;		m_pMotionInfo[i].pRotate		= nullptr;
		delete[] m_pMotionInfo[i].pRotateFrame; m_pMotionInfo[i].pRotateFrame	= nullptr;

		//	ボーンデータを解放
		delete[] m_pBoneInfo[i].pIndices;		m_pBoneInfo[i].pIndices = nullptr;
		delete[] m_pBoneInfo[i].pInfluence;		m_pBoneInfo[i].pInfluence = nullptr;
	}

	delete[] m_pMotionInfo; m_pMotionInfo	= nullptr;
	delete[] m_pBoneInfo;	m_pBoneInfo		= nullptr;

	

}


//-----------------------------------------------------------------------------------------
//
//  読み込み用ファイルを作成する
//
//-----------------------------------------------------------------------------------------
void IEMLoader::_CreateFile(const char* pFilePath)
{
	m_file = CreateFile(pFilePath, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	if (m_file == INVALID_HANDLE_VALUE){
		MessageBox(NULL, "IEMの読み込みに失敗しました", "error", MB_OK);
		exit(EXIT_FAILURE);
	}

}

//-----------------------------------------------------------------------------------------
//
//  IEMのバージョンを取得する
//
//-----------------------------------------------------------------------------------------
int IEMLoader::GetVersion()const
{
	return m_version;
}

//------------------------------------------------------------------------------
//
//	マテリアル数を取得する
//
//------------------------------------------------------------------------------
WORD IEMLoader::GetMaterialCount()const
{
	return m_materialCount;
}

//------------------------------------------------------------------------------
//
//	頂点数を取得する
//
//------------------------------------------------------------------------------
WORD IEMLoader::GetVertexCount()const
{
	return m_vertexCount;
}

//------------------------------------------------------------------------------
//
//	面数を取得する
//
//------------------------------------------------------------------------------
WORD IEMLoader::GetFaceCount()const
{
	return m_faceCount;
}

//------------------------------------------------------------------------------
//
//	フレーム数を取得する
//
//------------------------------------------------------------------------------
WORD IEMLoader::GetFrameCount()const
{
	return m_frameCount;
}

//------------------------------------------------------------------------------
//
//	ボーン数を取得する
//
//------------------------------------------------------------------------------
WORD IEMLoader::GetBoneCount()const
{
	return m_boneCount;
}

//------------------------------------------------------------------------------
//
//	インデックス数を取得する
//
//------------------------------------------------------------------------------
WORD IEMLoader::GetIndexCount()const
{
	return m_indexCount;
}

//------------------------------------------------------------------------------
//
//	テクスチャのファイルパスを取得する
//
//------------------------------------------------------------------------------
char (*IEMLoader::GetTextures())[MAX_PATH]
{
	return m_pTextures;
}

//------------------------------------------------------------------------------
//
//	頂点情報を取得する
//
//------------------------------------------------------------------------------
tagIEMMeshVertex*	IEMLoader::GetVertices()
{
	return m_pVertices;
}

//------------------------------------------------------------------------------
//
//	属性データを取得する
//
//------------------------------------------------------------------------------
LPDWORD IEMLoader::GetAttributes()
{
	return m_pAttributes;
}

//------------------------------------------------------------------------------
//
//	インデックスを取得する
//
//------------------------------------------------------------------------------
WORD* IEMLoader::GetIndices()
{
	return m_pIndices;
}

//------------------------------------------------------------------------------
//
//	ボーンデータを取得する
//
//------------------------------------------------------------------------------
tagIEMBone* IEMLoader::GetBoneInfo()
{
	return m_pBoneInfo;
}

//------------------------------------------------------------------------------
//
//	モーションデータを取得する
//
//------------------------------------------------------------------------------
tagIEMMotion* IEMLoader::GetMotionInfo()
{
	return m_pMotionInfo;
}

//------------------------------------------------------------------------------
//
//	offsetを取得する
//
//------------------------------------------------------------------------------
WORD* IEMLoader::GetOffSet()
{
	return m_offset;
}

//------------------------------------------------------------------------------
//
//	フレーム情報を取得する
//
//------------------------------------------------------------------------------
WORD* IEMLoader::GetFrameFlag()
{
	return m_frameFlag;
}

//------------------------------------------------------------------------------
//
//	IEMのバーションを判別する
//
//------------------------------------------------------------------------------
void IEMLoader::VersionJuage()
{
	unsigned long l_fileID = 0;

	//	IEMバージョンを読み込む
	ReadFile(&l_fileID, 4);
	switch (l_fileID){
	case '1MEI':m_version = 1;	break;
	case '2MEI':m_version = 2;	break;
	case '3MEI':m_version = 3;	break;
	case '4MEI':m_version = 4;	break;
	default:	m_version = -1; break;
	}

	if (m_version <= 0){
		MessageBox(NULL, "IEMのバージョンが不明です", "error", MB_OK);
		exit(EXIT_FAILURE);
	}

}

//------------------------------------------------------------------------------
//
//	頂点情報を読み込む
//
//------------------------------------------------------------------------------
void IEMLoader::ReadVertexInfo(void* pEIMFILE)
{
	//	IEMFILEにキャスト
	IEMFILE* l_pIEMFILE = GET_IME_FILE_POINTER(pEIMFILE);

	//	頂点数を読み込む
	ReadFile(&l_pIEMFILE->vertexCount, sizeof(WORD));
	
	//	頂点数保存
	m_vertexCount = l_pIEMFILE->vertexCount;

	//	バージョン別で頂点情報を読み込む
	if (m_version < 4){
		ReadVertexInfoIEMVersion1(pEIMFILE);
	}
	else{
		ReadVertexInfoIEMVersion2(pEIMFILE);
	}
}

//------------------------------------------------------------------------------
//
//	旧IEMバージョンを読み込む
//
//------------------------------------------------------------------------------
void IEMLoader::ReadVertexInfoIEMVersion1(void* pEIMFILE)
{
	//	IEMFILEにキャスト
	IEMFILE* l_pIEMFILE = GET_IME_FILE_POINTER(pEIMFILE);

	//	頂点数分生成
	l_pIEMFILE->pVertex = new IEMVERTEX[m_vertexCount];

	//	頂点データを読み込む
	ReadFile(l_pIEMFILE->pVertex, sizeof(IEMVERTEX) * m_vertexCount);

	//	頂点数分だけ生成
	m_pVertices = new tagIEMMeshVertex[l_pIEMFILE->vertexCount];

	//	 tagIEMVertexにキャスト
	IEMVERTEX* l_pVertex = (IEMVERTEX*)l_pIEMFILE->pVertex;

	//	データをコピーするが頂点色だけないので、追加していく
	for (WORD i = 0; i < l_pIEMFILE->vertexCount; ++i){
		m_pVertices[i].pos		= l_pVertex[i].pos;
		m_pVertices[i].normal	= l_pVertex[i].normal;
		m_pVertices[i].color	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_pVertices[i].uv		= l_pVertex[i].uv;
	}

	//	解放
	if (l_pIEMFILE->pVertex){ delete[] l_pIEMFILE->pVertex; l_pIEMFILE->pVertex = nullptr; }
}

//------------------------------------------------------------------------------
//
//	新IEMバージョンを読み込む
//
//------------------------------------------------------------------------------
void IEMLoader::ReadVertexInfoIEMVersion2(void* pEIMFILE)
{
	//	IEMFILEにキャスト
	IEMFILE* l_pIEMFILE = GET_IME_FILE_POINTER(pEIMFILE);

	//	頂点数分生成
	l_pIEMFILE->pVertex = new IEMVERTEX2[m_vertexCount];

	//	頂点データを読み込む
	ReadFile(l_pIEMFILE->pVertex, sizeof(IEMVERTEX2) * m_vertexCount);

	//	データをコピー
	m_pVertices = (tagIEMMeshVertex*)l_pIEMFILE->pVertex;
}

//------------------------------------------------------------------------------
//
//	ポリゴン情報を読み込む
//
//------------------------------------------------------------------------------
void IEMLoader::ReadPolygonInfo(void* pEIMFILE)
{
	//	IEMFILEにキャスト
	IEMFILE* l_pIEMFILE = GET_IME_FILE_POINTER(pEIMFILE);

	//	ポリゴン数を読み込む
	ReadFile(&l_pIEMFILE->faceCount, sizeof(WORD));

	//	面数保存
	m_faceCount = l_pIEMFILE->faceCount;

	//	インデックス数保存
	m_indexCount = m_faceCount * 3;

	l_pIEMFILE->pFace = new WORD[m_indexCount];
	l_pIEMFILE->pAtr = new DWORD[m_faceCount];

	//	面情報を読み込む
	ReadFile(l_pIEMFILE->pFace, sizeof(WORD)*m_indexCount);

	//	材質属性を読み込む
	ReadFile(l_pIEMFILE->pAtr, sizeof(DWORD)*m_faceCount);


	//	データをコピーする
	m_pIndices		= l_pIEMFILE->pFace;
	m_pAttributes	= l_pIEMFILE->pAtr;
}

//------------------------------------------------------------------------------
//
//	マテリアル情報を読み込む
//
//------------------------------------------------------------------------------
void IEMLoader::ReadMaterialInfo(void* pEIMFILE)
{
	IEMFILE* l_pIEMFILE = GET_IME_FILE_POINTER(pEIMFILE);

	//	マテリアル数を読み込む
	ReadFile(&l_pIEMFILE->materialCount, sizeof(unsigned short));

	//	マテリアル数保存
	m_materialCount = l_pIEMFILE->materialCount;

	//	マテリアル情報を読み込む
	ReadFile(&l_pIEMFILE->material, sizeof(IEMMATERIAL)* m_materialCount);

	//	テクスチャ情報を読み込む
	ReadFile(&l_pIEMFILE->texture, sizeof(char)* m_materialCount * IEM_TEXTURE_STRING_MAX);

}

//------------------------------------------------------------------------------
//
//	ボーン情報を読み込む
//
//------------------------------------------------------------------------------
void IEMLoader::ReadBoneInfo(void* pEIMFILE)
{
	IEMFILE* l_pIEMFILE = GET_IME_FILE_POINTER(pEIMFILE);

	//	ボーン数を読み込む
	ReadFile(&l_pIEMFILE->boneCount, sizeof(WORD));

	//	ボーン数を保存
	m_boneCount = l_pIEMFILE->boneCount;

	//	ボーン数分生成
	l_pIEMFILE->pBone = new tagIEMBone[m_boneCount];

	for (int i = 0; i < m_boneCount; ++i){

		//	ボーン情報を読み込む
		ReadFile(&l_pIEMFILE->pBone[i], sizeof(tagIEMBone));

		//	ボーンのインデックス数分生成
		l_pIEMFILE->pBone[i].pIndices = new DWORD[l_pIEMFILE->pBone[i].indexCount];

		//	ボーンのインデックス情報を読み込む
		ReadFile(l_pIEMFILE->pBone[i].pIndices, sizeof(DWORD)* l_pIEMFILE->pBone[i].indexCount);

		//	ボーンのインデックス数分生成
		l_pIEMFILE->pBone[i].pInfluence = new float[l_pIEMFILE->pBone[i].indexCount];

		//	ボーンのウェイト情報を読み込む
		ReadFile(l_pIEMFILE->pBone[i].pInfluence, sizeof(float)* l_pIEMFILE->pBone[i].indexCount);
	}

	//	データをコピー
	m_pBoneInfo = l_pIEMFILE->pBone;
}

//------------------------------------------------------------------------------
//
//	モーション情報を読み込む
//
//------------------------------------------------------------------------------
void IEMLoader::ReadMotionInfo(void* pEIMFILE)
{
	IEMFILE* l_pIEMFILE = GET_IME_FILE_POINTER(pEIMFILE);

	//	モーション数を読み込む
	ReadFile(&l_pIEMFILE->motionCount, sizeof(WORD));

	//	モーション数を保存
	m_motionCount = l_pIEMFILE->motionCount;

	//	フレーム数を読み込む
	ReadFile(&l_pIEMFILE->frameCount, sizeof(WORD));

	//	フレーム数を保存
	m_frameCount = l_pIEMFILE->frameCount;

	//	オフセット数情報を読み込む
	ReadFile(&l_pIEMFILE->offset, sizeof(WORD)* 256);

	//	フレームのフラグ情報を読み込む
	ReadFile(&l_pIEMFILE->frameFlag, sizeof(WORD)* m_frameCount);

	//	ボーン数生成
	l_pIEMFILE->pMotion = new tagIEMMotion[m_boneCount];

	//	Motionデータのポインタを保持
	tagIEMMotion* l_pTempMotion = nullptr;

	//	ボーン数回す
	for (WORD i = 0; i < m_boneCount; ++i){

		//	i番目のモーションデータのポインタを取得
		l_pTempMotion = &l_pIEMFILE->pMotion[i];

		//	モーションデータを読み込む
		ReadFile(l_pTempMotion, sizeof(tagIEMMotion));

		//	回転キーフレーム分生成
		l_pTempMotion->pRotate		= new XMFLOAT4[l_pTempMotion->rotateCount];
		l_pTempMotion->pRotateFrame = new WORD[l_pTempMotion->rotateCount];

		//	回転フレームと回転情報を読み込む
		ReadFile(l_pTempMotion->pRotate,		sizeof(XMFLOAT4)	* l_pTempMotion->rotateCount);
		ReadFile(l_pTempMotion->pRotateFrame,	sizeof(WORD)		* l_pTempMotion->rotateCount);

		//	座標数生成
		l_pTempMotion->pPos			= new XMFLOAT3[l_pTempMotion->posCount];
		l_pTempMotion->pPosFrame	= new WORD[l_pTempMotion->posCount];

		//	座標を読み込む
		ReadFile(l_pTempMotion->pPos,		sizeof(XMFLOAT3)	*	l_pTempMotion->posCount);
		ReadFile(l_pTempMotion->pPosFrame,	sizeof(WORD)	*	l_pTempMotion->posCount);
	}

	//	データをコピー
	m_pMotionInfo = l_pIEMFILE->pMotion;

	//	オフセットとフレームグラグ情報をコピー
	CopyMemory(m_offset,	l_pIEMFILE->offset,		sizeof(WORD)* 256);
	CopyMemory(m_frameFlag, l_pIEMFILE->frameFlag,	sizeof(WORD)* m_frameCount);

}

//------------------------------------------------------------------------------
//
//	ディレクトリを考慮したテクスチャのファイルパスを作成する
//
//------------------------------------------------------------------------------
void IEMLoader::CreateTextureFilepath(const char* pFilePath, void* pEIMFILE)
{
	IEMFILE*	l_pIEMFILE = GET_IME_FILE_POINTER(pEIMFILE);

	char	l_directoryPath[MAX_PATH];

	//	モデルのファイルパスをコピーする
	CopyMemory(l_directoryPath, pFilePath, lstrlen(pFilePath) + 1);

	//	パスの尾から調べていく
	for (int i = lstrlen(pFilePath); i>0; i--){

		//	文字が先行バイトなら、ダブルバイト文字セットなどでもう一度デクリメントする
		if (IsDBCSLeadByte(l_directoryPath[i - 2])){
			i--;
			continue;
		}

		if (l_directoryPath[i - 1] == '\\' || l_directoryPath[i - 1] == '/'){
			l_directoryPath[i] = '\0';
			break;
		}
	}

	for (int i = 0; i < GetMaterialCount(); ++i){
		sprintf_s(m_pTextures[i], "%s%s", l_directoryPath, l_pIEMFILE->texture[i]);
	}
}

//------------------------------------------------------------------------------
//
//	ReadFileのラッパー関数
//
//------------------------------------------------------------------------------
void IEMLoader::ReadFile(LPVOID pBuffer, DWORD dataSize)
{
	DWORD l_readByteSize = 0;

	if (::ReadFile(m_file, pBuffer, dataSize, &l_readByteSize, nullptr) == -1){
		MessageBox(NULL, "ReadFileに失敗しました。頂点情報か何かが間違っています", "ReadFile(LPVOID argp_buffer, DWORD arg_byteToRead) error", MB_OK);
		exit(EXIT_FAILURE);
	}
}

