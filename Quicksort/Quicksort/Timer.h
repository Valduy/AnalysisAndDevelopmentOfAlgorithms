#ifndef QUICKSORT_TIMER_H_
#define QUICKSORT_TIMER_H_

#include <chrono>

class Timer {
public:
	Timer();

	bool IsRun() const;
	void Start();
	void Stop();
	long long EllapsedMilliseconds();

private:
	bool is_run_;
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time_point_;
	std::chrono::time_point<std::chrono::high_resolution_clock> stop_time_point_;
};

#endif // QUICKSORT_TIMER_H_
