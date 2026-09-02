#include "Timer.h"
#include <chrono>
#include <thread>

/// # CONSTRUCTRION
/// 1. It must need interval as input in second.
Timer::Timer(float s) : _interval{ std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<float>{s}) }, _last{ std::chrono::steady_clock::now() } {}
/// # DESCRIPTION
/// 1. Wait until specified interval at the construction.
void Timer::Wait() {
	this->_last += this->_interval;
	std::this_thread::sleep_until(this->_last);
}
/// # DESCRIPTION
/// 1. For tick based calculation.
/// # RETURN
/// 1. The accumulated ticks from now to last successfully function calling.
unsigned Timer::Ticks() {
	auto duration = std::chrono::steady_clock::now() - this->_last;
	int multiplier = static_cast<int>(duration / this->_interval);
	if (multiplier > 0) {
		this->_last += duration;
	}
	return multiplier;
}
/// # DESCRIPTION
/// 1. For time based calculation.
/// # RETURN
/// 1. The accumulated time interval from up till the last function call.
float Timer::Seconds() {
	std::chrono::duration<float> duration = std::chrono::steady_clock::now() - this->_last;
	this->_last = std::chrono::steady_clock::now();
	return duration.count();
}
