#include "common/matrix.hpp"

namespace xna {
    Matrix Matrix::CreateBillboard(Vector3 const& objectPosition, Vector3 const& cameraPosition, Vector3 const& cameraUpVector, Vector3* cameraForwardVector)
    {
        Vector3 result1 = Vector3::Subtract(objectPosition, cameraPosition);
        const auto d = result1.LengthSquared();

        if (d < 9.9999997473787516E-05)
            result1 = cameraForwardVector ? -*cameraForwardVector : Vector3::Forward();
        else
            result1 = Vector3::Multiply(result1, static_cast<float>(1.0 / sqrt(d)));

        Vector3 result2 = Vector3::Cross(cameraUpVector, result1);
        result2.Normalize();

        Vector3 result3 = Vector3::Cross(result1, result2);

        Matrix result;
        result.M11 = result2.X;
        result.M12 = result2.Y;
        result.M13 = result2.Z;
        result.M14 = 0.0f;
        result.M21 = result3.X;
        result.M22 = result3.Y;
        result.M23 = result3.Z;
        result.M24 = 0.0f;
        result.M31 = result1.X;
        result.M32 = result1.Y;
        result.M33 = result1.Z;
        result.M34 = 0.0f;
        result.M41 = objectPosition.X;
        result.M42 = objectPosition.Y;
        result.M43 = objectPosition.Z;
        result.M44 = 1.0f;

        return result;
    }

    Matrix Matrix::CreateConstrainedBillboard(Vector3 const& objectPosition, Vector3 const& cameraPosition, Vector3 const& rotateAxis, Vector3* cameraForwardVector, Vector3* objectForwardVector)
    {
        Vector3 result1 = Vector3::Subtract(objectPosition, cameraPosition);
        const auto d = result1.LengthSquared();

        if (d < 9.9999997473787516E-05)
            result1 = cameraForwardVector ? -*cameraForwardVector : Vector3::Forward();
        else
            result1 = Vector3::Multiply(result1, static_cast<float>(1.0 / sqrt(d)));

        //Vector3 vector2 = rotateAxis;
        float result2 = Vector3::Dot(rotateAxis, result1);
        const auto factor = 0.998254656791687;
        const auto forward = Vector3::Forward();
        Vector3 result3 = forward;
        Vector3 result4;

        if (abs(result2) > factor) {
            if (objectForwardVector) {
                result2 = Vector3::Dot(rotateAxis, *objectForwardVector);

                if (abs(result2) > factor) {
                    auto const _abs = abs(rotateAxis.X * forward.X + rotateAxis.Y * forward.Y + rotateAxis.Z * forward.Z);
                    
                    if (_abs > factor)
                        result3 = Vector3::Right();
                }                    
            }
            else {
                if (abs(rotateAxis.X * forward.X + rotateAxis.Y * forward.Y + rotateAxis.Z * forward.Z) > factor)
                    result3 = Vector3::Right();
            }
            
            result4 = Vector3::Cross(rotateAxis, result3);
            result4.Normalize();

            result3 = Vector3::Cross(result4, rotateAxis);
            result3.Normalize();
        }
        else
        {
            result4 = Vector3::Cross(rotateAxis, result1);
            result4.Normalize();
            result3 = Vector3::Cross(result4, rotateAxis);
            result3.Normalize();
        }

        Matrix constrainedBillboard;
        constrainedBillboard.M11 = result4.X;
        constrainedBillboard.M12 = result4.Y;
        constrainedBillboard.M13 = result4.Z;
        constrainedBillboard.M14 = 0.0f;
        constrainedBillboard.M21 = rotateAxis.X;
        constrainedBillboard.M22 = rotateAxis.Y;
        constrainedBillboard.M23 = rotateAxis.Z;
        constrainedBillboard.M24 = 0.0f;
        constrainedBillboard.M31 = result3.X;
        constrainedBillboard.M32 = result3.Y;
        constrainedBillboard.M33 = result3.Z;
        constrainedBillboard.M34 = 0.0f;
        constrainedBillboard.M41 = objectPosition.X;
        constrainedBillboard.M42 = objectPosition.Y;
        constrainedBillboard.M43 = objectPosition.Z;
        constrainedBillboard.M44 = 1.0f;
        return constrainedBillboard;
    }

