/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-01-30 15:01:10
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-01-30 15:01:38
 */
#ifndef __SINGLETON__
#define __SINGLETON__

#include <cstddef>
#include <pthread.h>

/**
 * @brief 单例模板类
 *
 * @tparam T
 */
template <typename T>
class Singleton
{
public:
    /**
     * @brief 析构函数
     */
    virtual ~Singleton() {}

    /**
     * @brief 返回唯一实例
     *
     * @return 指向实例的指针
     */
    static T* getInstance()
    {
        pthread_once( &m_instanceInitFlag, &Singleton::initInstance );
        return m_pInstance;
    }

	static void destroyInstance()
	{
	    pthread_once( &m_instanceDeleteFlag, &Singleton::deleteInstance );
	}
		
protected:
    // can't new
    Singleton() {}

private:
    static pthread_once_t m_instanceInitFlag;  // to protect m_pInstance
    static pthread_once_t m_instanceDeleteFlag;
    static T* m_pInstance;  // singleton

private:
    // can't copy
    Singleton( const Singleton& s );
    Singleton& operator=( const Singleton& s );

    // init m_pInstance
    static void initInstance()
    {
        m_pInstance = new T();
    }
	static void deleteInstance()
	{
	    if ( m_pInstance != NULL)
	    	{
	    	    delete m_pInstance;
		    m_pInstance = NULL;
	    	}
	}
};

template <typename T>
pthread_once_t Singleton<T>::m_instanceInitFlag 
   = PTHREAD_ONCE_INIT;

template <typename T>
pthread_once_t Singleton<T>::m_instanceDeleteFlag 
   = PTHREAD_ONCE_INIT;

template <typename T>
T* Singleton<T>::m_pInstance = NULL;

#endif