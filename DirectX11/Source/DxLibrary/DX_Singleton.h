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
	DX_Singleton() {} // 外部でのインスタンス作成は禁止
	virtual ~DX_Singleton() {}

private:
	void operator=(const DX_Singleton& obj) {} // 代入演算子禁止
	DX_Singleton(const DX_Singleton &obj) {} // コピーコンストラクタ禁止
};

template<class T> T* DX_Singleton<T>::m_pInstance = nullptr;
#endif