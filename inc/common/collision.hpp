#ifndef XNA_COMMON_SHAPES_HPP
#define XNA_COMMON_SHAPES_HPP

#include "default.hpp"
#include "vectors.hpp"
#include "matrix.hpp"
#include "gjk.hpp"
#include <optional>

namespace xna {
	struct Plane {
		Vector3 Normal{ 0 };
		float D{ 0 };
	};

	struct BoundingFrustum {
		inline static constexpr int CornerCount = 8;
		inline static constexpr int PlaneCount = 6;

		constexpr BoundingFrustum() = default;
		constexpr BoundingFrustum(Matrix const& matrix) {
			SetMatrix(matrix);
		}

		Plane Near() { return planes[0]; }
		Plane Far() { return planes[1]; }
		Plane Left() { return planes[2]; }
		Plane Right() { return planes[3]; }
		Plane Top() { return planes[4]; }
		Plane Bottom() { return planes[5]; }

		constexpr bool operator==(BoundingFrustum const& other) const {
			return matrix == other.matrix;
		}

		constexpr Vector3 operator[](size_t index) const {
			if (index >= CornerCount)
				index = CornerCount - 1;

			return corners[index];
		}

		constexpr void GetCorners(std::vector<Vector3>& destination) const;
		constexpr Matrix GetMatrix() const { return matrix; }
		constexpr void SetMatrix(Matrix const& value);
		bool Intersects(BoundingBox const& box);
		bool Intersects(BoundingSphere const& box);
		constexpr PlaneIntersectionType Intersects(Plane const& plane) const;
		bool Intersects(BoundingFrustum const& frustum);
		std::optional<float> Intersects(Ray const& ray) const;
		constexpr ContainmentType Contains(BoundingBox const& box) const;
		ContainmentType Contains(BoundingFrustum const& box);
		constexpr ContainmentType Contains(Vector3 const& point) const;
		constexpr ContainmentType Contains(BoundingSphere const& box) const;
		constexpr void SupportMapping(Vector3 const& v, Vector3& result) const;	

	private:
		std::vector<Vector3> corners{ 8 };
		std::vector<Plane> planes{ 6 };
		Matrix matrix{ Matrix::Identity() };
		Gjk gjk{};

	private:
		static constexpr Ray ComputeIntersectionLine(Plane const& p1, Plane const& p2);
		static constexpr Vector3 ComputeIntersection(Plane const& plane, Ray const& ray);
	};

	struct BoundingBox {
		inline static constexpr int CornerCount = 8;

		Vector3 Min{};
		Vector3 Max{};

		constexpr BoundingBox() = default;
		constexpr BoundingBox(Vector3 const& min, Vector3 const& max):
			Min(min), Max(max){}

		constexpr bool operator==(BoundingBox const& other) const {
			return Min == other.Min && Max == other.Max;
		}

		constexpr void GetCorners(std::vector<Vector3>& corners) const;

		static constexpr BoundingBox CreateMerged(BoundingBox const& original, BoundingBox const& additional);
		static constexpr BoundingBox CreateFromSphere(BoundingSphere const& sphere);
		static constexpr BoundingBox CreateFromPoints(std::vector<Vector3> const& points);

		constexpr bool Intersects(BoundingBox const& box) const;
		bool Intersects(BoundingFrustum& frustum) const;
		constexpr PlaneIntersectionType Intersects(Plane const& plane) const;
		std::optional<float> Intersects(Ray const& ray) const;
		constexpr bool Intersects(BoundingSphere const& sphere) const;
		
		constexpr ContainmentType Contains(BoundingBox const& box) const;
		ContainmentType Contains(BoundingFrustum& frustum) const;
		constexpr ContainmentType Contains(Vector3 const& point) const;
		constexpr ContainmentType Contains(BoundingSphere const& sphere) const;

		constexpr void SupportMapping(Vector3 const& v, Vector3& result) const;
	};

	struct BoundingSphere {
		Vector3 Center{};
		float Radius{ 0 };

		void SupportMapping(Vector3 const& v, Vector3& result) const {
			//TODO
		}
	};

	struct Ray {
		Vector3 Position{};
		Vector3 Direction{};
	};

	//---------------------------------------------------------------------------------//
	//								IMPLEMENTATIONS									   //
	//---------------------------------------------------------------------------------//

	constexpr void BoundingFrustum::GetCorners(std::vector<Vector3>& destination) const {
		if (destination.size() < CornerCount)
			destination.resize(CornerCount);

		for (size_t i = 0; i < CornerCount; ++i)
			destination[i] = corners[i];
	}

