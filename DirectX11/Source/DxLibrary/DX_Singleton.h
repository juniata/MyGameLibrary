#ifndef __DX_SINGLETON_H_
#define __DX_SINGLETON_H_

template<class T>
class DX_Singleton
{
public:
	static T* GetInstance()
	{
		return m_pInstance;
	}

	static void Create()
	{
		if (m_pInstance == nullptr) {
			m_pInstance = new T();
		}
	}

	static void Destroy()
	{
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}


protected:
	static T* m_pInstance;
	DX_Singleton() {} // �O���ł̃C���X�^���X�쐬�͋֎~
	virtual ~DX_Singleton() {}

private:
	void operator=(const DX_Singleton& obj) {} // ������Z�q�֎~
	DX_Singleton(const DX_Singleton &obj) {} // �R�s�[�R���X�g���N�^�֎~
};

template<class T> T* DX_Singleton<T>::m_pInstance = nullptr;
#endif