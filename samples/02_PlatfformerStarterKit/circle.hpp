#ifndef PLATFORMSTARTERKIT_CIRCLE_HPP
#define PLATFORMSTARTERKIT_CIRCLE_HPP

#include "headers.hpp"

namespace PlatformerStarterKit {
	//Represents a 2D circle.
	struct Circle {
		xna::Vector2 Center{};
		float Radius{ 0 };

		constexpr Circle() = default;
		constexpr Circle(xna::Vector2 const& position, float radius):
			Center(position), Radius(radius){}

		constexpr bool Intersects(xna::Rectangle const& rectangle) const {
			const auto v = 
				xna::Vector2(xna::MathHelper::Clamp(Center.X, static_cast<float>(rectangle.Left()), static_cast<float>(rectangle.Right())),
				xna::MathHelper::Clamp(Center.Y, static_cast<float>(rectangle.Top()), static_cast<float>(rectangle.Bottom())));

			const auto direction = Center - v;
			auto distanceSquared = direction.LengthSquared();

			return ((distanceSquared > 0) && (distanceSquared < Radius * Radius));
		}
	};
}

#endif