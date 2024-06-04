#include "xna/common/numerics.hpp"

namespace xna {
    bool Vector2::Transform(Vector2 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector2* destinationArray, size_t destinationArrayLength) {
        if (!sourceArray || !destinationArray || destinationArrayLength < sourceArrayLength)
            return false;

        for (size_t index = 0; index < sourceArrayLength; ++index)
        {
            const auto& source = sourceArray[index];
            destinationArray[index].X = (source.X * matrix.M11 + source.Y * matrix.M21) + matrix.M41;
            destinationArray[index].Y = (source.X * matrix.M12 + source.Y * matrix.M22) + matrix.M42;
        }

        return true;
    }

    bool Vector2::Transform(std::vector<Vector2> sourceArray, Matrix const& matrix, std::vector<Vector2>& destinationArray)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return Transform(sourceArray.data(), sourceArray.size(), matrix, destinationArray.data(), destinationArray.size());
    }

    bool Vector2::Transform(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix,
        Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length) {
        if (!sourceArray || !destinationArray || destinationArrayLength < sourceArrayLength
            || sourceArrayLength < sourceIndex + length || destinationArrayLength < destinationIndex + length)
            return false;

        for (size_t i = 0; i < length; ++i) {
            const auto& source = sourceArray[sourceIndex + i];
            destinationArray[destinationIndex + i].X = (source.X * matrix.M11 + source.Y * matrix.M21) + matrix.M41;
            destinationArray[destinationIndex + i].Y = (source.X * matrix.M12 + source.Y * matrix.M22) + matrix.M42;
        }

        return true;
    }

    bool Vector2::Transform(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return Transform(sourceArray.data(), sourceArray.size(), sourceIndex, matrix, destinationArray.data(), destinationArray.size(), destinationIndex, length);
    }

    bool Vector2::TransformNormal(Vector2 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector2* destinationArray, size_t destinationArrayLength) {
        if (!sourceArray || !destinationArray || destinationArrayLength < sourceArrayLength)
            return false;

        for (size_t index = 0; index < sourceArrayLength; ++index) {
            const auto& source = sourceArray[index];
            destinationArray[index].X = source.X * matrix.M11 + source.Y * matrix.M21;
            destinationArray[index].Y = source.X * matrix.M12 + source.Y * matrix.M22;
        }

        return true;
    }

    bool Vector2::TransformNormal(std::vector<Vector2> const& sourceArray, Matrix const& matrix, std::vector<Vector2>& destinationArray)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return TransformNormal(sourceArray.data(), sourceArray.size(), matrix, destinationArray.data(), destinationArray.size());
    }

    bool Vector2::TransformNormal(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix, Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length) {
        if (!sourceArray || !destinationArray || destinationArrayLength < sourceArrayLength
            || sourceArrayLength < sourceIndex + length || destinationArrayLength < destinationIndex + length)
            return false;

        for (size_t i = 0; i < length; ++i)
        {
            const auto& source = sourceArray[sourceIndex + i];
            destinationArray[destinationIndex].X = (source.X * matrix.M11 + source.Y * matrix.M21);
            destinationArray[destinationIndex].Y = (source.X * matrix.M12 + source.Y * matrix.M22);
        }

        return true;
    }

    bool Vector2::TransformNormal(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return TransformNormal(sourceArray.data(), sourceArray.size(), sourceIndex, matrix, destinationArray.data(), destinationArray.size(), destinationIndex, length);
    }

    bool Vector2::Transform(Vector2 const* sourceArray, size_t sourceArrayLength, Quaternion const& rotation, Vector2* destinationArray, size_t destinationArrayLength) {
        if (!sourceArray || !destinationArray || destinationArrayLength < sourceArrayLength)
            return false;

        const auto rx = rotation.X + rotation.X;
        const auto ry = rotation.Y + rotation.Y;
        const auto rz = rotation.Z + rotation.Z;
        const auto rwz = rotation.W * rz;
        const auto rxx = rotation.X * rx;
        const auto rxy = rotation.X * ry;
        const auto ryy = rotation.Y * ry;
        const auto rzz = rotation.Z * rz;
        const auto a = 1.0f - ryy - rzz;
        const auto b = rxy - rwz;
        const auto c = rxy + rwz;
        const auto d = 1.0f - rxx - rzz;

        for (size_t index = 0; index < sourceArrayLength; ++index) {
            const auto& source = sourceArray[index];
            destinationArray[index].X = source.X * a + source.Y * b;
            destinationArray[index].Y = source.X * c + source.Y * d;
        }

        return true;
    }

    bool Vector2::Transform(std::vector<Vector2> const& sourceArray, Quaternion const& rotation, std::vector<Vector2>& destinationArray)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return Transform(sourceArray.data(), sourceArray.size(), rotation, destinationArray.data(), destinationArray.size());
    }

    bool Vector2::Transform(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Quaternion const& rotation,
        Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length) {
        if (!sourceArray || !destinationArray || destinationArrayLength < sourceArrayLength
            || sourceArrayLength < sourceIndex + length || destinationArrayLength < destinationIndex + length)
            return false;

        const auto rx = rotation.X + rotation.X;
        const auto ry = rotation.Y + rotation.Y;
        const auto rz = rotation.Z + rotation.Z;
        const auto rwz = rotation.W * rz;
        const auto rxx = rotation.X * rx;
        const auto rxy = rotation.X * ry;
        const auto ryy = rotation.Y * ry;
        const auto rzz = rotation.Z * rz;
        const auto a = 1.0f - ryy - rzz;
        const auto b = rxy - rwz;
        const auto c = rxy + rwz;
        const auto d = 1.0f - rxx - rzz;

        for (size_t i = 0; i < length; ++i) {
            const auto& source = sourceArray[sourceIndex = i];

            destinationArray[destinationIndex].X = source.X * a + source.Y * b;
            destinationArray[destinationIndex].Y = source.X * c + source.Y * d;
            ++sourceIndex;
            ++destinationIndex;
        }

        return true;
    }

    bool Vector2::Transform(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return Transform(sourceArray.data(), sourceArray.size(), sourceIndex, rotation, destinationArray.data(), destinationArray.size(), destinationIndex, length);
    }

    bool Vector3::Transform(Vector3 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector3* destinationArray, size_t destinationLength)
    {
        if (!sourceArray || !destinationArray || destinationLength < sourceArrayLength)
            return false;

        for (size_t index = 0; index < sourceArrayLength; ++index)
        {
            const auto& source = sourceArray[index];
            destinationArray[index].X = (source.X * matrix.M11 + source.Y * matrix.M21 + source.Z * matrix.M31) + matrix.M41;
            destinationArray[index].Y = (source.X * matrix.M12 + source.Y * matrix.M22 + source.Z * matrix.M32) + matrix.M42;
            destinationArray[index].Z = (source.X * matrix.M13 + source.Y * matrix.M23 + source.Z * matrix.M33) + matrix.M43;
        }

        return true;
    }

    bool Vector3::Transform(std::vector<Vector3> const& sourceArray, Matrix const& matrix, std::vector<Vector3>& destinationArray)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return Transform(sourceArray.data(), sourceArray.size(), matrix, destinationArray.data(), destinationArray.size());
    }

    bool Vector3::Transform(Vector3 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix, Vector3* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length)
    {
        if (!sourceArray || !destinationArray || sourceArrayLength < sourceIndex + length || destinationLength < destinationIndex + length)
            return false;

        for (size_t index = 0; index < length; ++index)
        {
            const auto& source = sourceArray[sourceIndex + index];
            destinationArray[destinationIndex + index].X = (source.X * matrix.M11 + source.Y * matrix.M21 + source.Z * matrix.M31) + matrix.M41;
            destinationArray[destinationIndex + index].Y = (source.X * matrix.M12 + source.Y * matrix.M22 + source.Z * matrix.M32) + matrix.M42;
            destinationArray[destinationIndex + index].Z = (source.X * matrix.M13 + source.Y * matrix.M23 + source.Z * matrix.M33) + matrix.M43;
        }

        return true;
    }

    bool Vector3::Transform(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return Transform(sourceArray.data(), sourceArray.size(), sourceIndex, matrix, destinationArray.data(), destinationArray.size(), destinationIndex, length);
    }

    bool Vector3::TransformNormal(Vector3 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector3* destinationArray, size_t destionationArrayLength)
    {
        if (!sourceArray || !destinationArray || sourceArrayLength < destionationArrayLength)
            return false;

        for (size_t index = 0; index < sourceArrayLength; ++index)
        {
            const auto& source = sourceArray[index];
            destinationArray[index].X = source.X * matrix.M11 + source.Y * matrix.M21 + source.Z * matrix.M31;
            destinationArray[index].Y = source.X * matrix.M12 + source.Y * matrix.M22 + source.Z * matrix.M32;
            destinationArray[index].Z = source.X * matrix.M13 + source.Y * matrix.M23 + source.Z * matrix.M33;
        }

        return true;
    }

    bool Vector3::TransformNormal(std::vector<Vector3> const& sourceArray, Matrix const& matrix, std::vector<Vector3>& destinationArray)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return TransformNormal(sourceArray.data(), sourceArray.size(), matrix, destinationArray.data(), destinationArray.size());
    }

    bool Vector3::TransformNormal(Vector3 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix, Vector3* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length)
    {
        if (!sourceArray || !destinationArray || sourceArrayLength < sourceIndex + length || destinationLength < destinationIndex + length)
            return false;

        for (size_t index = 0; index < length; ++index) {
            const auto& source = sourceArray[sourceIndex + index];
            destinationArray[destinationIndex + index].X = source.X * matrix.M11 + source.Y * matrix.M21 + source.Z * matrix.M31;
            destinationArray[destinationIndex + index].Y = source.X * matrix.M12 + source.Y * matrix.M22 + source.Z * matrix.M32;
            destinationArray[destinationIndex + index].Z = source.X * matrix.M13 + source.Y * matrix.M23 + source.Z * matrix.M33;
        }

        return true;
    }

    bool Vector3::TransformNormal(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return TransformNormal(sourceArray.data(), sourceArray.size(), sourceIndex, matrix, destinationArray.data(), destinationArray.size(), destinationIndex, length);
    }

    bool Vector3::TransformNormal(Vector3 const* sourceArray, size_t sourceArrayLength, Quaternion const& rotation, Vector3* destinationArray, size_t destinationLength)
    {
        if (!sourceArray || !destinationArray || destinationLength < sourceArrayLength)
            return false;

        const auto num1 = rotation.X + rotation.X;
        const auto num2 = rotation.Y + rotation.Y;
        const auto num3 = rotation.Z + rotation.Z;
        const auto num4 = rotation.W * num1;
        const auto num5 = rotation.W * num2;
        const auto num6 = rotation.W * num3;
        const auto num7 = rotation.X * num1;
        const auto num8 = rotation.X * num2;
        const auto num9 = rotation.X * num3;
        const auto num10 = rotation.Y * num2;
        const auto num11 = rotation.Y * num3;
        const auto num12 = rotation.Z * num3;
        const auto num13 = 1.0f - num10 - num12;
        const auto num14 = num8 - num6;
        const auto num15 = num9 + num5;
        const auto num16 = num8 + num6;
        const auto num17 = 1.0f - num7 - num12;
        const auto num18 = num11 - num4;
        const auto num19 = num9 - num5;
        const auto num20 = num11 + num4;
        const auto num21 = 1.0f - num7 - num10;

        for (size_t index = 0; index < sourceArrayLength; ++index)
        {
            const auto& source = sourceArray[index];
            destinationArray[index].X = source.X * num13 + source.Y * num14 + source.Z * num15;
            destinationArray[index].Y = source.X * num16 + source.Y * num17 + source.Z * num18;
            destinationArray[index].Z = source.X * num19 + source.Y * num20 + source.Z * num21;
        }

        return true;
    }

    bool Vector3::TransformNormal(std::vector<Vector3> const& sourceArray, Quaternion const& rotation, std::vector<Vector3>& destinationArray)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return TransformNormal(sourceArray.data(), sourceArray.size(), rotation, destinationArray.data(), destinationArray.size());
    }

    bool Vector3::TransformNormal(Vector3 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Quaternion const& rotation, Vector3* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length)
    {
        if (!sourceArray || !destinationArray || sourceArrayLength < sourceIndex + length || destinationLength < destinationIndex + length)
            return false;

        const auto num1 = rotation.X + rotation.X;
        const auto num2 = rotation.Y + rotation.Y;
        const auto num3 = rotation.Z + rotation.Z;
        const auto num4 = rotation.W * num1;
        const auto num5 = rotation.W * num2;
        const auto num6 = rotation.W * num3;
        const auto num7 = rotation.X * num1;
        const auto num8 = rotation.X * num2;
        const auto num9 = rotation.X * num3;
        const auto num10 = rotation.Y * num2;
        const auto num11 = rotation.Y * num3;
        const auto num12 = rotation.Z * num3;
        const auto num13 = 1.0f - num10 - num12;
        const auto num14 = num8 - num6;
        const auto num15 = num9 + num5;
        const auto num16 = num8 + num6;
        const auto num17 = 1.0f - num7 - num12;
        const auto num18 = num11 - num4;
        const auto num19 = num9 - num5;
        const auto num20 = num11 + num4;
        const auto num21 = 1.0f - num7 - num10;

        for (size_t index = 0; index < length; ++index)
        {
            const auto& source = sourceArray[sourceIndex + index];
            destinationArray[destinationIndex + index].X = source.X * num13 + source.Y * num14 + source.Z * num15;
            destinationArray[destinationIndex + index].Y = source.X * num16 + source.Y * num17 + source.Z * num18;
            destinationArray[destinationIndex + index].Z = source.X * num19 + source.Y * num20 + source.Z * num21;
        }

        return true;
    }

    bool Vector3::TransformNormal(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return TransformNormal(sourceArray.data(), sourceArray.size(), sourceIndex, rotation, destinationArray.data(), destinationArray.size(), destinationIndex, length);
    }

    bool Vector4::Transform(Vector4 const* sourceArray, size_t sourceLength, Matrix const& matrix, Vector4* destinationArray, size_t destinationLength)
    {
        if (!sourceArray || !destinationArray || destinationLength < sourceLength)
            return false;

        for (size_t index = 0; index < sourceLength; ++index)
        {
            const auto& source = sourceArray[index];
            destinationArray[index].X = source.X * matrix.M11 + source.Y * matrix.M21 + source.Z * matrix.M31 + source.W * matrix.M41;
            destinationArray[index].Y = source.X * matrix.M12 + source.Y * matrix.M22 + source.Z * matrix.M32 + source.W * matrix.M42;
            destinationArray[index].Z = source.X * matrix.M13 + source.Y * matrix.M23 + source.Z * matrix.M33 + source.W * matrix.M43;
            destinationArray[index].W = source.X * matrix.M14 + source.Y * matrix.M24 + source.Z * matrix.M34 + source.W * matrix.M44;
        }

        return true;
    }

    bool Vector4::Transform(std::vector<Vector4> const& sourceArray, size_t sourceLength, Matrix const& matrix, std::vector<Vector4>& destinationArray)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return Transform(sourceArray.data(), sourceArray.size(), matrix, destinationArray.data(), destinationArray.size());
    }

    bool Vector4::Transform(Vector4 const* sourceArray, size_t sourceLength, size_t sourceIndex, Matrix const& matrix, Vector4* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length)
    {
        if (!sourceArray || !destinationArray || sourceLength < sourceIndex + length || destinationLength < destinationIndex + length)
            return false;

        for (size_t i = 0; i < length; ++i)
        {
            const auto& source = sourceArray[sourceIndex + i];
            destinationArray[destinationIndex].X = source.X * matrix.M11 + source.Y * matrix.M21 + source.Z * matrix.M31 + source.W * matrix.M41;
            destinationArray[destinationIndex].Y = source.X * matrix.M12 + source.Y * matrix.M22 + source.Z * matrix.M32 + source.W * matrix.M42;
            destinationArray[destinationIndex].Z = source.X * matrix.M13 + source.Y * matrix.M23 + source.Z * matrix.M33 + source.W * matrix.M43;
            destinationArray[destinationIndex].W = source.X * matrix.M14 + source.Y * matrix.M24 + source.Z * matrix.M34 + source.W * matrix.M44;
        }

        return true;
    }

    bool Vector4::Transform(std::vector<Vector4> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector4>& destinationArray, size_t destinationIndex, size_t length)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return Transform(sourceArray.data(), sourceArray.size(), sourceIndex, matrix, destinationArray.data(), destinationArray.size(), destinationIndex, length);
    }

    bool Vector4::Transform(Vector4 const* sourceArray, size_t sourceLength, Quaternion const& rotation, Vector4* destinationArray, size_t destinationLength)
    {
        if (!sourceArray || !destinationArray || destinationLength < sourceLength)
            return false;

        const auto num1 = rotation.X + rotation.X;
        const auto num2 = rotation.Y + rotation.Y;
        const auto num3 = rotation.Z + rotation.Z;
        const auto num4 = rotation.W * num1;
        const auto num5 = rotation.W * num2;
        const auto num6 = rotation.W * num3;
        const auto num7 = rotation.X * num1;
        const auto num8 = rotation.X * num2;
        const auto num9 = rotation.X * num3;
        const auto num10 = rotation.Y * num2;
        const auto num11 = rotation.Y * num3;
        const auto num12 = rotation.Z * num3;
        const auto num13 = 1.0f - num10 - num12;
        const auto num14 = num8 - num6;
        const auto num15 = num9 + num5;
        const auto num16 = num8 + num6;
        const auto num17 = 1.0f - num7 - num12;
        const auto num18 = num11 - num4;
        const auto num19 = num9 - num5;
        const auto num20 = num11 + num4;
        const auto num21 = 1.0f - num7 - num10;

        for (size_t index = 0; index < sourceLength; ++index)
        {
            const auto& source = sourceArray[index];
            destinationArray[index].X = source.Z * num13 + source.Y * num14 + source.Z * num15;
            destinationArray[index].Y = source.Z * num16 + source.Y * num17 + source.Z * num18;
            destinationArray[index].Z = source.Z * num19 + source.Y * num20 + source.Z * num21;
            destinationArray[index].W = sourceArray[index].W;
        }

        return true;
    }

    bool Vector4::Transform(std::vector<Vector4> const& sourceArray, Quaternion const& rotation, std::vector<Vector4>& destinationArray)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return Transform(sourceArray.data(), sourceArray.size(), rotation, destinationArray.data(), destinationArray.size());
    }

    bool Vector4::Transform(Vector4 const* sourceArray, size_t sourceLength, size_t sourceIndex, Quaternion const& rotation, Vector4* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length)
    {
        if (!sourceArray || !destinationArray || sourceLength < sourceIndex + length || destinationLength < destinationIndex + length)
            return false;

        const auto num1 = rotation.X + rotation.X;
        const auto num2 = rotation.Y + rotation.Y;
        const auto num3 = rotation.Z + rotation.Z;
        const auto num4 = rotation.W * num1;
        const auto num5 = rotation.W * num2;
        const auto num6 = rotation.W * num3;
        const auto num7 = rotation.X * num1;
        const auto num8 = rotation.X * num2;
        const auto num9 = rotation.X * num3;
        const auto num10 = rotation.Y * num2;
        const auto num11 = rotation.Y * num3;
        const auto num12 = rotation.Z * num3;
        const auto num13 = 1.0f - num10 - num12;
        const auto num14 = num8 - num6;
        const auto num15 = num9 + num5;
        const auto num16 = num8 + num6;
        const auto num17 = 1.0f - num7 - num12;
        const auto num18 = num11 - num4;
        const auto num19 = num9 - num5;
        const auto num20 = num11 + num4;
        const auto num21 = 1.0f - num7 - num10;

        for (size_t i = 0; i < length; ++i)
        {
            const auto& source = sourceArray[sourceIndex + i];
            destinationArray[destinationIndex].X = source.X * num13 + source.Y * num14 + source.Z * num15;
            destinationArray[destinationIndex].Y = source.X * num16 + source.Y * num17 + source.Z * num18;
            destinationArray[destinationIndex].Z = source.X * num19 + source.Y * num20 + source.Z * num21;
            destinationArray[destinationIndex].W = source.W;
        }

        return true;
    }
    bool Vector4::Transform(std::vector<Vector4> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector4>& destinationArray, size_t destinationIndex, size_t length)
    {
        if (destinationArray.empty())
            destinationArray.resize(sourceArray.size());

        return Transform(sourceArray.data(), sourceArray.size(), sourceIndex, rotation, destinationArray.data(), destinationArray.size(), destinationIndex, length);
    }    

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
