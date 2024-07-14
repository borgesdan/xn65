#ifndef XNA_GAME_TIME_HPP
#define XNA_GAME_TIME_HPP

#include "../default.hpp"
#include "../csharp/timespan.hpp"

namespace xna {
	class GameTime {	
	public:
		constexpr GameTime() = default;

		constexpr GameTime(const TimeSpan& elapsedGameTime, const TimeSpan& totalGameTime, bool isRunningSlowly) :
			ElapsedGameTime(elapsedGameTime),
			IsRunningSlowly(isRunningSlowly), 
			TotalGameTime(totalGameTime) { }		

		TimeSpan ElapsedGameTime{ 0 };
		bool IsRunningSlowly{ false };
		TimeSpan TotalGameTime{ 0 };		
	};
}

#endif