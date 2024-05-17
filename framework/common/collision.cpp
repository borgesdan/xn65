#include "common/collision.hpp"

namespace xna {
	bool BoundingFrustum::Intersects(BoundingBox const& box) {
		gjk.Reset();
		Vector3 result1 = Vector3::Subtract(corners[0], box.Min);

		if (result1.LengthSquared() < 9.9999997473787516E-06)
			result1 = Vector3::Subtract(corners[0], box.Max);

		auto num1 = FloatMaxValue;
		float num2 = 0;
		do
		{
			Vector3 v;
			v.X = -result1.X;
			v.Y = -result1.Y;
			v.Z = -result1.Z;
			Vector3 result2;
			SupportMapping(v, result2);
			Vector3 result3;
			box.SupportMapping(result1, result3);
			Vector3 result4 = Vector3::Subtract(result2, result3);

			if (result1.X * result4.X + result1.Y * result4.Y + result1.Z * result4.Z > 0.0)
				return false;

			gjk.AddSupportPoint(result4);
			result1 = gjk.ClosestPoint();
			float num3 = num1;
			num1 = result1.LengthSquared();

			if (num3 - num1 <= 9.9999997473787516E-06 * num3)
				return false;

			num2 = 4E-05f * gjk.MaxLengthSquared();
		} while (!gjk.FullSimplex() && num1 >= num2);

		return true;
	}

	std::optional<float> BoundingFrustum::Intersects(Ray const& ray) const {
		auto result1 = Contains(ray.Position);

		if (result1 == ContainmentType::Contains)
			return 0.0F;

		auto num1 = FloatMinValue;
		auto num2 = FloatMaxValue;

		for (size_t i = 0; i < planes.size(); ++i) {
			const auto& plane = planes[i];

			Vector3 normal = plane.Normal;
			const auto result2 = Vector3::Dot(ray.Direction, normal);
			float result3 = Vector3::Dot(ray.Position, normal);

			result3 += plane.D;

			if (std::abs(static_cast<double>(result2)) < 9.9999997473787516E-06)
			{
				if (result3 > 0.0)
					return {};
			}
			else
			{
				const auto num3 = -result3 / result2;
				if (result2 < 0.0)
				{
					if (num3 > num2)
						return {};
					if (num3 > num1)
						num1 = num3;
				}
				else
				{
					if (num3 < num1)
						return {};
					if (num3 < num2)
						num2 = num3;
				}
			}
		}

		auto num4 = num1 >= 0.0F ? num1 : num2;

		if (num4 < 0.0)
			return{};

		return num4;
	}

	bool BoundingFrustum::Intersects(BoundingSphere const& sphere) {
		gjk.Reset();
		auto result1 = Vector3::Subtract(corners[0], sphere.Center);

		if (result1.LengthSquared() < 9.9999997473787516E-06)
			result1 = Vector3::UnitX();

		auto num1 = FloatMaxValue;
		auto num2 = 0.0F;

		do
		{
			Vector3 v;
			v.X = -result1.X;
			v.Y = -result1.Y;
			v.Z = -result1.Z;

			Vector3 result2;
			SupportMapping(v, result2);

			Vector3 result3;
			sphere.SupportMapping(result1, result3);

			Vector3 result4 = Vector3::Subtract(result2, result3);

			if (result1.X * result4.X + result1.Y * result4.Y + result1.Z * result4.Z > 0.0)
				return false;

			gjk.AddSupportPoint(result4);

			result1 = gjk.ClosestPoint();

			auto num3 = num1;

			num1 = result1.LengthSquared();

			if (num3 - num1 <= 9.9999997473787516E-06 * num3)
				return false;

			num2 = 4E-05f * gjk.MaxLengthSquared();

		} while (!gjk.FullSimplex() && num1 >= num2);

		return true;
	}

	bool BoundingFrustum::Intersects(BoundingFrustum const& frustum) {
		gjk.Reset();
		Vector3 result1 = Vector3::Subtract(corners[0], frustum.corners[0]);

		if (result1.LengthSquared() < 9.9999997473787516E-06)
			result1 = Vector3::Subtract(corners[0], frustum.corners[1]);

		float num1 = FloatMaxValue;
		float num2 = 0;

		do
		{
			Vector3 v;
			v.X = -result1.X;
			v.Y = -result1.Y;
			v.Z = -result1.Z;

			Vector3 result2;
			SupportMapping(v, result2);

			Vector3 result3;
			frustum.SupportMapping(result1, result3);

			const auto result4 = Vector3::Subtract(result2, result3);

			if (result1.X * result4.X + result1.Y * result4.Y + result1.Z * result4.Z > 0.0)
				return false;

			gjk.AddSupportPoint(result4);

			result1 = gjk.ClosestPoint();

			const auto num3 = num1;

			num1 = result1.LengthSquared();
			num2 = 4E-05f * gjk.MaxLengthSquared();

			if (num3 - num1 <= 9.9999997473787516E-06 * num3)
				return false;

		} while (!gjk.FullSimplex() && num1 >= num2);
		return true;
	}

	ContainmentType BoundingFrustum::Contains(BoundingFrustum const& frustum) {
		ContainmentType containmentType = ContainmentType::Disjoint;

		if (Intersects(frustum)) {
			containmentType = ContainmentType::Contains;

			for (size_t index = 0; index < corners.size(); ++index) {
				if (Contains(frustum.corners[index]) == ContainmentType::Disjoint)
				{
					containmentType = ContainmentType::Intersects;
					break;
				}
			}
		}
		return containmentType;
	}

	std::optional<float> BoundingBox::Intersects(Ray const& ray) const {
		float num1 = 0.0f;
		float num2 = FloatMaxValue;

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