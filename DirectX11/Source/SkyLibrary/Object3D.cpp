#include "SkyLibrary.h"

Object3D::Object3D() :
	m_pFbx(nullptr),
	m_pos(DirectX::XMFLOAT3(0.0f,0.0f,0.0f)),
	m_scale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)),
	m_angle(DirectX::XMFLOAT3(0.0f,0.0f,0.0f)),
	m_bChanged(true)
{}

Object3D::~Object3D()
{
	DELETE_OBJ(m_pFbx);
}

bool Object3D::Initialize(const char* pFilepath)
{
	m_pFbx = new FBXLoader();

	char path[MAX_PATH] = { '\n' };
	sprintf_s(path, "%s%s", "Resource\\", pFilepath);

	bool isReadObj = m_pFbx->ReadObj(path);
	
	m_wolrdMatrixBuffer = DX_BufferCreater::ConstantBuffer(sizeof(m_worldMatrix));

	m_pos.z = 10.0f;
	m_pos.y = 12.0f;

	Update();

	return isReadObj;
}

void Object3D::Update()
{
	//	Set関数で座標等を変更された場合のみ、行列計算を行う
	if (m_bChanged){

		//	拡大行列 * 回転行列 * 平行行列
		DirectX::XMMATRIX scale			= DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		DirectX::XMMATRIX angle			= DirectX::XMMatrixRotationRollPitchYaw(m_angle.x, m_angle.y, m_angle.z);
		DirectX::XMMATRIX translation	= DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
		
		DirectX::XMStoreFloat4x4(&m_worldMatrix, angle * scale * translation);

		m_bChanged = false;
	}
}
bool Object3D::Render()
{
	DX_System*				system			= DX_System::GetInstance();
	ID3D11DeviceContext*	deviceContext	= system->GetDeviceContext();

	DX_ShaderManager*	shaderManager	= DX_ShaderManager::GetInstance();
	DX_Shader*	vertexShader			= shaderManager->GetShader(SHADER_OBJECT_3D::VERTEX_SHADER);
	DX_Shader*	pixelShader				= shaderManager->GetShader(SHADER_OBJECT_3D::PIXEL_SHADER);
	ID3D11InputLayout*	inputLayout		= shaderManager->GetInputLayout3D();

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	vertexShader->Begin();
	pixelShader->Begin();

	FBXLoader::tagMeshData* meshList	= m_pFbx->GetMeshData();
	FBXLoader::tagMeshData* mesh		= nullptr;
	UINT strides	= sizeof(tagVertexInfo);
	UINT offset		= 0;

	auto	materialLinks = m_pFbx->GetMaterialLinks();
	int	meshCount = m_pFbx->GetMeshCount();
	
	for (int i = 0; i < meshCount; ++i)
	{
		mesh = &meshList[i];

		deviceContext->IASetInputLayout(inputLayout);
		deviceContext->IASetVertexBuffers(0, 1, &mesh->pVertexBuffer, &strides, &offset);
		deviceContext->IASetIndexBuffer(mesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// テクスチャの有無
		if (materialLinks.count(mesh->pMaterialName))
		{
			shaderManager->SetShaderResources(0, 1, &materialLinks[mesh->pMaterialName], SHADER_TYPE::PIXEL_SHADER);
		}
		else
		{
			shaderManager->SetShaderResources(0, 1, nullptr, SHADER_TYPE::PIXEL_SHADER);
		}

		shaderManager->SetWorldMat(m_worldMatrix, SHADER_TYPE::VERTEX_SHADER);
		deviceContext->DrawIndexed(mesh->indexCount, 0, 0);
	}

	vertexShader->End();
	pixelShader->End();

	return true;
}

DirectX::XMFLOAT3 Object3D::GetPos()
{
	return m_pos;
}
DirectX::XMFLOAT3 Object3D::GetAngle()
{
	return m_angle;
}
void Object3D::SetPos(const DirectX::XMFLOAT3& pos)
{
	m_bChanged = true;
	m_pos = pos;
}
void Object3D::SetAngle(const DirectX::XMFLOAT3& angle)
{
	m_bChanged = true;
	m_angle = angle;
}