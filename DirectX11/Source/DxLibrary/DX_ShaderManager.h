#ifndef __DX_SHADER_MANAGER_H_
#define __DX_SHADER_MANAGER_H_

struct SHADER {
	struct OBJECT2D {
		static const char* VERTEX_SHADER;
		static const char* PIXEL_SHADER;
	};

	struct OBJECT3D {
		static const char* VERTEX_SHADER;
		static const char* PIXEL_SHADER;
	};

	struct BOX {
		static const char* VERTEX_SHADER;
		static const char* PIXEL_SHADER;
	};
};

typedef SHADER::OBJECT2D	SHADER_OBJECT_2D;
typedef SHADER::OBJECT3D	SHADER_OBJECT_3D;
typedef SHADER::BOX			SHADER_BOX;

//****************************************************************************************************
//
//	ShaderManager
//
//****************************************************************************************************
/// <summary>
///	�V�F�[�_�[�̊e�N���X���t�@�C�����ɊǗ�����B
/// �܂��A�V�F�[�_�[�Ƀf�[�^�𑗂�
/// </summary>
class DX_ShaderManager : public DX_Singleton<DX_ShaderManager>
{
public:
	/// <summary>
	/// �����o�ϐ��̏������y�уV�F�[�_�[�̍쐬���s��
	/// </summary>
	bool Initialize();

	/// <summary>
	/// �V�F�[�_�[�Ɋւ���ϐ������
	/// </summary>
	~DX_ShaderManager();

	/// <summary>
	/// �w�肵���V�F�[�_�[���擾����
	/// </summary>
	/// <param name="filepath">�t�@�C���p�X</param>
	/// <returns></returns>
	DX_Shader* GetShader(const char* filepath);

	/// <summary>
	/// 2D�`��pInputLayout���擾����
	/// </summary>
	/// <returns>2D�`��pInputLayout</returns>
	ID3D11InputLayout* GetInputLayout2D();


	/// <summary>
	/// 3D�`��pInputLayout���擾����
	/// </summary>
	/// <returns>3D�`��pInputLayout</returns>
	ID3D11InputLayout* GetInputLayout3D();

	/// <summary>
	/// Box�`��pInputLayout���擾����
	/// </summary>
	/// <returns>Box�`��pInputLayout</returns>
	ID3D11InputLayout* GetInputLayoutBox();

	/// <summary>
	/// ���[���h�s��𑗂�
	/// ���W�X�^�ԍ���1
	/// </summary>
	/// <param name="worldMat">���[���h�s��</param>
	/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
	void SetWorldMat(const DirectX::XMFLOAT4X4& worldMat, SHADER_TYPE shaderType);

	/// <summary>
	/// int���V�F�[�_�[�ɑ���
	/// </summary>
	/// <param name="registerNum">�g�p���郌�W�X�^�ԍ�</param>
	/// <param name="value">����l</param>
	/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
	void SetInt(const unsigned int registerNum, const int value, SHADER_TYPE shaderType);
	
	/// <summary>
	/// Vector2���V�F�[�_�[�ɑ���
	/// </summary>
	/// <param name="registerNum">�g�p���郌�W�X�^�ԍ�</param>
	/// <param name="value">����l</param>
	/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
	void SetVector2(const unsigned int registerNum, const DirectX::XMFLOAT2& value, SHADER_TYPE shaderType);

	/// <summary>
	/// Vector3���V�F�[�_�[�ɑ���
	/// </summary>
	/// <param name="registerNum">�g�p���郌�W�X�^�ԍ�</param>
	/// <param name="value">����l</param>
	/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
	void SetVector3(const unsigned int registerNum, const DirectX::XMFLOAT3& value, SHADER_TYPE shaderType);

	/// <summary>
	/// Vector4���V�F�[�_�[�ɑ���
	/// </summary>
	/// <param name="registerNum">�g�p���郌�W�X�^�ԍ�</param>
	/// <param name="value">����l</param>
	/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
	void SetVector4(const unsigned int registerNum, const DirectX::XMFLOAT4& value, SHADER_TYPE shaderType);
	
