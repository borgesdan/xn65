#include "common/collision.hpp"

namespace xna {
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

	BoundingSphere BoundingSphere::CreateFromBoundingBox(BoundingBox const& box) {
		BoundingSphere fromBoundingBox;
		fromBoundingBox.Center = Vector3::Lerp(box.Min, box.Max, 0.5f);
		const auto result = Vector3::Distance(box.Min, box.Max);
		fromBoundingBox.Radius = result * 0.5f;
		return fromBoundingBox;
	}

	BoundingSphere BoundingSphere::CreateFromFrustum(BoundingFrustum const& frustum) {
		return BoundingSphere::CreateFromPoints(frustum.corners);
	}

	BoundingSphere BoundingSphere::CreateFromPoints(std::vector<Vector3> const& points) {
		Vector3 current;
		auto vector3_1 = current = points[0];
		auto vector3_2 = current;
		auto vector3_3 = current;
		auto vector3_4 = current;
		auto vector3_5 = current;
		auto vector3_6 = current;

		for (size_t i = 0; i < points.size(); ++i) {
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

		for (size_t i = 0; i < points.size(); ++i) {
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