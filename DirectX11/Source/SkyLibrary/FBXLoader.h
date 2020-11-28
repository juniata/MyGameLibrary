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
/// FBX�t�@�C����ǂݍ��݁A�`��ł���`�ɂ���B
/// 
/// ���낢��Ȏ擾���@�����邪�AMesh��Mesh�ŁAMaterial��Material�Ŏ擾����(Node��T���Ă͂��Ȃ�)
/// ���R�Ƃ��ẮA���̂ق����ȒP�Ƀv���O�����������邩��B
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
	/// �����o�ϐ��̏��������s��
	/// </summary>
	FBXLoader();

	/// <summary>
	/// fbx�A�ϐ����̉���������s��
	/// </summary>
	~FBXLoader();

	/// <summary>
	/// fbx�t�@�C����ǂݍ��ށB
	/// </summary>
	/// <param name="pFilename">�ǂݍ���fbx�̃t�@�C����(��.fbx���܂܂�܂�)</param>
	/// <returns>�ǂݍ��݂̐���</returns>
	bool ReadObj(const char* pFilename);

	/// <summary>
	/// �`��p���b�V���f�[�^���擾����
	/// </summary>
	/// <returns>�`��p�ɐ��`�������b�V���f�[�^���Ԃ�</returns>
	tagMeshData* GetMeshData();

	/// <summary>
	/// �}�e���A������SRV�̕R�Â����X�g���擾����
	/// </summary>
	/// <returns>�R�Â����X�g</returns>
	std::map<const char*, ID3D11ShaderResourceView*>& GetMaterialLinks();

	/// <summary>
	/// ���b�V���̐����擾����
	/// </summary>
	/// <returns>���b�V����</returns>
	int GetMeshCount() const;

private:
	FbxManager*		m_pFbxManager;
	FbxIOSettings*	m_pFbxIos;
	FbxScene*		m_pFbxScene;


	tagMeshData* m_pMeshList;
	int m_meshCount;

	std::map<const char*, ID3D11ShaderResourceView*> m_materialLinks;

	/// <summary>
	/// FBX SDK�̏��������s��
	/// </summary>
	/// <returns>�������ł������ǂ���</returns>
	bool Initialize();

	/// <summary>
	/// FBX�V�[���̓ǂݍ��݂��s��
	/// </summary>
	/// <param name="pFilepath">�ǂݍ���fbx�̃t�@�C����(��.fbx���܂܂�܂�)</param>
	/// <returns>�V�[���̓ǂݍ��݂��ł������ǂ���</returns>
	bool LoadScene(const char* pFilepath);

	/// <summary>
	/// �m�[�h�Ɋ܂܂�Ă���}�e���A������ǂݍ���
	/// </summary>
	/// <param name="pMaterial">�}�e���A�����</param>
	void LoadMaterials(FbxSurfaceMaterial* pMaterial);

	/// <summary>
	/// �e�N�X�`������ǂݍ���
	/// </summary>
	/// <param name="pMesh">�}�e���A�����</param>
	void LoadTextures(FbxSurfaceMaterial* pMaterial);


	/// <summary>
	/// ���b�V������ǂݍ���
	/// <param name="pMesh">���b�V�����</param>
	/// <param name="pMeshData">�`�悷�邽�߂̃��b�V�������i�[����</param>
	/// </summary>
	void LoadMesh(FbxMesh* pMesh, tagMeshData* pMeshData);

	/// <summary>
	/// �C���f�b�N�X��ǂݍ���
	/// </summary>
	/// <param name="pMesh">���b�V�����</param>
	/// <param name="ppMeshData">�C���f�b�N�X�f�[�^���i�[����</param>
	void LoadIndices(FbxMesh* pMesh, tagMeshData** ppMeshData);

	/// <summary>
	/// ���_���W��ǂݍ���
	/// </summary>
	/// <param name="pMesh">���b�V�����</param>
	/// <param name="ppMeshData">���_���W�f�[�^���i�[����</param>
	void LoadVertices(FbxMesh* pMesh, tagMeshData** ppMeshData);

	/// <summary>
	/// ���_�@����ǂݍ���
	/// </summary>
	/// <param name="pMesh">���b�V�����</param>
	/// <param name="ppMeshData">���_�@���f�[�^���i�[����</param>
	void LoadNormal(FbxMesh* pMesh, tagMeshData** ppMeshData);

	/// <summary>
	/// ���_�J���[��ǂݍ���
	/// </summary>
	/// <param name="pMesh">���b�V�����</param>
	/// <param name="ppMeshData">���_�J���[�f�[�^���i�[����</param>
	void LoadVertexColors(FbxMesh* pMesh, tagMeshData** ppMeshData);

	/// <summary>
	/// ���_UV��ǂݍ���
	/// </summary>
	/// <param name="pMesh">���b�V�����</param>
	/// <param name="ppMeshData">���_UV�f�[�^���i�[����</param>
	void LoadUVSetNames(FbxMesh* pMesh, tagMeshData** ppMeshData);

	/// <summary>
	/// ���b�V���ɃA�^�b�`����Ă���}�e���A���̖��O��ǂݍ���
	/// </summary>
	/// <param name="pMesh">���b�V�����</param>
	/// <param name="ppMeshData">�}�e���A���̖��O���i�[����</param>
	void LoadMaterialName(FbxMesh* pMesh, tagMeshData** ppMeshData);
};