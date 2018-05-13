#ifndef __DX_INSTANCE_MESH_H_
#define __DX_INSTANCE_MESH_H_


//--------------------------------------------------
//	�C���X�^���X�`��p�\����
//--------------------------------------------------
struct tagInstanceMeshInfo{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 angle;
	DirectX::XMFLOAT3 scale;
};

//****************************************************************************************************
//
//	InstanceMesh
//
//****************************************************************************************************
class InstanceMesh : public dx_library::Mesh
{
private:
	//	�R���s���[�g�Ō��ʂ��������ރ��\�[�X
	struct tagResultResource{
		ComPtr<ID3D11UnorderedAccessView>	m_uav;
		ComPtr<ID3D11Buffer>				m_uavBuffer;
	}m_resultResource;
	
	//	�R���s���[�g�V�F�[�_�[�̌��ʂ�CPU�Ŏ󂯎��o�b�t�@
	ComPtr<ID3D11Buffer>	m_readBuffer;

	//	�R���s���[�g�V�F�[�_�[�ŏ������ޑΏۃo�b�t�@
	ComPtr<ID3D11Buffer>	m_gpuWriteBuffer;

	//	�C���X�^���X��
	int m_instanceCount;

public:
	InstanceMesh();
	InstanceMesh(const char* pFilepath, const int instanceCount);
	~InstanceMesh();

	//	�C���X�^���X�����X�V����
	void Update(dx_library::tagInstanceMeshInfo* pInstanceInfo);

	void Render();
	
	void SetPos(DirectX::XMFLOAT3* pos){};
};
#endif // !__DX_INSTANCE_MESH_H_
