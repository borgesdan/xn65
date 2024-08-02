#ifndef XNA_GAME_TIME_HPP
#define XNA_GAME_TIME_HPP

#include "../default.hpp"
#include "../csharp/timespan.hpp"

namespace xna {
	//Snapshot of the game timing state expressed in values that can be used by variable-step (real time) or fixed-step (game time) games. 
	class GameTime {	
	public:
		constexpr GameTime() = default;

		constexpr GameTime(const TimeSpan& elapsedGameTime, const TimeSpan& totalGameTime, bool isRunningSlowly) :
			ElapsedGameTime(elapsedGameTime),
			IsRunningSlowly(isRunningSlowly), 
			TotalGameTime(totalGameTime) { }		

		//The amount of elapsed game time since the last update.
		TimeSpan ElapsedGameTime{ 0 };
		//Gets a value indicating that the game loop is taking longer than its TargetElapsedTime. In this case, the game loop can be considered to be running too slowly and should do something to "catch up." 
		bool IsRunningSlowly{ false };
		//The amount of game time since the start of the game.
		TimeSpan TotalGameTime{ 0 };		
	};
}

#endif