#ifndef XNA_FRAMEWORK_BOUNDINGFRUSTUM_HPP
#define XNA_FRAMEWORK_BOUNDINGFRUSTUM_HPP

#include "Plane.hpp"
#include "Gjk.hpp"
#include "Matrix.hpp"

namespace Xna {
	//Defines a frustum and helps determine whether forms intersect with it. 
	struct BoundingFrustum {
		static constexpr int CornerCount = 8;
		static constexpr int PlaneCount = 6;

		constexpr BoundingFrustum() = default;
		constexpr BoundingFrustum(Matrix const& matrix) {
			SetMatrix(matrix);
		}

		//Gets the near plane of the BoundingFrustum.
		constexpr Plane Near() const { return planes[0]; }
		//Gets the far plane of the BoundingFrustum.
		constexpr Plane Far() const { return planes[1]; }
		//Gets the left plane of the BoundingFrustum.
		constexpr Plane Left() const { return planes[2]; }
		//Gets the right plane of the BoundingFrustum.
		constexpr Plane Right() const { return planes[3]; }
		//Gets the top plane of the BoundingFrustum.
		constexpr Plane Top() const { return planes[4]; }
		//Gets the bottom plane of the BoundingFrustum.
		constexpr Plane Bottom() const { return planes[5]; }

		constexpr bool operator==(BoundingFrustum const& other) const {
			return matrix == other.matrix;
		}

		constexpr Vector3 operator[](size_t index) const {
			if (index >= CornerCount)
				index = CornerCount - 1;

			return corners[index];
		}

		//Gets an array of points that make up the corners of the BoundingFrustum.
		constexpr void GetCorners(std::vector<Vector3>& destination) const;
		//Gets or sets the Matrix that describes this bounding frustum.
		constexpr Matrix GetMatrix() const { return matrix; }
		//Gets or sets the Matrix that describes this bounding frustum.
		constexpr void SetMatrix(Matrix const& value);
		//Gets or sets the Matrix that describes this bounding frustum.
		bool Intersects(BoundingBox const& box);
		//Gets or sets the Matrix that describes this bounding frustum.
		bool Intersects(BoundingSphere const& box);
		//Gets or sets the Matrix that describes this bounding frustum.
		constexpr PlaneIntersectionType Intersects(Plane const& plane) const;
		//Gets or sets the Matrix that describes this bounding frustum.
		bool Intersects(BoundingFrustum const& frustum);
		//Gets or sets the Matrix that describes this bounding frustum.
		std::optional<float> Intersects(Ray const& ray) const;
		//Checks whether the current BoundingFrustum contains a specified bounding volume. 
		constexpr ContainmentType Contains(BoundingBox const& box) const;
		//Checks whether the current BoundingFrustum contains a specified bounding volume. 
		ContainmentType Contains(BoundingFrustum const& box);
		//Checks whether the current BoundingFrustum contains a specified bounding volume. 
		constexpr ContainmentType Contains(Vector3 const& point) const;
		//Checks whether the current BoundingFrustum contains a specified bounding volume. 
		constexpr ContainmentType Contains(BoundingSphere const& box) const;

		constexpr void SupportMapping(Vector3 const& v, Vector3& result) const;
	public:
		Vector3 corners[8];
		Plane planes[6];

	private:
		Matrix matrix{ Matrix::Identity() };
		Gjk gjk{};

	private:
		static constexpr Ray ComputeIntersectionLine(Plane const& p1, Plane const& p2);
		static constexpr Vector3 ComputeIntersection(Plane const& plane, Ray const& ray);

	private:
		static constexpr float FLOAT_MIN_VALUE = (std::numeric_limits<float>::min)();
		static constexpr float FLOAT_MAX_VALUE = (std::numeric_limits<float>::max)();
	};

	constexpr void BoundingFrustum::GetCorners(std::vector<Vector3>& destination) const {
		if (destination.size() < CornerCount)
			destination.resize(CornerCount);

		for (size_t i = 0; i < CornerCount; ++i)
			destination[i] = corners[i];
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

	constexpr PlaneIntersectionType Plane::Intersects(BoundingFrustum const& frustum) const {
		return frustum.Intersects(*this);
	}
}

#endif