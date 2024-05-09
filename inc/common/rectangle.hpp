#ifndef XNA_COMMON_RECTANGLE_HPP
#define XNA_COMMON_RECTANGLE_HPP

#include "../types.hpp"
#include "point.hpp"

namespace xna {
	struct Rectangle {
		Int X{ 0 };
		Int Y{ 0 };
		Int Width{ 0 };
		Int Height{ 0 };

		constexpr Rectangle() = default;

		constexpr Rectangle(const Int& X, const Int& Y, const Int& Width, const Int& Height):
			X(X), Y(Y), Width(Width), Height(Height) {}

		constexpr bool operator==(const Rectangle& other) const	{
			return Height == other.Height && Width == other.Width && X == other.X && Y == other.Y;
		}

		constexpr Int Left() const { return X; }
		constexpr Int Right() const { return X + Width; }
		constexpr Int Top() const { return Y; }
		constexpr Int Bottom() const { return Y + Height; }

		constexpr Point Location() const { return { X, Y }; }
		constexpr void Location(Point const& p) { 
			X = p.X;
			Y = p.Y;
		}

		constexpr Point Center() const { return { X + Width / 2, Y + Height / 2 }; }

		constexpr static Rectangle Empty() { return {}; }

		constexpr bool IsEmpty() const { return Width == 0 && Height == 0 && X == 0 && Y == 0; }

		constexpr void Offset(Point const& amount) {
			X += amount.X;
			Y += amount.Y;
		}

		constexpr void Offset(Int x, Int y) {
			X += x;
			Y += y;
		}

		constexpr void Inflate(Int horizontalAmount, Int verticalAmount) {
			X -= horizontalAmount;
			Y -= verticalAmount;
			Width += horizontalAmount * 2;
			Height += verticalAmount * 2;
		}

		constexpr bool Contains(Int x, Int y) const {
			return X <= x && x < X + Width && Y <= y && y < Y + Height;
		}

		constexpr bool Contains(Point const& value) const {
			return X <= value.X && value.X < X + Width && Y <= value.Y && value.Y < Y + Height;
		}

		constexpr bool Contains(Rectangle const& value) const {
			return X <= value.X && value.X + value.Width <= X + Width && Y <= value.Y && value.Y + value.Height <= Y + Height;
		}

		constexpr bool Intersects(Rectangle const& value) const {
			return value.X < X + Width && X < value.X + value.Width && value.Y < Y + Height && Y < value.Y + value.Height;
		}

		constexpr static Rectangle Intersect(Rectangle const& value1, Rectangle const& value2) {
			const auto left1 = value1.Left();
			const auto left2 = value2.Left();
			const auto bottom1 = value1.Bottom();
			const auto bottom2 = value2.Bottom();
			const auto maxX = value1.X > value2.X ? value1.X : value2.X;
			const auto maxY = value1.Y > value2.Y ? value1.Y : value2.Y;
			const auto maxl = left1 < left2 ? left1 : left2;
			const auto maxb = bottom1 < bottom2 ? bottom1 : bottom2;

			Rectangle rectangle{};

			if (maxl > maxX && maxb > maxY)	{
				rectangle.X = maxX;
				rectangle.Y = maxY;
				rectangle.Width = maxl - maxX;
				rectangle.Height = maxb - maxY;
			}
			
			return rectangle;
		}

		constexpr static Rectangle Union(Rectangle const& value1, Rectangle const& value2) {
			const auto left1 = value1.Left();
			const auto left2 = value2.Left();
			const auto bottom1 = value1.Bottom();
			const auto bottom2 = value2.Bottom();
			const auto minX = value1.X < value2.X ? value1.X : value2.X;
			const auto miny = value1.Y < value2.Y ? value1.Y : value2.Y;
			const auto maxl = left1 > left2 ? left1 : left2;
			const auto maxb = bottom1 > bottom2 ? bottom1 : bottom2;

			Rectangle rectangle;
			rectangle.X = minX;
			rectangle.Y = miny;
			rectangle.Width = maxl - minX;
			rectangle.Height = maxb - miny;
			return rectangle;
		}
	};
}

#endif