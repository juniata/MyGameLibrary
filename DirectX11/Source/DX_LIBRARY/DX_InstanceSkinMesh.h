#ifndef __DX_ISNTANCE_SKIN_MESH_H_
#define __DX_ISNTANCE_SKIN_MESH_H_


//****************************************************************************************************
//
//	InstanceSkinMesh
//
//****************************************************************************************************
class InstanceSkinMesh : public dx_library::SkinMesh
{
private:
	//	コンピュートで結果を書き込むリソース
	struct tagResultResource{
		ID3D11UnorderedAccessView*	m_pUAV;
		ID3D11Buffer*				m_pUAVBuffer;
	}m_resultResource;

	//	コンピュートシェーダーの結果をCPUで受け取るバッファ
	ID3D11Buffer*	m_pReadBuffer;

	//	コンピュートシェーダーで書き込む対象バッファ
	ID3D11Buffer*	m_pGPUWriteBuffer;

	//	インスタンス数
	int m_instanceCount;

public:
	InstanceSkinMesh();
	InstanceSkinMesh(const char* pFilepath, const int instanceCount);
	~InstanceSkinMesh();

	void Update();
	void Animation();
	void Render();
};
#endif // !__DX_ISNTANCE_SKIN_MESH_H_
