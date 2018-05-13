#ifndef __DX_INSTANCE_MESH_H_
#define __DX_INSTANCE_MESH_H_


//--------------------------------------------------
//	インスタンス描画用構造体
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
class DX_InstanceMesh : public DX_Mesh
{
private:
	//	コンピュートで結果を書き込むリソース
	struct tagResultResource{
		ComPtr<ID3D11UnorderedAccessView>	m_uav;
		ComPtr<ID3D11Buffer>				m_uavBuffer;
	}m_resultResource;
	
	//	コンピュートシェーダーの結果をCPUで受け取るバッファ
	ComPtr<ID3D11Buffer>	m_readBuffer;

	//	コンピュートシェーダーで書き込む対象バッファ
	ComPtr<ID3D11Buffer>	m_gpuWriteBuffer;

	//	インスタンス数
	int m_instanceCount;

public:
	DX_InstanceMesh();
	DX_InstanceMesh(const char* pFilepath, const int instanceCount);
	~DX_InstanceMesh();

	//	インスタンス情報を更新する
	void Update(tagInstanceMeshInfo* pInstanceInfo);

	void Render();
	
	void SetPos(DirectX::XMFLOAT3* pos){};
};
#endif // !__DX_INSTANCE_MESH_H_
