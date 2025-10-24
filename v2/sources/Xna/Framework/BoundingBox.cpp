#include "Xna/Framework/BoundingBox.hpp"
#include "Xna/Framework/Ray.hpp"
#include "Xna/Framework/MathHelper.hpp"

namespace Xna {
	std::optional<float> BoundingBox::Intersects(Ray const& ray) const {
		float num1 = 0.0f;
		float num2 = FLOAT_MAX_VALUE;

		if (std::abs(ray.Direction.X) < 9.9999999747524271E-07)
		{
			if (ray.Position.X < Min.X || ray.Position.X > Max.X)
				return {};
		}
		else
		{
			const auto num3 = 1.0f / ray.Direction.X;
			auto num4 = (Min.X - ray.Position.X) * num3;
			auto num5 = (Max.X - ray.Position.X) * num3;
			if (num4 > num5)
			{
				float num6 = num4;
				num4 = num5;
				num5 = num6;
			}
			num1 = MathHelper::Max(num4, num1);
			num2 = MathHelper::Min(num5, num2);
			if (num1 > num2)
				return {};
		}
		if (std::abs(ray.Direction.Y) < 9.9999999747524271E-07)
		{
			if (ray.Position.Y < Min.Y || ray.Position.Y > Max.Y)
				return {};
		}
		else
		{
			const auto num7 = 1.0f / ray.Direction.Y;
			auto num8 = (Min.Y - ray.Position.Y) * num7;
			auto num9 = (Max.Y - ray.Position.Y) * num7;
			if (num8 > num9)
			{
				float num10 = num8;
				num8 = num9;
				num9 = num10;
			}
			num1 = MathHelper::Max(num8, num1);
			num2 = MathHelper::Min(num9, num2);
			if (num1 > num2)
				return {};
		}
		if (std::abs(ray.Direction.Z) < 9.9999999747524271E-07)
		{
			if (ray.Position.Z < Min.Z || ray.Position.Z > Max.Z)
				return {};
		}
		else
		{
			const auto num11 = 1.0f / ray.Direction.Z;
			auto num12 = (Min.Z - ray.Position.Z) * num11;
			auto num13 = (Max.Z - ray.Position.Z) * num11;
			if (num12 > num13)
			{
				float num14 = num12;
				num12 = num13;
				num13 = num14;
			}
			num1 = MathHelper::Max(num12, num1);
			const auto num15 = MathHelper::Min(num13, num2);
			if (num1 > num15)
				return {};
		}
		return num1;
	}

	ContainmentType BoundingBox::Contains(BoundingFrustum& frustum) const {
		if (!frustum.Intersects(*this))
			return ContainmentType::Disjoint;

		for (size_t i = 0; i < frustum.CornerCount; ++i) {
			const auto corner = frustum[i];

			if (Contains(corner) == ContainmentType::Disjoint)
				return ContainmentType::Intersects;
		}
		return ContainmentType::Contains;
	}
}