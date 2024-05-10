#include "common/quaternion.hpp"
#include "common/matrix.hpp"

namespace xna {
	Quaternion Quaternion::CreateFromAxisAngle(Vector3 const& axis, float angle) {
		const auto num1 = angle * 0.5f;
		const auto num2 = std::sin(num1);
		const auto num3 = std::cos(num1);
		Quaternion fromAxisAngle;
		fromAxisAngle.X = axis.X * num2;
		fromAxisAngle.Y = axis.Y * num2;
		fromAxisAngle.Z = axis.Z * num2;
		fromAxisAngle.W = num3;
		return fromAxisAngle;
	}

	Quaternion Quaternion::CreateFromYawPitchRoll(float yaw, float pitch, float roll) {
		const auto num1 = roll * 0.5f;
		const auto num2 = std::sin(num1);
		const auto num3 = std::cos(num1);
		const auto num4 = pitch * 0.5f;
		const auto num5 = std::sin(num4);
		const auto num6 = std::cos(num4);
		const auto num7 = yaw * 0.5f;
		const auto num8 = std::sin(num7);
		const auto num9 = std::cos(num7);
		Quaternion fromYawPitchRoll;
		fromYawPitchRoll.X = (num9 * num5 * num3 + num8 * num6 * num2);
		fromYawPitchRoll.Y = (num8 * num6 * num3 - num9 * num5 * num2);
		fromYawPitchRoll.Z = (num9 * num6 * num2 - num8 * num5 * num3);
		fromYawPitchRoll.W = (num9 * num6 * num3 + num8 * num5 * num2);
		return fromYawPitchRoll;
	}

	Quaternion Quaternion::CreateFromRotationMatrix(Matrix const& matrix) {
        const auto num1 = matrix.M11 + matrix.M22 + matrix.M33;

        Quaternion fromRotationMatrix;
        if (num1 > 0.0)
        {
            const auto num2 = std::sqrt(num1 + 1.0F);
            fromRotationMatrix.W = num2 * 0.5f;
            const auto num3 = 0.5f / num2;
            fromRotationMatrix.X = (matrix.M23 - matrix.M32) * num3;
            fromRotationMatrix.Y = (matrix.M31 - matrix.M13) * num3;
            fromRotationMatrix.Z = (matrix.M12 - matrix.M21) * num3;
        }
        else if (matrix.M11 >= matrix.M22 && matrix.M11 >= matrix.M33)
        {
            const auto num4 = std::sqrt(1.0F + matrix.M11 - matrix.M22 - matrix.M33);
            const auto num5 = 0.5f / num4;
            fromRotationMatrix.X = 0.5f * num4;
            fromRotationMatrix.Y = (matrix.M12 + matrix.M21) * num5;
            fromRotationMatrix.Z = (matrix.M13 + matrix.M31) * num5;
            fromRotationMatrix.W = (matrix.M23 - matrix.M32) * num5;
        }
        else if (matrix.M22 > matrix.M33)
        {
            const auto num6 = std::sqrt(1.0F + matrix.M22 - matrix.M11 - matrix.M33);
            const auto num7 = 0.5f / num6;
            fromRotationMatrix.X = (matrix.M21 + matrix.M12) * num7;
            fromRotationMatrix.Y = 0.5f * num6;
            fromRotationMatrix.Z = (matrix.M32 + matrix.M23) * num7;
            fromRotationMatrix.W = (matrix.M31 - matrix.M13) * num7;
        }
        else
        {
            const auto num8 = std::sqrt(1.0F + matrix.M33 - matrix.M11 - matrix.M22);
            const auto num9 = 0.5f / num8;
            fromRotationMatrix.X = (matrix.M31 + matrix.M13) * num9;
            fromRotationMatrix.Y = (matrix.M32 + matrix.M23) * num9;
            fromRotationMatrix.Z = 0.5f * num8;
            fromRotationMatrix.W = (matrix.M12 - matrix.M21) * num9;
        }
        return fromRotationMatrix;
	}

    Quaternion Quaternion::Slerp(Quaternion const& quaternion1, Quaternion const& quaternion2, float amount) {
        const auto num1 = amount;
        auto d = quaternion1.X * quaternion2.X + quaternion1.Y * quaternion2.Y + quaternion1.Z * quaternion2.Z + quaternion1.W * quaternion2.W;
        bool flag = false;

        if (d < 0.0) {
            flag = true;
            d = -d;
        }

        float num2 = 0;
        float num3 = 0;

        if (d > 0.99999898672103882) {
            num2 = 1.0f - num1;
            num3 = flag ? -num1 : num1;
        }
        else {
            const auto a = std::acos(d);
            const auto num4 = 1.0F / std::sin(a);
            num2 = std::sin((1.0F - num1) * a) * num4;
            num3 = flag ? -std::sin(num1 * a) * num4 : std::sin(num1 * a) * num4;
        }
        Quaternion quaternion;
        quaternion.X = num2 * quaternion1.X + num3 * quaternion2.X;
        quaternion.Y = num2 * quaternion1.Y + num3 * quaternion2.Y;
        quaternion.Z = num2 * quaternion1.Z + num3 * quaternion2.Z;
        quaternion.W = num2 * quaternion1.W + num3 * quaternion2.W;
        return quaternion;
    }

    Quaternion Quaternion::Lerp(Quaternion const& quaternion1, Quaternion const& quaternion2, float amount) {
        const auto num1 = amount;
        const auto num2 = 1.0f - num1;
        Quaternion quaternion;

        if (quaternion1.X * quaternion2.X + quaternion1.Y * quaternion2.Y + quaternion1.Z * quaternion2.Z + quaternion1.W * quaternion2.W >= 0.0) {
            quaternion.X = num2 * quaternion1.X + num1 * quaternion2.X;
            quaternion.Y = num2 * quaternion1.Y + num1 * quaternion2.Y;
            quaternion.Z = num2 * quaternion1.Z + num1 * quaternion2.Z;
            quaternion.W = num2 * quaternion1.W + num1 * quaternion2.W;
        }
        else {
            quaternion.X = num2 * quaternion1.X - num1 * quaternion2.X;
            quaternion.Y = num2 * quaternion1.Y - num1 * quaternion2.Y;
            quaternion.Z = num2 * quaternion1.Z - num1 * quaternion2.Z;
            quaternion.W = num2 * quaternion1.W - num1 * quaternion2.W;
        }
        const auto num3 = 1.0f / std::sqrt(quaternion.X * quaternion.X + quaternion.Y * quaternion.Y + quaternion.Z * quaternion.Z + quaternion.W * quaternion.W);
        quaternion.X *= num3;
        quaternion.Y *= num3;
        quaternion.Z *= num3;
        quaternion.W *= num3;
        return quaternion;
    }
}