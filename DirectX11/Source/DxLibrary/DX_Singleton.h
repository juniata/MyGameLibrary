#ifndef __DX_SINGLETON_H_
#define __DX_SINGLETON_H_

/// <summary>
///	�V���O���g���̊��N���X
/// </summary>
template<class T>
class DX_Singleton
{
public:
	/// <summary>
	/// �C���X�^���X���擾����
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static T* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// �C���X�^���X�𐶐�����
	/// </summary>
	static void Create()
	{
		if (m_instance == nullptr)
		{
			m_instance = new T();
		}
	}

	/// <summary>
	/// �C���X�^���X���폜����
	/// </summary>
	static void Destroy()
	{
		if (m_instance)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}


protected:
	static T* m_instance;

	DX_Singleton() {}
	virtual ~DX_Singleton() {}

private:
	// ������Z�q�֎~
	void operator=(const DX_Singleton& obj) {}

	// �R�s�[�R���X�g���N�^�֎~
	DX_Singleton(const DX_Singleton &obj) {}
};

template<class T> T* DX_Singleton<T>::m_instance = nullptr;
#endif