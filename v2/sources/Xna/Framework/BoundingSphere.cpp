#include "Xna/Framework/BoundingSphere.hpp"
#include "Xna/Framework/Ray.hpp"
#include "Xna/Framework/MathHelper.hpp"

namespace Xna {
	BoundingSphere BoundingSphere::CreateFromBoundingBox(BoundingBox const& box) {
		BoundingSphere fromBoundingBox;
		fromBoundingBox.Center = Vector3::Lerp(box.Min, box.Max, 0.5f);
		const auto result = Vector3::Distance(box.Min, box.Max);
		fromBoundingBox.Radius = result * 0.5f;
		return fromBoundingBox;
	}

	BoundingSphere BoundingSphere::CreateFromFrustum(BoundingFrustum const& frustum) {
		return BoundingSphere::CreateFromPoints(frustum.corners, frustum.CornerCount);
	}

	BoundingSphere BoundingSphere::CreateFromPoints(Vector3 const* points, size_t size) {
		Vector3 current;
		auto vector3_1 = current = points[0];
		auto vector3_2 = current;
		auto vector3_3 = current;
		auto vector3_4 = current;
		auto vector3_5 = current;
		auto vector3_6 = current;

		for (size_t i = 0; i < size; ++i) {
			const auto& point = points[i];

			if (point.X < vector3_6.X)
				vector3_6 = point;
			if (point.X > vector3_5.X)
				vector3_5 = point;
			if (point.Y < vector3_4.Y)
				vector3_4 = point;
			if (point.Y > vector3_3.Y)
				vector3_3 = point;
			if (point.Z < vector3_2.Z)
				vector3_2 = point;
			if (point.Z > vector3_1.Z)
				vector3_1 = point;
		}

		auto result1 = Vector3::Distance(vector3_5, vector3_6);
		auto result2 = Vector3::Distance(vector3_3, vector3_4);
		auto result3 = Vector3::Distance(vector3_1, vector3_2);
		Vector3 result4;
		float num1 = 0;

		if (result1 > result2)
		{
			if (result1 > result3)
			{
				result4 = Vector3::Lerp(vector3_5, vector3_6, 0.5f);
				num1 = result1 * 0.5f;
			}
			else
			{
				result4 = Vector3::Lerp(vector3_1, vector3_2, 0.5f);
				num1 = result3 * 0.5f;
			}
		}
		else if (result2 > result3)
		{
			result4 = Vector3::Lerp(vector3_3, vector3_4, 0.5f);
			num1 = result2 * 0.5f;
		}
		else
		{
			result4 = Vector3::Lerp(vector3_1, vector3_2, 0.5f);
			num1 = result3 * 0.5f;
		}

		for (size_t i = 0; i < size; ++i) {
			const auto& point = points[i];

			Vector3 vector3_7;
			vector3_7.X = point.X - result4.X;
			vector3_7.Y = point.Y - result4.Y;
			vector3_7.Z = point.Z - result4.Z;
			float num2 = vector3_7.Length();

			if (num2 > num1) {
				num1 = (num1 + num2) * 0.5F;
				result4 += (1.0F - num1 / num2) * vector3_7;
			}
		}

		BoundingSphere fromPoints;
		fromPoints.Center = result4;
		fromPoints.Radius = num1;
		return fromPoints;
	}

	ContainmentType BoundingSphere::Contains(BoundingFrustum& frustum) const {
		if (!frustum.Intersects(*this))
			return ContainmentType::Disjoint;

		float num = Radius * Radius;
		for (size_t i = 0; i < BoundingFrustum::CornerCount; ++i)
		{
			const auto corner = frustum[i];

			Vector3 vector3;
			vector3.X = corner.X - Center.X;
			vector3.Y = corner.Y - Center.Y;
			vector3.Z = corner.Z - Center.Z;

			if (vector3.LengthSquared() > num)
				return ContainmentType::Intersects;
		}

		return ContainmentType::Contains;
	}

	ContainmentType BoundingSphere::Contains(BoundingSphere const& sphere) const {
		const auto result = Vector3::Distance(Center, sphere.Center);
		const auto radius1 = Radius;
		const auto radius2 = sphere.Radius;
		if (radius1 + radius2 < result)
			return ContainmentType::Disjoint;
		return radius1 - radius2 < result
			? ContainmentType::Intersects
			: ContainmentType::Contains;
	}

	void BoundingSphere::SupportMapping(Vector3 const& v, Vector3& result) const {
		const auto num = Radius / v.Length();
		result.X = Center.X + v.X * num;
		result.Y = Center.Y + v.Y * num;
		result.Z = Center.Z + v.Z * num;
	}

