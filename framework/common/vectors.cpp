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
}