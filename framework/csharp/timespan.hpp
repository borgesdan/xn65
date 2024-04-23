/*
	TimeSpan in C++
	Source code: https://source.dot.net/#System.Private.CoreLib/src/libraries/System.Private.CoreLib/src/System/TimeSpan.cs,865ef7b89f41b632 
*/

#ifndef XNA_CSHARP_TIMESPAN_HPP
#define XNA_CSHARP_TIMESPAN_HPP

#include <limits>
#include <cstdint>
#include <cmath>

namespace xna {
	struct TimeSpan {
		static constexpr int64_t NanosecondsPerTick = 100;
		static constexpr int64_t TicksPerMicrosecond = 10;
		static constexpr int64_t TicksPerMillisecond = TicksPerMicrosecond * 1000;
		static constexpr int64_t TicksPerSecond = TicksPerMillisecond * 1000;
		static constexpr int64_t TicksPerMinute = TicksPerSecond * 60;
		static constexpr int64_t TicksPerHour = TicksPerMinute * 60;
		static constexpr int64_t TicksPerDay = TicksPerHour * 24;
		static constexpr int64_t MicrosecondsPerMillisecond = TicksPerMillisecond / TicksPerMicrosecond;
		static constexpr int64_t MicrosecondsPerSecond = TicksPerSecond / TicksPerMicrosecond;
		static constexpr int64_t MicrosecondsPerMinute = TicksPerMinute / TicksPerMicrosecond;
		static constexpr int64_t MicrosecondsPerHour = TicksPerHour / TicksPerMicrosecond;
		static constexpr int64_t MicrosecondsPerDay = TicksPerDay / TicksPerMicrosecond;
		static constexpr int64_t MillisecondsPerSecond = TicksPerSecond / TicksPerMillisecond;
		static constexpr int64_t MillisecondsPerMinute = TicksPerMinute / TicksPerMillisecond;
		static constexpr int64_t MillisecondsPerHour = TicksPerHour / TicksPerMillisecond;
		static constexpr int64_t MillisecondsPerDay = TicksPerDay / TicksPerMillisecond;
		static constexpr int64_t SecondsPerMinute = TicksPerMinute / TicksPerSecond;
		static constexpr int64_t SecondsPerHour = TicksPerHour / TicksPerSecond;
		static constexpr int64_t SecondsPerDay = TicksPerDay / TicksPerSecond;
		static constexpr int64_t MinutesPerHour = TicksPerHour / TicksPerMinute;
		static constexpr int64_t MinutesPerDay = TicksPerDay / TicksPerMinute;
		static constexpr int64_t HoursPerDay = TicksPerDay / TicksPerHour;
		static constexpr int64_t MinTicks = (std::numeric_limits<int64_t>::min)();
		static constexpr int64_t MaxTicks = (std::numeric_limits<int64_t>::max)();
		static constexpr int64_t MinMicroseconds = MinTicks / TicksPerMicrosecond;
		static constexpr int64_t MaxMicroseconds = MaxTicks / TicksPerMicrosecond;
		static constexpr int64_t MinMilliseconds = MinTicks / TicksPerMillisecond;
		static constexpr int64_t MaxMilliseconds = MaxTicks / TicksPerMillisecond;
		static constexpr int64_t MinSeconds = MinTicks / TicksPerSecond;
		static constexpr int64_t MaxSeconds = MaxTicks / TicksPerSecond;
		static constexpr int64_t MinMinutes = MinTicks / TicksPerMinute;
		static constexpr int64_t MaxMinutes = MaxTicks / TicksPerMinute;
		static constexpr int64_t MinHours = MinTicks / TicksPerHour;
		static constexpr int64_t MaxHours = MaxTicks / TicksPerHour;
		static constexpr int64_t MinDays = MinTicks / TicksPerDay;
		static constexpr int64_t MaxDays = MaxTicks / TicksPerDay;
		static constexpr int64_t TicksPerTenthSecond = TicksPerMillisecond * 100;

		static constexpr TimeSpan Zero() { return TimeSpan(); }
		static constexpr TimeSpan MaxValue() { return TimeSpan(MaxTicks); }
		static constexpr TimeSpan MinValue() { return TimeSpan(MinTicks); }

		constexpr TimeSpan() = default;
		constexpr TimeSpan(int64_t ticks) : _ticks(ticks) {}
		constexpr TimeSpan(int32_t hours, int32_t minutes, int32_t seconds) {
			_ticks = TimeToTicks(hours, minutes, seconds);
		}
		constexpr TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds, int32_t microseconds = 0) {
			_ticks = TimeToTicks(days, hours, minutes, seconds, milliseconds, microseconds);
		}

