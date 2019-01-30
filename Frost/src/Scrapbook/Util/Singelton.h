#pragma once

template<class T> class Singleton
{
private:
	static T* s_instance;

protected:
	Singleton()
	{
		if (s_instance)
			throw;
	};

	~Singleton()
	{
		if(s_instance != nullptr)
			delete s_instance;
	}

public:
	static T* Get()
	{
		if (!s_instance)
		{
			s_instance = new T();
		}
		return s_instance;
	}
};

template<class T> T* Singleton<T>::s_instance = (0);