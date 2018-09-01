#include	"DX_Library.h"

//-----------------------------------------------------------------------------------------
//
//  �萔�o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateConstantBuffer(ID3D11Device*	pDevice, UINT bufferSize)
{
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= bufferSize;
	desc.Usage				= D3D11_USAGE_DEFAULT;
	desc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;

	return CreateBuffer(pDevice, &desc);
}

//-----------------------------------------------------------------------------------------
//
//  ���_�o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateVertexBuffer(ID3D11Device* pDevice, const size_t vertexDataSize, const void*	pVertex)
{
	//	buffer�̐ݒ�
	D3D11_BUFFER_DESC		desc = { NULL };
	desc.ByteWidth		= static_cast<UINT>(vertexDataSize);
	desc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;

	//	subResourceData�̐ݒ�
	D3D11_SUBRESOURCE_DATA	subResource = { NULL };
	subResource.pSysMem = pVertex;

	return CreateBuffer(pDevice, &desc, &subResource);
}


//-----------------------------------------------------------------------------------------
//
//  �\�����o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateStructuredBuffer(ID3D11Device* pDevice, const size_t	elementSize, const size_t elementCount, void* pData)
{
	D3D11_BUFFER_DESC desc		= { NULL };
	desc.BindFlags				= D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth				= static_cast<UINT>(elementSize * elementCount);
	desc.MiscFlags				= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride	= static_cast<UINT>(elementSize);

	D3D11_SUBRESOURCE_DATA subResource = { NULL };
	subResource.pSysMem = pData;

	return CreateBuffer(pDevice, &desc, pData ? &subResource : nullptr);
}

//-----------------------------------------------------------------------------------------
//
//  �o�C�g�A�h���X�o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateByteAddressBuffer(ID3D11Device* pDevice, const size_t dataSize, void* pData, const bool isVertex)
{
	D3D11_BUFFER_DESC desc		= { NULL };
	desc.BindFlags				= isVertex ? (D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_VERTEX_BUFFER) : (D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	desc.ByteWidth				= static_cast<UINT>(dataSize);
	desc.MiscFlags				= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	desc.StructureByteStride	= 0;

	D3D11_SUBRESOURCE_DATA subResource = { NULL };
	subResource.pSysMem = pData;

	return CreateBuffer(pDevice, &desc, pData ? &subResource : nullptr);
}

//-----------------------------------------------------------------------------------------
//
//  �C���f�b�N�X�o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateIndexBuffer(ID3D11Device* pDevice, const size_t indexDataSize, const void* pIndex)
{
	//	buffer�̐ݒ�
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= static_cast<UINT>(indexDataSize);
	desc.BindFlags			= D3D11_BIND_INDEX_BUFFER;

	//	subResourceData�̐ݒ�
	D3D11_SUBRESOURCE_DATA	subResource = { NULL };
	subResource.pSysMem = pIndex;

	return CreateBuffer(pDevice, &desc, &subResource);
}

//-----------------------------------------------------------------------------------------
//
//  �X�L���o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer*	DX_Buffer::CreateSkinBuffer(ID3D11Device* pDevice, const size_t skinDatasize, const void* pIndex)
{
	//	buffer�̐ݒ�
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= static_cast<UINT>(skinDatasize);
	desc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;

	//	subResourceData�̐ݒ�
	D3D11_SUBRESOURCE_DATA	subResource = { NULL };
	subResource.pSysMem = pIndex;

	return CreateBuffer(pDevice, &desc, &subResource);
}

//-----------------------------------------------------------------------------------------
//
//  �W�I���g���X�g���[���o�͗p�o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer*	DX_Buffer::CreateStreamOutputBuffer(ID3D11Device* pDevice, const size_t	bufferDateSize)
{
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= static_cast<UINT>(bufferDateSize);
	desc.Usage				= D3D11_USAGE_DEFAULT;
	desc.BindFlags			= D3D11_BIND_STREAM_OUTPUT;

	return CreateBuffer(pDevice, &desc);
}


//-----------------------------------------------------------------------------------------
//
//  CPU�ŏ������݉\�ȃo�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer*	DX_Buffer::CPUWriteBuffer(ID3D11Device*	pDevice, const size_t bufferDateSize)
{
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= static_cast<UINT>(bufferDateSize);
	desc.Usage				= D3D11_USAGE_DYNAMIC;
	desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;

	return CreateBuffer(pDevice, &desc);
}


//-----------------------------------------------------------------------------------------
//
//  CPU�œǂݍ��݉\�ȃo�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer*	DX_Buffer::CPUReadBuffer(ID3D11Device* pDevice, const size_t bufferDateSize, const size_t strctureByteStride)
{	
	D3D11_BUFFER_DESC desc		= { NULL };
	desc.ByteWidth				= static_cast<UINT>(bufferDateSize);
	desc.Usage					= D3D11_USAGE_STAGING;
	desc.BindFlags				= 0;
	desc.CPUAccessFlags			= D3D11_CPU_ACCESS_READ;
	desc.StructureByteStride	= static_cast<UINT>(strctureByteStride);

	return CreateBuffer(pDevice, &desc);
}
//-----------------------------------------------------------------------------------------
//
//  2D��`��
//
//-----------------------------------------------------------------------------------------
bool DX_Buffer::Render2D(DX_ShaderManager* pShaderManager, ID3D11DeviceContext* pContext, ID3D11Buffer* pVertexBuffer, ID3D11ShaderResourceView* pShaderRerousceView)
{
	bool result = false;

	unsigned int l_stride = sizeof(tagVertex2D);
	unsigned int l_offset = 0;

	//	VertexBuffer�𑗂�
	pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &l_stride, &l_offset);

	//	InputLayout�̐ݒ�𑗂�
	pContext->IASetInputLayout(pShaderManager->GetDefaultInputLayout2D());

	//	Primitive�̐ݒ�𑗂�
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//	PixelShader�Ƀe�N�X�`�����𑗂�
	result = DX_ResourceManager::SetShaderResources(pContext, 0, 1, &pShaderRerousceView,DX_SHADER_TYPE::PIXEL_SHADER);

	//	�`��
	pContext->Draw(4, 0);

	return result;
}

//-----------------------------------------------------------------------------------------
//
//  �o�b�t�@���쐬����
//
//-----------------------------------------------------------------------------------------
ID3D11Buffer* DX_Buffer::CreateBuffer(ID3D11Device* pDevice, D3D11_BUFFER_DESC* pDesc, D3D11_SUBRESOURCE_DATA* pSubResource)
{
	ID3D11Buffer* pBuffer = nullptr;

	//	buffer���쐬����
	if (!DX_Debug::GetInstance()->IsHresultCheck(pDevice->CreateBuffer(pDesc, pSubResource, &pBuffer))) {
		SAFE_RELEASE(pBuffer);
	}
	
	return pBuffer;
}