#include "../DX_LIBRARY/DX_Library.h"
#include	<stdio.h>

#include	"IMOLoader.h"
using namespace DirectX;
//-----------------------------------------------------------------------------------------
//
//  imo�t�@�C����ǂݍ��ނ��߂̍\���̌Q
//
//-----------------------------------------------------------------------------------------
namespace imo_object_read_from_file{

	//	IMO�ǂݍ��ݗp ���_���
	struct tagIMOVertex {
		XMFLOAT3 pos;
		DWORD	color;
		XMFLOAT2 uv;
	};

	//	IMO�ǂݍ��ݗp �}�e���A�����
	struct tagIMOMaterial{
		XMFLOAT4   diffuse;
		XMFLOAT4   ambient;
		XMFLOAT4   specular;
		XMFLOAT4   emissive;
		float      power;
	};

	//	IMO�ǂݍ��ݗp �S���
	struct tagIMOObject {
		DWORD		id;
		int			type;
		DWORD		vertexCount;
		tagIMOVertex* p_vertices;

		DWORD		faceCount;
		LPWORD		p_faces;
		LPDWORD		p_attribute;

		DWORD			materialCount;	//	�}�e���A����
		tagIMOMaterial	material[32];	//	�}�e���A��
		char			textures[32][32];//	�e�N�X�`���t�@�C��

	};
};

//-----------------------------------------------------------------------------------------
//
//  �^����`
//
//-----------------------------------------------------------------------------------------
typedef imo_object_read_from_file::tagIMOVertex IMO_OBJECT_VERTEX;
typedef imo_object_read_from_file::tagIMOObject IMO_OBJECT;


//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ��̏�����
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
//  IMO�t�@�C����ǂݍ���
//
//-----------------------------------------------------------------------------------------
IMOLoader::IMOLoader(const char* pFilePath) : IMOLoader()
{
	IMO_OBJECT l_imoObj;

	//	�t�@�C���𐶐�
	m_file = CreateFile(pFilePath, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	if (m_file == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, "�t�@�C�����A�܂��̓t�@�C���p�X���Ԉ���Ă��܂�", "class IMOLoader(char*) : CreateFile error", MB_OK);
		exit(EXIT_FAILURE);
	}

	//	IMO����ǂݍ���
	ReadFile(&l_imoObj, sizeof(IMO_OBJECT));

	//	���_���A�C���f�b�N�X���A�ʐ��A�}�e���A�����A�ގ����ۑ�
	m_vertexCount	= l_imoObj.vertexCount;
	m_indexCount	= l_imoObj.faceCount * 3;
	m_faceCount		= l_imoObj.faceCount;
	m_materialCount = l_imoObj.materialCount;
	
	//	�e�퐶��
	m_pVertices		= new IMOMESHVERTEX[m_vertexCount];
	m_pIndices		= new WORD[m_indexCount];
	m_pAttributes	= new DWORD[m_faceCount];
	
	//	IMO��version�ɏ]���ă��[�h���@��ς���
	if (l_imoObj.id != '2OMI'){
		LoadVersion1();
	}
	else{
		LoadVersion2();
	}

	//	�ʂ��ۗL����}�e���A���ԍ����擾
	ReadFile(m_pAttributes, sizeof(DWORD) * m_faceCount);

	//	�p�X�𕪊����A�}�e���A�������e�N�X�`���p�X���擾����
	char	workpath[MAX_PATH] = { NULL };

	//	�t�@�C���p�X�𕪊�����
	SplitThePath(workpath, pFilePath);

	//	m_pTextures�Ƀe�N�X�`���p�X�����Ă���
	BindPath(workpath, l_imoObj.textures);

	//	�t�@�C�������
	CloseHandle(m_file);
}

//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ������
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
//  ���_�����擾
//
//-----------------------------------------------------------------------------------------
DWORD IMOLoader::GetVertexCount()const
{
	return m_vertexCount;
}

//-----------------------------------------------------------------------------------------
//
//  �C���f�b�N�X�����擾
//
//-----------------------------------------------------------------------------------------
DWORD IMOLoader::GetIndexCount()const
{
	return m_indexCount; 
}

//-----------------------------------------------------------------------------------------
//
//  �ʐ����擾
//
//-----------------------------------------------------------------------------------------
DWORD IMOLoader::GetFaceCount()const
{
	return m_faceCount;
}


//-----------------------------------------------------------------------------------------
//
//  �}�e���A�������擾
//
//-----------------------------------------------------------------------------------------
DWORD IMOLoader::GetMaterialCount()const
{
	return m_materialCount;
}

//-----------------------------------------------------------------------------------------
//
//  �����f�[�^���擾
//
//-----------------------------------------------------------------------------------------
const LPDWORD IMOLoader::GetAttributes()const
{
	return m_pAttributes;
}

