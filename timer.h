#pragma once
#include <chrono>
using namespace std::chrono;

class Timer
{
public:
	void Start ()
	{
		start_time = high_resolution_clock::now();
	}
	int Stop ()
	{
		return duration_cast<milliseconds>(high_resolution_clock::now() - start_time).count();
	}
private:
	high_resolution_clock::time_point start_time;
};
