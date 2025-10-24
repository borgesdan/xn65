#ifndef XNA_FRAMEWORK_RAY_HPP
#define XNA_FRAMEWORK_RAY_HPP

#include "BoundingSphere.hpp"

namespace Xna {
	//Defines a ray.
	struct Ray {
		//Specifies the starting point of the Ray.
		Vector3 Position{};
		//Unit vector specifying the direction the Ray is pointing.
		Vector3 Direction{};

		constexpr Ray() = default;
		constexpr Ray(Vector3 const& position, Vector3 const& direction) :
			Position(position), Direction(direction) {
		}

		constexpr bool operator==(Ray const& other) const {
			return Position == other.Position && Direction == other.Direction;
		}

		//Checks whether the Ray intersects a specified plane or bounding volume. 
		inline std::optional<float> Intersects(BoundingBox const& box) const {
			return box.Intersects(*this);
		}

		//Checks whether the Ray intersects a specified plane or bounding volume. 
		inline std::optional<float> Intersects(BoundingFrustum const& frustum) const {
			return frustum.Intersects(*this);
		}

		//Checks whether the Ray intersects a specified plane or bounding volume. 
		inline std::optional<float> Intersects(Plane const& plane) const {
			return plane.Intersects(*this);
		}

		//Checks whether the Ray intersects a specified plane or bounding volume. 
		inline std::optional<float> Intersects(BoundingSphere const& sphere) const {
			return sphere.Intersects(*this);
		}
	};

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
}

#endif