//-----------------------------------------------------------------------------------------
//
//  �C���f�b�N�X�f�[�^���擾
//
//-----------------------------------------------------------------------------------------
const LPWORD  IMOLoader::GetIndices()const
{
	return m_pIndices; 
}

//-----------------------------------------------------------------------------------------
//
//  ���_�f�[�^���擾
//
//-----------------------------------------------------------------------------------------
const LPIMOMESHVERTEX IMOLoader::GetVertices()const
{
	return m_pVertices; 
}

//-----------------------------------------------------------------------------------------
//
//	�e�N�X�`���p�X���擾
//
//-----------------------------------------------------------------------------------------
char (*IMOLoader::GetTextures())[MAX_PATH]
{
	return m_pTextures;
}

//-----------------------------------------------------------------------------------------
//
//	���o�[�W������IMO�t�@�C����ǂݍ���
//
//-----------------------------------------------------------------------------------------
void IMOLoader::LoadVersion1()
{
	//	���_��������
	IMO_OBJECT_VERTEX* l_pReadIMOVertex = new IMO_OBJECT_VERTEX[m_vertexCount];
	
	//	���_�ǂݍ���
	ReadFile(l_pReadIMOVertex, sizeof(IMO_OBJECT_VERTEX)* m_vertexCount);

	//	�C���f�b�N�X�ǂݍ���
	ReadFile(m_pIndices, sizeof(WORD) * m_indexCount);

	const float l_par = 1.0f / 255.0f;

	LPIMOMESHVERTEX l_pIMOMeshVertex = nullptr;
	IMO_OBJECT_VERTEX*	l_pIMOVertex = nullptr;
	//	���_���������o�[�ɃR�s�[
	for (DWORD i = 0; i < m_vertexCount; ++i){

		//	�A�h���X���擾
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

	//	���_�@�����쐬
	XMVECTOR pos1, pos2,pos3;
	XMVECTOR nor1, nor2, nor3;
	XMVECTOR temp;
	for (DWORD i = 0; i < m_faceCount; ++i){

		
		//	�C���f�b�N�X���擾
		l_index1 = m_pIndices[i * 3 + 0];
		l_index2 = m_pIndices[i * 3 + 1];
		l_index3 = m_pIndices[i * 3 + 2];

		//	���_�����擾
		l_pVertex1 = &l_pIMOVertex[l_index1];
		l_pVertex2 = &l_pIMOVertex[l_index2];
		l_pVertex3 = &l_pIMOVertex[l_index3];

		//	�ʖ@���v�Z
		pos1 = XMLoadFloat3(&l_pVertex1->pos);
		pos2 = XMLoadFloat3(&l_pVertex2->pos);
		pos3 = XMLoadFloat3(&l_pVertex3->pos);

		XMStoreFloat3(&l_v1, pos1 - pos2);
		XMStoreFloat3(&l_v2, pos2 - pos3);

		//	�O��
		l_n.x = (l_v1.y * l_v2.z - l_v1.z * l_v2.y);
		l_n.y = (l_v1.z * l_v2.x - l_v1.x * l_v2.z);
		l_n.z = (l_v1.x * l_v2.y - l_v1.y * l_v2.x);

		//	����
		l_d = sqrtf(l_n.x * l_n.x + l_n.y * l_n.y + l_n.z * l_n.z);

		l_d = 1.0f / l_d;
		l_n.x *= l_d;
		l_n.y *= l_d;
		l_n.z *= l_d;

		//	���_�����擾
		l_pIMOMeshVertex1 = &m_pVertices[l_index1];
		l_pIMOMeshVertex2 = &m_pVertices[l_index2];
		l_pIMOMeshVertex3 = &m_pVertices[l_index3];

		//	���_�@�����쐬
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

	//	���_�@���𐳋K��
	for (DWORD i = 0; i < m_vertexCount; ++i){
		XMStoreFloat3(&m_pVertices[i].normal, XMVector3Normalize(XMLoadFloat3(&m_pVertices[i].normal)));
	}

	//	���
	if (l_pReadIMOVertex){ delete[]	l_pReadIMOVertex; l_pReadIMOVertex = nullptr; }
}

//-----------------------------------------------------------------------------------------
//
//	�V�o�[�W������IMO�t�@�C����ǂݍ���
//
//-----------------------------------------------------------------------------------------
void IMOLoader::LoadVersion2()
{
	//	���_�ǂݍ���
	ReadFile(m_pVertices, sizeof(tagIMOMeshVertex)* m_vertexCount);

	//	�C���f�b�N�X�ǂݍ���
	ReadFile(m_pIndices, sizeof(WORD)*m_indexCount);

}

//-----------------------------------------------------------------------------------------
//
//	ReadFile�̃��b�p�[�֐�
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
//	�p�X�𕪊�����
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
//	�p�X��m_pTextures�ɓo�^����
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

