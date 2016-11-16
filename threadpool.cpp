#include <iostream>
#include "threadpool.h"

ThreadPool::ThreadPool (std::unique_ptr<Task> task):
	num_threads(std::thread::hardware_concurrency ()),
	num_working_threads(0)
{
	AddTask (std::move(task));
	threads.reserve (num_threads);
	for (unsigned int i = 0; i < num_threads; i++)
		threads.emplace_back (&ThreadPool::Worker, this);
}

void ThreadPool::AddTask (std::unique_ptr<Task> task)
{
	std::lock_guard<std::mutex> guard (queue_mutex);
	tasks.push (std::move(task));
}

ThreadPool::~ThreadPool ()
{
	for (unsigned int i = 0; i < num_threads; i++)
		threads[i].join();
}

void ThreadPool::Worker ()
{
	std::unique_ptr<Task> task;
	while (true)
	{
		queue_mutex.lock();
		if (!tasks.empty())
		{
			task = std::move(tasks.front());
			tasks.pop();
			queue_mutex.unlock();
			++num_working_threads;
			(*task)(*this);
			--num_working_threads;
		}
		else
		{
			queue_mutex.unlock();
			if (num_working_threads)
				std::this_thread::yield();
			else
				return;
		}
	}
}
