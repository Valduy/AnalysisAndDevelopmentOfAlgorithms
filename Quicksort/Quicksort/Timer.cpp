#include "Timer.h"

Timer::Timer() : is_run_(false) {}

bool Timer::IsRun() const {
	return is_run_;
}

void Timer::Start() {
	is_run_ = true;
	start_time_point_ = std::chrono::high_resolution_clock::now();
}

void Timer::Stop() {
	is_run_ = false;
	stop_time_point_ = std::chrono::high_resolution_clock::now();
}

long long Timer::EllapsedMilliseconds() {
	auto end_time_point = is_run_
		? std::chrono::high_resolution_clock::now()
		: stop_time_point_;

	auto start_millis = std::chrono::time_point_cast<std::chrono::milliseconds>(start_time_point_);
	auto end_millis = std::chrono::time_point_cast<std::chrono::milliseconds>(end_time_point);
	auto duration = end_millis - start_millis;
	return duration.count();
}