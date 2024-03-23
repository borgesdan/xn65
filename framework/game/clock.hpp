#ifndef XNA_GAME_CLOCK_HPP
#define XNA_GAME_CLOCK_HPP

#include "../types.hpp"
#include "../forward.hpp"
#include "../enums.hpp"
#include "../csharp/timespan.hpp"

namespace xna {
	class IGameClock {
	public:
		virtual ~IGameClock() {}

		virtual void Start() = 0;
		virtual void Resume() = 0;
		virtual void Reset() = 0;		
		virtual void Suspend() = 0;
		virtual void Stop() = 0;
		virtual TimeSpan ElapsedTime() = 0;
		virtual TimeSpan TotalTime() = 0;
	};
}

#endif