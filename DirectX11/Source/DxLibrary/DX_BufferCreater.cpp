#include	"DX_Library.h"

/// <summary>
/// �萔�o�b�t�@���쐬����
/// </summary>
/// <param name="bufferSize">�o�b�t�@�T�C�Y</param>
/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
ID3D11Buffer* DX_BufferCreater::ConstantBuffer(const UINT bufferSize)
{
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= bufferSize;
	desc.Usage				= D3D11_USAGE_DEFAULT;
	desc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;

	return Buffer(&desc);
}

/// <summary>
/// ���_�o�b�t�@���쐬����(�X�L���o�b�t�@������ō쐬���܂�)
/// </summary>
/// <param name="vertexDataSize">���_�f�[�^�̃T�C�Y</param>
/// <param name="vertices">���_���</param>
/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
ID3D11Buffer* DX_BufferCreater::VertexBuffer(const UINT vertexDataSize, const void*	vertices)
{
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= vertexDataSize;
	desc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA	subResource = { NULL };
	subResource.pSysMem = vertices;

	return Buffer(&desc, &subResource);
}

/// <summary>
/// �C���f�b�N�X�o�b�t�@���쐬����
/// </summary>
/// <param name="indexDataSize">�C���f�b�N�X�f�[�^�̃T�C�Y</param>
/// <param name="indicies">�C���f�b�N�X�f�[�^</param>
/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
ID3D11Buffer* DX_BufferCreater::IndexBuffer(const UINT indexDataSize, const void* indicies)
{
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= indexDataSize;
	desc.BindFlags			= D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA	subResource = { NULL };
	subResource.pSysMem = indicies;

	return Buffer(&desc, &subResource);
}

/// <summary>
/// �\�����o�b�t�@���쐬����
/// </summary>
/// <param name="elementSize">�z��̗v�f�̃f�[�^�T�C�Y</param>
/// <param name="elementCount">�z��̗v�f��</param>
/// <param name="data">�f�[�^</param>
/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
ID3D11Buffer* DX_BufferCreater::StructuredBuffer(const UINT	elementSize, const UINT elementCount, void* data)
{
	D3D11_BUFFER_DESC desc		= { NULL };
	desc.BindFlags				= D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth				= elementSize * elementCount;
	desc.MiscFlags				= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride	= elementSize;

	D3D11_SUBRESOURCE_DATA subResource = { NULL };
	subResource.pSysMem = data;

	return Buffer(&desc, data ? &subResource : nullptr);
}

/// <summary>
/// �o�C�g�A�h���X�o�b�t�@���쐬����
/// </summary>
/// <param name="dataSize">�z��̃f�[�^�T�C�Y</param>
/// <param name="data">�f�[�^</param>
/// <param name="usedVertex ">���_�o�b�t�@�ł��g�������ꍇ��true�ɂ���</param>
/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
ID3D11Buffer* DX_BufferCreater::ByteAddressBuffer(const UINT dataSize, void* data, const bool usedVertex)
{
	D3D11_BUFFER_DESC desc		= { NULL };
	desc.BindFlags				= usedVertex ? (D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_VERTEX_BUFFER) : (D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	desc.ByteWidth				= dataSize;
	desc.MiscFlags				= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	desc.StructureByteStride	= 0;

	D3D11_SUBRESOURCE_DATA subResource = { NULL };
	subResource.pSysMem = data;

	return Buffer(&desc, data ? &subResource : nullptr);
}

/// <summary>
/// �W�I���g���X�g���[���o�͗p�o�b�t�@���쐬����
/// </summary>
/// <param name="bufferDateSize">�W�I���g������o�͂���f�[�^�T�C�Y</param>
/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
ID3D11Buffer*	DX_BufferCreater::StreamOutputBuffer(const UINT	bufferDateSize)
{
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= bufferDateSize;
	desc.Usage				= D3D11_USAGE_DEFAULT;
	desc.BindFlags			= D3D11_BIND_STREAM_OUTPUT;

	return Buffer(&desc);
}


/// <summary>
/// CPU�ŏ������݉\�ȃo�b�t�@���쐬����
/// </summary>
/// <param name="bufferDateSize">�������ރf�[�^�T�C�Y</param>
/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
ID3D11Buffer*	DX_BufferCreater::CPUWriteBuffer(const size_t bufferDateSize)
{
	D3D11_BUFFER_DESC desc	= { NULL };
	desc.ByteWidth			= static_cast<UINT>(bufferDateSize);
	desc.Usage				= D3D11_USAGE_DYNAMIC;
	desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;

	return Buffer(&desc);
}

/// <summary>
/// CPU�œǂݍ��݉\�ȃo�b�t�@���쐬����
/// </summary>
/// <param name="bufferDateSize">�ǂݍ��ރf�[�^�T�C�Y</param>
/// <param name="strctureByteStride">�\���̂̃o�C�g�T�C�Y</param>
/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
ID3D11Buffer*	DX_BufferCreater::CPUReadBuffer(const UINT bufferDateSize, const UINT strctureByteStride)
{	
	D3D11_BUFFER_DESC desc		= { NULL };
	desc.ByteWidth				= bufferDateSize;
	desc.Usage					= D3D11_USAGE_STAGING;
	desc.BindFlags				= 0;
	desc.CPUAccessFlags			= D3D11_CPU_ACCESS_READ;
	desc.StructureByteStride	= strctureByteStride;

	return Buffer(&desc);
}

/// <summary>
/// �o�b�t�@���쐬����
/// </summary>
/// <param name="desc">�쐬����o�b�t�@�̐ݒ�</param>
/// <param name="subResource">�쐬����o�b�t�@�ɐݒ肷��f�[�^</param>
/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
ID3D11Buffer* DX_BufferCreater::Buffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* subResource)
{
	ID3D11Device* device = DX_System::GetInstance()->GetDevice();
	ID3D11Buffer* buffer = nullptr;

	if (DX_Debug::GetInstance()->IsFailedHresult(device->CreateBuffer(desc, subResource, &buffer))) {
		buffer = nullptr;
	}
	
	return buffer;
}