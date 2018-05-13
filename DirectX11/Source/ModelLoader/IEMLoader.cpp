//#include	<D3DX10math.h>
#include	<Windows.h>
#include	<DirectXMath.h>
#include	<stdio.h>
#define GET_IME_FILE_POINTER(x) static_cast<IEMFILE*>(x)


#include	"IEMLoader.h"

//-----------------------------------------------------------------------------------------
//
//  �}�N����`
//
//-----------------------------------------------------------------------------------------
#define IEM_TEXTURE_STRING_MAX 64
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//  iem�t�@�C����ǂݍ��ނ��߂̍\���̌Q
//
//-----------------------------------------------------------------------------------------
namespace iem_object_read_from_file{

	//	IEM�p���_���(�o�[�W������)
	struct tagIEMVertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};

	//	IEM�p���_���(�o�[�W������)
	struct tagIEMVertex2
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT4 color;
		XMFLOAT2 uv;
	};

	//	IEM�p�}�e���A�����
	struct tagIEMMaterial{
		XMFLOAT4 diffuse;
		XMFLOAT4 ambient;
		XMFLOAT4 specular;
		XMFLOAT4 emissive;
		float power;
	};

	//	IEM���
	struct tagIEMFILE
	{
		int				version;			//	�o�[�W����	

		//	���b�V�����
		unsigned short	vertexCount;		//	���_��
		void*			pVertex;			//	���_�o�b�t�@

		unsigned short	faceCount;			//	�|���S����
		unsigned short*	pFace;				//	�|���S���C���f�b�N�X
		unsigned long*	pAtr;				//	�|���S���ގ�

		unsigned short	materialCount;		//	�}�e���A����
		tagIEMMaterial	material[32];		//	�}�e���A��
		char			texture[32][IEM_TEXTURE_STRING_MAX];	//	�e�N�X�`���t�@�C��

		//	�{�[�����
		unsigned short  boneCount;
		tagIEMBone*		pBone;

		//	���[�V�������
		unsigned short	frameCount;
		unsigned short	motionCount;
		unsigned short	offset[256];
		unsigned short	frameFlag[65535];
		tagIEMMotion*	pMotion;
	};
};


//-----------------------------------------------------------------------------------------
//
//  �^����`
//
//-----------------------------------------------------------------------------------------
typedef iem_object_read_from_file::tagIEMFILE		IEMFILE;
typedef iem_object_read_from_file::tagIEMVertex		IEMVERTEX;
typedef iem_object_read_from_file::tagIEMVertex2	IEMVERTEX2;
typedef iem_object_read_from_file::tagIEMMaterial	IEMMATERIAL;

//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ�������
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
//  IEM�t�@�C����ǂݍ���
//
//-----------------------------------------------------------------------------------------
IEMLoader::IEMLoader(const char* argpFilapath) : IEMLoader()
{
	//	IEM�t�@�C�����
	IEMFILE l_iemFile;

	//	�ǂݍ��ݗp�t�@�C�����쐬
	_CreateFile(argpFilapath);
	
	//	�o�[�W�����𔻒f����
	VersionJuage();

	//	���_����ǂݍ���
	ReadVertexInfo(&l_iemFile);
	
	//	�|���S������ǂݍ���
	ReadPolygonInfo(&l_iemFile);
	
	//	�}�e���A������ǂݍ���
	ReadMaterialInfo(&l_iemFile);

	//	�{�[������ǂݍ���
	ReadBoneInfo(&l_iemFile);

	//	���[�V��������ǂݍ���
	ReadMotionInfo(&l_iemFile);

	//	�n���h�������
	CloseHandle(m_file);

	//	�f�B���N�g�����l�������e�N�X�`���̃t�@�C���p�X���쐬����
	CreateTextureFilepath(argpFilapath, &l_iemFile);
}

