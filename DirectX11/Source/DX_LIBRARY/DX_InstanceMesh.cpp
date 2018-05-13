#include	"DX_Library.h"
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//  メンバ変数を初期化
//
//-----------------------------------------------------------------------------------------
DX_InstanceMesh::DX_InstanceMesh() : m_instanceCount(0)
{
	ZeroMemory(&m_resultResource, sizeof(m_resultResource));
}

//-----------------------------------------------------------------------------------------
//
//  メンバ変数を初期化、及び生成
//
//-----------------------------------------------------------------------------------------
DX_InstanceMesh::DX_InstanceMesh(const char* pFilepath, const int instanceCount) : DX_InstanceMesh()
{
	auto l_device = DX_System::GetInstance()->GetDevice();

	m_instanceCount = instanceCount;
	
	m_readBuffer = DX_Buffer::CPUReadBuffer(l_device, sizeof(XMFLOAT4X4) * instanceCount);

	//	UAVBufferを作成
	m_resultResource.m_uavBuffer = DX_Buffer::CreateByteAddressBuffer(l_device, sizeof(XMFLOAT4X4)* m_instanceCount, nullptr,true);

	//	UAVを作成
	m_resultResource.m_uav = DX_Resource::CreateUnorderedAccessView(l_device, m_resultResource.m_uavBuffer.Get());

	//	メッシュを作成
	LoadModel(pFilepath);

}

//-----------------------------------------------------------------------------------------
//
//  全データ解放
//
//-----------------------------------------------------------------------------------------
DX_InstanceMesh::~DX_InstanceMesh()
{
}

//-----------------------------------------------------------------------------------------
//
//  行列計算処理
//
//-----------------------------------------------------------------------------------------
void DX_InstanceMesh::Update(tagInstanceMeshInfo* pInstanceInfo)
{
	//	デバイス、コンテキスト、コンピュートシェーダーを取得
	auto		l_device			= DX_System::GetInstance()->GetDevice();
	auto		l_deviceContext		= DX_System::GetInstance()->GetDeviceContext();
	DX_Shader*	l_pComputeShader	= DX_ShaderManager::GetInstanceMeshComputeShader();

	//	StrcutredBufferを作成
	ComPtr<ID3D11Buffer> l_buffer	= DX_Buffer::CreateStructuredBuffer(l_device, sizeof(tagInstanceMeshInfo), m_instanceCount, pInstanceInfo);

	//	SRVを作成
	ComPtr<ID3D11ShaderResourceView> l_srv	= DX_Resource::CreateShaderResourceView(l_device, l_buffer.Get());
	ID3D11UnorderedAccessView* l_pNull = nullptr;

	//	CSにSRVとUAVを設定し、利用を開始する
	DX_ResourceManager::SetShaderResources(l_deviceContext, 0, 1, &l_srv, DX_SHADER_TYPE::COMPUTE_SHADER);
	l_deviceContext->CSSetUnorderedAccessViews(0, 1, &m_resultResource.m_uav, nullptr);
	l_pComputeShader->Begin(l_deviceContext);
	
	//	計算を行う
	l_deviceContext->Dispatch(m_instanceCount / 8 + 1, 1, 1);

	//	CSを終了する
	l_pComputeShader->End(l_deviceContext);

	l_deviceContext->CSSetUnorderedAccessViews(0, 1, &l_pNull, nullptr);
}


void DX_InstanceMesh::Render()
{
	//	デバイスコンテキストを取得
	auto	l_deviceContext = DX_System::GetInstance()->GetDeviceContext();
	
	//	インスタンスの数を送る
	DX_ShaderManager::SetVector(2, XMFLOAT4(CAST_F(m_instanceCount), 0, 0, 0), DX_System::GetInstance()->GetDevice(), l_deviceContext, DX_SHADER_TYPE::VERTEX_SHADER);

	DX_Shader* l_pVertexShader		= DX_ShaderManager::GetShader("ShaderFile\\VS_InstanceMesh.hlsl");
	DX_Shader* l_pGeometryShader	= DX_ShaderManager::GetDefaultGeometryShader3D();
	DX_Shader* l_pPixelShader		= DX_ShaderManager::GetDefaultPixelShader3D();

	//	buffer stride and offset
	//unsigned int l_stride = sizeof(tagMeshVertex);
	unsigned int l_stride[] = {
		sizeof(tagMeshVertex),
		sizeof(XMFLOAT4X4)
	};
	unsigned int l_offset = 0;
	
	//	VertexBufferを送る
	l_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &l_stride[0], &l_offset);
	
	//	行列を送る
	l_deviceContext->IASetVertexBuffers(1, 1, &m_resultResource.m_uavBuffer, &l_stride[1], &l_offset);

	//	IndexBufferを送る
	l_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//	InputLayoutの設定を送る
	l_deviceContext->IASetInputLayout(DX_ShaderManager::GetInputLayoutInstanceMesh());

	//	Primitiveの設定を送る
	l_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Viewに関するデータを送る
	DX_View::SetMatrixForTheView();

	//	シェーダー利用を開始
	l_pVertexShader->Begin(l_deviceContext);
	l_pGeometryShader->Begin(l_deviceContext);
	l_pPixelShader->Begin(l_deviceContext);

	//	マテリアルグループ描画
	tagGroupMesh* l_pGroupMesh = nullptr;

	//	テクスチャ情報を纏めて送る
	ID3D11ShaderResourceView*	l_pSRV[4];

	//	マテリアル順に描画
	for (DWORD i = 0; i < m_materialCount; ++i){

		//	アドレスを取得
		l_pGroupMesh = &m_pGroupMesh[i];

		//	テクスチャを設定
		l_pSRV[0] = l_pGroupMesh->pTextureDecal;
		l_pSRV[1] = l_pGroupMesh->pTextureNormal;
		l_pSRV[2] = l_pGroupMesh->pTextureSpecular;
		l_pSRV[3] = l_pGroupMesh->pTextureHeight;

		//	register(t0)に送る
		DX_ResourceManager::SetShaderResources(l_deviceContext, 0, 4, l_pSRV, DX_SHADER_TYPE::PIXEL_SHADER);

		//	インデックス描画
		l_deviceContext->DrawIndexedInstanced(
			l_pGroupMesh->indexCount,
			m_instanceCount,
			l_pGroupMesh->startIndexLocation,
			l_pGroupMesh->baseVertexLocation,
			0
			);

	}

	//	シェーダー利用を終了
	l_pVertexShader->End(l_deviceContext);
	l_pGeometryShader->End(l_deviceContext);
	l_pPixelShader->End(l_deviceContext);

	ID3D11Buffer* l_pNull = nullptr;
	l_deviceContext->IASetVertexBuffers(1, 1, &l_pNull, &l_stride[1], &l_offset);

}