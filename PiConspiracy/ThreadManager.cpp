#include "PiEnginePreCompiled.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager(int a_numThreads)
	: m_shutdown(false)
{
	m_threads.reserve(a_numThreads);
	for (int i = 0; i < a_numThreads; ++i) 
	{
		m_threads.emplace_back(std::bind(&ThreadManager::ThreadUpdate, this));
	}
}

ThreadManager::~ThreadManager()
{
	//TODO: make sure to handle joining and clearing threads properly
}

void ThreadManager::AddJob(std::function<void(void)> a_job)
{
	//lock for safe access
	std::unique_lock<std::mutex> lock(m_mutexLock);
	//push job into queue
	m_jobsQueue.emplace(std::move(a_job));
	//unblock a thread
	m_conditionVariable.notify_one();
}

void ThreadManager::ThreadUpdate(void)
{
	// a place holder for jobs popped from queue
	std::function<void(void)> jobHolder;

	while (1)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutexLock);

			while (!m_shutdown && m_jobsQueue.empty())
			{
				m_conditionVariable.wait(lock);
			}

			jobHolder = std::move(m_jobsQueue.front());
			m_jobsQueue.pop();
		}

		jobHolder();
	}
}