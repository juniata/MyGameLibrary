#include	"DX_Library.h"


//-----------------------------------------------------------------------------------------
//
//  メンバ変数を初期化
//
//-----------------------------------------------------------------------------------------
DX_InstanceSkinMesh::DX_InstanceSkinMesh() :
m_pReadBuffer(nullptr),
m_pGPUWriteBuffer(nullptr),
m_instanceCount(0)
{
	ZeroMemory(&m_resultResource, sizeof(m_resultResource));
}

//-----------------------------------------------------------------------------------------
//
//  メンバ変数を初期化、及び生成
//
//-----------------------------------------------------------------------------------------
DX_InstanceSkinMesh::DX_InstanceSkinMesh(const char* pFilepath, const int instanceCount) : DX_InstanceSkinMesh()
{
	ID3D11Device* l_pDevice = DX_System::GetInstance()->GetDevice();

	m_instanceCount = instanceCount;

	m_pReadBuffer = DX_Buffer::CPUReadBuffer(l_pDevice, sizeof(DirectX::XMFLOAT4X4)* instanceCount);

	//	UAVBufferを作成
	m_resultResource.m_pUAVBuffer = DX_Buffer::CreateStructuredBuffer(l_pDevice, sizeof(DirectX::XMFLOAT4X4), m_instanceCount, nullptr);

	//	UAVを作成
	m_resultResource.m_pUAV = DX_Resource::CreateUnorderedAccessView(l_pDevice, m_resultResource.m_pUAVBuffer);

	//	メッシュを作成
	DX_SkinMesh::LoadModel(pFilepath);
}

DX_InstanceSkinMesh::~DX_InstanceSkinMesh()
{
	SAFE_RELEASE(m_resultResource.m_pUAV);
	SAFE_RELEASE(m_resultResource.m_pUAVBuffer);
	SAFE_RELEASE(m_pReadBuffer);
}
void DX_InstanceSkinMesh::Update()
{

}
void DX_InstanceSkinMesh::Animation()
{

}
void DX_InstanceSkinMesh::Render()
{

}