	constexpr Ray BoundingFrustum::ComputeIntersectionLine(Plane const& p1, Plane const& p2) {
		Ray intersectionLine{};
		intersectionLine.Direction = Vector3::Cross(p1.Normal, p2.Normal);
		const auto num = intersectionLine.Direction.LengthSquared();
		intersectionLine.Position = Vector3::Cross(-p1.D * p2.Normal + p2.D * p1.Normal, intersectionLine.Direction) / num;
		return intersectionLine;
	}

	constexpr Vector3 BoundingFrustum::ComputeIntersection(Plane const& plane, Ray const& ray) {
		const auto num = (-plane.D - Vector3::Dot(plane.Normal, ray.Position)) / Vector3::Dot(plane.Normal, ray.Direction);
		return ray.Position + ray.Direction * num;
	}

	constexpr void BoundingFrustum::SupportMapping(Vector3 const& v, Vector3& result) const {
		size_t index1 = 0;
		auto result1 = Vector3::Dot(corners[0], v);

		for (size_t index2 = 1; index2 < CornerCount; ++index2)
		{
			auto result2 = Vector3::Dot(corners[index2], v);
			if (result2 > result1)
			{
				index1 = index2;
				result1 = result2;
			}
		}
		result = corners[index1];
	}

	constexpr PlaneIntersectionType BoundingFrustum::Intersects(Plane const& plane) const {
		int num = 0;
		for (size_t index = 0; index < CornerCount; ++index)
		{
			auto result = Vector3::Dot(corners[index], plane.Normal);

			if (result + plane.D > 0.0F)
				num |= 1;
			else
				num |= 2;

			if (num == 3)
				return PlaneIntersectionType::Intersecting;
		}
		return num != 1 ? PlaneIntersectionType::Back : PlaneIntersectionType::Front;
	}

	constexpr ContainmentType BoundingFrustum::Contains(Vector3 const& point) const {
		for (size_t i = 0; i < PlaneCount; ++i) {
			const auto& plane = planes[i];

			if ((static_cast<double>(plane.Normal.X * point.X + plane.Normal.Y * point.Y + plane.Normal.Z * point.Z) + plane.D) > 9.9999997473787516E-06)
				return ContainmentType::Disjoint;
		}

		return ContainmentType::Contains;
	}

	constexpr ContainmentType BoundingFrustum::Contains(BoundingBox const& box) const {
		bool flag = false;
		for (size_t i = 0; i < PlaneCount; ++i)
		{
			const auto& plane = planes[i];

			switch (box.Intersects(plane))
			{
			case PlaneIntersectionType::Front:
				return ContainmentType::Disjoint;
			case PlaneIntersectionType::Intersecting:
				flag = true;
				break;
			}
		}
		return !flag ? ContainmentType::Contains : ContainmentType::Intersects;
	}	

	constexpr ContainmentType BoundingFrustum::Contains(BoundingSphere const& sphere) const {
		auto center = sphere.Center;
		auto radius = sphere.Radius;
		auto num1 = 0;
		for (size_t i = 0; i < PlaneCount; ++i)
		{
			const auto& plane = planes[i];

			auto num2 = (plane.Normal.X * center.X + plane.Normal.Y * center.Y + plane.Normal.Z * center.Z) + plane.D;

			if (num2 > radius)
				return ContainmentType::Disjoint;

			if (num2 < -radius)
				++num1;
		}

		return num1 != 6 ? ContainmentType::Intersects : ContainmentType::Contains;
	}