	/// <summary>
	/// Vector2�����\�[�X�Ƃ��ăV�F�[�_�[�ɑ���
	/// </summary>
	/// <param name="registerNum">�g�p���郌�W�X�^�ԍ�</param>
	/// <param name="vecList">����l�̃��X�g</param>
	/// <param name="vecCount">���X�g�̗v�f�̐�</param>
	/// <param name="shader_type">����V�F�[�_�[�̃^�C�v</param>
	void SetVector2Resource(const unsigned int registerNum, const DirectX::XMFLOAT2* vecList, const unsigned int vecCount, SHADER_TYPE shader_type);

	/// <summary>
	/// Vector3�����\�[�X�Ƃ��ăV�F�[�_�[�ɑ���
	/// </summary>
	/// <param name="registerNum">�g�p���郌�W�X�^�ԍ�</param>
	/// <param name="vecList">����l�̃��X�g</param>
	/// <param name="vecCount">���X�g�̗v�f�̐�</param>
	/// <param name="shader_type">����V�F�[�_�[�̃^�C�v</param>
	void SetVector3Resource(const unsigned int registerNum, const DirectX::XMFLOAT3* vecList, const unsigned int vecCount, SHADER_TYPE shader_type);

	/// <summary>
	/// Vector4�����\�[�X�Ƃ��ăV�F�[�_�[�ɑ���
	/// </summary>
	/// <param name="registerNum">�g�p���郌�W�X�^�ԍ�</param>
	/// <param name="vecList">����l�̃��X�g</param>
	/// <param name="vecCount">���X�g�̗v�f�̐�</param>
	/// <param name="shader_type">����V�F�[�_�[�̃^�C�v</param>
	void SetVector4Resource(const unsigned int registerNum, const DirectX::XMFLOAT4* vecList, const unsigned int vecCount, SHADER_TYPE shader_type);

	/// <summary>
	/// �s����V�F�[�_�[�ɑ���
	/// </summary>
	/// <param name="regiserNum">�g�p���郌�W�X�^�ԍ�</param>
	/// <param name="mat">����l</param>
	/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
	void SetMatrix(const unsigned int regiserNum, const DirectX::XMFLOAT4X4& mat, SHADER_TYPE shaderType);

	/// <summary>
	/// �����̍s����V�F�[�_�[�ɑ���
	/// </summary>
	/// <param name="ppBuffer">�o�b�t�@</param>
	/// <param name="regiserNum">�g�p���郌�W�X�^�ԍ�</param>
	/// <param name="mat">����l</param>
	/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
	void SetMatrix(ID3D11Buffer* const* ppBuffer, const unsigned int regiserNum, const DirectX::XMFLOAT4X4*	pMat, const unsigned int matCount, SHADER_TYPE shaderType);


	/// <summary>
	/// �����̍s����V�F�[�_�[�ɑ���
	/// </summary>
	/// <param name="regiserNum">�g�p���郌�W�X�^�̔ԍ�</param>
	/// <param name="matList">�s��̃��X�g</param>
	/// <param name="matCount">�s��̃��X�g�̐�</param>
	/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
	void SetMatrixResoruce(const unsigned int regiserNum, const DirectX::XMFLOAT4X4* matList, const unsigned int matCount, SHADER_TYPE shaderType);
	
	/// <summary>
	/// �����̍s����o�b�t�@�ŃV�F�[�_�[�ɑ���
	/// </summary>
	/// <param name="regiserNum">�g�p���郌�W�X�^�̔ԍ�</param>
	/// <param name="matList">�s��̃��X�g</param>
	/// <param name="matCount">�s��̃��X�g�̐�</param>
	/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
	void SetMatrixResoruce(const unsigned int regiserNum, ID3D11Buffer* buffer, const unsigned int matCount, SHADER_TYPE shaderType);
	
