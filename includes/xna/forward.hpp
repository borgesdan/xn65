#ifndef XNA_FORWARD_HPP
#define XNA_FORWARD_HPP

#include <memory>



struct RationalNumber;
struct Point;
struct Vector2;
struct Vector3;
struct Vector4;
struct Matrix;
struct Quaternion;

using P_RationalNumber = std::shared_ptr<RationalNumber>;
using P_Pointer = std::shared_ptr<Point>;
using P_Vector2 = std::shared_ptr<Vector2>;
using P_Vector3 = std::shared_ptr<Vector3>;
using P_Vector4 = std::shared_ptr<Vector4>;
using P_Quaternion = std::shared_ptr<Quaternion>;

#endif