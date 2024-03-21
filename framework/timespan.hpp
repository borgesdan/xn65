#ifndef XNA_TIMESPAN_HPP
#define XNA_TIMESPAN_HPP

#include <limits>
#include <cstdint>
#include "types.hpp"

namespace xna {
	struct TimeSpan {
		static constexpr Long TicksPerMillisecond{ 10000 };
		static constexpr Long TicksPerSecond{ TicksPerMillisecond * 1000 };
		static constexpr Long TicksPerMinute{ TicksPerSecond * 60 };
		static constexpr Long TicksPerHour{ TicksPerMinute * 60 };
		static constexpr Long TicksPerDay{ TicksPerHour * 24 };

		constexpr TimeSpan() = default;
		constexpr TimeSpan(Long ticks) :
			_ticks(ticks) {}
		constexpr TimeSpan(Int hours, Int minutes, Int seconds) :
			_ticks(TimeToTicks(hours, minutes, seconds)) {}
		constexpr TimeSpan(Int days, Int hours, Int minutes, Int seconds) :
			_ticks(DayToTicks(days, hours, minutes, seconds, 0)) {}
		constexpr TimeSpan(Int days, Int hours, Int minutes, Int seconds, Int milliseconds) :
			_ticks(DayToTicks(days, hours, minutes, seconds, milliseconds)) {}

		constexpr TimeSpan operator -(TimeSpan const& t) {
			return TimeSpan(-t._ticks);
		}

		constexpr TimeSpan operator +(TimeSpan const& t) {
			return t;
		}

		constexpr friend TimeSpan operator +(TimeSpan const& t1, TimeSpan const& t2) {
			return t1.Add(t2);
		}

		constexpr friend TimeSpan operator -(TimeSpan const& t1, TimeSpan const& t2) {
			return t1.Subtract(t2);
		}

		constexpr friend bool operator ==(TimeSpan const& t1, TimeSpan const& t2) {
			return t1._ticks == t2._ticks;
		}

		constexpr friend bool operator !=(TimeSpan const& t1, TimeSpan const& t2) {
			return t1._ticks != t2._ticks;
		}

		constexpr friend bool operator <(TimeSpan const& t1, TimeSpan const& t2) {
			return t1._ticks < t2._ticks;
		}

		constexpr friend bool operator <=(TimeSpan const& t1, TimeSpan const& t2) {
			return t1._ticks <= t2._ticks;
		}

		constexpr friend bool operator >(TimeSpan const& t1, TimeSpan const& t2) {
			return t1._ticks > t2._ticks;
		}

		constexpr friend bool operator >=(TimeSpan const& t1, TimeSpan const& t2) {
			return t1._ticks >= t2._ticks;
		}

		static constexpr TimeSpan Zero() {
			return TimeSpan();
		}

		static constexpr TimeSpan MaxValue() {
			return TimeSpan(std::numeric_limits<Long>::max());
		}

		static constexpr TimeSpan MinValue() {
			return TimeSpan(std::numeric_limits<Long>::min());
		}

		static constexpr Int Compare(TimeSpan const& t1, TimeSpan const& t2) {
			if (t1._ticks > t2._ticks)
				return 1;

			if (t1._ticks < t2._ticks)
				return -1;

			return 0;
		}

		static constexpr TimeSpan FromDays(double value) {
			return Interval(value, MillisPerDay);
		}

		static constexpr TimeSpan FromHours(double value) {
			return Interval(value, MillisPerHour);
		}

		static constexpr TimeSpan Interval(double value, Int scale) {
			double tmp = value * scale;
			double millis = tmp + (value >= 0 ? 0.5 : -0.5);

			return TimeSpan(static_cast<Long>(millis) * TicksPerMillisecond);
		}

		static constexpr TimeSpan FromMilliseconds(double value) {
			return Interval(value, 1);
		}

		static constexpr TimeSpan FromMinutes(double value) {
			return Interval(value, MillisPerMinute);
		}

		static constexpr TimeSpan FromSeconds(double value) {
			return Interval(value, MillisPerSecond);
		}

		static constexpr TimeSpan FromTicks(Long value) {
			return TimeSpan(value);
		}

