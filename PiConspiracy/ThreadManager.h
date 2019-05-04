#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadManager : public Singleton<ThreadManager>
{
public:
	ThreadManager(int a_numThreads);
	~ThreadManager();
	
	void AddJob(std::function<void(void)> a_job);

private:
	void ThreadUpdate(void);

	std::vector<std::thread> m_threads;
	std::queue<std::function<void(void)>> m_jobsQueue;
	std::condition_variable m_conditionVariable;
	std::mutex m_mutexLock;
	bool m_shutdown;

};