	constexpr void BoundingFrustum::SetMatrix(Matrix const& value) {
		matrix = value;
		planes[2].Normal.X = -value.M14 - value.M11;
		planes[2].Normal.Y = -value.M24 - value.M21;
		planes[2].Normal.Z = -value.M34 - value.M31;
		planes[2].D = -value.M44 - value.M41;
		planes[3].Normal.X = -value.M14 + value.M11;
		planes[3].Normal.Y = -value.M24 + value.M21;
		planes[3].Normal.Z = -value.M34 + value.M31;
		planes[3].D = -value.M44 + value.M41;
		planes[4].Normal.X = -value.M14 + value.M12;
		planes[4].Normal.Y = -value.M24 + value.M22;
		planes[4].Normal.Z = -value.M34 + value.M32;
		planes[4].D = -value.M44 + value.M42;
		planes[5].Normal.X = -value.M14 - value.M12;
		planes[5].Normal.Y = -value.M24 - value.M22;
		planes[5].Normal.Z = -value.M34 - value.M32;
		planes[5].D = -value.M44 - value.M42;
		planes[0].Normal.X = -value.M13;
		planes[0].Normal.Y = -value.M23;
		planes[0].Normal.Z = -value.M33;
		planes[0].D = -value.M43;
		planes[1].Normal.X = -value.M14 + value.M13;
		planes[1].Normal.Y = -value.M24 + value.M23;
		planes[1].Normal.Z = -value.M34 + value.M33;
		planes[1].D = -value.M44 + value.M43;
		
		for (size_t index = 0; index < PlaneCount; ++index)
		{
			const auto num = planes[index].Normal.Length();
			planes[index].Normal = planes[index].Normal / num;
			planes[index].D /= num;
		}

		Ray intersectionLine1 = BoundingFrustum::ComputeIntersectionLine(planes[0], planes[2]);
		corners[0] = BoundingFrustum::ComputeIntersection(planes[4], intersectionLine1);
		corners[3] = BoundingFrustum::ComputeIntersection(planes[5], intersectionLine1);
		Ray intersectionLine2 = BoundingFrustum::ComputeIntersectionLine(planes[3], planes[0]);
		corners[1] = BoundingFrustum::ComputeIntersection(planes[4], intersectionLine2);
		corners[2] = BoundingFrustum::ComputeIntersection(planes[5], intersectionLine2);
		intersectionLine2 = BoundingFrustum::ComputeIntersectionLine(planes[2], planes[1]);
		corners[4] = BoundingFrustum::ComputeIntersection(planes[4], intersectionLine2);
		corners[7] = BoundingFrustum::ComputeIntersection(planes[5], intersectionLine2);
		intersectionLine2 = BoundingFrustum::ComputeIntersectionLine(planes[1], planes[3]);
		corners[5] = BoundingFrustum::ComputeIntersection(planes[4], intersectionLine2);
		corners[6] = BoundingFrustum::ComputeIntersection(planes[5], intersectionLine2);
	}


	constexpr void BoundingBox::GetCorners(std::vector<Vector3>& corners) const {
		if (corners.size() < 8)
			corners.resize(CornerCount);

		corners[0].X = Min.X;
		corners[0].Y = Max.Y;
		corners[0].Z = Max.Z;
		corners[1].X = Max.X;
		corners[1].Y = Max.Y;
		corners[1].Z = Max.Z;
		corners[2].X = Max.X;
		corners[2].Y = Min.Y;
		corners[2].Z = Max.Z;
		corners[3].X = Min.X;
		corners[3].Y = Min.Y;
		corners[3].Z = Max.Z;
		corners[4].X = Min.X;
		corners[4].Y = Max.Y;
		corners[4].Z = Min.Z;
		corners[5].X = Max.X;
		corners[5].Y = Max.Y;
		corners[5].Z = Min.Z;
		corners[6].X = Max.X;
		corners[6].Y = Min.Y;
		corners[6].Z = Min.Z;
		corners[7].X = Min.X;
		corners[7].Y = Min.Y;
		corners[7].Z = Min.Z;
	}

	constexpr BoundingBox BoundingBox::CreateMerged(BoundingBox const& original, BoundingBox const& additional) {
		BoundingBox merged;
		merged.Min = Vector3::Min(original.Min, additional.Min);
		merged.Max = Vector3::Max(original.Max, additional.Max);
		return merged;
	}

	constexpr BoundingBox BoundingBox::CreateFromSphere(BoundingSphere const& sphere) {
		BoundingBox fromSphere;
		fromSphere.Min.X = sphere.Center.X - sphere.Radius;
		fromSphere.Min.Y = sphere.Center.Y - sphere.Radius;
		fromSphere.Min.Z = sphere.Center.Z - sphere.Radius;
		fromSphere.Max.X = sphere.Center.X + sphere.Radius;
		fromSphere.Max.Y = sphere.Center.Y + sphere.Radius;
		fromSphere.Max.Z = sphere.Center.Z + sphere.Radius;
		return fromSphere;
	}

	constexpr BoundingBox BoundingBox::CreateFromPoints(std::vector<Vector3> const& points) {
		Vector3 result1 = Vector3(FloatMaxValue);
		Vector3 result2 = Vector3(FloatMinValue);
		
		for (size_t i = 0; i < points.size(); ++i) {
			const auto& point = points[i];
			result1 = Vector3::Min(result1, point);
			result2 = Vector3::Max(result2, point);
		}

		return BoundingBox(result1, result2);
	}

