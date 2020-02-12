
#ifndef __TIMER_H__
#define __TIMER_H__ 

namespace app { namespace core {

	struct TimePoint {
		friend class Timer;
	public:
		inline float GetS() const { return Time / 1000000.0f; }
		inline float GetMS() const { return Time / 1000.0f; }
		inline float GetUS() const { return Time; }
		inline float GetNS() const { return Time * 1000.0f; }

		TimePoint operator-(const TimePoint & other) {
			return TimePoint(Time - other.Time);
		}
	private:
		TimePoint() : Time(0.0f) {}
		TimePoint(float time) : Time(time) {}

		float Time;
	};

	class Timer {
	public:
		static void Update();
		static TimePoint GetTime();

		inline static TimePoint GetDeltaTime() { return s_DeltaTime; }
	private:
		static TimePoint s_PrevTime, s_CurrentTime, s_DeltaTime;
	};

} }

#endif //__TIMER_H__