    Matrix Matrix::CreateRotationX(float radians)
	{
        const auto num1 = static_cast<float>(cos(radians));
        const auto num2 = static_cast<float>(sin(radians));

        Matrix rotationX;
        rotationX.M11 = 1.0f;
        rotationX.M12 = 0.0f;
        rotationX.M13 = 0.0f;
        rotationX.M14 = 0.0f;
        rotationX.M21 = 0.0f;
        rotationX.M22 = num1;
        rotationX.M23 = num2;
        rotationX.M24 = 0.0f;
        rotationX.M31 = 0.0f;
        rotationX.M32 = -num2;
        rotationX.M33 = num1;
        rotationX.M34 = 0.0f;
        rotationX.M41 = 0.0f;
        rotationX.M42 = 0.0f;
        rotationX.M43 = 0.0f;
        rotationX.M44 = 1.0f;
        return rotationX;
	}

    Matrix Matrix::CreateRotationY(float radians)
    {
        const auto num1 = static_cast<float>(cos(radians));
        const auto num2 = static_cast<float>(sin(radians));
        Matrix rotationY;
        rotationY.M11 = num1;
        rotationY.M12 = 0.0f;
        rotationY.M13 = -num2;
        rotationY.M14 = 0.0f;
        rotationY.M21 = 0.0f;
        rotationY.M22 = 1.0f;
        rotationY.M23 = 0.0f;
        rotationY.M24 = 0.0f;
        rotationY.M31 = num2;
        rotationY.M32 = 0.0f;
        rotationY.M33 = num1;
        rotationY.M34 = 0.0f;
        rotationY.M41 = 0.0f;
        rotationY.M42 = 0.0f;
        rotationY.M43 = 0.0f;
        rotationY.M44 = 1.0f;
        return rotationY;
    }

    Matrix Matrix::CreateRotationZ(float radians)
    {
        const auto num1 = static_cast<float>(cos(radians));
        const auto num2 = static_cast<float>(sin(radians));
        Matrix rotationZ;
        rotationZ.M11 = num1;
        rotationZ.M12 = num2;
        rotationZ.M13 = 0.0f;
        rotationZ.M14 = 0.0f;
        rotationZ.M21 = -num2;
        rotationZ.M22 = num1;
        rotationZ.M23 = 0.0f;
        rotationZ.M24 = 0.0f;
        rotationZ.M31 = 0.0f;
        rotationZ.M32 = 0.0f;
        rotationZ.M33 = 1.0f;
        rotationZ.M34 = 0.0f;
        rotationZ.M41 = 0.0f;
        rotationZ.M42 = 0.0f;
        rotationZ.M43 = 0.0f;
        rotationZ.M44 = 1.0f;
        return rotationZ;
    }

    Matrix Matrix::CreateFromAxisAngle(Vector3 const& axis, float angle)
    {
        const auto x = axis.X;
        const auto y = axis.Y;
        const auto z = axis.Z;
        const auto num1 = static_cast<float>(sin(angle));
        const auto num2 = static_cast<float>(cos(angle));
        const auto num3 = x * x;
        const auto num4 = y * y;
        const auto num5 = z * z;
        const auto num6 = x * y;
        const auto num7 = x * z;
        const auto num8 = y * z;

        Matrix fromAxisAngle;
        fromAxisAngle.M11 = num3 + num2 * (1.0f - num3);
        fromAxisAngle.M12 = (num6 - num2 * num6 + num1 * z);
        fromAxisAngle.M13 = (num7 - num2 * num7 - num1 * y);
        fromAxisAngle.M14 = 0.0f;
        fromAxisAngle.M21 = (num6 - num2 * num6 - num1 * z);
        fromAxisAngle.M22 = num4 + num2 * (1.0f - num4);
        fromAxisAngle.M23 = (num8 - num2 * num8 + num1 * x);
        fromAxisAngle.M24 = 0.0f;
        fromAxisAngle.M31 = (num7 - num2 * num7 + num1 * y);
        fromAxisAngle.M32 = (num8 - num2 * num8 - num1 * x);
        fromAxisAngle.M33 = num5 + num2 * (1.0f - num5);
        fromAxisAngle.M34 = 0.0f;
        fromAxisAngle.M41 = 0.0f;
        fromAxisAngle.M42 = 0.0f;
        fromAxisAngle.M43 = 0.0f;
        fromAxisAngle.M44 = 1.0f;
        return fromAxisAngle;
    }    

