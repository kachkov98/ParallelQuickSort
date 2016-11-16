#pragma once
#include <memory>
#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <mutex>

class ThreadPool
{
public:
	class Task
	{
	public:
		Task () = default;
		virtual void operator() (ThreadPool& thread_pool) = 0;
		virtual ~Task () = default;
	};

	ThreadPool (const std::unique_ptr<Task> task);
	ThreadPool (const ThreadPool&) = delete;
	ThreadPool operator= (const ThreadPool&) = delete;
	void AddTask (const std::unique_ptr<Task> task);
	~ThreadPool ();
private:
	unsigned int num_threads;
	std::vector<std::thread> threads;
	std::queue<std::unique_ptr<Task>> tasks;
	std::mutex queue_mutex;
	std::atomic<int> num_working_threads;

	void Worker ();
};