		constexpr int64_t Ticks() const { return _ticks; }
		constexpr int32_t Days() const { return static_cast<int32_t>(_ticks / TicksPerDay); }
		constexpr int32_t Hours() const { return static_cast<int32_t>(_ticks / TicksPerHour % HoursPerDay); }
		constexpr int32_t Milliseconds() const { return static_cast<int32_t>(_ticks / TicksPerMillisecond % MillisecondsPerSecond); }
		constexpr int32_t Microseconds() const { return static_cast<int32_t>(_ticks / TicksPerMicrosecond % MicrosecondsPerMillisecond); }
		constexpr int32_t Nanoseconds() const { return static_cast<int32_t>(_ticks % TicksPerMicrosecond * NanosecondsPerTick); }
		constexpr int32_t Minutes() const { return static_cast<int32_t>(_ticks / TicksPerMinute % MinutesPerHour); }
		constexpr int32_t Seconds() const { return static_cast<int32_t>(_ticks / TicksPerSecond % SecondsPerMinute); }
		constexpr double TotalDays() const { return static_cast<double>(_ticks) / TicksPerDay; }
		constexpr double TotalHours() const { return static_cast<double>(_ticks) / TicksPerHour; }

		constexpr double TotalMilliseconds() const {
			double temp = static_cast<double>(_ticks) / TicksPerMillisecond;

			if (temp > MaxMilliseconds) {
				return MaxMilliseconds;
			}

			if (temp < MinMilliseconds) {
				return MinMilliseconds;
			}
			return temp;
		}

		constexpr double TotalMicroseconds() const { return static_cast<double>(_ticks) / TicksPerMicrosecond; }
		constexpr double TotalNanoseconds() const { return static_cast<double>(_ticks) * NanosecondsPerTick; }
		constexpr double TotalMinutes() const { return static_cast<double>(_ticks) / TicksPerMinute; }
		constexpr double TotalSeconds() const { return static_cast<double>(_ticks) / TicksPerSecond; }

		constexpr TimeSpan Add(TimeSpan const& ts) const {
			int64_t result = _ticks + ts._ticks;
			int64_t t1Sign = _ticks >> 63;

			if ((t1Sign == (ts._ticks >> 63)) && (t1Sign != (result >> 63))) {
				return TimeSpan::Zero();
				//exception
			}

			return result;
		}

		static TimeSpan FromDays(double value) {
			return Interval(value, TicksPerDay);
		}

		constexpr TimeSpan Duration() const {
			if (_ticks == MinTicks) {
				return TimeSpan::Zero();
			}

			return TimeSpan(_ticks >= 0 ? _ticks : -_ticks);
		}

		static constexpr TimeSpan FromUnits(int64_t units, int64_t ticksPerUnit, int64_t minUnits, int64_t maxUnits) {
			if (units > maxUnits || units < minUnits) {
				return TimeSpan::Zero();
			}

			return TimeSpan::FromTicks(units * ticksPerUnit);
		}

		static constexpr TimeSpan FromDays(int32_t days) {
			return FromUnits(days, TicksPerDay, MinDays, MaxDays);
		}

		//TODO: Not implemented.
		//static constexpr TimeSpan FromDays(int32_t days, int32_t hours = 0, int64_t minutes = 0, int64_t seconds = 0, int64_t milliseconds = 0, int64_t microseconds = 0);

		static constexpr TimeSpan FromHours(int32_t hours) {
			return FromUnits(hours, TicksPerHour, MinHours, MaxHours);
		}

		//TODO: Not implemented.
		//static constexpr TimeSpan FromHours(int32_t hours, int64_t minutes = 0, int64_t seconds = 0, int64_t milliseconds = 0, int64_t microseconds = 0);

		static constexpr TimeSpan FromMinutes(int64_t minutes) {
			return FromUnits(minutes, TicksPerMinute, MinMinutes, MaxMinutes);
		}

		//TODO: Not implemented.
		//static constexpr TimeSpan FromMinutes(int64_t minutes, int64_t seconds = 0, int64_t milliseconds = 0, int64_t microseconds = 0);

		static constexpr TimeSpan FromSeconds(int64_t seconds) {
			return FromUnits(seconds, TicksPerSecond, MinSeconds, MaxSeconds);
		}

		//TODO: Not implemented.
		//static constexpr TimeSpan FromSeconds(int64_t seconds, int64_t milliseconds = 0, int64_t microseconds = 0);

		//TODO: Not implemented.
		//static constexpr TimeSpan FromMilliseconds(int64_t milliseconds, int64_t microseconds = 0);

		static constexpr TimeSpan FromMicroseconds(int64_t microseconds) {
			return FromUnits(microseconds, TicksPerMicrosecond, MinMicroseconds, MaxMicroseconds);
		}