		constexpr Long Ticks() const { return _ticks; }
		constexpr Int Days() const { return static_cast<Int>(_ticks / TicksPerDay); }
		constexpr Int Hours() const { return static_cast<Int>((_ticks / TicksPerHour) % 24); }
		constexpr Int Milliseconds() const { return static_cast<Int>((_ticks / TicksPerMillisecond) % 1000); }
		constexpr Int Minutes() const { return static_cast<Int>((_ticks / TicksPerMinute) % 60); }
		constexpr Int Seconds() const { return static_cast<Int>((_ticks / TicksPerSecond) % 60); }
		constexpr double TotalDays() const { return static_cast<double>(_ticks) * DaysPerTick; }
		constexpr double TotalHours() const { return static_cast<double>(_ticks) * HoursPerTick; }

		constexpr double TotalMilliseconds() const {
			double temp = static_cast<double>(_ticks) * MillisecondsPerTick;

			if (temp > MaxMilliSeconds)
				return  static_cast<double>(MaxMilliSeconds);

			if (temp < MinMilliSeconds)
				return  static_cast<double>(MinMilliSeconds);

			return temp;
		}

		constexpr double TotalMinutes() const { return static_cast<double>(_ticks) * MinutesPerTick; }
		constexpr double TotalSeconds() const { return static_cast<double>(_ticks) * SecondsPerTick; }

		constexpr TimeSpan Add(TimeSpan const& ts) const {
			Long result = _ticks + ts._ticks;

			if ((_ticks >> 63 == ts._ticks >> 63) && (_ticks >> 63 != result >> 63)) {
				return TimeSpan(result, true);
			}

			return TimeSpan(result);
		}

		constexpr TimeSpan Duration() const {
			if (Ticks() == TimeSpan::MinValue().Ticks()) {
				return TimeSpan(Ticks(), true);
			}

			return TimeSpan(_ticks >= 0 ? _ticks : -_ticks);
		}

		constexpr bool Equals(TimeSpan other) const {
			return _ticks == other._ticks;
		}

		constexpr TimeSpan Negate() const {
			if (Ticks() == TimeSpan::MinValue().Ticks()) {
				return TimeSpan(Ticks(), true);
			}

			return TimeSpan(-_ticks);
		}

		constexpr TimeSpan Subtract(TimeSpan const& ts) const {
			Long result = _ticks - ts._ticks;

			if ((_ticks >> 63 != ts._ticks >> 63) && (_ticks >> 63 != result >> 63)) {
				return TimeSpan(result, true);
			}

			return TimeSpan(result);
		}

		constexpr bool HasOverflowException() {
			return hasOverflowException;
		}

	private:
		constexpr TimeSpan(Long ticks, bool overflow) :
			_ticks(ticks),
			hasOverflowException(overflow) {
		}

		static constexpr double MillisecondsPerTick = 1.0 / TicksPerMillisecond;
		static constexpr double SecondsPerTick = 1.0 / TicksPerSecond;
		static constexpr double MinutesPerTick = 1.0 / TicksPerMinute;
		static constexpr double HoursPerTick = 1.0 / TicksPerHour;
		static constexpr double DaysPerTick = 1.0 / TicksPerDay;
		static constexpr Int MillisPerSecond = 1000;
		static constexpr Int MillisPerMinute = MillisPerSecond * 60;
		static constexpr Int MillisPerHour = MillisPerMinute * 60;
		static constexpr Int MillisPerDay = MillisPerHour * 24;
		static constexpr Long MaxSeconds = std::numeric_limits<Long>::max() / TicksPerSecond;
		static constexpr Long MinSeconds = std::numeric_limits<Long>::min() / TicksPerSecond;
		static constexpr Long MaxMilliSeconds = std::numeric_limits<Long>::max() / TicksPerMillisecond;
		static constexpr Long MinMilliSeconds = std::numeric_limits<Long>::min() / TicksPerMillisecond;
		static constexpr Long TicksPerTenthSecond = TicksPerMillisecond * 100;

		bool hasOverflowException{ false };
		Long _ticks{ 0 };

		constexpr static Long TimeToTicks(Int const& hour, Int const& minute, Int const& second) {
			Long totalSeconds =
				static_cast<Long>(hour) * 3600 +
				static_cast<Long>(minute) * 60 +
				static_cast<Long>(second);

			return totalSeconds * TicksPerSecond;
		}

		constexpr static Long DayToTicks(Int days, Int hours, Int minutes, Int seconds, Int milliseconds) {
			Long totalMilliSeconds =
				(static_cast<Long>(days) * 3600 * 24 +
					static_cast<Long>(hours) * 3600 +
					static_cast<Long>(minutes) * 60 + seconds) * 1000 + milliseconds;

			return totalMilliSeconds * TicksPerMillisecond;
		}
	};
}

#endif