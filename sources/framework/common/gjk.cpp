#include "xna/common/collision.hpp"

namespace xna {
	Vector3 Gjk::ComputeClosestPoint() {
		auto num1 = 0.0f;
		Vector3 zero = Vector3::Zero();
		maxLengthSq = 0.0f;

		for (auto bitsToIndex = Gjk::BitsToIndices[simplexBits]; bitsToIndex != 0; bitsToIndex >>= 3)
		{
			auto index = (bitsToIndex & 7) - 1;
			auto num2 = det[simplexBits][index];
			num1 += num2;
			zero += y[index] * num2;
			maxLengthSq = MathHelper::Max(maxLengthSq, yLengthSq[index]);
		}

		return zero / num1;
	}

	bool Gjk::UpdateSimplex(Int newIndex) {
        auto yBits = simplexBits | 1 << newIndex;
        auto xBits = 1 << newIndex;
        for (auto sb = simplexBits; sb != 0; --sb)
        {
            if ((sb & yBits) == sb && IsSatisfiesRule(sb | xBits, yBits))
            {
                simplexBits = sb | xBits;
                closestPoint = ComputeClosestPoint();
                return true;
            }
        }

        bool flag = false;
        
        if (IsSatisfiesRule(xBits, yBits)) {
            simplexBits = xBits;
            closestPoint = y[newIndex];
            maxLengthSq = yLengthSq[newIndex];
            flag = true;
        }

        return flag;
	}

    void Gjk::UpdateDeterminant(Int xmIdx) {
        auto index1 = 1 << xmIdx;
        det[index1][xmIdx] = 1.0f;
        auto bitsToIndex = Gjk::BitsToIndices[simplexBits];
        auto num1 = bitsToIndex;
        auto num2 = 0;
        while (num1 != 0)
        {
            auto index2 = (num1 & 7) - 1;
            auto num3 = 1 << index2;
            auto index3 = num3 | index1;
            det[index3][index2] = Vector3::Dot(edges[xmIdx][index2], y[xmIdx]);
            det[index3][xmIdx] = Vector3::Dot(edges[index2][xmIdx], y[index2]);
            auto num4 = bitsToIndex;
            for (auto index4 = 0; index4 < num2; ++index4)
            {
                auto index5 = (num4 & 7) - 1;
                auto num5 = 1 << index5;
                auto index6 = index3 | num5;
                auto index7 = Gjk::edgeLengthSq[index2][index5] < Gjk::edgeLengthSq[xmIdx][index5] ? index2 : xmIdx;
                det[index6][index5] = (Gjk::det[index3][index2] * Vector3::Dot(edges[index7][index5], y[index2]) + Gjk::det[index3][xmIdx] * Vector3::Dot(edges[index7][index5], y[xmIdx]));
                auto index8 = Gjk::edgeLengthSq[index5][index2] < Gjk::edgeLengthSq[xmIdx][index2] ? index5 : xmIdx;
                det[index6][index2] = (Gjk::det[num5 | index1][index5] * Vector3::Dot(edges[index8][index2], y[index5]) + Gjk::det[num5 | index1][xmIdx] * Vector3::Dot(edges[index8][index2], y[xmIdx]));
                auto index9 = Gjk::edgeLengthSq[index2][xmIdx] < Gjk::edgeLengthSq[index5][xmIdx] ? index2 : index5;
                det[index6][xmIdx] = (Gjk::det[num3 | num5][index5] * Vector3::Dot(edges[index9][xmIdx], y[index5]) + Gjk::det[num3 | num5][index2] * Vector3::Dot(edges[index9][xmIdx], y[index2]));
                num4 >>= 3;
            }
            num1 >>= 3;
            ++num2;
        }

        if ((simplexBits | index1) != 15)
            return;
        auto index10 = Gjk::edgeLengthSq[1][0] < Gjk::edgeLengthSq[2][0] ? (Gjk::edgeLengthSq[1][0] < Gjk::edgeLengthSq[3][0] ? 1 : 3) : (Gjk::edgeLengthSq[2][0] < Gjk::edgeLengthSq[3][0] ? 2 : 3);
        det[15][0] = (Gjk::det[14][1] * Vector3::Dot(edges[index10][0], y[1]) + Gjk::det[14][2] * Vector3::Dot(edges[index10][0], y[2]) + Gjk::det[14][3] * Vector3::Dot(edges[index10][0], y[3]));
        auto index11 = Gjk::edgeLengthSq[0][1] < Gjk::edgeLengthSq[2][1] ? (Gjk::edgeLengthSq[0][1] < Gjk::edgeLengthSq[3][1] ? 0 : 3) : (Gjk::edgeLengthSq[2][1] < Gjk::edgeLengthSq[3][1] ? 2 : 3);
        det[15][1] = (Gjk::det[13][0] * Vector3::Dot(edges[index11][1], y[0]) + Gjk::det[13][2] * Vector3::Dot(edges[index11][1], y[2]) + Gjk::det[13][3] * Vector3::Dot(edges[index11][1], y[3]));
        auto index12 = Gjk::edgeLengthSq[0][2] < Gjk::edgeLengthSq[1][2] ? (Gjk::edgeLengthSq[0][2] < Gjk::edgeLengthSq[3][2] ? 0 : 3) : (Gjk::edgeLengthSq[1][2] < Gjk::edgeLengthSq[3][2] ? 1 : 3);
        det[15][2] = (Gjk::det[11][0] * Vector3::Dot(edges[index12][2], y[0]) + Gjk::det[11][1] * Vector3::Dot(edges[index12][2], y[1]) + Gjk::det[11][3] * Vector3::Dot(edges[index12][2], y[3]));
        auto index13 = Gjk::edgeLengthSq[0][3] < Gjk::edgeLengthSq[1][3] ? (Gjk::edgeLengthSq[0][3] < Gjk::edgeLengthSq[2][3] ? 0 : 2) : (Gjk::edgeLengthSq[1][3] < Gjk::edgeLengthSq[2][3] ? 1 : 2);
        det[15][3] = (Gjk::det[7][0] * Vector3::Dot(edges[index13][3], y[0]) + Gjk::det[7][1] * Vector3::Dot(edges[index13][3], y[1]) + Gjk::det[7][2] * Vector3::Dot(edges[index13][3], y[2]));
    }

    bool Gjk::AddSupportPoint(Vector3 const& newPoint) {
        auto index1 = (Gjk::BitsToIndices[simplexBits ^ 15] & 7) - 1;
        y[index1] = newPoint;
        yLengthSq[index1] = newPoint.LengthSquared();
        for (auto bitsToIndex = Gjk::BitsToIndices[simplexBits]; bitsToIndex != 0; bitsToIndex >>= 3)
        {
            auto index2 = (bitsToIndex & 7) - 1;
            Vector3 vector3 = y[index2] - newPoint;
            edges[index2][index1] = vector3;
            edges[index1][index2] = -vector3;
            edgeLengthSq[index1][index2] = edgeLengthSq[index2][index1] = vector3.LengthSquared();
        }
        UpdateDeterminant(index1);
        return UpdateSimplex(index1);
    }
}