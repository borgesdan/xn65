#ifndef XNA_FRAMEWORK_BOUNDINGSPHERE_HPP
#define XNA_FRAMEWORK_BOUNDINGSPHERE_HPP

#include "BoundingBox.hpp"

namespace Xna {
	//Defines a sphere. 
	struct BoundingSphere {
		//The center point of the sphere.
		Vector3 Center{};
		//The radius of the sphere.
		float Radius{ 0 };

		constexpr BoundingSphere() = default;
		constexpr BoundingSphere(Vector3 const& center, float radius) :
			Center(center), Radius(radius < 0 ? 0 : radius) {
		}

		constexpr bool operator==(BoundingSphere const& other) const {
			return Center == other.Center && Radius == other.Radius;
		}

		//Creates a BoundingSphere that contains the two specified BoundingSphere instances. 
		static BoundingSphere CreateMerged(BoundingSphere const& original, BoundingSphere const& additional);
		//Creates the smallest BoundingSphere that can contain a specified BoundingBox. 
		static BoundingSphere CreateFromBoundingBox(BoundingBox const& box);
		//Creates a BoundingSphere that can contain a specified list of points.
		static BoundingSphere CreateFromPoints(std::vector<Vector3> const& points) {
			return CreateFromPoints(points.data(), points.size());
		}
		//Creates a BoundingSphere that can contain a specified list of points.
		static BoundingSphere CreateFromPoints(Vector3 const* points, size_t size);

		//Creates the smallest BoundingSphere that can contain a specified BoundingFrustum. 
		static BoundingSphere CreateFromFrustum(BoundingFrustum const& points);

		//Checks whether the current BoundingSphere intersects another bounding volume.
		constexpr bool Intersects(BoundingBox const& box) const;
		//Checks whether the current BoundingSphere intersects another bounding volume.
		bool Intersects(BoundingFrustum& frustum) const;
		//Checks whether the current BoundingSphere intersects another bounding volume.
		constexpr PlaneIntersectionType Intersects(Plane const& plane) const;
		//Checks whether the current BoundingSphere intersects another bounding volume.
		std::optional<float> Intersects(Ray const& ray) const;
		//Checks whether the current BoundingSphere intersects another bounding volume.
		constexpr bool Intersects(BoundingSphere const& sphere) const;

		//Checks whether the current BoundingSphere contains a specified bounding volume.
		ContainmentType Contains(BoundingBox const& box) const;
		//Checks whether the current BoundingSphere contains a specified bounding volume.
		ContainmentType Contains(BoundingFrustum& frustum) const;
		//Checks whether the current BoundingSphere contains a specified bounding volume.
		ContainmentType Contains(Vector3 const& point) const;
		//Checks whether the current BoundingSphere contains a specified bounding volume.
		ContainmentType Contains(BoundingSphere const& sphere) const;

		//Translates and scales the BoundingSphere using a given Matrix.
		BoundingSphere Transform(Matrix const& matrix) const;

		void SupportMapping(Vector3 const& v, Vector3& result) const;
	};

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

	constexpr bool BoundingBox::Intersects(BoundingSphere const& sphere) const {
		const auto result1 = Vector3::Clamp(sphere.Center, Min, Max);
		const auto result2 = Vector3::DistanceSquared(sphere.Center, result1);
		return result2 <= sphere.Radius * sphere.Radius;
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

	constexpr bool BoundingSphere::Intersects(BoundingBox const& box) const {
		Vector3 result1 = Vector3::Clamp(Center, box.Min, box.Max);
		float result2 = Vector3::DistanceSquared(Center, result1);
		return result2 <= Radius * Radius;
	}

	inline bool BoundingSphere::Intersects(BoundingFrustum& frustum) const {
		return frustum.Intersects(*this);
	}

	constexpr PlaneIntersectionType BoundingSphere::Intersects(Plane const& plane) const {
		const auto num = (Center.X * plane.Normal.X + Center.Y * plane.Normal.Y + Center.Z * plane.Normal.Z) + plane.D;

		if (num > Radius)
			return PlaneIntersectionType::Front;

		return num < -Radius ? PlaneIntersectionType::Back : PlaneIntersectionType::Intersecting;
	}

	constexpr bool BoundingSphere::Intersects(BoundingSphere const& sphere) const {
		const auto result = Vector3::DistanceSquared(Center, sphere.Center);
		const auto radius1 = Radius;
		const auto radius2 = sphere.Radius;
		return radius1 * radius1 + 2.0F * radius1 * radius2 + radius2 * radius2 > result;
	}

	constexpr PlaneIntersectionType Plane::Intersects(BoundingSphere const& sphere) const {
		const auto num = (sphere.Center.X * Normal.X + sphere.Center.Y * Normal.Y + sphere.Center.Z * Normal.Z) + D;

		if (num > sphere.Radius)
			return PlaneIntersectionType::Front;

		return num < -sphere.Radius
			? PlaneIntersectionType::Back : PlaneIntersectionType::Intersecting;
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
}

#endif