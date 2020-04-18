#ifndef __DX_BUFFER_H_
#define __DX_BUFFER_H_



//****************************************************************************************************
//
//	DX_Buffer
//
//****************************************************************************************************
class DX_Buffer
{
public:
	
	//------------------------------------------------------------------------------
	//
	//  @brief		�萔�o�b�t�@���쐬����
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	bufferSize		�o�b�t�@�T�C�Y
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer* CreateConstantBuffer(ID3D11Device*	pDevice, UINT bufferSize);

	//------------------------------------------------------------------------------
	//
	//  @brief		���_�o�b�t�@���쐬����
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	vertexDataSize	���_�f�[�^�̃T�C�Y
	//	@param[in]	pVertex			���_���
	//	@return		�쐬�������_�o�b�t�@
	//	@note		�쐬�Ɏ��s�����ꍇ�Anullptr���Ԃ�
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer* CreateVertexBuffer(ID3D11Device* pDevice, const size_t	vertexDataSize, const void* pVertex);

	//------------------------------------------------------------------------------
	//
	//  @brief		�\�����o�b�t�@���쐬����
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	elementSize		�z��̗v�f�̃f�[�^�T�C�Y
	//	@param[in]	elementCount	�z��̗v�f��
	//	@param[in]	pData			�f�[�^
	//	@return		�쐬�����\�����o�b�t�@
	//	@note		�쐬�Ɏ��s�����ꍇ�Anullptr���Ԃ�
	//				pData�̓o�b�t�@�Ƀf�[�^���������݂������Ɏg��
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer* CreateStructuredBuffer(ID3D11Device* pDevice, const size_t	elementSize, const size_t elementCount, void* pData);

	//------------------------------------------------------------------------------
	//
	//  @brief		�o�C�g�A�h���X�o�b�t�@���쐬����
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	dataSize		�z��̃f�[�^�T�C�Y
	//	@param[in]	pData			�f�[�^
	//	@param[in]	isVertex		���_�o�b�t�@�ł��g�������ꍇ��true�ɂ���
	//	@return		�쐬�����o�C�g�A�h���X�o�b�t�@
	//	@note		�쐬�Ɏ��s�����ꍇ�Anullptr���Ԃ�
	//				pData�̓o�b�t�@�Ƀf�[�^���������݂������Ɏg��
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer* CreateByteAddressBuffer(ID3D11Device* pDevice, const size_t dataSize, void* pData, const bool isVertex = false);

	//------------------------------------------------------------------------------
	//
	//  @brief		�C���f�b�N�X�o�b�t�@���쐬����
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	indexDataSize	�C���f�b�N�X�f�[�^�̃T�C�Y
	//	@param[in]	pIndex			�C���f�b�N�X���
	//	@return		�쐬�������_�o�b�t�@
	//	@note		�쐬�Ɏ��s�����ꍇ�Anullptr���Ԃ�
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer* CreateIndexBuffer(ID3D11Device* pDevice, const size_t indexDataSize, const void* pIndex);

	//------------------------------------------------------------------------------
	//
	//  @brief		�X�L���o�b�t�@���쐬����
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	indexDataSize	�C���f�b�N�X�f�[�^�̃T�C�Y
	//	@param[in]	pIndex			�C���f�b�N�X���
	//	@return		�쐬�������_�o�b�t�@
	//	@note		�쐬�Ɏ��s�����ꍇ�Anullptr���Ԃ�
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer*	CreateSkinBuffer(ID3D11Device* pDevice, const size_t skinDatasize, const void* pIndex);

	//------------------------------------------------------------------------------
	//
	//  @brief		�W�I���g���X�g���[���o�͗p�o�b�t�@���쐬����
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	bufferDateSize	�W�I���g������o�͂���f�[�^�T�C�Y
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer*	CreateStreamOutputBuffer(ID3D11Device* pDevice, const size_t bufferDateSize);

	//------------------------------------------------------------------------------
	//
	//  @brief		CPU�ŏ������݉\�ȃo�b�t�@���쐬����
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	bufferDateSize	�������ރf�[�^�T�C�Y
	//	@note		CPU�œǂݍ��ނ��Ƃ͕s�\
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer*	CPUWriteBuffer(ID3D11Device* pDevice, const size_t bufferDateSize);

	//------------------------------------------------------------------------------
	//
	//  @brief		CPU�œǂݍ��݉\�ȃo�b�t�@���쐬����
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	bufferDateSize	�ǂݍ��ރf�[�^�T�C�Y
	//	@note		CPU�œǂݍ��ނ��Ƃ͕s�\
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer*	CPUReadBuffer(ID3D11Device* pDevice, const size_t bufferDateSize, const size_t strctureByteStride = 0);

private:

	//------------------------------------------------------------------------------
	//
	//  @brief		�o�b�t�@���쐬����
	//	@param[in]	pDevice			DX�f�o�C�X
	//	@param[in]	pDesc			�쐬����o�b�t�@�ݒ�
	//	@param[in]	pSubResource	�쐬����o�b�t�@�ɐݒ肷��f�[�^
	//	@return		�o�b�t�@�̃|�C���^�@�쐬�Ɏ��s�����ꍇ��nullptr
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer* CreateBuffer(ID3D11Device* pDevice, D3D11_BUFFER_DESC* pDesc, D3D11_SUBRESOURCE_DATA* pSubResource = nullptr);
};
#endif // !__DX_BUFFER_H_
