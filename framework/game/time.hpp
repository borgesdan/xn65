#ifndef XNA_GAME_TIME_HPP
#define XNA_GAME_TIME_HPP

#include "../forward.hpp"
#include "../types.hpp"
#include "../timespan.hpp"

namespace xna {
	class GameTime {	
	public:
		constexpr GameTime() = default;

		constexpr GameTime(const TimeSpan& elapsedGameTime, bool isRunningSlowly, const TimeSpan& totalGameTime) : 
			_elapsedGameTime(elapsedGameTime),
			_isRunningSlowly(isRunningSlowly), 
			_totalGameTime(totalGameTime) { }

		constexpr TimeSpan ElapsedGameTime() const {
			return _elapsedGameTime;
		}

		constexpr bool IsRunningSlowly() const {
			return _isRunningSlowly;
		}

		constexpr TimeSpan TotalGameTime() const {
			return _totalGameTime;
		}

	private:
		TimeSpan _elapsedGameTime{ 0 };
		bool _isRunningSlowly{ false };
		TimeSpan _totalGameTime{ 0 };		
	};
}

#endif