		static TimeSpan FromHours(double value) {
			return Interval(value, TicksPerHour);
		}

		static constexpr int64_t TimeToTicks(int32_t hour, int32_t minute, int32_t second) {
			int64_t totalSeconds =
				(hour * SecondsPerHour)
				+ (minute * SecondsPerMinute)
				+ second;

			//exception
			if (totalSeconds > MaxSeconds) {
				return MaxValue()._ticks;
			}
			else if (totalSeconds < MinSeconds) {
				return MinValue()._ticks;
			}

			return totalSeconds * TicksPerSecond;
		}

		static TimeSpan FromMilliseconds(double value) {
			return Interval(value, TicksPerMillisecond);
		}

		static TimeSpan FromMicroseconds(double value) {
			return Interval(value, TicksPerMicrosecond);
		}

		static TimeSpan FromMinutes(double value) {
			return Interval(value, TicksPerMinute);
		}

		constexpr TimeSpan Negate() const {
			if (_ticks == MinTicks) {
				return MinTicks;
			}

			return -_ticks;
		}

		static TimeSpan FromSeconds(double value) {
			return Interval(value, TicksPerSecond);
		}

		constexpr TimeSpan Subtract(TimeSpan const& ts) const {
			int64_t result = _ticks - ts._ticks;
			int64_t t1Sign = _ticks >> 63;

			if ((t1Sign != (ts._ticks >> 63)) && (t1Sign != (result >> 63))) {
				return TimeSpan::Zero();
				//exception
			}

			return result;
		}

		TimeSpan Multiply(double factor) const {
			if (isnan(factor)) {
				return TimeSpan::Zero();
				//exception
			}

			const auto ticks = std::round(_ticks * factor);
			return IntervalFromDoubleTicks(ticks);
		}

		TimeSpan Divide(double divisor) const {
			if (isnan(divisor)) {
				return TimeSpan::Zero();
				//exception
			}

			const auto ticks = std::round(_ticks / divisor);
			return IntervalFromDoubleTicks(ticks);
		}

		constexpr double Divide(TimeSpan const& ts) const {
			return  _ticks / static_cast<double>(ts._ticks);
		}

		static constexpr TimeSpan FromTicks(int64_t value) {
			return TimeSpan(value);
		}

		static constexpr int64_t TimeToTicks(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds, int32_t microseconds) {
			int64_t totalMicroseconds =
				(days * MicrosecondsPerDay)
				+ (hours * MicrosecondsPerHour)
				+ (minutes * MicrosecondsPerMinute)
				+ (seconds * MicrosecondsPerSecond)
				+ (milliseconds * MicrosecondsPerMillisecond)
				+ microseconds;

			//exception
			if (totalMicroseconds > MaxMicroseconds) {
				return MaxValue()._ticks;
			}
			else if (totalMicroseconds < MinMicroseconds) {
				return MinValue()._ticks;
			}

			return totalMicroseconds * TicksPerMicrosecond;
		}

		constexpr bool operator==(TimeSpan const& other) const {
			return _ticks == other._ticks;
		}

		constexpr TimeSpan operator-() const {
			return Negate();
		}

		friend constexpr TimeSpan operator-(TimeSpan const& a, TimeSpan const& b) {
			return a.Subtract(b);
		}

		friend constexpr TimeSpan operator+(TimeSpan const& a, TimeSpan const& b) {
			return a.Add(b);
		}

		friend TimeSpan operator*(TimeSpan const& a, double factor) {
			return a.Multiply(factor);
		}

		friend TimeSpan operator*(double factor, TimeSpan const& b) {
			return b.Multiply(factor);
		}		

		friend double operator/(TimeSpan const& a, TimeSpan const& b) {
			return a.Divide(b);
		}

		friend TimeSpan operator<(TimeSpan const& a, TimeSpan const& b) {
			return a._ticks < b._ticks;
		}

		friend TimeSpan operator<=(TimeSpan const& a, TimeSpan const& b) {
			return a._ticks <= b._ticks;
		}

	private:
		int64_t _ticks{ 0 };

		static TimeSpan Interval(double value, double scale) {
			if (isnan(value)) {
				//exception
				return TimeSpan::Zero();
			}

			return IntervalFromDoubleTicks(value * scale);
		}

		static constexpr TimeSpan IntervalFromDoubleTicks(double ticks) {
			if ((ticks > MaxTicks) || (ticks < MinTicks) || isnan(ticks)) {
				//exception
				return TimeSpan::Zero();
			}

			if (ticks == MaxTicks) {
				return MaxValue();
			}

			return TimeSpan(static_cast<int64_t>(ticks));
		}

		//TODO: Not implemented.
		//static TimeSpan FromMicroseconds(__int128 microseconds);
	};
}

#endif