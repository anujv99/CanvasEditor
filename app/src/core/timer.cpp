#include "timer.h"

#include <chrono>

namespace app { namespace core {

	static std::chrono::time_point<std::chrono::steady_clock> START_TIME = std::chrono::high_resolution_clock::now();

	TimePoint Timer::s_PrevTime			= 0.0f;
	TimePoint Timer::s_CurrentTime		= 0.0f;
	TimePoint Timer::s_DeltaTime		= 0.0f;

	void Timer::Update() {
		s_PrevTime = s_CurrentTime;
		s_CurrentTime = GetTime();
		s_DeltaTime = s_CurrentTime - s_PrevTime;
	}

	TimePoint Timer::GetTime() {
		std::chrono::duration<float, std::micro> time = std::chrono::high_resolution_clock::now() - START_TIME;
		return TimePoint(time.count());
	}

} }