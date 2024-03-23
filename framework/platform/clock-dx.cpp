#include "clock-dx.hpp"
#include <Windows.h>

namespace xna {
	void GameClock::Reset() {
		_start = ClockNow();
		_end = std::chrono::steady_clock::time_point();		
		_stopped = false;
		_suspended = false;
	}

	void GameClock::Start() {
		Reset();
		_total = 0;
		_start = ClockNow();
	}
	
	void GameClock::Resume() {
		if (_stopped)
			return;

		if (_suspended) {
			auto elapsed = _end - _start;
			auto now = ClockNow();

			_start = now - elapsed;
			_suspended = false;
		}
		else {
			_start = ClockNow();
		}
	}

	void GameClock::Suspend() {
		if (_suspended || _stopped)
			return;

		_end = ClockNow();
		_suspended = true;		
	}

	void GameClock::Stop() {
		Reset();	
		_total = 0;
		_stopped = true;
	}

	TimeSpan GameClock::ElapsedTime() {
		if (_stopped) return TimeSpan();

		if (!_suspended) {
			_end = ClockNow();
		}

		auto nano = GetNanoseconds(_start, _end);
		return TimeSpan::FromSeconds(nano / 1'000'000'000.0);
	}

	TimeSpan GameClock::TotalTime() {
		if (_stopped) return TimeSpan();

		if (!_suspended) {
			_end = ClockNow();
		}

		auto nano = GetNanoseconds(_start, _end);
		_total += nano;

		return TimeSpan::FromSeconds(_total / 1'000'000'000.0);
	}
}