#ifndef XNA_FRAMEWORK_GJK_HPP
#define XNA_FRAMEWORK_GJK_HPP

#include "Vector3.hpp"

namespace Xna {
	class Gjk {
	public:
		constexpr Gjk() {}

		constexpr bool FullSimplex() const { return simplexBits == 15; }
		constexpr float MaxLengthSquared() const { return maxLengthSq; }
		constexpr Vector3 ClosestPoint() const { return closestPoint; }

		constexpr void Reset() {
			simplexBits = 0;
			maxLengthSq = 0.0f;
		}

		bool AddSupportPoint(Vector3 const& newPoint);

	private:
		using listv3 = std::vector<Vector3>;
		using listv3v3 = std::vector<std::vector<Vector3>>;
		using listf = std::vector<float>;
		using listff = std::vector<std::vector<float>>;

		void UpdateDeterminant(int32_t xmIdx);
		bool UpdateSimplex(int32_t newIndex);
		Vector3 ComputeClosestPoint();

		constexpr bool IsSatisfiesRule(int32_t xBits, int32_t yBits) const {
			bool flag = true;
			for (auto bitsToIndex = Gjk::BitsToIndices[yBits]; bitsToIndex != 0; bitsToIndex >>= 3)
			{
				auto index = (bitsToIndex & 7) - 1;
				auto num = 1 << index;

				if ((num & xBits) != 0) {
					if (static_cast<float>(det[xBits][index]) <= 0.0F) {
						flag = false;
						break;
					}
				}
				else if (static_cast<float>(det[xBits | num][index]) > 0.0F) {
					flag = false;
					break;
				}
			}
			return flag;
		}

	private:
		inline static auto BitsToIndices = std::vector<int32_t>{
			0, 1, 2, 17, 3, 25, 26, 209, 4, 33, 34, 273, 35, 281, 282, 2257
		};

		Vector3 closestPoint{};
		int32_t simplexBits{ 0 };
		float maxLengthSq{ 0 };
		listv3 y = listv3(4);
		listf yLengthSq = listf(4);
		listv3v3 edges = listv3v3(4, listv3(4));
		listff edgeLengthSq = listff(4, listf(4));
		listff det = listff(16, listf(4));
	};

}

#endif