	/// <summary>
	/// �萔�o�b�t�@���V�F�[�_�[�ɑ���
	/// </summary>
	/// <param name="registerNum">�g�p���郌�W�X�^�̔ԍ�</param>
	/// <param name="numBuffers">�o�b�t�@�̐�</param>
	/// <param name="constantBuffers">�V�F�[�_�[�ɑ���o�b�t�@�̃��X�g</param>
	/// <param name="shaderType">�g�p����V�F�[�_�[�̃^�C�v</param>
	void SetConstantBuffers(UINT registerNum, UINT numBuffers, ID3D11Buffer* const* constantBuffers, SHADER_TYPE shaderType);
	
	/// <summary>
	/// ���\�[�X���V�F�[�_�[�ɑ���
	/// </summary>
	/// <param name="registerNum">�g�p���郌�W�X�^�̔ԍ�</param>
	/// <param name="numBuffers">�g�p���郊�\�[�X�̐�</param>
	/// <param name="shaderResourceView">�V�F�[�_�[�ɑ��郊�\�[�X�̃��X�g</param>
	/// <param name="shaderType">�g�p����V�F�[�_�[�̃^�C�v</param>
	void SetShaderResources(const unsigned int registerNum, const unsigned int numBuffers, ID3D11ShaderResourceView* const* shaderResourceView, SHADER_TYPE shaderType);

private:
	friend class DX_Singleton<DX_ShaderManager>;

	static const size_t SHADER_NUM = 10;
	struct Shader {
		DX_Shader* shader;
		const char* filepath;
	};
	Shader m_shaders[SHADER_NUM];

	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout2D;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout3D;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayoutBox;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_constantBuffer;

	/// <summary>
	/// �ϐ��̏��������s��
	/// </summary>
	DX_ShaderManager();

	/// <summary>
	/// �R���s���[�g�V�F�[�_�[���g���邩�ǂ�����
	/// </summary>
	/// <returns>�g�p�ł��邩�ǂ���</returns>
	bool UsedComputeShader();

	/// <summary>
	/// �V�F�[�_�[���쐬����
	/// </summary>
	/// <param name="filepath">�V�F�[�_�[�̃t�@�C���p�X</param>
	/// <returns>����</returns>
	bool CreateShader(const char* filepath);

	/// <summary>
	/// �C���v�b�g���C�A�E�g���쐬����
	/// </summary>
	/// <returns>�쐬�ł������ǂ���</returns>
	bool CreateInputLayout();

	/// <summary>
	/// �w�肵���C���v�b�g���C�A�E�g���쐬����
	/// </summary>
	/// <param name="inputLayoutDesc">�쐬����C���v�b�g���C�A�E�g�̐ݒ�ꗗ</param>
	/// <param name="inputLayoutCount">�쐬����C���v�b�g���C�A�E�g�̐ݒ萔</param>
	/// <param name="byteCord">�쐬����C���v�b�g���C�A�E�g���g�p����V�F�[�_�[�̃o�C�g�R�[�h</param>
	/// <param name="inputLayout">�쐬�����C���v�b�g���C�A�E�g</param>
	/// <returns>�쐬�ł������ǂ���</returns>
	bool CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc, const unsigned int inputLayoutCount, ID3DBlob* byteCord, ID3D11InputLayout** inputLayout);


	/// <summary>
	/// Vector2,3,4�����\�[�X�Ƃ��ăV�F�[�_�[�ɑ���
	/// </summary>
	/// <param name="registerNum">�g�p���郌�W�X�^�ԍ�</param>
	/// <param name="vecList">����l�̃��X�g</param>
	/// <param name="vecCount">���X�g�̗v�f�̐�</param>
	/// <param name="elementSize">���X�g�̗v�f�̃T�C�Y</param>
	/// <param name="shaderType">����V�F�[�_�[�̃^�C�v</param>
	void SetVectorResource(const unsigned int registerNum, const void* vecList, const unsigned int vecCount, const unsigned int elementSize, SHADER_TYPE shaderType);
};

#endif // !__DX_SHADER_MANAGER_H_
