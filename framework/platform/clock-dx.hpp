#ifndef XNA_PLATFORM_CLOCK_DX_HPP
#define XNA_PLATFORM_CLOCK_DX_HPP

#include "../game/clock.hpp"
#include <chrono>

namespace xna {
	using SteadyClock = std::chrono::steady_clock;

	class GameClock : public IGameClock {
	public:
		virtual ~GameClock() override {}
		virtual void Reset() override;
		virtual void Start() override;
		virtual void Resume() override;
		virtual void Suspend() override;
		virtual void Stop() override;
		virtual TimeSpan ElapsedTime() override;
		virtual TimeSpan TotalTime() override;
		
		virtual constexpr bool IsActive() override {
			return !_suspended && !_stopped;
		}

		inline SteadyClock::time_point ClockNow() {
			return SteadyClock::now();
		}

		constexpr long long GetNanoseconds(SteadyClock::time_point start, SteadyClock::time_point end) {
			return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		}

		constexpr long long GetMicroseconds(SteadyClock::time_point start, SteadyClock::time_point end) {
			return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		}

		constexpr long long GetMilliSeconds(SteadyClock::time_point start, SteadyClock::time_point end) {
			return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		}

	private:
		SteadyClock _clock;
		SteadyClock::time_point _start;		
		SteadyClock::time_point _end;	
		long long _total{ 0 };		
		bool _suspended{ false };
		bool _stopped { true };		
	};	
}

#endif