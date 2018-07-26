#ifndef __DX_BUFFER_H_
#define __DX_BUFFER_H_



//****************************************************************************************************
//
//	DX_Buffer
//
//****************************************************************************************************
class DX_Buffer
{
	DX_Buffer() = delete;
public:
	
	//------------------------------------------------------------------------------
	//
	//  @brief		�萔�o�b�t�@���쐬����
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	bufferSize		�o�b�t�@�T�C�Y
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer* CreateConstantBuffer(
		ID3D11Device*	pDevice,
		const size_t	bufferSize
		);

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
	static ID3D11Buffer* CreateVertexBuffer(
		ID3D11Device*	pDevice,
		const size_t	vertexDataSize, 
		const void*		pVertex
		);

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
	static ID3D11Buffer* CreateStructuredBuffer(
		ID3D11Device*	pDevice,
		const size_t	elementSize,
		const size_t	elementCount,
		void*	pData
		);

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
	static ID3D11Buffer* CreateByteAddressBuffer(
		ID3D11Device*	pDevice,
		const size_t	dataSize,
		void*			pData,
		const bool		isVertex = false
		);

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
	static ID3D11Buffer* CreateIndexBuffer(
		ID3D11Device*	pDevice,
		const size_t	indexDataSize,
		const void*		pIndex
		);

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
	static ID3D11Buffer*	CreateSkinBuffer(
		ID3D11Device*	pDevice,
		const size_t	skinDatasize,
		const void*		pIndex
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		�W�I���g���X�g���[���o�͗p�o�b�t�@���쐬����
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	bufferDateSize	�W�I���g������o�͂���f�[�^�T�C�Y
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer*	CreateStreamOutputBuffer(
		ID3D11Device*	pDevice,
		const size_t	bufferDateSize
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		CPU�ŏ������݉\�ȃo�b�t�@���쐬����
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	bufferDateSize	�������ރf�[�^�T�C�Y
	//	@note		CPU�œǂݍ��ނ��Ƃ͕s�\
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer*	CPUWriteBuffer(
		ID3D11Device*	pDevice,
		const size_t	bufferDateSize
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		CPU�œǂݍ��݉\�ȃo�b�t�@���쐬����
	//	@param[in]	pDevice			DX_System::GetDevice()
	//	@param[in]	bufferDateSize	�ǂݍ��ރf�[�^�T�C�Y
	//	@note		CPU�œǂݍ��ނ��Ƃ͕s�\
	//
	//------------------------------------------------------------------------------
	static ID3D11Buffer*	CPUReadBuffer(
		ID3D11Device*		pDevice,
		const size_t		bufferDateSize,
		const size_t		strctureByteStride = 0
		);

	//------------------------------------------------------------------------------
	//
	//  @brief		2D��`�悷��
	//	@param[in]	pVertexBuffer		�쐬�������_�o�b�t�@
	//	@param[in]	pShaderRerousceView	�쐬����SRV
	//	@note		Render2D���ĂԑO�ɒ��_�V�F�[�_�[�ƃs�N�Z���V�F�[�_��
	//				�ݒ肷�邱��(DX_Shader::Begin())
	//				�Ă񂾌�̓V�F�[�_�[�̏I�����I���鎖(DX_Shader::End())
	//				�T���v���[��DX���������ɐݒ肵�Ă���̂ŁA
	//				�Ǝ��ɐݒ���ł��邪�A�`���Ƀf�t�H���g�ɖ߂���
	//
	//------------------------------------------------------------------------------
	static void Render2D(
		ID3D11Buffer*				pVertexBuffer,
		ID3D11ShaderResourceView*	pShaderRerousceView
		);

	
};
#endif // !__DX_BUFFER_H_