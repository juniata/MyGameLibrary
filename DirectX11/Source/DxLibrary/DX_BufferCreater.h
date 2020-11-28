#ifndef __DX_BUFFER_CREATER_H_
#define __DX_BUFFER_CREATER_H_


/// <summary>
/// �o�b�t�@�̍쐬���s��
/// </summary>
class DX_BufferCreater
{
public:
	
	/// <summary>
	/// �萔�o�b�t�@���쐬����
	/// </summary>
	/// <param name="bufferSize">�o�b�t�@�T�C�Y</param>
	/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
	static ID3D11Buffer* ConstantBuffer(const UINT bufferSize);

	/// <summary>
	/// ���_�o�b�t�@���쐬����(�X�L���o�b�t�@������ō쐬���܂�)
	/// </summary>
	/// <param name="vertexDataSize">���_�f�[�^�̃T�C�Y</param>
	/// <param name="vertices">���_���</param>
	/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
	static ID3D11Buffer* VertexBuffer(const UINT vertexDataSize, const void* vertices);

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@���쐬����
	/// </summary>
	/// <param name="indexDataSize">�C���f�b�N�X�f�[�^�̃T�C�Y</param>
	/// <param name="indicies">�C���f�b�N�X�f�[�^</param>
	/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
	static ID3D11Buffer* IndexBuffer(const UINT indexDataSize, const void* indicies);

	/// <summary>
	/// �\�����o�b�t�@���쐬����
	/// </summary>
	/// <param name="elementSize">�z��̗v�f�̃f�[�^�T�C�Y</param>
	/// <param name="elementCount">�z��̗v�f��</param>
	/// <param name="data">�f�[�^</param>
	/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
	static ID3D11Buffer* StructuredBuffer(const UINT elementSize, const UINT elementCount, void* data);

	/// <summary>
	/// �o�C�g�A�h���X�o�b�t�@���쐬����
	/// </summary>
	/// <param name="dataSize">�z��̃f�[�^�T�C�Y</param>
	/// <param name="data">�f�[�^</param>
	/// <param name="usedVertex ">���_�o�b�t�@�ł��g�������ꍇ��true�ɂ���</param>
	/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
	static ID3D11Buffer* ByteAddressBuffer(const UINT dataSize, void* data, const bool usedVertex = false);

	/// <summary>
	/// �W�I���g���X�g���[���o�͗p�o�b�t�@���쐬����
	/// </summary>
	/// <param name="bufferDateSize">�W�I���g������o�͂���f�[�^�T�C�Y</param>
	/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
	static ID3D11Buffer* StreamOutputBuffer(const UINT bufferDateSize);

	/// <summary>
	/// CPU�ŏ������݉\�ȃo�b�t�@���쐬����
	/// </summary>
	/// <param name="bufferDateSize">�������ރf�[�^�T�C�Y</param>
	/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
	static ID3D11Buffer* CPUWriteBuffer(const UINT bufferDateSize);

	/// <summary>
	/// CPU�œǂݍ��݉\�ȃo�b�t�@���쐬����
	/// </summary>
	/// <param name="bufferDateSize">�ǂݍ��ރf�[�^�T�C�Y</param>
	/// <param name="strctureByteStride">�\���̂̃o�C�g�T�C�Y</param>
	/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
	static ID3D11Buffer* CPUReadBuffer(const UINT bufferDateSize, const UINT strctureByteStride = 0);

private:
	DX_BufferCreater(){}
	~DX_BufferCreater(){}

	/// <summary>
	/// �o�b�t�@���쐬����
	/// </summary>
	/// <param name="desc">�쐬����o�b�t�@�̐ݒ�</param>
	/// <param name="subResource">�쐬����o�b�t�@�ɐݒ肷��f�[�^</param>
	/// <returns>�o�b�t�@(���s�����Ȃ�nullptr)</returns>
	static ID3D11Buffer* Buffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* subResource = nullptr);
};
#endif // !__DX_BUFFER_H_
