#ifndef _LOCK_
#define _LOCK_

#ifdef  __LINUX_CODE__
#include <pthread.h>


class CCriticalSection
{
private:
	pthread_mutex_t	m_mutex;

public:
	CCriticalSection()
	{
		Init();
	}
	
	void Init()
	{
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE_NP);
		pthread_mutex_init(&m_mutex, &attr); //PTHREAD_MUTEX_RECURSIVE_NP
		pthread_mutexattr_destroy(&attr);
	}

	~CCriticalSection()
	{
		pthread_mutex_destroy(&m_mutex);
	}

	void Lock()
	{
		pthread_mutex_lock(&m_mutex);
	}

	void Unlock()
	{
		pthread_mutex_unlock(&m_mutex);
	}
};


class CEvent
{
private:
	pthread_cond_t	m_cond;
	pthread_mutex_t	m_mutex;

public:
	CEvent()
	{
		Init();
	}

	void Init()
	{
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE_NP);
		pthread_mutex_init(&m_mutex, &attr); //PTHREAD_MUTEX_RECURSIVE_NP
		pthread_mutexattr_destroy(&attr);
		pthread_cond_init(&m_cond, NULL);
	}

	~CEvent()
	{
		pthread_cond_destroy(&m_cond);
		pthread_mutex_destroy(&m_mutex);
	}

	void Lock()
	{
		pthread_mutex_lock(&m_mutex);
	}

	void Unlock()
	{
		pthread_mutex_unlock(&m_mutex);
	}

	void Wait()
	{
		//pthread_mutex_lock(&m_mutex);//use Lock()
		//while (条件不满足)
		pthread_cond_wait(&m_cond, &m_mutex);
		//pthread_mutex_unlock(&m_mutex);//use Unlock()
	}

	void Post()
	{
		//pthread_mutex_lock(&m_mutex);//use Lock()
		//if (条件满足)
		pthread_cond_signal(&m_cond);
		//pthread_mutex_unlock(&m_mutex);//use Unlock()
	}
};
#else
//#define _WIN32_WINNT 0x0500

#include <Winbase.h>

//#pragma comment (lib, "Coredll.lib")

class CWaitEvent
{
private:
	HANDLE m_mutex;
	HANDLE m_event;

public:
	CWaitEvent()
	{
		Init();
	}

	~CWaitEvent()
	{
		ReleaseMutex(m_mutex);
		CloseHandle(m_event);
	}

	void Init()
	{
		m_mutex=CreateMutex(NULL,FALSE,NULL);
		m_event=CreateEvent(NULL,FALSE,FALSE,NULL);
	}

	void Lock()
	{
		WaitForSingleObject(m_mutex,INFINITE);
	}

	void Unlock()
	{
		ReleaseMutex(m_mutex);
	}

	void Wait()
	{
		SignalObjectAndWait(m_mutex,m_event,INFINITE,false);
		WaitForSingleObject(m_mutex,INFINITE);
	}

	void Post()
	{
		SetEvent(m_event);
	}
};

#endif

#endif

