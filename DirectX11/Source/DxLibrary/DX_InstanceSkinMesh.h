//#ifndef __DX_ISNTANCE_SKIN_MESH_H_
//#define __DX_ISNTANCE_SKIN_MESH_H_
//
//
////****************************************************************************************************
////
////	InstanceSkinMesh
////
////****************************************************************************************************
//class DX_InstanceSkinMesh : public DX_SkinMesh
//{
//private:
//	//	�R���s���[�g�Ō��ʂ��������ރ��\�[�X
//	struct tagResultResource{
//		ID3D11UnorderedAccessView*	m_pUAV;
//		ID3D11Buffer*				m_pUAVBuffer;
//	}m_resultResource;
//
//	//	�R���s���[�g�V�F�[�_�[�̌��ʂ�CPU�Ŏ󂯎��o�b�t�@
//	ID3D11Buffer*	m_pReadBuffer;
//
//	//	�R���s���[�g�V�F�[�_�[�ŏ������ޑΏۃo�b�t�@
//	ID3D11Buffer*	m_pGPUWriteBuffer;
//
//	//	�C���X�^���X��
//	int m_instanceCount;
//
//public:
//	DX_InstanceSkinMesh();
//	DX_InstanceSkinMesh(const char* pFilepath, const int instanceCount);
//	~DX_InstanceSkinMesh();
//
//	void Update();
//	void Animation();
//	void Render();
//};
//#endif // !__DX_ISNTANCE_SKIN_MESH_H_
