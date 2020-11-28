#ifndef __DX_SINGLETON_H_
#define __DX_SINGLETON_H_

/// <summary>
///	シングルトンの基底クラス
/// </summary>
template<class T>
class DX_Singleton
{
public:
	/// <summary>
	/// インスタンスを取得する
	/// </summary>
	/// <returns>インスタンス</returns>
	static T* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// インスタンスを生成する
	/// </summary>
	static void Create()
	{
		if (m_instance == nullptr)
		{
			m_instance = new T();
		}
	}

	/// <summary>
	/// インスタンスを削除する
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
	// 代入演算子禁止
	void operator=(const DX_Singleton& obj) {}

	// コピーコンストラクタ禁止
	DX_Singleton(const DX_Singleton &obj) {}
};

template<class T> T* DX_Singleton<T>::m_instance = nullptr;
#endif