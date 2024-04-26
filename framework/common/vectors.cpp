#include "vectors.hpp"
#include "matrix.hpp"
#include "quaternion.hpp"

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

            destinationArray[destinationIndex].X = source.X * a + source.Y* b;
            destinationArray[destinationIndex].Y = source.X * c +source.Y * d;
            ++sourceIndex;
            ++destinationIndex;
        }

        return true;
    }

    void Vector3::Normalize() {
        const auto num = 1.0f / std::sqrt(X * X + Y * Y + Z * Z);
        X *= num;
        Y *= num;
        Z *= num;
    }

    Vector3 Vector3::Normalize(Vector3 const& value) {
        const auto num = 1.0f / std::sqrt(value.X * value.X + value.Y * value.Y + value.Z * value.Z);
        
        Vector3 vector3;
        vector3.X = value.X * num;
        vector3.Y = value.Y * num;
        vector3.Z = value.Z * num;

        return vector3;
    }

    bool xna::Vector3::Transform(std::vector<Vector3> const& sourceArray, Matrix const& matrix, std::vector<Vector3>& destinationArray)
    {
        if (destinationArray.size() < sourceArray.size())
            return false;

        for (size_t index = 0; index < sourceArray.size(); ++index)
        {
            const auto& source = sourceArray[index];
            destinationArray[index].X = (source.X * matrix.M11 + source.Y * matrix.M21 + source.Z * matrix.M31) + matrix.M41;
            destinationArray[index].Y = (source.X * matrix.M12 + source.Y * matrix.M22 + source.Z * matrix.M32) + matrix.M42;
            destinationArray[index].Z = (source.X * matrix.M13 + source.Y * matrix.M23 + source.Z * matrix.M33) + matrix.M43;
        }

        return true;
    }

    bool Vector3::Transform(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length)
    {
        if (sourceArray.size() < sourceIndex + length || destinationArray.size() < destinationIndex + length)
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

    bool Vector3::TransformNormal(std::vector<Vector3> const& sourceArray, Matrix const& matrix, std::vector<Vector3>& destinationArray)
    {
        if (destinationArray.size() < sourceArray.size())
            return false;

        for (size_t index = 0; index < sourceArray.size(); ++index)
        {
            const auto& source = sourceArray[index];
            destinationArray[index].X = source.X * matrix.M11 + source.Y * matrix.M21 + source.Z * matrix.M31;
            destinationArray[index].Y = source.X * matrix.M12 + source.Y * matrix.M22 + source.Z * matrix.M32;
            destinationArray[index].Z = source.X * matrix.M13 + source.Y * matrix.M23 + source.Z * matrix.M33;
        }

        return true;
    }

    bool Vector3::TransformNormal(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length)
    {
        if (sourceArray.size() < sourceIndex + length || destinationArray.size() < destinationIndex + length)
            return false;

        for (size_t index = 0; index < length; ++index) {
            const auto& source = sourceArray[sourceIndex + index];
            destinationArray[destinationIndex + index].X = source.X * matrix.M11 + source.Y * matrix.M21 + source.Z * matrix.M31;
            destinationArray[destinationIndex + index].Y = source.X * matrix.M12 + source.Y * matrix.M22 + source.Z * matrix.M32;
            destinationArray[destinationIndex + index].Z = source.X * matrix.M13 + source.Y * matrix.M23 + source.Z * matrix.M33;
        }

        return true;
    }

    bool Vector3::TransformNormal(std::vector<Vector3> const& sourceArray, Quaternion const& rotation, std::vector<Vector3>& destinationArray)
    {
        if (destinationArray.size() < sourceArray.size())
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

        for (size_t index = 0; index < sourceArray.size(); ++index)
        {
            const auto& source = sourceArray[index];
            destinationArray[index].X = source.X * num13 + source.Y * num14 + source.Z * num15;
            destinationArray[index].Y = source.X * num16 + source.Y * num17 + source.Z * num18;
            destinationArray[index].Z = source.X * num19 + source.Y * num20 + source.Z * num21;
        }

        return true;
    }

    bool Vector3::TransformNormal(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length)
    {
        if (sourceArray.size() < sourceIndex + length || destinationArray.size() < destinationIndex + length)
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
}