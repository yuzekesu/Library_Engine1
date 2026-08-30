#include "Timer.h"
#include <chrono>
#include <thread>

Timer::Timer(float s) : _interval{ std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>{s}) }, _last{ std::chrono::steady_clock::now() } {}

void Timer::Wait() {
	this->_last += this->_interval;
	std::this_thread::sleep_until(this->_last);
}

unsigned Timer::Ticks() {
	auto duration = std::chrono::steady_clock::now() - this->_last;
	int multiplier = static_cast<int>(duration / this->_interval);
	if (multiplier > 0) {
		this->_last += duration;
	}
	return multiplier;
}

float Timer::Seconds() {
	std::chrono::duration<float> duration = std::chrono::steady_clock::now() - this->_last;
	this->_last = std::chrono::steady_clock::now();
	return duration.count();
}
