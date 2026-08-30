#pragma once
#include <chrono>
#include <ratio>

class Timer {
public:
	Timer() = delete;
	Timer(float s = 1.f);
	void Wait();
	unsigned Ticks();
	float Seconds();
private:
	std::chrono::time_point<std::chrono::steady_clock> _last;
	std::chrono::steady_clock::duration _interval;
};

