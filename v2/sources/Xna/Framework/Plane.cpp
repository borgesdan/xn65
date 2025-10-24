#include "Xna/Framework/Plane.hpp"
#include <cmath>
#include "Xna/Framework/Ray.hpp"

namespace Xna {
	Plane::Plane(Vector3 const& point1, Vector3 const& point2, Vector3 const& point3) {
		const auto num1 = point2.X - point1.X;
		const auto num2 = point2.Y - point1.Y;
		const auto num3 = point2.Z - point1.Z;
		const auto num4 = point3.X - point1.X;
		const auto num5 = point3.Y - point1.Y;
		const auto num6 = point3.Z - point1.Z;
		const auto num7 = (num2 * num6 - num3 * num5);
		const auto num8 = (num3 * num4 - num1 * num6);
		const auto num9 = (num1 * num5 - num2 * num4);
		const auto num10 = 1.0f / std::sqrt(num7 * num7 + num8 * num8 + num9 * num9);
		Normal.X = num7 * num10;
		Normal.Y = num8 * num10;
		Normal.Z = num9 * num10;
		D = -(Normal.X * point1.X + Normal.Y * point1.Y + Normal.Z * point1.Z);
	}

	void Plane::Normalize() {
		const auto d = (Normal.X * Normal.X + Normal.Y * Normal.Y + Normal.Z * Normal.Z);

		if (std::abs(d - 1.0f) < 1.1920928955078125E-07)
			return;

		const auto num = 1.0f / std::sqrt(d);

		Normal.X *= num;
		Normal.Y *= num;
		Normal.Z *= num;
		D *= num;
	}

	Plane Plane::Normalize(Plane const& value) {
		auto p = value;
		p.Normalize();
		return p;
	}

	std::optional<float> Plane::Intersects(Ray const& ray) const {
		const auto num1 = (Normal.X * ray.Direction.X + Normal.Y * ray.Direction.Y + Normal.Z * ray.Direction.Z);

		if (std::abs(num1) < 9.9999997473787516E-06)
			return {};

		const auto num2 = (Normal.X * ray.Position.X + Normal.Y * ray.Position.Y + Normal.Z * ray.Position.Z);
		auto num3 = (-D - num2) / num1;

		if (num3 < 0.0)
		{
			if (num3 < -9.9999997473787516E-06)
				return {};
			num3 = 0.0f;
		}

		return num3;
	}
}