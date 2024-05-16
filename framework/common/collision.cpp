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
}