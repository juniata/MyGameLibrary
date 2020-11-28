#include "SkyLibrary.h"
#include <string.h>

/// <summary>
/// �����o�ϐ��̏��������s��
/// </summary>
FBXLoader::FBXLoader() :
	m_pFbxManager(nullptr),
	m_pFbxIos(nullptr),
	m_pFbxScene(nullptr),
	m_pMeshList(nullptr),
	m_meshCount(0)
{}

/// <summary>
/// fbx�A�ϐ����̉���������s��
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
/// fbx�t�@�C����ǂݍ��ށB
/// </summary>
/// <param name="pFilename">�ǂݍ���fbx�̃t�@�C����(��.fbx���܂܂�܂�)</param>
/// <returns>�ǂݍ��݂̐���</returns>
bool FBXLoader::ReadObj(const char* pFilename)
{
	if (!Initialize()) {
		return false;
	}

	if (!LoadScene(pFilename)) {
		return false;
	}


	// ���b�V���Ɏg���Ă���}�e���A���P�ʂŃ��b�V���𕪊�����
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
/// �`��p���b�V���f�[�^���擾����
/// </summary>
/// <returns>�`��p�ɐ��`�������b�V���f�[�^���Ԃ�</returns>
FBXLoader::tagMeshData* FBXLoader::GetMeshData()
{
	return m_pMeshList;
}

/// <summary>
/// �}�e���A������SRV�̕R�Â����X�g���擾����
/// </summary>
/// <returns>�R�Â����X�g</returns>
std::map<const char*, ID3D11ShaderResourceView*>& FBXLoader::GetMaterialLinks()
{
	return m_materialLinks;
}

/// <summary>
/// ���b�V���̐����擾����
/// </summary>
/// <returns>���b�V����</returns>
int FBXLoader::GetMeshCount() const
{
	return m_meshCount;
}

/// <summary>
/// FBX SDK�̏��������s��
/// </summary>
/// <returns>�������ł������ǂ���</returns>
bool FBXLoader::Initialize()
{
	m_pFbxManager = FbxManager::Create();
	if (!m_pFbxManager) {
		TRACE("FBXManager�̍쐬�Ɏ��s���܂����B")
		return false;
	}

	m_pFbxIos = FbxIOSettings::Create(m_pFbxManager, IOSROOT);
	m_pFbxManager->SetIOSettings(m_pFbxIos);

	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
	if (!m_pFbxScene) {
		TRACE("FBXScene�̍쐬�Ɏ��s���܂���")
		return false;
	}

	return true;
}

/// <summary>
/// FBX�V�[���̓ǂݍ��݂��s��
/// </summary>
/// <param name="pFilepath">�ǂݍ���fbx�̃t�@�C����(��.fbx���܂܂�܂�)</param>
/// <returns>�V�[���̓ǂݍ��݂��ł������ǂ���</returns>
bool FBXLoader::LoadScene(const char* pFilepath)
{
	FbxImporter* pImpoter = FbxImporter::Create(m_pFbxManager, "");
	if (!pImpoter->Initialize(pFilepath, -1, m_pFbxManager->GetIOSettings())) {
		FbxString str = pImpoter->GetStatus().GetErrorString();
		TRACE(str.Buffer())
		return false;
	}
	
	if (!pImpoter->IsFBX()) {
		TRACE("�ǂݍ��񂾃t�@�C����FBX�ł͂���܂���")
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
/// �m�[�h�Ɋ܂܂�Ă���}�e���A������ǂݍ���
/// </summary>
/// <param name="pMaterial">�}�e���A�����</param>
void FBXLoader::LoadMaterials(FbxSurfaceMaterial* pMaterial)
{
	// FbxSurfacePhong�̏ꍇ�A�ŏI�I�Ȍp����̂��߁AFbxSurfaceLambert�̒l�������邪�A
	// FbxSurfaceLambert�̏ꍇFbxSuracePhone�̒l�͈����Ȃ����߁A�����Ńv���p�e�B���擾���Ă����A
	// FbxSurfaceLambert�̏ꍇ�̂Ƃ������v���p�e�B���^�u�擾����
	FbxDouble3 colors[2];
	FbxDouble factors[2];

	// TODO:Material�擾�����쐬��
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
/// �e�N�X�`������ǂݍ���
/// </summary>
/// <param name="pMesh">�}�e���A�����</param>
void FBXLoader::LoadTextures(FbxSurfaceMaterial* pMaterial)
{
	FbxProperty prop = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	FbxFileTexture* pTexture = nullptr;

	// TODO:��̃e�N�X�`���ɂ����Ή����Ă��܂���B
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
/// ���b�V�������擾
/// <param name="pNode">���b�V�����</param>
/// <param name="ppMeshData">�`�悷�邽�߂̃��b�V�������i�[����</param>
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
/// �C���f�b�N�X��ǂݍ���
/// </summary>
/// <param name="pMesh">���b�V�����</param>
/// <param name="ppMeshData">�C���f�b�N�X�f�[�^���i�[����</param>
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
/// ���_���W��ǂݍ���
/// </summary>
/// <param name="pMesh">���b�V�����</param>
/// <param name="ppMeshData">���_���W�f�[�^���i�[����</param>
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
/// ���_�@�����擾
/// </summary>
/// <param name="pMesh">���b�V�����</param>
/// <param name="ppMeshData">���_���W�f�[�^���i�[����</param>
void FBXLoader::LoadNormal(FbxMesh* pMesh, FBXLoader::tagMeshData** ppMeshData)
{
	tagMeshData* pMeshData = *ppMeshData;

	FbxArray<FbxVector4> normals;
	pMesh->GetPolygonVertexNormals(normals);

	for (int i = 0; i < normals.Size(); ++i) {
		// �E��n���獶��n�ɕϊ�
		pMeshData->pVertices[i].normal.x = -static_cast<float>(normals[i][0]);
		pMeshData->pVertices[i].normal.y = static_cast<float>(normals[i][1]);
		pMeshData->pVertices[i].normal.z = static_cast<float>(normals[i][2]);
	}
}

/// <summary>
/// ���_�J���[��ǂݍ���
/// </summary>
/// <param name="pMesh">���b�V�����</param>
/// <param name="ppMeshData">���_�J���[�f�[�^���i�[����</param>
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
	// TODO:���_�F�擾�쐬��

}

/// <summary>
/// ���_UV��ǂݍ���
/// </summary>
/// <param name="pMesh">���b�V�����</param>
/// <param name="ppMeshData">���_UV�f�[�^���i�[����</param>
void FBXLoader::LoadUVSetNames(FbxMesh* pMesh, tagMeshData** ppMeshData)
{
	tagMeshData* pMeshData = *ppMeshData;

	FbxStringList uvsetName;
	pMesh->GetUVSetNames(uvsetName);


	// TODO:UVset�������������ꍇ�͌�X���
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
/// ���b�V���ɃA�^�b�`����Ă���}�e���A���̖��O��ǂݍ���
/// </summary>
/// <param name="pMesh">���b�V�����</param>
/// <param name="ppMeshData">�}�e���A���̖��O���i�[����</param>
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