//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ������
//
//-----------------------------------------------------------------------------------------
IEMLoader::~IEMLoader()
{
	delete[] m_pVertices;	m_pVertices		= nullptr; 
	delete[] m_pIndices;	m_pIndices		= nullptr; 
	delete[] m_pAttributes; m_pAttributes	= nullptr; 

	for (int i = 0; i < m_boneCount; ++i){

		//	���[�V�����f�[�^�����
		delete[] m_pMotionInfo[i].pPos;			m_pMotionInfo[i].pPos			= nullptr;
		delete[] m_pMotionInfo[i].pPosFrame;	m_pMotionInfo[i].pPosFrame		= nullptr;
		delete[] m_pMotionInfo[i].pRotate;		m_pMotionInfo[i].pRotate		= nullptr;
		delete[] m_pMotionInfo[i].pRotateFrame; m_pMotionInfo[i].pRotateFrame	= nullptr;

		//	�{�[���f�[�^�����
		delete[] m_pBoneInfo[i].pIndices;		m_pBoneInfo[i].pIndices = nullptr;
		delete[] m_pBoneInfo[i].pInfluence;		m_pBoneInfo[i].pInfluence = nullptr;
	}

	delete[] m_pMotionInfo; m_pMotionInfo	= nullptr;
	delete[] m_pBoneInfo;	m_pBoneInfo		= nullptr;

	

}