	constexpr bool BoundingBox::Intersects(BoundingBox const& box) const {
		return Max.X >= box.Min.X 
			&& Min.X <= box.Max.X 
			&& Max.Y >= box.Min.Y 
			&& Min.Y <= box.Max.Y 
			&& Max.Z >= box.Min.Z 
			&& Min.Z <= box.Max.Z;
	}

	inline bool BoundingBox::Intersects(BoundingFrustum& frustum) const {
		return frustum.Intersects(*this);
	}

	constexpr PlaneIntersectionType BoundingBox::Intersects(Plane const& plane) const {
		Vector3 vector3_1;
		vector3_1.X = plane.Normal.X >= 0.0 ? Min.X : Max.X;
		vector3_1.Y = plane.Normal.Y >= 0.0 ? Min.Y : Max.Y;
		vector3_1.Z = plane.Normal.Z >= 0.0 ? Min.Z : Max.Z;
		Vector3 vector3_2;
		vector3_2.X = plane.Normal.X >= 0.0 ? Max.X : Min.X;
		vector3_2.Y = plane.Normal.Y >= 0.0 ? Max.Y : Min.Y;
		vector3_2.Z = plane.Normal.Z >= 0.0 ? Max.Z : Min.Z;
		if (plane.Normal.X * vector3_1.X + plane.Normal.Y * vector3_1.Y + plane.Normal.Z * vector3_1.Z + plane.D > 0.0)
			return PlaneIntersectionType::Front;

		return plane.Normal.X * vector3_2.X + plane.Normal.Y * vector3_2.Y + plane.Normal.Z * vector3_2.Z + plane.D < 0.0 
			? PlaneIntersectionType::Back 
			: PlaneIntersectionType::Intersecting;
	}	

	constexpr bool BoundingBox::Intersects(BoundingSphere const& sphere) const {
		const auto result1 = Vector3::Clamp(sphere.Center, Min, Max);
		const auto result2 =  Vector3::DistanceSquared(sphere.Center, result1);
		return result2 <= sphere.Radius * sphere.Radius;
	}

	constexpr ContainmentType BoundingBox::Contains(BoundingBox const& box) const {
		if (Max.X < box.Min.X || Min.X > box.Max.X || Max.Y < box.Min.Y || Min.Y > box.Max.Y || Max.Z < box.Min.Z || Min.Z > box.Max.Z)
			return ContainmentType::Disjoint;
		return Min.X > box.Min.X || box.Max.X > Max.X || Min.Y > box.Min.Y || box.Max.Y > Max.Y || Min.Z > box.Min.Z || box.Max.Z > Max.Z 
			? ContainmentType::Intersects
			: ContainmentType::Contains;
	}	

	constexpr ContainmentType BoundingBox::Contains(Vector3 const& point) const {
		return Min.X >  point.X || point.X > Max.X || Min.Y > point.Y || point.Y > Max.Y || Min.Z > point.Z || point.Z > Max.Z 
			? ContainmentType::Disjoint 
			: ContainmentType::Contains;
	}

	constexpr ContainmentType BoundingBox::Contains(BoundingSphere const& sphere) const {
		Vector3 result1 = Vector3::Clamp(sphere.Center, Min, Max);
		float result2 = Vector3::DistanceSquared(sphere.Center, result1);
		float radius = sphere.Radius;
		
		if (result2 > radius * radius)
			return ContainmentType::Disjoint;

		return Min.X + radius > sphere.Center.X || sphere.Center.X > Max.X - radius || Max.X - Min.X <= radius || Min.Y + radius > sphere.Center.Y || sphere.Center.Y > Max.Y - radius || Max.Y - Min.Y <= radius || Min.Z + radius > sphere.Center.Z || sphere.Center.Z > Max.Z - radius || Max.X - Min.X <= radius 
			? ContainmentType::Intersects 
			: ContainmentType::Contains;
	}

	constexpr void BoundingBox::SupportMapping(Vector3 const& v, Vector3& result) const {
		result.X = v.X >= 0.0 ? Max.X : Min.X;
		result.Y = v.Y >= 0.0 ? Max.Y : Min.Y;
		result.Z = v.Z >= 0.0 ? Max.Z : Min.Z;
	}
}

#endif