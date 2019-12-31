//#include	"DX_Library.h"
//#include	<stdio.h>
//
////-----------------------------------------------------------------------------------------
////
////  �R���X�g���N�^
////
////-----------------------------------------------------------------------------------------
//DX_MaptipObject::DX_MaptipObject() :
//	m_pShaderResourceView(nullptr),
//	m_pVertexBuffer(nullptr),
//	m_width(0),
//	m_height(0),
//	m_splitCountX(0),
//	m_splitCountY(0),
//	m_instanceNum(0),
//	m_enabled(true),
//	m_pPosList(nullptr)
//{}
//
////-----------------------------------------------------------------------------------------
////
////  �f�X�g���N�^
////
////-----------------------------------------------------------------------------------------
//DX_MaptipObject::~DX_MaptipObject()
//{
//	DX_TextureManager::GetInstance()->Release(m_pShaderResourceView);
//	DELETE_OBJ_ARRAY(m_pPosList);
//	SAFE_RELEASE(m_pVertexBuffer);
//}
//
//bool DX_MaptipObject::Initialize(const char* pFilepath, const unsigned int splitCountX, const unsigned int splitCountY, const DirectX::XMFLOAT2& renderSize)
//{
//	m_splitCountX = splitCountX;
//	m_splitCountY = splitCountY;
//	m_instanceNum = m_splitCountX * m_splitCountY;
//	m_pPosList = new DirectX::XMFLOAT3[m_instanceNum];
//	ZeroMemory(m_pPosList, sizeof(m_pPosList[0]) * m_instanceNum);
//
//	if (false == LoadTexture(pFilepath)) {
//		return false;
//	}
//
//	//	1 ~ 0�̒l�ɕϊ�
//	const float centerX = 1.0f / (DX::CAST::F(DX_System::GetWindowWidth()) * 0.5f);
//	const float centerY = 1.0f / (DX::CAST::F(DX_System::GetWindowHeight()) * 0.5f);
//
//
//	DX::tagVertex2D pVertex[] = {
//		/* ���� */	DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f),
//		/* ���� */	DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f),
//		/* �E�� */	DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f),
//		/* �E�� */	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)
//	};
//
//	DX::tagRect renderPos(0.0f, 0.0f, renderSize.x, renderSize.y);
//
//	//	���̍��W
//	pVertex[1].pos.x = pVertex[0].pos.x = centerX * renderPos.x - 1.0f;
//
//	//	���̍��W
//	pVertex[2].pos.y = pVertex[0].pos.y = 1.0f - centerY * renderPos.h;
//
//	//	��̍��W
//	pVertex[3].pos.y = pVertex[1].pos.y = 1.0f - centerY * renderPos.y;
//
//	//	�E�̍��W
//	pVertex[3].pos.x = pVertex[2].pos.x = centerX * renderPos.w - 1.0f;
//
//
//	if (nullptr == (m_pVertexBuffer = DX_Buffer::CreateVertexBuffer(DX_System::GetDevice(), sizeof(pVertex[0]) * 4, pVertex))) {
//		return false;
//	}
//
//	return true;
//}
//
////-----------------------------------------------------------------------------------------
////
////  �e�N�X�`����ǂݍ���
////
////-----------------------------------------------------------------------------------------
//bool DX_MaptipObject::LoadTexture(const char* pFilepath)
//{
//	bool result = false;
//
//	char texturePath[MAX_PATH] = { '\n' };
//	sprintf_s(texturePath, "%s%s", "Resource\\2dobject\\", pFilepath);
//
//	//	�e�N�X�`�����擾
//	m_pShaderResourceView = DX_TextureManager::GetInstance()->GetTexture(texturePath);
//
//	//	�e�N�X�`�������[�h�ł��Ă邩�`�F�b�N
//	if (m_pShaderResourceView) {
//		//	�e�N�X�`�����ׂ��ȏ����擾
//		ID3D11Resource* pResource = nullptr;
//		m_pShaderResourceView->GetResource(&pResource);
//
//		ID3D11Texture2D* pTex2D = nullptr;
//		if (FAILED(pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&pTex2D))) {
//			D3D11_TEXTURE2D_DESC texDesc;
//			pTex2D->GetDesc(&texDesc);
//
//			//	�e�N�X�`���T�C�Y���擾
//			m_height = texDesc.Height;
//			m_width = texDesc.Width;
//		}
//		else {
//			result = true;
//		}
//
//		SAFE_RELEASE(pResource);
//		SAFE_RELEASE(pTex2D);
//	}
//
//	return result;
//}
//
//
////-----------------------------------------------------------------------------------------
////
////  ���W�ꗗ���擾����
////
////-----------------------------------------------------------------------------------------
//DirectX::XMFLOAT3* DX_MaptipObject::GetPosList()
//{
//	return m_pPosList;
//}
//
//
////-----------------------------------------------------------------------------------------
////
////  �w�肵���ӏ�����̍��W�ꗗ���擾����
////
////-----------------------------------------------------------------------------------------
//DirectX::XMFLOAT3* DX_MaptipObject::GetPosList(const unsigned int y, const unsigned int x)
//{
//	return &m_pPosList[y * m_splitCountX + x];
//}
//
////-----------------------------------------------------------------------------------------
////
////  �`�悷��
////
////-----------------------------------------------------------------------------------------
//bool DX_MaptipObject::Render()
//{
//	bool result = false;
//
//	DEBUG_VALUE_CHECK(m_pVertexBuffer, "���_�o�b�t�@�̍쐬�Ɏ��s���܂����B");
//	DEBUG_VALUE_CHECK(m_pShaderResourceView, "�e�N�X�`���̃t�@�C���p�X���Ԉ���Ă��܂�");
//
//	if (m_enabled)
//	{
//		DX_System* pSystem = DX_System::GetInstance();
//		DX_ShaderManager* pShaderManager = DX_ShaderManager::GetInstance();
//		ID3D11Device* pDevice = pSystem->GetDevice();
//		ID3D11DeviceContext* pDeviceContext = pSystem->GetDeviceContext();
//
//
//		DX_Shader* pVertexShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::INSTANCE_VERTEX_SHADER);
//		DX_Shader* pPixelShader = pShaderManager->GetShader(DEFAULT_2D_SHADER::PIXEL_SHADER);
//
//		//	�V�F�[�_�[���p���J�n
//		pVertexShader->Begin(pDeviceContext);
//		pPixelShader->Begin(pDeviceContext);
//
//		unsigned int strides[] = { sizeof(DX::tagVertex2D) ,sizeof(m_pPosList[0]) };
//		unsigned int offsets[] = { 0,0 };
//
//		ID3D11Buffer* pInstanceBuffer = DX_Buffer::CreateVertexBuffer(pDevice, sizeof(m_pPosList[0]) * m_instanceNum, m_pPosList);
//		ID3D11Buffer* buffers[] = { m_pVertexBuffer, pInstanceBuffer };
//		DEBUG_VALUE_CHECK(pInstanceBuffer, "�C���X�^���X�o�b�t�@�̍쐬�Ɏ��s���܂����B");
//
//
//		// ���K�������E�B���h�E�T�C�Y�𑗂�
//		pShaderManager->SetVector(0, DirectX::XMFLOAT4(2.0f / DX::CAST::F(pSystem->GetWindowWidth()), 2.0f / DX::CAST::F(pSystem->GetWindowHeight()), 0.0f, 0.0f), pDevice, pDeviceContext, DX_SHADER_TYPE::VERTEX_SHADER);
//
//		//	VertexBuffer�𑗂�
//		pDeviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
//
//		//	InputLayout�̐ݒ�𑗂�
//		pDeviceContext->IASetInputLayout(pShaderManager->GetDefaultInputLayoutInstance2D());
//
//		//	Primitive�̐ݒ�𑗂�
//		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//
//		// �e�N�X�`�����𑗂�
//		result = DX_ResourceManager::SetShaderResources(pDeviceContext, 0, 1, &m_pShaderResourceView, DX_SHADER_TYPE::PIXEL_SHADER);
//
//		// �C���X�^���X�`����s��
//		pDeviceContext->DrawInstanced(4, m_instanceNum, 0, 0);
//
//		//	�V�F�[�_�[���p���I��
//		pVertexShader->End(pDeviceContext);
//		pPixelShader->End(pDeviceContext);
//
//		SAFE_RELEASE(pInstanceBuffer);
//	}
//
//	return result;
//}
//
////-----------------------------------------------------------------------------------------
////
////  �w�肵���ԍ��̃I�u�W�F�N�g�𖳌������܂�(�`�悳��Ȃ��Ȃ�)
////
////-----------------------------------------------------------------------------------------
//void DX_MaptipObject::Disable(const size_t index)
//{
//	// �`�悳��Ȃ��Ȃ�悤z�l��1���傫������(����ȊO�������킩���B�B�B)
//	m_pPosList[index].z = 1.1f;
//}
//
//
////-----------------------------------------------------------------------------------------
////
////  �w�肵���ԍ��̃I�u�W�F�N�g�𖳌������܂�(�`�悳��Ȃ��Ȃ�)
////
////-----------------------------------------------------------------------------------------
//void DX_MaptipObject::Disable()
//{
//	m_enabled = false;
//}
//
//
////-----------------------------------------------------------------------------------------
////
////  �w�肵���ԍ��̃I�u�W�F�N�g�̗L��������i�`�悳���悤�ɂȂ�j
////
////-----------------------------------------------------------------------------------------
//void DX_MaptipObject::Enable(const size_t index)
//{
//	m_pPosList[index].z = 0.0f;
//}
//
////------------------------------------------------------------------------------
////
////  @brief		�C���^���X�`����s���悤�ɂ���
////
////------------------------------------------------------------------------------
//void DX_MaptipObject::Enable()
//{
//	m_enabled = true;
//}
//
////------------------------------------------------------------------------------
////
////  @brief		�w�肵���C���X�^���X���������ǂ���
////
////------------------------------------------------------------------------------
//bool DX_MaptipObject::IsDisable(const size_t index) const
//{
//	return (m_pPosList[index].z >= 1.0f);
//}
//
////------------------------------------------------------------------------------
////
////  @brief		�w�肵���C���X�^���X���L�����ǂ���
////
////------------------------------------------------------------------------------
//bool DX_MaptipObject::IsEnable(const size_t index) const
//{
//	return (m_pPosList[index].z < 1.0f);
//}
//
////------------------------------------------------------------------------------
////
////  @brief		�w�肵���C���X�^���X���������ǂ���
////
////------------------------------------------------------------------------------
//bool DX_MaptipObject::IsDisable() const
//{
//	return !m_enabled;
//}
//
////------------------------------------------------------------------------------
////
////  @brief		�w�肵���C���X�^���X���L�����ǂ���
////
////------------------------------------------------------------------------------
//bool DX_MaptipObject::IsEnable() const
//{
//	return m_enabled;
//}