//-----------------------------------------------------------------------------------------
//
//  �ǂݍ��ݗp�t�@�C�����쐬����
//
//-----------------------------------------------------------------------------------------
void IEMLoader::_CreateFile(const char* pFilePath)
{
	m_file = CreateFile(pFilePath, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	if (m_file == INVALID_HANDLE_VALUE){
		MessageBox(NULL, "IEM�̓ǂݍ��݂Ɏ��s���܂���", "error", MB_OK);
		exit(EXIT_FAILURE);
	}

}

//-----------------------------------------------------------------------------------------
//
//  IEM�̃o�[�W�������擾����
//
//-----------------------------------------------------------------------------------------
int IEMLoader::GetVersion()const
{
	return m_version;
}

//------------------------------------------------------------------------------
//
//	�}�e���A�������擾����
//
//------------------------------------------------------------------------------
WORD IEMLoader::GetMaterialCount()const
{
	return m_materialCount;
}

//------------------------------------------------------------------------------
//
//	���_�����擾����
//
//------------------------------------------------------------------------------
WORD IEMLoader::GetVertexCount()const
{
	return m_vertexCount;
}

//------------------------------------------------------------------------------
//
//	�ʐ����擾����
//
//------------------------------------------------------------------------------
WORD IEMLoader::GetFaceCount()const
{
	return m_faceCount;
}

//------------------------------------------------------------------------------
//
//	�t���[�������擾����
//
//------------------------------------------------------------------------------
WORD IEMLoader::GetFrameCount()const
{
	return m_frameCount;
}

//------------------------------------------------------------------------------
//
//	�{�[�������擾����
//
//------------------------------------------------------------------------------
WORD IEMLoader::GetBoneCount()const
{
	return m_boneCount;
}

//------------------------------------------------------------------------------
//
//	�C���f�b�N�X�����擾����
//
//------------------------------------------------------------------------------
WORD IEMLoader::GetIndexCount()const
{
	return m_indexCount;
}

//------------------------------------------------------------------------------
//
//	�e�N�X�`���̃t�@�C���p�X���擾����
//
//------------------------------------------------------------------------------
char (*IEMLoader::GetTextures())[MAX_PATH]
{
	return m_pTextures;
}

//------------------------------------------------------------------------------
//
//	���_�����擾����
//
//------------------------------------------------------------------------------
tagIEMMeshVertex*	IEMLoader::GetVertices()
{
	return m_pVertices;
}

//------------------------------------------------------------------------------
//
//	�����f�[�^���擾����
//
//------------------------------------------------------------------------------
LPDWORD IEMLoader::GetAttributes()
{
	return m_pAttributes;
}

//------------------------------------------------------------------------------
//
//	�C���f�b�N�X���擾����
//
//------------------------------------------------------------------------------
WORD* IEMLoader::GetIndices()
{
	return m_pIndices;
}

//------------------------------------------------------------------------------
//
//	�{�[���f�[�^���擾����
//
//------------------------------------------------------------------------------
tagIEMBone* IEMLoader::GetBoneInfo()
{
	return m_pBoneInfo;
}

//------------------------------------------------------------------------------
//
//	���[�V�����f�[�^���擾����
//
//------------------------------------------------------------------------------
tagIEMMotion* IEMLoader::GetMotionInfo()
{
	return m_pMotionInfo;
}

//------------------------------------------------------------------------------
//
//	offset���擾����
//
//------------------------------------------------------------------------------
WORD* IEMLoader::GetOffSet()
{
	return m_offset;
}

//------------------------------------------------------------------------------
//
//	�t���[�������擾����
//
//------------------------------------------------------------------------------
WORD* IEMLoader::GetFrameFlag()
{
	return m_frameFlag;
}

//------------------------------------------------------------------------------
//
//	IEM�̃o�[�V�����𔻕ʂ���
//
//------------------------------------------------------------------------------
void IEMLoader::VersionJuage()
{
	unsigned long l_fileID = 0;

	//	IEM�o�[�W������ǂݍ���
	ReadFile(&l_fileID, 4);
	switch (l_fileID){
	case '1MEI':m_version = 1;	break;
	case '2MEI':m_version = 2;	break;
	case '3MEI':m_version = 3;	break;
	case '4MEI':m_version = 4;	break;
	default:	m_version = -1; break;
	}

	if (m_version <= 0){
		MessageBox(NULL, "IEM�̃o�[�W�������s���ł�", "error", MB_OK);
		exit(EXIT_FAILURE);
	}

}

//------------------------------------------------------------------------------
//
//	���_����ǂݍ���
//
//------------------------------------------------------------------------------
void IEMLoader::ReadVertexInfo(void* pEIMFILE)
{
	//	IEMFILE�ɃL���X�g
	IEMFILE* l_pIEMFILE = GET_IME_FILE_POINTER(pEIMFILE);

	//	���_����ǂݍ���
	ReadFile(&l_pIEMFILE->vertexCount, sizeof(WORD));
	
	//	���_���ۑ�
	m_vertexCount = l_pIEMFILE->vertexCount;

	//	�o�[�W�����ʂŒ��_����ǂݍ���
	if (m_version < 4){
		ReadVertexInfoIEMVersion1(pEIMFILE);
	}
	else{
		ReadVertexInfoIEMVersion2(pEIMFILE);
	}
}

//------------------------------------------------------------------------------
//
//	��IEM�o�[�W������ǂݍ���
//
//------------------------------------------------------------------------------
void IEMLoader::ReadVertexInfoIEMVersion1(void* pEIMFILE)
{
	//	IEMFILE�ɃL���X�g
	IEMFILE* l_pIEMFILE = GET_IME_FILE_POINTER(pEIMFILE);

	//	���_��������
	l_pIEMFILE->pVertex = new IEMVERTEX[m_vertexCount];

	//	���_�f�[�^��ǂݍ���
	ReadFile(l_pIEMFILE->pVertex, sizeof(IEMVERTEX) * m_vertexCount);

	//	���_������������
	m_pVertices = new tagIEMMeshVertex[l_pIEMFILE->vertexCount];

	//	 tagIEMVertex�ɃL���X�g
	IEMVERTEX* l_pVertex = (IEMVERTEX*)l_pIEMFILE->pVertex;

	//	�f�[�^���R�s�[���邪���_�F�����Ȃ��̂ŁA�ǉ����Ă���
	for (WORD i = 0; i < l_pIEMFILE->vertexCount; ++i){
		m_pVertices[i].pos		= l_pVertex[i].pos;
		m_pVertices[i].normal	= l_pVertex[i].normal;
		m_pVertices[i].color	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_pVertices[i].uv		= l_pVertex[i].uv;
	}

	//	���
	if (l_pIEMFILE->pVertex){ delete[] l_pIEMFILE->pVertex; l_pIEMFILE->pVertex = nullptr; }
}

//------------------------------------------------------------------------------
//
//	�VIEM�o�[�W������ǂݍ���
//
//------------------------------------------------------------------------------
void IEMLoader::ReadVertexInfoIEMVersion2(void* pEIMFILE)
{
	//	IEMFILE�ɃL���X�g
	IEMFILE* l_pIEMFILE = GET_IME_FILE_POINTER(pEIMFILE);

	//	���_��������
	l_pIEMFILE->pVertex = new IEMVERTEX2[m_vertexCount];

	//	���_�f�[�^��ǂݍ���
	ReadFile(l_pIEMFILE->pVertex, sizeof(IEMVERTEX2) * m_vertexCount);

	//	�f�[�^���R�s�[
	m_pVertices = (tagIEMMeshVertex*)l_pIEMFILE->pVertex;
}

//------------------------------------------------------------------------------
//
//	�|���S������ǂݍ���
//
//------------------------------------------------------------------------------
void IEMLoader::ReadPolygonInfo(void* pEIMFILE)
{
	//	IEMFILE�ɃL���X�g
	IEMFILE* l_pIEMFILE = GET_IME_FILE_POINTER(pEIMFILE);

	//	�|���S������ǂݍ���
	ReadFile(&l_pIEMFILE->faceCount, sizeof(WORD));

	//	�ʐ��ۑ�
	m_faceCount = l_pIEMFILE->faceCount;

	//	�C���f�b�N�X���ۑ�
	m_indexCount = m_faceCount * 3;

	l_pIEMFILE->pFace = new WORD[m_indexCount];
	l_pIEMFILE->pAtr = new DWORD[m_faceCount];

	//	�ʏ���ǂݍ���
	ReadFile(l_pIEMFILE->pFace, sizeof(WORD)*m_indexCount);

	//	�ގ�������ǂݍ���
	ReadFile(l_pIEMFILE->pAtr, sizeof(DWORD)*m_faceCount);


	//	�f�[�^���R�s�[����
	m_pIndices		= l_pIEMFILE->pFace;
	m_pAttributes	= l_pIEMFILE->pAtr;
}

//------------------------------------------------------------------------------
//
//	�}�e���A������ǂݍ���
//
//------------------------------------------------------------------------------
void IEMLoader::ReadMaterialInfo(void* pEIMFILE)
{
	IEMFILE* l_pIEMFILE = GET_IME_FILE_POINTER(pEIMFILE);

	//	�}�e���A������ǂݍ���
	ReadFile(&l_pIEMFILE->materialCount, sizeof(unsigned short));

	//	�}�e���A�����ۑ�
	m_materialCount = l_pIEMFILE->materialCount;

	//	�}�e���A������ǂݍ���
	ReadFile(&l_pIEMFILE->material, sizeof(IEMMATERIAL)* m_materialCount);

	//	�e�N�X�`������ǂݍ���
	ReadFile(&l_pIEMFILE->texture, sizeof(char)* m_materialCount * IEM_TEXTURE_STRING_MAX);

}

//------------------------------------------------------------------------------
//
//	�{�[������ǂݍ���
//
//------------------------------------------------------------------------------
void IEMLoader::ReadBoneInfo(void* pEIMFILE)
{
	IEMFILE* l_pIEMFILE = GET_IME_FILE_POINTER(pEIMFILE);

	//	�{�[������ǂݍ���
	ReadFile(&l_pIEMFILE->boneCount, sizeof(WORD));

	//	�{�[������ۑ�
	m_boneCount = l_pIEMFILE->boneCount;

	//	�{�[����������
	l_pIEMFILE->pBone = new tagIEMBone[m_boneCount];

	for (int i = 0; i < m_boneCount; ++i){

		//	�{�[������ǂݍ���
		ReadFile(&l_pIEMFILE->pBone[i], sizeof(tagIEMBone));

		//	�{�[���̃C���f�b�N�X��������
		l_pIEMFILE->pBone[i].pIndices = new DWORD[l_pIEMFILE->pBone[i].indexCount];

		//	�{�[���̃C���f�b�N�X����ǂݍ���
		ReadFile(l_pIEMFILE->pBone[i].pIndices, sizeof(DWORD)* l_pIEMFILE->pBone[i].indexCount);

		//	�{�[���̃C���f�b�N�X��������
		l_pIEMFILE->pBone[i].pInfluence = new float[l_pIEMFILE->pBone[i].indexCount];

		//	�{�[���̃E�F�C�g����ǂݍ���
		ReadFile(l_pIEMFILE->pBone[i].pInfluence, sizeof(float)* l_pIEMFILE->pBone[i].indexCount);
	}

	//	�f�[�^���R�s�[
	m_pBoneInfo = l_pIEMFILE->pBone;
}

//------------------------------------------------------------------------------
//
//	���[�V��������ǂݍ���
//
//------------------------------------------------------------------------------
void IEMLoader::ReadMotionInfo(void* pEIMFILE)
{
	IEMFILE* l_pIEMFILE = GET_IME_FILE_POINTER(pEIMFILE);

	//	���[�V��������ǂݍ���
	ReadFile(&l_pIEMFILE->motionCount, sizeof(WORD));

	//	���[�V��������ۑ�
	m_motionCount = l_pIEMFILE->motionCount;

	//	�t���[������ǂݍ���
	ReadFile(&l_pIEMFILE->frameCount, sizeof(WORD));

	//	�t���[������ۑ�
	m_frameCount = l_pIEMFILE->frameCount;

	//	�I�t�Z�b�g������ǂݍ���
	ReadFile(&l_pIEMFILE->offset, sizeof(WORD)* 256);

	//	�t���[���̃t���O����ǂݍ���
	ReadFile(&l_pIEMFILE->frameFlag, sizeof(WORD)* m_frameCount);

	//	�{�[��������
	l_pIEMFILE->pMotion = new tagIEMMotion[m_boneCount];

	//	Motion�f�[�^�̃|�C���^��ێ�
	tagIEMMotion* l_pTempMotion = nullptr;

	//	�{�[������
	for (WORD i = 0; i < m_boneCount; ++i){

		//	i�Ԗڂ̃��[�V�����f�[�^�̃|�C���^���擾
		l_pTempMotion = &l_pIEMFILE->pMotion[i];

		//	���[�V�����f�[�^��ǂݍ���
		ReadFile(l_pTempMotion, sizeof(tagIEMMotion));

		//	��]�L�[�t���[��������
		l_pTempMotion->pRotate		= new XMFLOAT4[l_pTempMotion->rotateCount];
		l_pTempMotion->pRotateFrame = new WORD[l_pTempMotion->rotateCount];

		//	��]�t���[���Ɖ�]����ǂݍ���
		ReadFile(l_pTempMotion->pRotate,		sizeof(XMFLOAT4)	* l_pTempMotion->rotateCount);
		ReadFile(l_pTempMotion->pRotateFrame,	sizeof(WORD)		* l_pTempMotion->rotateCount);

		//	���W������
		l_pTempMotion->pPos			= new XMFLOAT3[l_pTempMotion->posCount];
		l_pTempMotion->pPosFrame	= new WORD[l_pTempMotion->posCount];

		//	���W��ǂݍ���
		ReadFile(l_pTempMotion->pPos,		sizeof(XMFLOAT3)	*	l_pTempMotion->posCount);
		ReadFile(l_pTempMotion->pPosFrame,	sizeof(WORD)	*	l_pTempMotion->posCount);
	}

	//	�f�[�^���R�s�[
	m_pMotionInfo = l_pIEMFILE->pMotion;

	//	�I�t�Z�b�g�ƃt���[���O���O�����R�s�[
	CopyMemory(m_offset,	l_pIEMFILE->offset,		sizeof(WORD)* 256);
	CopyMemory(m_frameFlag, l_pIEMFILE->frameFlag,	sizeof(WORD)* m_frameCount);

}

//------------------------------------------------------------------------------
//
//	�f�B���N�g�����l�������e�N�X�`���̃t�@�C���p�X���쐬����
//
//------------------------------------------------------------------------------
void IEMLoader::CreateTextureFilepath(const char* pFilePath, void* pEIMFILE)
{
	IEMFILE*	l_pIEMFILE = GET_IME_FILE_POINTER(pEIMFILE);

	char	l_directoryPath[MAX_PATH];

	//	���f���̃t�@�C���p�X���R�s�[����
	CopyMemory(l_directoryPath, pFilePath, lstrlen(pFilePath) + 1);

	//	�p�X�̔����璲�ׂĂ���
	for (int i = lstrlen(pFilePath); i>0; i--){

		//	��������s�o�C�g�Ȃ�A�_�u���o�C�g�����Z�b�g�Ȃǂł�����x�f�N�������g����
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
//	ReadFile�̃��b�p�[�֐�
//
//------------------------------------------------------------------------------
void IEMLoader::ReadFile(LPVOID pBuffer, DWORD dataSize)
{
	DWORD l_readByteSize = 0;

	if (::ReadFile(m_file, pBuffer, dataSize, &l_readByteSize, nullptr) == -1){
		MessageBox(NULL, "ReadFile�Ɏ��s���܂����B���_��񂩉������Ԉ���Ă��܂�", "ReadFile(LPVOID argp_buffer, DWORD arg_byteToRead) error", MB_OK);
		exit(EXIT_FAILURE);
	}
}