    Matrix Matrix::CreatePerspectiveFieldOfView(float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance) {
        if (fieldOfView <= 0.0 || fieldOfView >= MathHelper::PI || nearPlaneDistance <= 0.0 || farPlaneDistance <= 0.0 || nearPlaneDistance >= farPlaneDistance) {           
            return Matrix();
        }

        const auto num1 = static_cast<float>(1.0 / tan(fieldOfView * 0.5));
        const auto num2 = num1 / aspectRatio;

        Matrix perspectiveFieldOfView;
        perspectiveFieldOfView.M11 = num2;
        perspectiveFieldOfView.M12 = perspectiveFieldOfView.M13 = perspectiveFieldOfView.M14 = 0.0f;
        perspectiveFieldOfView.M22 = num1;
        perspectiveFieldOfView.M21 = perspectiveFieldOfView.M23 = perspectiveFieldOfView.M24 = 0.0f;
        perspectiveFieldOfView.M31 = perspectiveFieldOfView.M32 = 0.0f;
        perspectiveFieldOfView.M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
        perspectiveFieldOfView.M34 = -1.0f;
        perspectiveFieldOfView.M41 = perspectiveFieldOfView.M42 = perspectiveFieldOfView.M44 = 0.0f;
        perspectiveFieldOfView.M43 = (nearPlaneDistance * farPlaneDistance / (nearPlaneDistance - farPlaneDistance));
        return perspectiveFieldOfView;
    }

    Matrix Matrix::CreateLookAt(Vector3 const& cameraPosition, Vector3 const& cameraTarget, Vector3 const& cameraUpVector)
    {
        Vector3 vector3_1 = Vector3::Normalize(cameraPosition - cameraTarget);
        Vector3 vector3_2 = Vector3::Normalize(Vector3::Cross(cameraUpVector, vector3_1));
        Vector3 vector1 = Vector3::Cross(vector3_1, vector3_2);

        Matrix lookAt;
        lookAt.M11 = vector3_2.X;
        lookAt.M12 = vector1.X;
        lookAt.M13 = vector3_1.X;
        lookAt.M14 = 0.0f;
        lookAt.M21 = vector3_2.Y;
        lookAt.M22 = vector1.Y;
        lookAt.M23 = vector3_1.Y;
        lookAt.M24 = 0.0f;
        lookAt.M31 = vector3_2.Z;
        lookAt.M32 = vector1.Z;
        lookAt.M33 = vector3_1.Z;
        lookAt.M34 = 0.0f;
        lookAt.M41 = -Vector3::Dot(vector3_2, cameraPosition);
        lookAt.M42 = -Vector3::Dot(vector1, cameraPosition);
        lookAt.M43 = -Vector3::Dot(vector3_1, cameraPosition);
        lookAt.M44 = 1.0f;
        return lookAt;
    }

    Matrix Matrix::CreateWorld(Vector3 const& position, Vector3 const& forward, Vector3 const& up)
    {
        Vector3 vector3_1 = Vector3::Normalize(-forward);
        Vector3 vector2 = Vector3::Normalize(Vector3::Cross(up, vector3_1));
        Vector3 vector3_2 = Vector3::Cross(vector3_1, vector2);

        Matrix world;
        world.M11 = vector2.X;
        world.M12 = vector2.Y;
        world.M13 = vector2.Z;
        world.M14 = 0.0f;
        world.M21 = vector3_2.X;
        world.M22 = vector3_2.Y;
        world.M23 = vector3_2.Z;
        world.M24 = 0.0f;
        world.M31 = vector3_1.X;
        world.M32 = vector3_1.Y;
        world.M33 = vector3_1.Z;
        world.M34 = 0.0f;
        world.M41 = position.X;
        world.M42 = position.Y;
        world.M43 = position.Z;
        world.M44 = 1.0f;

        return world;
    }    
}