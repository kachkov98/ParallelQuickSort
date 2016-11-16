#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <random>
#include <chrono>
#include "threadpool.h"
#include "timer.h"

const size_t array_size = 10000000;
int granulation = 1000;

template <typename TIterator, typename TPredicate>
class QuickSortTask : public ThreadPool::Task
{
public:
	QuickSortTask (TIterator fst, TIterator lst, TPredicate predicate):
		fst_(fst),
		lst_(lst),
		predicate_(predicate)
	{
	}
	void operator() (ThreadPool& thread_pool)
	{
		if (lst_ - fst_ < granulation)
		{
			std::sort (fst_, lst_, predicate_);
			return;
		}
		typedef typename std::iterator_traits<TIterator>::value_type T;
		TIterator l = fst_, r = lst_;
		T m = *(fst_ + (lst_ - fst_) / 2);
		do
		{
			while (predicate_ (*l, m)) ++l;
			while (predicate_ (m, *r)) --r;
			if (l <= r)
			{
				if (predicate_ (*r, *l))
					std::swap (*l, *r);
				++l;
				--r;
			}
		}
		while (l <= r);

		if (r > fst_)
			thread_pool.AddTask (std::make_unique<QuickSortTask<TIterator, TPredicate>> (fst_, r, predicate_));
		if (l < lst_)
			thread_pool.AddTask (std::make_unique<QuickSortTask<TIterator, TPredicate>> (l, lst_, predicate_));
	}
	~QuickSortTask ()
	{
	}
private:
	TIterator fst_, lst_;
	TPredicate predicate_;
};

template <typename TIterator, typename TPredicate>
void QuickSort (TIterator fst, TIterator lst, TPredicate predicate)
{
	ThreadPool pool (std::make_unique<QuickSortTask<TIterator, TPredicate>> (fst, lst - 1, predicate));
}

int main()
{
	// generate two arrays
	std::uniform_int_distribution<int> distribution (1, array_size);
	std::random_device generator;

	std::vector<int> array1, array2;
	array1.reserve (array_size);
	array2.reserve (array_size);

	for (size_t i = 0; i < array_size; i++)
	{
		int elem = distribution (generator);
		array1.push_back (elem);
		array2.push_back (elem);
	}

	// measurements
	Timer timer;
	std::cout << "Number of elements: " << array_size << std::endl;
	// single-threaded
	timer.Start();
	std::sort(array1.begin(), array1.end(), std::less<int>());
	std::cout << "Single-threaded: " << timer.Stop() << " ms" << std::endl;
	// multi-threaded
	timer.Start ();
	QuickSort(array2.begin(), array2.end(), std::less<int>());
	std::cout << " Multi-threaded: " << timer.Stop() << " ms" << std::endl;
	return 0;
}
