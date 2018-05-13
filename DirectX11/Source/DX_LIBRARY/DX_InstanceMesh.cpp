#include	"DX_Library.h"
using namespace DirectX;

//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ���������
//
//-----------------------------------------------------------------------------------------
DX_InstanceMesh::DX_InstanceMesh() : m_instanceCount(0)
{
	ZeroMemory(&m_resultResource, sizeof(m_resultResource));
}

//-----------------------------------------------------------------------------------------
//
//  �����o�ϐ����������A�y�ѐ���
//
//-----------------------------------------------------------------------------------------
DX_InstanceMesh::DX_InstanceMesh(const char* pFilepath, const int instanceCount) : DX_InstanceMesh()
{
	auto l_device = DX_System::GetInstance()->GetDevice();

	m_instanceCount = instanceCount;
	
	m_readBuffer = DX_Buffer::CPUReadBuffer(l_device, sizeof(XMFLOAT4X4) * instanceCount);

	//	UAVBuffer���쐬
	m_resultResource.m_uavBuffer = DX_Buffer::CreateByteAddressBuffer(l_device, sizeof(XMFLOAT4X4)* m_instanceCount, nullptr,true);

	//	UAV���쐬
	m_resultResource.m_uav = DX_Resource::CreateUnorderedAccessView(l_device, m_resultResource.m_uavBuffer.Get());

	//	���b�V�����쐬
	LoadModel(pFilepath);

}

//-----------------------------------------------------------------------------------------
//
//  �S�f�[�^���
//
//-----------------------------------------------------------------------------------------
DX_InstanceMesh::~DX_InstanceMesh()
{
}

//-----------------------------------------------------------------------------------------
//
//  �s��v�Z����
//
//-----------------------------------------------------------------------------------------
void DX_InstanceMesh::Update(tagInstanceMeshInfo* pInstanceInfo)
{
	//	�f�o�C�X�A�R���e�L�X�g�A�R���s���[�g�V�F�[�_�[���擾
	auto		l_device			= DX_System::GetInstance()->GetDevice();
	auto		l_deviceContext		= DX_System::GetInstance()->GetDeviceContext();
	DX_Shader*	l_pComputeShader	= DX_ShaderManager::GetInstanceMeshComputeShader();

	//	StrcutredBuffer���쐬
	ComPtr<ID3D11Buffer> l_buffer	= DX_Buffer::CreateStructuredBuffer(l_device, sizeof(tagInstanceMeshInfo), m_instanceCount, pInstanceInfo);

	//	SRV���쐬
	ComPtr<ID3D11ShaderResourceView> l_srv	= DX_Resource::CreateShaderResourceView(l_device, l_buffer.Get());
	ID3D11UnorderedAccessView* l_pNull = nullptr;

	//	CS��SRV��UAV��ݒ肵�A���p���J�n����
	DX_ResourceManager::SetShaderResources(l_deviceContext, 0, 1, &l_srv, DX_SHADER_TYPE::COMPUTE_SHADER);
	l_deviceContext->CSSetUnorderedAccessViews(0, 1, &m_resultResource.m_uav, nullptr);
	l_pComputeShader->Begin(l_deviceContext);
	
	//	�v�Z���s��
	l_deviceContext->Dispatch(m_instanceCount / 8 + 1, 1, 1);

	//	CS���I������
	l_pComputeShader->End(l_deviceContext);

	l_deviceContext->CSSetUnorderedAccessViews(0, 1, &l_pNull, nullptr);
}


void DX_InstanceMesh::Render()
{
	//	�f�o�C�X�R���e�L�X�g���擾
	auto	l_deviceContext = DX_System::GetInstance()->GetDeviceContext();
	
	//	�C���X�^���X�̐��𑗂�
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
	
	//	VertexBuffer�𑗂�
	l_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &l_stride[0], &l_offset);
	
	//	�s��𑗂�
	l_deviceContext->IASetVertexBuffers(1, 1, &m_resultResource.m_uavBuffer, &l_stride[1], &l_offset);

	//	IndexBuffer�𑗂�
	l_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//	InputLayout�̐ݒ�𑗂�
	l_deviceContext->IASetInputLayout(DX_ShaderManager::GetInputLayoutInstanceMesh());

	//	Primitive�̐ݒ�𑗂�
	l_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//View�Ɋւ���f�[�^�𑗂�
	DX_View::SetMatrixForTheView();

	//	�V�F�[�_�[���p���J�n
	l_pVertexShader->Begin(l_deviceContext);
	l_pGeometryShader->Begin(l_deviceContext);
	l_pPixelShader->Begin(l_deviceContext);

	//	�}�e���A���O���[�v�`��
	tagGroupMesh* l_pGroupMesh = nullptr;

	//	�e�N�X�`������Z�߂đ���
	ID3D11ShaderResourceView*	l_pSRV[4];

	//	�}�e���A�����ɕ`��
	for (DWORD i = 0; i < m_materialCount; ++i){

		//	�A�h���X���擾
		l_pGroupMesh = &m_pGroupMesh[i];

		//	�e�N�X�`����ݒ�
		l_pSRV[0] = l_pGroupMesh->pTextureDecal;
		l_pSRV[1] = l_pGroupMesh->pTextureNormal;
		l_pSRV[2] = l_pGroupMesh->pTextureSpecular;
		l_pSRV[3] = l_pGroupMesh->pTextureHeight;

		//	register(t0)�ɑ���
		DX_ResourceManager::SetShaderResources(l_deviceContext, 0, 4, l_pSRV, DX_SHADER_TYPE::PIXEL_SHADER);

		//	�C���f�b�N�X�`��
		l_deviceContext->DrawIndexedInstanced(
			l_pGroupMesh->indexCount,
			m_instanceCount,
			l_pGroupMesh->startIndexLocation,
			l_pGroupMesh->baseVertexLocation,
			0
			);

	}

	//	�V�F�[�_�[���p���I��
	l_pVertexShader->End(l_deviceContext);
	l_pGeometryShader->End(l_deviceContext);
	l_pPixelShader->End(l_deviceContext);

	ID3D11Buffer* l_pNull = nullptr;
	l_deviceContext->IASetVertexBuffers(1, 1, &l_pNull, &l_stride[1], &l_offset);

}