	BoundingSphere BoundingSphere::Transform(Matrix const& matrix) const {
		BoundingSphere boundingSphere;
		boundingSphere.Center = Vector3::Transform(Center, matrix);
		float d = MathHelper::Max((matrix.M11 * matrix.M11 + matrix.M12 * matrix.M12 + matrix.M13 * matrix.M13), MathHelper::Max((matrix.M21 * matrix.M21 + matrix.M22 * matrix.M22 + matrix.M23 * matrix.M23), (matrix.M31 * matrix.M31 + matrix.M32 * matrix.M32 + matrix.M33 * matrix.M33)));
		boundingSphere.Radius = Radius * std::sqrt(d);
		return boundingSphere;
	}

	ContainmentType BoundingSphere::Contains(BoundingBox const& box) const {
		if (!box.Intersects(*this))
			return ContainmentType::Disjoint;
		float num = Radius * Radius;
		Vector3 vector3;
		vector3.X = Center.X - box.Min.X;
		vector3.Y = Center.Y - box.Max.Y;
		vector3.Z = Center.Z - box.Max.Z;
		if (vector3.LengthSquared() > num)
			return ContainmentType::Intersects;
		vector3.X = Center.X - box.Max.X;
		vector3.Y = Center.Y - box.Max.Y;
		vector3.Z = Center.Z - box.Max.Z;
		if (vector3.LengthSquared() > num)
			return ContainmentType::Intersects;
		vector3.X = Center.X - box.Max.X;
		vector3.Y = Center.Y - box.Min.Y;
		vector3.Z = Center.Z - box.Max.Z;
		if (vector3.LengthSquared() > num)
			return ContainmentType::Intersects;
		vector3.X = Center.X - box.Min.X;
		vector3.Y = Center.Y - box.Min.Y;
		vector3.Z = Center.Z - box.Max.Z;
		if (vector3.LengthSquared() > num)
			return ContainmentType::Intersects;
		vector3.X = Center.X - box.Min.X;
		vector3.Y = Center.Y - box.Max.Y;
		vector3.Z = Center.Z - box.Min.Z;
		if (vector3.LengthSquared() > num)
			return ContainmentType::Intersects;
		vector3.X = Center.X - box.Max.X;
		vector3.Y = Center.Y - box.Max.Y;
		vector3.Z = Center.Z - box.Min.Z;
		if (vector3.LengthSquared() > num)
			return ContainmentType::Intersects;
		vector3.X = Center.X - box.Max.X;
		vector3.Y = Center.Y - box.Min.Y;
		vector3.Z = Center.Z - box.Min.Z;
		if (vector3.LengthSquared() > num)
			return ContainmentType::Intersects;
		vector3.X = Center.X - box.Min.X;
		vector3.Y = Center.Y - box.Min.Y;
		vector3.Z = Center.Z - box.Min.Z;
		return vector3.LengthSquared() > num
			? ContainmentType::Intersects
			: ContainmentType::Contains;
	}

	ContainmentType BoundingSphere::Contains(Vector3 const& point) const {
		return Vector3::DistanceSquared(point, Center) >= Radius * Radius
			? ContainmentType::Disjoint
			: ContainmentType::Contains;
	}

	std::optional<float> BoundingSphere::Intersects(Ray const& ray) const {
		const auto num1 = Center.X - ray.Position.X;
		const auto num2 = Center.Y - ray.Position.Y;
		const auto num3 = Center.Z - ray.Position.Z;
		const auto num4 = (num1 * num1 + num2 * num2 + num3 * num3);
		const auto num5 = Radius * Radius;

		if (num4 <= num5)
			return 0.0f;

		const auto num6 = (num1 * ray.Direction.X + num2 * ray.Direction.Y + num3 * ray.Direction.Z);

		if (num6 < 0.0)
			return {};

		const auto num7 = num4 - num6 * num6;

		if (num7 > num5)
			return {};

		const auto num8 = std::sqrt(num5 - num7);

		return num6 - num8;
	}

	BoundingSphere BoundingSphere::CreateMerged(BoundingSphere const& original, BoundingSphere const& additional) {
		Vector3 result = Vector3::Subtract(additional.Center, original.Center);
		float num1 = result.Length();
		float radius1 = original.Radius;
		float radius2 = additional.Radius;

		if (radius1 + radius2 >= num1) {
			if (radius1 - radius2 >= num1)
				return original;
			if (radius2 - radius1 >= num1)
				return additional;
		}

		Vector3 vector3 = result * (1.0f / num1);
		const auto num2 = MathHelper::Min(-radius1, num1 - radius2);
		const auto num3 = (MathHelper::Max(radius1, num1 + radius2) - num2) * 0.5F;
		BoundingSphere merged;
		merged.Center = original.Center + vector3 * (num3 + num2);
		merged.Radius = num3;
		return merged;
	}
}