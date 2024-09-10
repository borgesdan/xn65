#ifndef XNA_COMMON_VECTORS_HPP
#define XNA_COMMON_VECTORS_HPP

#include <cmath>
#include "../default.hpp"
#include <optional>

namespace xna {
	//Represents a rational number.
	struct RationalNumber {
		constexpr RationalNumber() = default;

		constexpr RationalNumber(Uint numerator, Uint denominator)
			: Numerator(numerator), Denominator(denominator) {}

		constexpr bool operator==(const RationalNumber& other) const {
			return Numerator == other.Numerator && Denominator == other.Denominator;
		}

		//An unsigned integer value representing the top of the rational number.
		Uint Numerator{ 0 };
		//An unsigned integer value representing the bottom of the rational number.
		Uint Denominator{ 0 };
	};

	//Defines a point in 2D space.
	struct Point {
		Int X{ 0 };
		Int Y{ 0 };

		constexpr Point() = default;

		constexpr Point(const Int& X, const Int& Y)
			: X(X), Y(Y) {}

		constexpr bool operator==(const Point& other) const {
			return X == other.X && Y == other.Y;
		}
	};

	//Defines a rectangle. 
	struct Rectangle {
		Int X{ 0 };
		Int Y{ 0 };
		Int Width{ 0 };
		Int Height{ 0 };

		constexpr Rectangle() = default;

		constexpr Rectangle(const Int& X, const Int& Y, const Int& Width, const Int& Height) :
			X(X), Y(Y), Width(Width), Height(Height) {}

		constexpr bool operator==(const Rectangle& other) const {
			return Height == other.Height && Width == other.Width && X == other.X && Y == other.Y;
		}

		constexpr operator std::optional<Rectangle>() const {
			return std::make_optional<Rectangle>(X, Y, Width, Height);
		}

		//Returns the x-coordinate of the left side of the rectangle.
		constexpr Int Left() const { return X; }
		//Returns the x-coordinate of the right side of the rectangle.
		constexpr Int Right() const { return X + Width; }
		//Returns the y-coordinate of the top of the rectangle.
		constexpr Int Top() const { return Y; }
		//Returns the y-coordinate of the bottom of the rectangle.
		constexpr Int Bottom() const { return Y + Height; }
		//Gets or sets the upper-left value of the Rectangle.
		constexpr Point Location() const { return { X, Y }; }
		//Gets or sets the upper-left value of the Rectangle.
		constexpr void Location(Point const& p) {
			X = p.X;
			Y = p.Y;
		}
		//Gets the Point that specifies the center of the rectangle.
		constexpr Point Center() const { return { X + Width / 2, Y + Height / 2 }; }
		//Returns a Rectangle with all of its values set to zero.
		constexpr static Rectangle Empty() { return {}; }
		//Gets a value that indicates whether the Rectangle is empty.
		constexpr bool IsEmpty() const { return Width == 0 && Height == 0 && X == 0 && Y == 0; }

		//Changes the position of the Rectangle.
		constexpr void Offset(Point const& amount) {
			X += amount.X;
			Y += amount.Y;
		}

		//Changes the position of the Rectangle.
		constexpr void Offset(Int x, Int y) {
			X += x;
			Y += y;
		}

		//Pushes the edges of the Rectangle out by the horizontal and vertical values specified.
		constexpr void Inflate(Int horizontalAmount, Int verticalAmount) {
			X -= horizontalAmount;
			Y -= verticalAmount;
			Width += horizontalAmount * 2;
			Height += verticalAmount * 2;
		}

		//Determines whether this Rectangle contains a specified point or Rectangle.
		constexpr bool Contains(Int x, Int y) const {
			return X <= x && x < X + Width && Y <= y && y < Y + Height;
		}

		//Determines whether this Rectangle contains a specified point or Rectangle.
		constexpr bool Contains(Point const& value) const {
			return X <= value.X && value.X < X + Width && Y <= value.Y && value.Y < Y + Height;
		}

		//Determines whether this Rectangle contains a specified point or Rectangle.
		constexpr bool Contains(Rectangle const& value) const {
			return X <= value.X && value.X + value.Width <= X + Width && Y <= value.Y && value.Y + value.Height <= Y + Height;
		}

		//Determines whether a specified Rectangle intersects with this Rectangle.
		constexpr bool Intersects(Rectangle const& value) const {
			return value.X < X + Width && X < value.X + value.Width && value.Y < Y + Height && Y < value.Y + value.Height;
		}

		//Creates a Rectangle defining the area where one rectangle overlaps another rectangle. 
		constexpr static Rectangle Intersect(Rectangle const& value1, Rectangle const& value2) {
			const auto num1 = value1.X + value1.Width;
			const auto num2 = value2.X + value2.Width;
			const auto num3 = value1.Y + value1.Height;
			const auto num4 = value2.Y + value2.Height;
			const auto num5 = value1.X > value2.X ? value1.X : value2.X;
			const auto num6 = value1.Y > value2.Y ? value1.Y : value2.Y;
			const auto num7 = num1 < num2 ? num1 : num2;
			const auto num8 = num3 < num4 ? num3 : num4;

			Rectangle rectangle{};

			if (num7 > num5 && num8 > num6)
			{
				rectangle.X = num5;
				rectangle.Y = num6;
				rectangle.Width = num7 - num5;
				rectangle.Height = num8 - num6;
			}
			
			return rectangle;
		}

		//Creates a new Rectangle that exactly contains two other rectangles. 
		constexpr static Rectangle Union(Rectangle const& value1, Rectangle const& value2) {
			const auto num1 = value1.X + value1.Width;
			const auto num2 = value2.X + value2.Width;
			const auto num3 = value1.Y + value1.Height;
			const auto num4 = value2.Y + value2.Height;
			const auto num5 = value1.X < value2.X ? value1.X : value2.X;
			const auto num6 = value1.Y < value2.Y ? value1.Y : value2.Y;
			const auto num7 = num1 > num2 ? num1 : num2;
			const auto num8 = num3 > num4 ? num3 : num4;

			Rectangle rectangle;
			rectangle.X = num5;
			rectangle.Y = num6;
			rectangle.Width = num7 - num5;
			rectangle.Height = num8 - num6;
			
			return rectangle;
		}
	};

	//Defines a vector with two components. 
	struct Vector2 {
		float X{ 0 };
		float Y{ 0 };

		constexpr Vector2() = default;
		constexpr Vector2(float value) : X(value), Y(value) {}

		constexpr Vector2(float X, float Y)
			: X(X), Y(Y) {}

		constexpr bool operator==(const Vector2& other) const {
			return X == other.X && Y == other.Y;
		}
		
		//Returns a Vector2 with all of its components set to zero.
		static constexpr Vector2 Zero() { return {}; }
		//Returns a Vector2 with both of its components set to one.
		static constexpr Vector2 One() { return { 1 }; }
		//Returns the unit vector for the x-axis.
		static constexpr Vector2 UnitX() { return { 1, 0 }; }
		//Returns the unit vector for the y-axis.
		static constexpr Vector2 UnitY() { return { 0, 1 }; }

		inline float Length() const {
			return sqrt(LengthSquared());
		}

		constexpr float LengthSquared() const {
			return (X * X + Y * Y);
		}

		static inline float Distance(Vector2 const& value1, Vector2 const& value2) {
			return sqrt(DistanceSquared(value1, value2));
		}

		static constexpr float DistanceSquared(Vector2 const& value1, Vector2 const& value2) {
			const auto x = value1.X - value2.X;
			const auto y = value1.Y - value2.Y;
			return x * x + y * y;
		}

		static constexpr float Dot(Vector2 const& value1, Vector2 const& value2) {
			return value1.X * value2.X + value1.Y * value2.Y;
		}

		inline void Normalize() {
			const auto normal = 1.0f / Length();
			X *= normal;
			Y *= normal;
		}

		static inline Vector2 Normalize(Vector2 const& value) {
			auto v = value;
			v.Normalize();
			return v;
		}

		static constexpr Vector2 Reflect(Vector2 const& vector, Vector2 const& normal) {
			const auto num = vector.X * normal.X + vector.Y * normal.Y;
			return { vector.X - 2.0f * num * normal.X , vector.Y - 2.0f * num * normal.Y };
		}

		static constexpr Vector2 Min(Vector2 const& value1, Vector2 const& value2) {
			return {
				value1.X < value2.X ? value1.X : value2.X,
				value1.Y < value2.Y ? value1.Y : value2.Y
			};
		}

		static constexpr Vector2 Max(Vector2 const& value1, Vector2 const& value2) {
			return {
				value1.X > value2.X ? value1.X : value2.X,
				value1.Y > value2.Y ? value1.Y : value2.Y
			};
		}

		static constexpr Vector2 Clamp(Vector2 const& value1, Vector2 const& min, Vector2 const& max) {
			const auto _maxx = value1.X > max.X ? max.X : value1.X;
			const auto _x = _maxx < min.X ? min.X : _maxx;
			const auto _maxy = value1.Y > max.Y ? max.Y : value1.Y;
			const auto _y = _maxy < min.Y ? min.Y : _maxy;

			return { _x, _y };
		}

		static constexpr Vector2 Lerp(Vector2 const& value1, Vector2 const& value2, float amount) {
			return{
				value1.X + (value2.X - value1.X) * amount,
				value1.Y + (value2.Y - value1.Y) * amount
			};
		}

		static constexpr Vector2 Barycentric(Vector2 const& value1, Vector2 const& value2, Vector2 const& value3, float amount1, float amount2) {
			return {
				value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X),
				value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y)
			};
		}

		static constexpr Vector2 SmoothStep(Vector2 const& value1, Vector2 const& value2, float amount) {
			amount = amount > 1.0F ? 1.0f : (amount < 0.0F ? 0.0f : amount);
			amount = amount * amount * (3.0F - 2.0F * amount);

			return {
				value1.X + (value2.X - value1.X) * amount,
				value1.Y + (value2.Y - value1.Y) * amount
			};
		}

		static constexpr Vector2 CatmullRom(Vector2 const& value1, Vector2 const& value2, Vector2 const& value3, Vector2 const& value4, float amount) {
			const auto am2 = amount * amount;
			const auto am3 = amount * am2;
			Vector2 vector2;
			vector2.X = 0.5F * (2.0F * value2.X + (-value1.X + value3.X) * amount + (2.0F * value1.X - 5.0F * value2.X + 4.0F * value3.X - value4.X) * am2 + (-value1.X + 3.0F * value2.X - 3.0F * value3.X + value4.X) * am3);
			vector2.Y = 0.5F * (2.0F * value2.Y + (-value1.Y + value3.Y) * amount + (2.0F * value1.Y - 5.0F * value2.Y + 4.0F * value3.Y - value4.Y) * am2 + (-value1.Y + 3.0F * value2.Y - 3.0F * value3.Y + value4.Y) * am3);
			return vector2;
		}

		static constexpr Vector2 Hermite(Vector2 const& value1, Vector2 const& tangent1, Vector2 const& value2, Vector2 const& tangent2, float amount) {
			const auto am2 = amount * amount;
			const auto am3 = amount * am2;
			const auto num3 = (2.0F * am3 - 3.0F * am2 + 1.0F);
			const auto num4 = (-2.0F * am3 + 3.0F * am2);
			const auto num5 = am3 - 2.0F * am2 + amount;
			const auto am4 = am3 - am2;

			return{
				value1.X * num3 + value2.X * num4 + tangent1.X * num5 + tangent2.X * am4,
				value1.Y * num3 + value2.Y * num4 + tangent1.Y * num5 + tangent2.Y * am4
			};
		}

		static constexpr Vector2 Transform(Vector2 const& position, Matrix const& matrix);
		static constexpr Vector2 TransformNormal(Vector2 const& normal, Matrix const& matrix);
		static constexpr Vector2 Transform(Vector2 const& value, Quaternion const& rotation);
		static bool Transform(Vector2 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector2* destinationArray, size_t destinationArrayLength);
		static bool Transform(std::vector<Vector2> sourceArray, Matrix const& matrix, std::vector<Vector2>& destinationArray);
		static bool Transform(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix,
			Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length);
		static bool Transform(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length);
		static bool TransformNormal(Vector2 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector2* destinationArray, size_t destinationArrayLength);
		static bool TransformNormal(std::vector<Vector2> const& sourceArray, Matrix const& matrix, std::vector<Vector2>& destinationArray);
		static bool TransformNormal(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix,
			Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length);
		static bool TransformNormal(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length);
		static bool Transform(Vector2 const* sourceArray, size_t sourceArrayLength, Quaternion const& rotation, Vector2* destinationArray, size_t destinationArrayLength);
		static bool Transform(std::vector<Vector2> const& sourceArray, Quaternion const& rotation, std::vector<Vector2>& destinationArray);
		static bool Transform(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Quaternion const& rotation,
			Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length);
		static bool Transform(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length);

		static constexpr Vector2 Negate(Vector2 const& value) {
			return { -value.X, -value.Y };
		}

		static constexpr Vector2 Add(Vector2 const& value1, Vector2 const& value2) {
			return { value1.X + value2.X, value1.Y + value2.Y };
		}

		static constexpr Vector2 Subtract(Vector2 const& value1, Vector2 const& value2) {
			return { value1.X - value2.X, value1.Y - value2.Y };
		}

		static constexpr Vector2 Multiply(Vector2 const& value1, Vector2 const& value2) {
			return { value1.X * value2.X, value1.Y * value2.Y };
		}

		static constexpr Vector2 Multiply(Vector2 const& value1, float scaleFactor) {
			return { value1.X * scaleFactor, value1.Y * scaleFactor };
		}

		static constexpr Vector2 Divide(Vector2 const& value1, Vector2 const& value2) {
			return { value1.X / value2.X, value1.Y / value2.Y };
		}

		static constexpr Vector2 Divide(Vector2 const& value1, float divider) {
			return { value1.X / divider, value1.Y / divider };
		}

		constexpr Vector2 operator-() const {
			return Negate(*this);
		}

		friend constexpr Vector2 operator+(Vector2 const& value1, Vector2 const& value2) {
			return Vector2::Add(value1, value2);
		}

		friend constexpr Vector2 operator-(Vector2 const& value1, Vector2 const& value2) {
			return Vector2::Subtract(value1, value2);
		}

		friend constexpr Vector2 operator*(Vector2 const& value1, Vector2 const& value2) {
			return Vector2::Multiply(value1, value2);
		}

		friend constexpr Vector2 operator/(Vector2 const& value1, Vector2 const& value2) {
			return Vector2::Divide(value1, value2);
		}

		friend constexpr Vector2 operator*(Vector2 const& value1, float factor) {
			return Vector2::Multiply(value1, factor);
		}

		friend constexpr Vector2 operator/(Vector2 const& value1, float divider) {
			return Vector2::Divide(value1, divider);
		}

		friend constexpr Vector2 operator*(float factor, Vector2 const& value1) {
			return Vector2::Multiply(value1, factor);
		}

		friend constexpr Vector2 operator/(float divider, Vector2 const& value1) {
			return Vector2::Divide(value1, divider);
		}
	};

	//Defines a vector with three components. 
	struct Vector3 {
		float X{ 0 };
		float Y{ 0 };
		float Z{ 0 };

		constexpr Vector3() = default;

		constexpr Vector3(float value)
			: X(value), Y(value), Z(value) { }

		constexpr Vector3(float X, float Y, float Z)
			: X(X), Y(Y), Z(Z) { }

		constexpr Vector3(Vector2 const& value, float z)
			: X(value.X), Y(value.Y), Z(z) { }

		constexpr bool operator==(const Vector3& other) const {
			return X == other.X && Y == other.Y && Z == other.Z;
		}

		//Returns a Vector3 with all of its components set to zero.
		static Vector3 Zero() { return {}; }
		//Returns a Vector2 with both of its components set to one.
		static Vector3 One() { return { 1 }; }
		//Returns the x unit Vector3(1, 0, 0).
		static Vector3 UnitX() { return { 1,0,0 }; }
		//Returns the y unit Vector3 (0, 1, 0). 
		static Vector3 UnitY() { return { 0,1,0 }; }
		//Returns the z unit Vector3 (0, 0, 1). 
		static Vector3 UnitZ() { return { 0,0,1 }; }
		//Returns a unit vector designating up (0, 1, 0).
		static Vector3 Up() { return UnitY(); }
		//Returns a unit Vector3 designating down (0, −1, 0).
		static Vector3 Down() { return -UnitY(); }
		//Returns a unit Vector3 pointing to the right (1, 0, 0).
		static Vector3 Right() { return UnitX(); }
		//Returns a unit Vector3 designating left (−1, 0, 0).
		static Vector3 Left() { return -UnitX(); }
		//Returns a unit Vector3 designating forward in a right-handed coordinate system(0, 0, −1).
		static Vector3 Forward() { return -UnitZ(); }
		//Returns a unit Vector3 designating backward in a right-handed coordinate system (0, 0, 1).
		static Vector3 Backward() { return UnitZ(); }

		inline float Length() const { return sqrt(LengthSquared());	}
		constexpr float LengthSquared() const {	return (X * X + Y * Y + Z * Z);	}

		inline static float Distance(Vector3 const& value1, Vector3 const& value2) {
			return std::sqrt(DistanceSquared(value1, value2));
		}

		static constexpr float DistanceSquared(Vector3 const& value1, Vector3 const& value2) {
			const auto x = value1.X - value2.X;
			const auto y = value1.Y - value2.Y;
			const auto z = value1.Z - value2.Z;
			return x * x + y * y + z * z;
		}

		static constexpr float Dot(Vector3 const& vector1, Vector3 const& vector2) {
			return vector1.X * vector2.X + vector1.Y * vector2.Y + vector1.Z * vector2.Z;
		}

		inline void Normalize() {
			const auto num = 1.0f / Length();
			X *= num;
			Y *= num;
			Z *= num;
		}
		
		static Vector3 Normalize(Vector3 const& value) {
			auto v = value;
			v.Normalize();
			return v;
		}

		static constexpr Vector3 Cross(Vector3 const& vector1, Vector3 const& vector2) {
			Vector3 vector3;
			vector3.X = vector1.Y * vector2.Z - vector1.Z * vector2.Y;
			vector3.Y = vector1.Z * vector2.X - vector1.X * vector2.Z;
			vector3.Z = vector1.X * vector2.Y - vector1.Y * vector2.X;
			return vector3;
		}
		
		static constexpr Vector3 Reflect(Vector3 const& vector, Vector3 const& normal) {
			const auto num = vector.X * normal.X + vector.Y * normal.Y + vector.Z * normal.Z;
			Vector3 vector3;
			vector3.X = vector.X - 2.0f * num * normal.X;
			vector3.Y = vector.Y - 2.0f * num * normal.Y;
			vector3.Z = vector.Z - 2.0f * num * normal.Z;
			return vector3;
		}

		static constexpr Vector3 Min(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X < value2.X ? value1.X : value2.X;
			vector3.Y = value1.Y < value2.Y ? value1.Y : value2.Y;
			vector3.Z = value1.Z < value2.Z ? value1.Z : value2.Z;
			return vector3;
		}

		static constexpr Vector3 Max(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X >  value2.X ? value1.X : value2.X;
			vector3.Y = value1.Y >  value2.Y ? value1.Y : value2.Y;
			vector3.Z = value1.Z >  value2.Z ? value1.Z : value2.Z;
			return vector3;
		}

		static constexpr Vector3 Clamp(Vector3 const& value1, Vector3 const& min, Vector3 const& max) {
			const auto x = value1.X > max.X ? max.X : value1.X;
			const auto _x = x < min.X ? min.X : x;
			const auto y = value1.Y >  max.Y ? max.Y : value1.Y;
			const auto _y = y < min.Y ? min.Y : y;
			const auto z = value1.Z >  max.Z ? max.Z : value1.Z;
			const auto _z = z < min.Z ? min.Z : z;

			Vector3 vector3;
			vector3.X = _x;
			vector3.Y = _y;
			vector3.Z = _z;
			return vector3;
		}

		static constexpr Vector3 Lerp(Vector3 const& value1, Vector3 const& value2, float amount) {
			Vector3 vector3;
			vector3.X = value1.X + (value2.X - value1.X) * amount;
			vector3.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector3.Z = value1.Z + (value2.Z - value1.Z) * amount;
			return vector3;
		}

		static constexpr Vector3 Barycentric(Vector3 const& value1, Vector3 const& value2, Vector3 const& value3,	float amount1, float amount2) {
			Vector3 vector3;
			vector3.X = value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X);
			vector3.Y = value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y);
			vector3.Z = value1.Z + amount1 * (value2.Z - value1.Z) + amount2 * (value3.Z - value1.Z);
			return vector3;
		}

		static constexpr Vector3 SmoothStep(Vector3 const& value1, Vector3 const& value2, float amount) {
			amount = amount > 1.0F ? 1.0f : (amount < 0.0f ? 0.0f : amount);
			amount = (amount * amount * (3.0f - 2.0f * amount));
			Vector3 vector3;
			vector3.X = value1.X + (value2.X - value1.X) * amount;
			vector3.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector3.Z = value1.Z + (value2.Z - value1.Z) * amount;
			return vector3;
		}

		static constexpr Vector3 CatmullRom(Vector3 const& value1, Vector3 const& value2,	Vector3 const& value3, Vector3 const& value4, float amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			Vector3 vector3;
			vector3.X = 0.5f * (2.0f * value2.X + (-value1.X + value3.X) * amount + (2.0f * value1.X - 5.0f * value2.X + 4.0f * value3.X - value4.X) * num1 + (-value1.X + 3.0f * value2.X - 3.0f * value3.X + value4.X) * num2);
			vector3.Y = 0.5f * (2.0f * value2.Y + (-value1.Y + value3.Y) * amount + (2.0f * value1.Y - 5.0f * value2.Y + 4.0f * value3.Y - value4.Y) * num1 + (-value1.Y + 3.0f * value2.Y - 3.0f * value3.Y + value4.Y) * num2);
			vector3.Z = 0.5f * (2.0f * value2.Z + (-value1.Z + value3.Z) * amount + (2.0f * value1.Z - 5.0f * value2.Z + 4.0f * value3.Z - value4.Z) * num1 + (-value1.Z + 3.0f * value2.Z - 3.0f * value3.Z + value4.Z) * num2);
			return vector3;
		}

		static Vector3 Hermite(Vector3 const& value1, Vector3 const& tangent1, Vector3 const& value2, Vector3 const& tangent2, float amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			const auto num3 = 2.0F * num2 - 3.0F * num1 + 1.0F;
			const auto num4 = -2.0F * num2 + 3.0F * num1;
			const auto num5 = num2 - 2.0f * num1 + amount;
			const auto num6 = num2 - num1;
			Vector3 vector3;
			vector3.X = value1.X * num3 + value2.X * num4 + tangent1.X * num5 + tangent2.X * num6;
			vector3.Y = value1.Y * num3 + value2.Y * num4 + tangent1.Y * num5 + tangent2.Y * num6;
			vector3.Z = value1.Z * num3 + value2.Z * num4 + tangent1.Z * num5 + tangent2.Z * num6;
			return vector3;
		}

		static constexpr Vector3 Transform(Vector3 const& position, Matrix const& matrix);
		static constexpr Vector3 TransformNormal(Vector3 const& normal, Matrix const& matrix);
		static constexpr Vector3 Transform(Vector3 const& value, Quaternion const& rotation);
		static bool Transform(Vector3 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector3* destinationArray, size_t destinationLength);
		static bool Transform(std::vector<Vector3> const& sourceArray, Matrix const& matrix, std::vector<Vector3>& destinationArray);
		static bool Transform(Vector3 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix, Vector3* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length);
		static bool Transform(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length);
		static bool TransformNormal(Vector3 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector3* destinationArray, size_t destionationArrayLength);
		static bool TransformNormal(std::vector<Vector3> const& sourceArray, Matrix const& matrix, std::vector<Vector3>& destinationArray);
		static bool TransformNormal(Vector3 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix, Vector3* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length);
		static bool TransformNormal(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length);
		static bool TransformNormal(Vector3 const* sourceArray, size_t sourceArrayLength, Quaternion const& rotation,Vector3* destinationArray, size_t destinationLength);
		static bool TransformNormal(std::vector<Vector3> const& sourceArray, Quaternion const& rotation, std::vector<Vector3>& destinationArray);
		static bool TransformNormal(Vector3 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Quaternion const& rotation, Vector3* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length);
		static bool TransformNormal(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length);

		static constexpr Vector3 Negate(Vector3 const& value)
		{
			Vector3 vector3;
			vector3.X = -value.X;
			vector3.Y = -value.Y;
			vector3.Z = -value.Z;
			return vector3;
		}

		static constexpr Vector3 Add(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X + value2.X;
			vector3.Y = value1.Y + value2.Y;
			vector3.Z = value1.Z + value2.Z;
			return vector3;
		}
		
		static constexpr Vector3 Subtract(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X - value2.X;
			vector3.Y = value1.Y - value2.Y;
			vector3.Z = value1.Z - value2.Z;
			return vector3;
		}
		
		static constexpr Vector3 Multiply(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X * value2.X;
			vector3.Y = value1.Y * value2.Y;
			vector3.Z = value1.Z * value2.Z;
			return vector3;
		}
		
		static constexpr Vector3 Multiply(Vector3 const& value1, float scaleFactor) {
			Vector3 vector3;
			vector3.X = value1.X * scaleFactor;
			vector3.Y = value1.Y * scaleFactor;
			vector3.Z = value1.Z * scaleFactor;
			return vector3;
		}

		static constexpr Vector3 Divide(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X / value2.X;
			vector3.Y = value1.Y / value2.Y;
			vector3.Z = value1.Z / value2.Z;
			return vector3;
		}

		static constexpr Vector3 Divide(Vector3 const& value1, float divider) {
			float num = 1.0f / divider;
			Vector3 vector3;
			vector3.X = value1.X * num;
			vector3.Y = value1.Y * num;
			vector3.Z = value1.Z * num;
			return vector3;
		}

		constexpr Vector3 operator-() const {
			return Vector3::Negate(*this);
		}

		friend constexpr Vector3 operator+(Vector3 const& value1, Vector3 const& value2) {
			return Vector3::Add(value1, value2);
		}

		friend constexpr Vector3 operator-(Vector3 const& value1, Vector3 const& value2) {
			return Vector3::Subtract(value1, value2);
		}

		friend constexpr Vector3 operator*(Vector3 const& value1, Vector3 const& value2) {
			return Vector3::Multiply(value1, value2);
		}

		friend constexpr Vector3 operator*(Vector3 const& value, float factor) {
			return Vector3::Multiply(value, factor);
		}

		friend constexpr Vector3 operator*(float factor, Vector3 const& value) {
			return Vector3::Multiply(value, factor);
		}

		friend constexpr Vector3 operator/(Vector3 const& value1, Vector3 const& value2) {
			return Vector3::Divide(value1, value2);
		}

		friend constexpr Vector3 operator/(Vector3 const& value, float divider) {
			return Vector3::Divide(value, divider);
		}

		friend constexpr Vector3 operator/(float divider, Vector3 const& value) {
			return Vector3::Divide(value, divider);
		}

		friend constexpr Vector3 operator+=(Vector3 const& value1, Vector3 const& value2) {
			auto vec = value1;
			vec.X += value2.X;
			vec.Y += value2.Y;
			return vec;
		}

		friend constexpr Vector3 operator-=(Vector3 const& value1, Vector3 const& value2) {
			auto vec = value1;
			vec.X -= value2.X;
			vec.Y -= value2.Y;
			return vec;
		}
	};

	struct Vector4 {
		float X{ 0 };
		float Y{ 0 };
		float Z{ 0 };
		float W{ 0 };

		constexpr Vector4() = default;

		constexpr Vector4(float value):
			X(value), Y(value), Z(value), W(value){}

		constexpr Vector4(float X, float Y, float Z, float W)
			: X(X), Y(Y), Z(Z), W(W) { }

		constexpr Vector4(Vector2 value, float Z, float W)
			: X(value.X), Y(value.Y), Z(Z), W(W) { }

		constexpr Vector4(Vector3 value, float W)
			: X(value.X), Y(value.Y), Z(value.Z), W(W) { }

		constexpr bool operator==(const Vector4& other) const {
			return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
		}

		static Vector4 Zero() { return {}; }
		static Vector4 One() { return { 1 }; }
		static Vector4 UnitX() { return { 1,0,0,0 }; }
		static Vector4 UnitY() { return { 0,1,0,0 }; }
		static Vector4 UnitZ() { return { 0,0,1,0 }; }
		static Vector4 UnitW() { return { 0,0,0,1 }; }

		float Length() const {
			return sqrt(LengthSquared());
		}

		constexpr float LengthSquared() const {
			return (X * X + Y * Y + Z * Z + W * W);
		}

		inline static float Distance(Vector4 const& value1, Vector4 const& value2) {
			return sqrt(DistanceSquared(value1, value2));
		}

		static constexpr float DistanceSquared(Vector4 const& value1, Vector4 const& value2) {
			const auto num1 = value1.X - value2.X;
			const auto num2 = value1.Y - value2.Y;
			const auto num3 = value1.Z - value2.Z;
			const auto num4 = value1.W - value2.W;
			return num1 * num1 + num2 * num2 + num3 * num3 + num4 * num4;
		}

		static constexpr float Dot(Vector4 const& vector1, Vector4 const& vector2) {
			return vector1.X * vector2.X + vector1.Y * vector2.Y + vector1.Z * vector2.Z + vector1.W * vector2.W;
		}

		void Normalize() {
			const auto num = 1.0f / Length();
			X *= num;
			Y *= num;
			Z *= num;
			W *= num;
		}

		inline static Vector4 Normalize(Vector4 const& vector) {
			auto v = vector;
			v.Normalize();
			return v;
		}

		static constexpr Vector4 Min(Vector4 const& value1, Vector4 const& value2) {
			Vector4 vector4;
			vector4.X = value1.X < value2.X ? value1.X : value2.X;
			vector4.Y = value1.Y < value2.Y ? value1.Y : value2.Y;
			vector4.Z = value1.Z < value2.Z ? value1.Z : value2.Z;
			vector4.W = value1.W < value2.W ? value1.W : value2.W;
			return vector4;
		}

		static constexpr Vector4 Max(Vector4 const& value1, Vector4 const& value2) {
			Vector4 vector4;
			vector4.X = value1.X >  value2.X ? value1.X : value2.X;
			vector4.Y = value1.Y >  value2.Y ? value1.Y : value2.Y;
			vector4.Z = value1.Z >  value2.Z ? value1.Z : value2.Z;
			vector4.W = value1.W > value2.W ? value1.W : value2.W;
			return vector4;
		}

		static Vector4 Clamp(Vector4 const& value1, Vector4 const& min, Vector4 const& max) {
			const auto x = value1.X;
			const auto num1 = x > max.X ? max.X : x;
			const auto num2 = num1 < min.X ? min.X : num1;
			const auto y = value1.Y;
			const auto num3 = y >  max.Y ? max.Y : y;
			const auto num4 = num3 < min.Y ? min.Y : num3;
			const auto z = value1.Z;
			const auto num5 = z >  max.Z ? max.Z : z;
			const auto num6 = num5 < min.Z ? min.Z : num5;
			const auto w = value1.W;
			const auto num7 = w >  max.W ? max.W : w;
			const auto num8 = num7 < min.W ? min.W : num7;
			Vector4 vector4;
			vector4.X = num2;
			vector4.Y = num4;
			vector4.Z = num6;
			vector4.W = num8;
			return vector4;
		}

		static Vector4 Lerp(Vector4 const& value1, Vector4 const& value2, float amount) {
			Vector4 vector4;
			vector4.X = value1.X + (value2.X - value1.X) * amount;
			vector4.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector4.Z = value1.Z + (value2.Z - value1.Z) * amount;
			vector4.W = value1.W + (value2.W - value1.W) * amount;
			return vector4;
		}

		static Vector4 Barycentric(Vector4 const& value1, Vector4 const& value2, Vector4 const& value3,	float amount1,	float amount2) {
			Vector4 vector4;
			vector4.X = value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X);
			vector4.Y = value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y);
			vector4.Z = value1.Z + amount1 * (value2.Z - value1.Z) + amount2 * (value3.Z - value1.Z);
			vector4.W = value1.W + amount1 * (value2.W - value1.W) + amount2 * (value3.W - value1.W);
			return vector4;
		}

		static Vector4 SmoothStep(Vector4 const& value1, Vector4 const& value2, float amount) {
			amount = amount > 1.0F ? 1.0f : (amount < 0.0F ? 0.0f : amount);
			amount = (amount * amount * (3.0F - 2.0F * amount));
			Vector4 vector4;
			vector4.X = value1.X + (value2.X - value1.X) * amount;
			vector4.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector4.Z = value1.Z + (value2.Z - value1.Z) * amount;
			vector4.W = value1.W + (value2.W - value1.W) * amount;
			return vector4;
		}

		static Vector4 CatmullRom(Vector4 const& value1, Vector4 const& value2, Vector4 const& value3, Vector4 const& value4, float amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			Vector4 vector4;
			vector4.X = 0.5F * (2.0f * value2.X + (-value1.X + value3.X) * amount + (2.0F * value1.X - 5.0f * value2.X + 4.0f * value3.X - value4.X) * num1 + (-value1.X + 3.0f * value2.X - 3.0f * value3.X + value4.X) * num2);
			vector4.Y = 0.5F * (2.0f * value2.Y + (-value1.Y + value3.Y) * amount + (2.0F * value1.Y - 5.0f * value2.Y + 4.0f * value3.Y - value4.Y) * num1 + (-value1.Y + 3.0f * value2.Y - 3.0f * value3.Y + value4.Y) * num2);
			vector4.Z = 0.5F * (2.0f * value2.Z + (-value1.Z + value3.Z) * amount + (2.0F * value1.Z - 5.0f * value2.Z + 4.0f * value3.Z - value4.Z) * num1 + (-value1.Z + 3.0f * value2.Z - 3.0f * value3.Z + value4.Z) * num2);
			vector4.W = 0.5F * (2.0f * value2.W + (-value1.W + value3.W) * amount + (2.0F * value1.W - 5.0f * value2.W + 4.0f * value3.W - value4.W) * num1 + (-value1.W + 3.0f * value2.W - 3.0f * value3.W + value4.W) * num2);
			return vector4;
		}

		static Vector4 Hermite(Vector4 const& value1, Vector4 const& tangent1, Vector4 const& value2, Vector4 const& tangent2, float amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			const auto num3 = (2.0F * num2 - 3.0F * num1 + 1.0F);
			const auto num4 = (-2.0F * num2 + 3.0F * num1);
			const auto num5 = num2 - 2.0f * num1 + amount;
			const auto num6 = num2 - num1;
			Vector4 vector4;
			vector4.X = (value1.X * num3 + value2.X * num4 + tangent1.X * num5 + tangent2.X * num6);
			vector4.Y = (value1.Y * num3 + value2.Y * num4 + tangent1.Y * num5 + tangent2.Y * num6);
			vector4.Z = (value1.Z * num3 + value2.Z * num4 + tangent1.Z * num5 + tangent2.Z * num6);
			vector4.W = (value1.W * num3 + value2.W * num4 + tangent1.W * num5 + tangent2.W * num6);
			return vector4;
		}

		static constexpr Vector4 Transform(Vector2 const& position, Matrix const& matrix);
		static constexpr Vector4 Transform(Vector3 const& position, Matrix const& matrix);
		static constexpr Vector4 Transform(Vector4 const& vector, Matrix const& matrix);
		static constexpr Vector4 Transform(Vector2 const& value, Quaternion const& rotation);
		static constexpr Vector4 Transform(Vector3 const& value, Quaternion const& rotation);
		static bool Transform(Vector4 const* sourceArray, size_t sourceLength, Matrix const& matrix, Vector4* destinationArray, size_t destinationLength);
		static bool Transform(std::vector<Vector4> const& sourceArray, size_t sourceLength, Matrix const& matrix, std::vector<Vector4>& destinationArray);
		static bool Transform(Vector4 const* sourceArray, size_t sourceLength, size_t sourceIndex, Matrix const& matrix, Vector4* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length);
		static bool Transform(std::vector<Vector4> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector4>& destinationArray, size_t destinationIndex, size_t length);
		static bool Transform(Vector4 const* sourceArray, size_t sourceLength, Quaternion const& rotation, Vector4* destinationArray, size_t destinationLength);
		static bool Transform(std::vector<Vector4> const& sourceArray, Quaternion const& rotation, std::vector<Vector4>& destinationArray);
		static bool Transform(Vector4 const* sourceArray, size_t sourceLength, size_t sourceIndex, Quaternion const& rotation, Vector4* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length);
		static bool Transform(std::vector<Vector4> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector4>& destinationArray, size_t destinationIndex, size_t length);

		static constexpr Vector4 Negate(Vector4 const& value) {
			Vector4 vector4;
			vector4.X = -value.X;
			vector4.Y = -value.Y;
			vector4.Z = -value.Z;
			vector4.W = -value.W;
			return vector4;
		}		

		static constexpr Vector4 Add(Vector4 const& value1, Vector4 const& value2) {
			Vector4 vector4;
			vector4.X = value1.X + value2.X;
			vector4.Y = value1.Y + value2.Y;
			vector4.Z = value1.Z + value2.Z;
			vector4.W = value1.W + value2.W;
			return vector4;
		}

		static constexpr Vector4 Subtract(Vector4 const& value1, Vector4 const& value2) {
			Vector4 vector4;
			vector4.X = value1.X - value2.X;
			vector4.Y = value1.Y - value2.Y;
			vector4.Z = value1.Z - value2.Z;
			vector4.W = value1.W - value2.W;
			return vector4;
		}		

		static constexpr Vector4 Multiply(Vector4 const& value1, Vector4 const& value2)
		{
			Vector4 vector4;
			vector4.X = value1.X * value2.X;
			vector4.Y = value1.Y * value2.Y;
			vector4.Z = value1.Z * value2.Z;
			vector4.W = value1.W * value2.W;
			return vector4;
		}
		

		static constexpr Vector4 Multiply(Vector4 const& value1, float scaleFactor)
		{
			Vector4 vector4;
			vector4.X = value1.X * scaleFactor;
			vector4.Y = value1.Y * scaleFactor;
			vector4.Z = value1.Z * scaleFactor;
			vector4.W = value1.W * scaleFactor;
			return vector4;
		}		

		static constexpr Vector4 Divide(Vector4 const& value1, Vector4 const& value2)
		{
			Vector4 vector4;
			vector4.X = value1.X / value2.X;
			vector4.Y = value1.Y / value2.Y;
			vector4.Z = value1.Z / value2.Z;
			vector4.W = value1.W / value2.W;
			return vector4;
		}		

		static constexpr Vector4 Divide(Vector4 const& value1, float divider)
		{
			float num = 1.0f / divider;
			Vector4 vector4;
			vector4.X = value1.X * num;
			vector4.Y = value1.Y * num;
			vector4.Z = value1.Z * num;
			vector4.W = value1.W * num;
			return vector4;
		}	

		constexpr Vector4 operator-() const {
			return Vector4::Negate(*this);
		}

		friend constexpr Vector4 operator+(Vector4 const& value1, Vector4 const& value2) {
			return Vector4::Add(value1, value2);
		}

		friend constexpr Vector4 operator-(Vector4 const& value1, Vector4 const& value2) {
			return Vector4::Subtract(value1, value2);
		}

		friend constexpr Vector4 operator*(Vector4 const& value1, Vector4 const& value2) {
			return Vector4::Multiply(value1, value2);
		}

		friend constexpr Vector4 operator*(Vector4 const& value, float factor) {
			return Vector4::Multiply(value, factor);
		}

		friend constexpr Vector4 operator*(float factor, Vector4 const& value) {
			return Vector4::Multiply(value, factor);
		}

		friend constexpr Vector4 operator/(Vector4 const& value1, Vector4 const& value2) {
			return Vector4::Divide(value1, value2);
		}

		friend constexpr Vector4 operator/(Vector4 const& value, float divider) {
			return Vector4::Divide(value, divider);
		}

		friend constexpr Vector4 operator/(float divider, Vector4 const& value) {
			return Vector4::Divide(value, divider);
		}
	};

	struct Matrix {
		float M11{ 0 };
		float M12{ 0 };
		float M13{ 0 };
		float M14{ 0 };
		float M21{ 0 };
		float M22{ 0 };
		float M23{ 0 };
		float M24{ 0 };
		float M31{ 0 };
		float M32{ 0 };
		float M33{ 0 };
		float M34{ 0 };
		float M41{ 0 };
		float M42{ 0 };
		float M43{ 0 };
		float M44{ 0 };

		constexpr Matrix() = default;

		constexpr Matrix(
			float M11, float M12, float M13, float M14,
			float M21, float M22, float M23, float M24,
			float M31, float M32, float M33, float M34,
			float M41, float M42, float M43, float M44) :
			M11(M11), M12(M12), M13(M13), M14(M14),
			M21(M21), M22(M22), M23(M23), M24(M24),
			M31(M31), M32(M32), M33(M33), M34(M34),
			M41(M41), M42(M42), M43(M43), M44(M44) { }

		constexpr bool operator==(const Matrix& other) const {
			return M11 == other.M11 && M12 == other.M12 && M13 == other.M13 && M14 == other.M14
				&& M21 == other.M21 && M22 == other.M22 && M23 == other.M23 && M24 == other.M24
				&& M31 == other.M31 && M32 == other.M32 && M33 == other.M33 && M34 == other.M34
				&& M41 == other.M41 && M42 == other.M42 && M43 == other.M43 && M44 == other.M44;
		}

		constexpr static Matrix Identity() {
			return Matrix(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
		}

		constexpr Vector3 Up() const noexcept { return Vector3(M21, M22, M23); }
		constexpr Vector3 Down() const noexcept { return Vector3(-M21, -M22, -M23); }
		constexpr Vector3 Right() const noexcept { return Vector3(M11, M12, M13); }
		constexpr Vector3 Left() const noexcept { return Vector3(-M11, -M12, -M13); }
		constexpr Vector3 Forward() const noexcept { return Vector3(-M31, -M32, -M33); }
		constexpr Vector3 Backward() const noexcept { return Vector3(M31, M32, M33); }
		constexpr Vector3 Translation() const noexcept { return Vector3(M41, M42, M43); }

		constexpr void Up(Vector3 const& value) noexcept {
			M21 = value.X;
			M22 = value.Y;
			M23 = value.Z;
		}

		constexpr void Down(Vector3 const& value) noexcept {
			M21 = -value.X;
			M22 = -value.Y;
			M23 = -value.Z;
		}

		constexpr void Right(Vector3 const& value) noexcept {
			M11 = value.X;
			M12 = value.Y;
			M13 = value.Z;
		}

		constexpr void Left(Vector3 const& value) noexcept {
			M11 = -value.X;
			M12 = -value.Y;
			M13 = -value.Z;
		}

		constexpr void Forward(Vector3 const& value) noexcept {
			M31 = -value.X;
			M32 = -value.Y;
			M33 = -value.Z;
		}

		constexpr void Backward(Vector3 const& value) noexcept {
			M31 = value.X;
			M32 = value.Y;
			M33 = value.Z;
		}

		constexpr void Translation(Vector3 const& value) noexcept {
			M41 = value.X;
			M42 = value.Y;
			M43 = value.Z;
		}

		static Matrix CreateBillboard(Vector3 const& objectPosition, Vector3 const& cameraPosition, Vector3 const& cameraUpVector, Vector3* cameraForwardVector);

		static Matrix CreateConstrainedBillboard(
			Vector3 const& objectPosition,
			Vector3 const& cameraPosition,
			Vector3 const& rotateAxis,
			Vector3* cameraForwardVector,
			Vector3* objectForwardVector);

		static constexpr Matrix CreateTranslation(Vector3 const& position) {
			Matrix translation;
			translation.M11 = 1.0f;
			translation.M12 = 0.0f;
			translation.M13 = 0.0f;
			translation.M14 = 0.0f;
			translation.M21 = 0.0f;
			translation.M22 = 1.0f;
			translation.M23 = 0.0f;
			translation.M24 = 0.0f;
			translation.M31 = 0.0f;
			translation.M32 = 0.0f;
			translation.M33 = 1.0f;
			translation.M34 = 0.0f;
			translation.M41 = position.X;
			translation.M42 = position.Y;
			translation.M43 = position.Z;
			translation.M44 = 1.0f;
			return translation;
		}

		static constexpr Matrix CreateTranslation(float xPosition, float yPosition, float zPosition) {
			Matrix translation;
			translation.M11 = 1.0f;
			translation.M12 = 0.0f;
			translation.M13 = 0.0f;
			translation.M14 = 0.0f;
			translation.M21 = 0.0f;
			translation.M22 = 1.0f;
			translation.M23 = 0.0f;
			translation.M24 = 0.0f;
			translation.M31 = 0.0f;
			translation.M32 = 0.0f;
			translation.M33 = 1.0f;
			translation.M34 = 0.0f;
			translation.M41 = xPosition;
			translation.M42 = yPosition;
			translation.M43 = zPosition;
			translation.M44 = 1.0f;
			return translation;
		}

		static constexpr Matrix CreateScale(float xScale, float yScale, float zScale) {
			Matrix scale;
			scale.M11 = xScale;
			scale.M12 = 0.0f;
			scale.M13 = 0.0f;
			scale.M14 = 0.0f;
			scale.M21 = 0.0f;
			scale.M22 = yScale;
			scale.M23 = 0.0f;
			scale.M24 = 0.0f;
			scale.M31 = 0.0f;
			scale.M32 = 0.0f;
			scale.M33 = zScale;
			scale.M34 = 0.0f;
			scale.M41 = 0.0f;
			scale.M42 = 0.0f;
			scale.M43 = 0.0f;
			scale.M44 = 1.0f;
			return scale;
		}

		static constexpr Matrix CreateScale(Vector3 const& scales) {
			Matrix scale;
			scale.M11 = scales.X;
			scale.M12 = 0.0f;
			scale.M13 = 0.0f;
			scale.M14 = 0.0f;
			scale.M21 = 0.0f;
			scale.M22 = scales.Y;
			scale.M23 = 0.0f;
			scale.M24 = 0.0f;
			scale.M31 = 0.0f;
			scale.M32 = 0.0f;
			scale.M33 = scales.Z;
			scale.M34 = 0.0f;
			scale.M41 = 0.0f;
			scale.M42 = 0.0f;
			scale.M43 = 0.0f;
			scale.M44 = 1.0f;
			return scale;
		}

		static constexpr Matrix CreateScale(float scale) {
			Matrix scale1;
			scale1.M11 = scale;
			scale1.M12 = 0.0f;
			scale1.M13 = 0.0f;
			scale1.M14 = 0.0f;
			scale1.M21 = 0.0f;
			scale1.M22 = scale;
			scale1.M23 = 0.0f;
			scale1.M24 = 0.0f;
			scale1.M31 = 0.0f;
			scale1.M32 = 0.0f;
			scale1.M33 = scale;
			scale1.M34 = 0.0f;
			scale1.M41 = 0.0f;
			scale1.M42 = 0.0f;
			scale1.M43 = 0.0f;
			scale1.M44 = 1.0f;
			return scale1;
		}

		static Matrix CreateRotationX(float radians);
		static Matrix CreateRotationY(float radians);
		static Matrix CreateRotationZ(float radians);
		static Matrix CreateFromAxisAngle(Vector3 const& axis, float angle);
		static Matrix CreatePerspectiveFieldOfView(
			float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance);

		static constexpr Matrix CreatePerspective(
			float width, float height, float nearPlaneDistance, float farPlaneDistance) {
			if (nearPlaneDistance <= 0.0 || farPlaneDistance <= 0.0 || nearPlaneDistance >= farPlaneDistance) {
				return Matrix();
			}

			Matrix perspective;
			perspective.M11 = 2.0f * nearPlaneDistance / width;
			perspective.M12 = perspective.M13 = perspective.M14 = 0.0f;
			perspective.M22 = 2.0f * nearPlaneDistance / height;
			perspective.M21 = perspective.M23 = perspective.M24 = 0.0f;
			perspective.M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
			perspective.M31 = perspective.M32 = 0.0f;
			perspective.M34 = -1.0f;
			perspective.M41 = perspective.M42 = perspective.M44 = 0.0f;
			perspective.M43 = (nearPlaneDistance * farPlaneDistance / (nearPlaneDistance - farPlaneDistance));
			return perspective;
		}

		static constexpr Matrix CreatePerspectiveOffCenter(
			float left, float right, float bottom, float top,
			float nearPlaneDistance, float farPlaneDistance) {
			if (nearPlaneDistance <= 0.0 || farPlaneDistance <= 0.0 || nearPlaneDistance >= farPlaneDistance) {
				return Matrix();
			}

			Matrix perspectiveOffCenter;
			perspectiveOffCenter.M11 = (2.0F * nearPlaneDistance / (right - left));
			perspectiveOffCenter.M12 = perspectiveOffCenter.M13 = perspectiveOffCenter.M14 = 0.0f;
			perspectiveOffCenter.M22 = (2.0F * nearPlaneDistance / (top - bottom));
			perspectiveOffCenter.M21 = perspectiveOffCenter.M23 = perspectiveOffCenter.M24 = 0.0f;
			perspectiveOffCenter.M31 = (left + right) / (right - left);
			perspectiveOffCenter.M32 = (top + bottom) / (top - bottom);
			perspectiveOffCenter.M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
			perspectiveOffCenter.M34 = -1.0f;
			perspectiveOffCenter.M43 = (nearPlaneDistance * farPlaneDistance / (nearPlaneDistance - farPlaneDistance));
			perspectiveOffCenter.M41 = perspectiveOffCenter.M42 = perspectiveOffCenter.M44 = 0.0f;
			return perspectiveOffCenter;
		}

		static constexpr Matrix CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane) {
			Matrix orthographic;
			orthographic.M11 = 2.0f / width;
			orthographic.M12 = orthographic.M13 = orthographic.M14 = 0.0f;
			orthographic.M22 = 2.0f / height;
			orthographic.M21 = orthographic.M23 = orthographic.M24 = 0.0f;
			orthographic.M33 = (1.0F / (zNearPlane - zFarPlane));
			orthographic.M31 = orthographic.M32 = orthographic.M34 = 0.0f;
			orthographic.M41 = orthographic.M42 = 0.0f;
			orthographic.M43 = zNearPlane / (zNearPlane - zFarPlane);
			orthographic.M44 = 1.0f;
			return orthographic;
		}

		static constexpr Matrix CreateOrthographicOffCenter(
			float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) {
			Matrix orthographicOffCenter;
			orthographicOffCenter.M11 = (2.0F / (right - left));
			orthographicOffCenter.M12 = orthographicOffCenter.M13 = orthographicOffCenter.M14 = 0.0f;
			orthographicOffCenter.M22 = (2.0F / (top - bottom));
			orthographicOffCenter.M21 = orthographicOffCenter.M23 = orthographicOffCenter.M24 = 0.0f;
			orthographicOffCenter.M33 = (1.0F / (zNearPlane - zFarPlane));
			orthographicOffCenter.M31 = orthographicOffCenter.M32 = orthographicOffCenter.M34 = 0.0f;
			orthographicOffCenter.M41 = ((left + right) / (left - right));
			orthographicOffCenter.M42 = ((top + bottom) / (bottom - top));
			orthographicOffCenter.M43 = zNearPlane / (zNearPlane - zFarPlane);
			orthographicOffCenter.M44 = 1.0f;
			return orthographicOffCenter;
		}

		static Matrix CreateLookAt(
			Vector3 const& cameraPosition, Vector3 const& cameraTarget, Vector3 const& cameraUpVector);
		static Matrix CreateWorld(
			Vector3 const& position, Vector3 const& forward, Vector3 const& up);

		static Matrix CreateFromQuaternion(Quaternion const& quaternion);
		static Matrix CreateFromYawPitchRoll(float yaw, float pitch, float roll);
		static Matrix CreateShadow(Vector3 lightDirection, Plane plane);
		static Matrix CreateReflection(Plane value);
		static Matrix Transform(Matrix value, Quaternion rotation);
		static Matrix Transpose(Matrix matrix);

		constexpr float Determinant() const {
			const auto num1 = (M33 * M44 - M34 * M43);
			const auto num2 = (M32 * M44 - M34 * M42);
			const auto num3 = (M32 * M43 - M33 * M42);
			const auto num4 = (M31 * M44 - M34 * M41);
			const auto num5 = (M31 * M43 - M33 * M41);
			const auto num6 = (M31 * M42 - M32 * M41);
			return
				(M11 * (M22 * num1 - M23 * num2 + M24 * num3) - M12
					* (M21 * num1 - M23 * num4 + M24 * num5) + M13
					* (M21 * num2 - M22 * num4 + M24 * num6) - M14
					* (M21 * num3 - M22 * num5 + M23 * num6));
		}

		static constexpr Matrix Invert(Matrix const& matrix) {
			const auto m11 = matrix.M11;
			const auto m12 = matrix.M12;
			const auto m13 = matrix.M13;
			const auto m14 = matrix.M14;
			const auto m21 = matrix.M21;
			const auto m22 = matrix.M22;
			const auto m23 = matrix.M23;
			const auto m24 = matrix.M24;
			const auto m31 = matrix.M31;
			const auto m32 = matrix.M32;
			const auto m33 = matrix.M33;
			const auto m34 = matrix.M34;
			const auto m41 = matrix.M41;
			const auto m42 = matrix.M42;
			const auto m43 = matrix.M43;
			const auto m44 = matrix.M44;

			const auto num1 = (m33 * m44 - m34 * m43);
			const auto num2 = (m32 * m44 - m34 * m42);
			const auto num3 = (m32 * m43 - m33 * m42);
			const auto num4 = (m31 * m44 - m34 * m41);
			const auto num5 = (m31 * m43 - m33 * m41);
			const auto num6 = (m31 * m42 - m32 * m41);
			const auto num7 = (m22 * num1 - m23 * num2 + m24 * num3);
			const auto num8 = -(m21 * num1 - m23 * num4 + m24 * num5);
			const auto num9 = (m21 * num2 - m22 * num4 + m24 * num6);
			const auto num10 = -(m21 * num3 - m22 * num5 + m23 * num6);
			const auto num11 = (1.0F / (m11 * num7 + m12 * num8 + m13 * num9 + m14 * num10));

			Matrix matrix1;
			matrix1.M11 = num7 * num11;
			matrix1.M21 = num8 * num11;
			matrix1.M31 = num9 * num11;
			matrix1.M41 = num10 * num11;
			matrix1.M12 = -(m12 * num1 - m13 * num2 + m14 * num3) * num11;
			matrix1.M22 = (m11 * num1 - m13 * num4 + m14 * num5) * num11;
			matrix1.M32 = -(m11 * num2 - m12 * num4 + m14 * num6) * num11;
			matrix1.M42 = (m11 * num3 - m12 * num5 + m13 * num6) * num11;

			const auto num12 = (m23 * m44 - m24 * m43);
			const auto num13 = (m22 * m44 - m24 * m42);
			const auto num14 = (m22 * m43 - m23 * m42);
			const auto num15 = (m21 * m44 - m24 * m41);
			const auto num16 = (m21 * m43 - m23 * m41);
			const auto num17 = (m21 * m42 - m22 * m41);

			matrix1.M13 = (m12 * num12 - m13 * num13 + m14 * num14) * num11;
			matrix1.M23 = -(m11 * num12 - m13 * num15 + m14 * num16) * num11;
			matrix1.M33 = (m11 * num13 - m12 * num15 + m14 * num17) * num11;
			matrix1.M43 = -(m11 * num14 - m12 * num16 + m13 * num17) * num11;

			const auto num18 = (m23 * m34 - m24 * m33);
			const auto num19 = (m22 * m34 - m24 * m32);
			const auto num20 = (m22 * m33 - m23 * m32);
			const auto num21 = (m21 * m34 - m24 * m31);
			const auto num22 = (m21 * m33 - m23 * m31);
			const auto num23 = (m21 * m32 - m22 * m31);

			matrix1.M14 = -(m12 * num18 - m13 * num19 + m14 * num20) * num11;
			matrix1.M24 = (m11 * num18 - m13 * num21 + m14 * num22) * num11;
			matrix1.M34 = -(m11 * num19 - m12 * num21 + m14 * num23) * num11;
			matrix1.M44 = (m11 * num20 - m12 * num22 + m13 * num23) * num11;

			return matrix1;
		}

		static constexpr Matrix Lerp(Matrix const& matrix1, Matrix const& matrix2, float amount) {
			Matrix matrix;
			matrix.M11 = matrix1.M11 + (matrix2.M11 - matrix1.M11) * amount;
			matrix.M12 = matrix1.M12 + (matrix2.M12 - matrix1.M12) * amount;
			matrix.M13 = matrix1.M13 + (matrix2.M13 - matrix1.M13) * amount;
			matrix.M14 = matrix1.M14 + (matrix2.M14 - matrix1.M14) * amount;
			matrix.M21 = matrix1.M21 + (matrix2.M21 - matrix1.M21) * amount;
			matrix.M22 = matrix1.M22 + (matrix2.M22 - matrix1.M22) * amount;
			matrix.M23 = matrix1.M23 + (matrix2.M23 - matrix1.M23) * amount;
			matrix.M24 = matrix1.M24 + (matrix2.M24 - matrix1.M24) * amount;
			matrix.M31 = matrix1.M31 + (matrix2.M31 - matrix1.M31) * amount;
			matrix.M32 = matrix1.M32 + (matrix2.M32 - matrix1.M32) * amount;
			matrix.M33 = matrix1.M33 + (matrix2.M33 - matrix1.M33) * amount;
			matrix.M34 = matrix1.M34 + (matrix2.M34 - matrix1.M34) * amount;
			matrix.M41 = matrix1.M41 + (matrix2.M41 - matrix1.M41) * amount;
			matrix.M42 = matrix1.M42 + (matrix2.M42 - matrix1.M42) * amount;
			matrix.M43 = matrix1.M43 + (matrix2.M43 - matrix1.M43) * amount;
			matrix.M44 = matrix1.M44 + (matrix2.M44 - matrix1.M44) * amount;
			return matrix;
		}

		static constexpr Matrix Negate(Matrix const& matrix) {
			Matrix matrix1;
			matrix1.M11 = -matrix.M11;
			matrix1.M12 = -matrix.M12;
			matrix1.M13 = -matrix.M13;
			matrix1.M14 = -matrix.M14;
			matrix1.M21 = -matrix.M21;
			matrix1.M22 = -matrix.M22;
			matrix1.M23 = -matrix.M23;
			matrix1.M24 = -matrix.M24;
			matrix1.M31 = -matrix.M31;
			matrix1.M32 = -matrix.M32;
			matrix1.M33 = -matrix.M33;
			matrix1.M34 = -matrix.M34;
			matrix1.M41 = -matrix.M41;
			matrix1.M42 = -matrix.M42;
			matrix1.M43 = -matrix.M43;
			matrix1.M44 = -matrix.M44;
			return matrix1;
		}

		static constexpr Matrix Add(Matrix const& matrix1, Matrix const& matrix2) {
			Matrix matrix;
			matrix.M11 = matrix1.M11 + matrix2.M11;
			matrix.M12 = matrix1.M12 + matrix2.M12;
			matrix.M13 = matrix1.M13 + matrix2.M13;
			matrix.M14 = matrix1.M14 + matrix2.M14;
			matrix.M21 = matrix1.M21 + matrix2.M21;
			matrix.M22 = matrix1.M22 + matrix2.M22;
			matrix.M23 = matrix1.M23 + matrix2.M23;
			matrix.M24 = matrix1.M24 + matrix2.M24;
			matrix.M31 = matrix1.M31 + matrix2.M31;
			matrix.M32 = matrix1.M32 + matrix2.M32;
			matrix.M33 = matrix1.M33 + matrix2.M33;
			matrix.M34 = matrix1.M34 + matrix2.M34;
			matrix.M41 = matrix1.M41 + matrix2.M41;
			matrix.M42 = matrix1.M42 + matrix2.M42;
			matrix.M43 = matrix1.M43 + matrix2.M43;
			matrix.M44 = matrix1.M44 + matrix2.M44;
			return matrix;
		}

		static constexpr Matrix Subtract(Matrix const& matrix1, Matrix const& matrix2) {
			Matrix matrix;
			matrix.M11 = matrix1.M11 - matrix2.M11;
			matrix.M12 = matrix1.M12 - matrix2.M12;
			matrix.M13 = matrix1.M13 - matrix2.M13;
			matrix.M14 = matrix1.M14 - matrix2.M14;
			matrix.M21 = matrix1.M21 - matrix2.M21;
			matrix.M22 = matrix1.M22 - matrix2.M22;
			matrix.M23 = matrix1.M23 - matrix2.M23;
			matrix.M24 = matrix1.M24 - matrix2.M24;
			matrix.M31 = matrix1.M31 - matrix2.M31;
			matrix.M32 = matrix1.M32 - matrix2.M32;
			matrix.M33 = matrix1.M33 - matrix2.M33;
			matrix.M34 = matrix1.M34 - matrix2.M34;
			matrix.M41 = matrix1.M41 - matrix2.M41;
			matrix.M42 = matrix1.M42 - matrix2.M42;
			matrix.M43 = matrix1.M43 - matrix2.M43;
			matrix.M44 = matrix1.M44 - matrix2.M44;
			return matrix;
		}

		static constexpr Matrix Multiply(Matrix const& matrix1, Matrix const& matrix2) {
			Matrix matrix;
			matrix.M11 = (matrix1.M11 * matrix2.M11 + matrix1.M12 * matrix2.M21 + matrix1.M13 * matrix2.M31 + matrix1.M14 * matrix2.M41);
			matrix.M12 = (matrix1.M11 * matrix2.M12 + matrix1.M12 * matrix2.M22 + matrix1.M13 * matrix2.M32 + matrix1.M14 * matrix2.M42);
			matrix.M13 = (matrix1.M11 * matrix2.M13 + matrix1.M12 * matrix2.M23 + matrix1.M13 * matrix2.M33 + matrix1.M14 * matrix2.M43);
			matrix.M14 = (matrix1.M11 * matrix2.M14 + matrix1.M12 * matrix2.M24 + matrix1.M13 * matrix2.M34 + matrix1.M14 * matrix2.M44);
			matrix.M21 = (matrix1.M21 * matrix2.M11 + matrix1.M22 * matrix2.M21 + matrix1.M23 * matrix2.M31 + matrix1.M24 * matrix2.M41);
			matrix.M22 = (matrix1.M21 * matrix2.M12 + matrix1.M22 * matrix2.M22 + matrix1.M23 * matrix2.M32 + matrix1.M24 * matrix2.M42);
			matrix.M23 = (matrix1.M21 * matrix2.M13 + matrix1.M22 * matrix2.M23 + matrix1.M23 * matrix2.M33 + matrix1.M24 * matrix2.M43);
			matrix.M24 = (matrix1.M21 * matrix2.M14 + matrix1.M22 * matrix2.M24 + matrix1.M23 * matrix2.M34 + matrix1.M24 * matrix2.M44);
			matrix.M31 = (matrix1.M31 * matrix2.M11 + matrix1.M32 * matrix2.M21 + matrix1.M33 * matrix2.M31 + matrix1.M34 * matrix2.M41);
			matrix.M32 = (matrix1.M31 * matrix2.M12 + matrix1.M32 * matrix2.M22 + matrix1.M33 * matrix2.M32 + matrix1.M34 * matrix2.M42);
			matrix.M33 = (matrix1.M31 * matrix2.M13 + matrix1.M32 * matrix2.M23 + matrix1.M33 * matrix2.M33 + matrix1.M34 * matrix2.M43);
			matrix.M34 = (matrix1.M31 * matrix2.M14 + matrix1.M32 * matrix2.M24 + matrix1.M33 * matrix2.M34 + matrix1.M34 * matrix2.M44);
			matrix.M41 = (matrix1.M41 * matrix2.M11 + matrix1.M42 * matrix2.M21 + matrix1.M43 * matrix2.M31 + matrix1.M44 * matrix2.M41);
			matrix.M42 = (matrix1.M41 * matrix2.M12 + matrix1.M42 * matrix2.M22 + matrix1.M43 * matrix2.M32 + matrix1.M44 * matrix2.M42);
			matrix.M43 = (matrix1.M41 * matrix2.M13 + matrix1.M42 * matrix2.M23 + matrix1.M43 * matrix2.M33 + matrix1.M44 * matrix2.M43);
			matrix.M44 = (matrix1.M41 * matrix2.M14 + matrix1.M42 * matrix2.M24 + matrix1.M43 * matrix2.M34 + matrix1.M44 * matrix2.M44);
			return matrix;
		}

		static constexpr Matrix Multiply(Matrix const& matrix1, float scaleFactor) {
			float num = scaleFactor;
			Matrix matrix;
			matrix.M11 = matrix1.M11 * num;
			matrix.M12 = matrix1.M12 * num;
			matrix.M13 = matrix1.M13 * num;
			matrix.M14 = matrix1.M14 * num;
			matrix.M21 = matrix1.M21 * num;
			matrix.M22 = matrix1.M22 * num;
			matrix.M23 = matrix1.M23 * num;
			matrix.M24 = matrix1.M24 * num;
			matrix.M31 = matrix1.M31 * num;
			matrix.M32 = matrix1.M32 * num;
			matrix.M33 = matrix1.M33 * num;
			matrix.M34 = matrix1.M34 * num;
			matrix.M41 = matrix1.M41 * num;
			matrix.M42 = matrix1.M42 * num;
			matrix.M43 = matrix1.M43 * num;
			matrix.M44 = matrix1.M44 * num;
			return matrix;
		}

		static constexpr Matrix Divide(Matrix const& matrix1, Matrix const& matrix2) {
			Matrix matrix;
			matrix.M11 = matrix1.M11 / matrix2.M11;
			matrix.M12 = matrix1.M12 / matrix2.M12;
			matrix.M13 = matrix1.M13 / matrix2.M13;
			matrix.M14 = matrix1.M14 / matrix2.M14;
			matrix.M21 = matrix1.M21 / matrix2.M21;
			matrix.M22 = matrix1.M22 / matrix2.M22;
			matrix.M23 = matrix1.M23 / matrix2.M23;
			matrix.M24 = matrix1.M24 / matrix2.M24;
			matrix.M31 = matrix1.M31 / matrix2.M31;
			matrix.M32 = matrix1.M32 / matrix2.M32;
			matrix.M33 = matrix1.M33 / matrix2.M33;
			matrix.M34 = matrix1.M34 / matrix2.M34;
			matrix.M41 = matrix1.M41 / matrix2.M41;
			matrix.M42 = matrix1.M42 / matrix2.M42;
			matrix.M43 = matrix1.M43 / matrix2.M43;
			matrix.M44 = matrix1.M44 / matrix2.M44;
			return matrix;
		}

		static constexpr Matrix Divide(Matrix const& matrix1, float divider) {
			float num = 1.0f / divider;
			Matrix matrix;
			matrix.M11 = matrix1.M11 * num;
			matrix.M12 = matrix1.M12 * num;
			matrix.M13 = matrix1.M13 * num;
			matrix.M14 = matrix1.M14 * num;
			matrix.M21 = matrix1.M21 * num;
			matrix.M22 = matrix1.M22 * num;
			matrix.M23 = matrix1.M23 * num;
			matrix.M24 = matrix1.M24 * num;
			matrix.M31 = matrix1.M31 * num;
			matrix.M32 = matrix1.M32 * num;
			matrix.M33 = matrix1.M33 * num;
			matrix.M34 = matrix1.M34 * num;
			matrix.M41 = matrix1.M41 * num;
			matrix.M42 = matrix1.M42 * num;
			matrix.M43 = matrix1.M43 * num;
			matrix.M44 = matrix1.M44 * num;
			return matrix;
		}

		constexpr Matrix operator-() const {
			return Matrix::Negate(*this);
		}

		friend constexpr Matrix operator+(Matrix const& matrix1, Matrix const& matrix2) {
			return Matrix::Add(matrix1, matrix2);
		}

		friend constexpr Matrix operator-(Matrix const& matrix1, Matrix const& matrix2) {
			return Matrix::Subtract(matrix1, matrix2);
		}

		friend constexpr Matrix operator*(Matrix const& matrix1, Matrix const& matrix2) {
			return Matrix::Multiply(matrix1, matrix2);
		}

		friend constexpr Matrix operator*(Matrix const& matrix, float factor) {
			return Matrix::Multiply(matrix, factor);
		}

		friend constexpr Matrix operator*(float factor, Matrix const& matrix) {
			return Matrix::Multiply(matrix, factor);
		}

		friend constexpr Matrix operator/(Matrix const& matrix1, Matrix const& matrix2) {
			return Matrix::Divide(matrix1, matrix2);
		}

		friend constexpr Matrix operator/(Matrix const& matrix, float divider) {
			return Matrix::Divide(matrix, divider);
		}
	};

	struct Quaternion {
		float X{ 0 };
		float Y{ 0 };
		float Z{ 0 };
		float W{ 0 };

		constexpr Quaternion() = default;

		constexpr Quaternion(float X, float Y, float Z, float W)
			: X(X), Y(Y), Z(Z), W(W) { }

		constexpr Quaternion(Vector3 vectorPart, float scalarPart)
			: X(vectorPart.X), Y(vectorPart.Y), Z(vectorPart.Z), W(scalarPart) { }

		constexpr bool operator==(const Quaternion& other) const {
			return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
		}

		static constexpr Quaternion Identity() {
			return { 0.0f, 0.0f, 0.0f, 1.0f };
		}

		float constexpr LengthSquared() const {
			return X * X + Y * Y + Z * Z + W * W;
		}

		inline float Length() const { return std::sqrt(LengthSquared()); }

		void Normalize() {
			const auto num = 1.0F / Length();
			X *= num;
			Y *= num;
			Z *= num;
			W *= num;
		}

		static Quaternion Normalize(Quaternion const& quaternion) {
			auto q = quaternion;
			q.Normalize();
			return q;
		}

		constexpr void Conjugate() {
			X = -X;
			Y = -Y;
			Z = -Z;
		}

		static constexpr Quaternion Conjugate(Quaternion const& value) {
			Quaternion quaternion;
			quaternion.X = -value.X;
			quaternion.Y = -value.Y;
			quaternion.Z = -value.Z;
			quaternion.W = value.W;
			return quaternion;
		}

		static constexpr Quaternion Inverse(Quaternion const& quaternion) {
			const auto num = 1.0f / quaternion.LengthSquared();
			Quaternion quaternion1;
			quaternion1.X = -quaternion.X * num;
			quaternion1.Y = -quaternion.Y * num;
			quaternion1.Z = -quaternion.Z * num;
			quaternion1.W = quaternion.W * num;
			return quaternion1;
		}

		static Quaternion CreateFromAxisAngle(Vector3 const& axis, float angle);
		static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll);
		static Quaternion CreateFromRotationMatrix(Matrix const& matrix);

		static constexpr float Dot(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			return quaternion1.X * quaternion2.X + quaternion1.Y * quaternion2.Y + quaternion1.Z * quaternion2.Z + quaternion1.W * quaternion2.W;
		}

		static Quaternion Slerp(Quaternion const& quaternion1, Quaternion const& quaternion2, float amount);
		static Quaternion Lerp(Quaternion const& quaternion1, Quaternion const& quaternion2, float amount);

		static constexpr Quaternion Concatenate(Quaternion const& value1, Quaternion const& value2) {
			const auto x1 = value2.X;
			const auto y1 = value2.Y;
			const auto z1 = value2.Z;
			const auto w1 = value2.W;
			const auto x2 = value1.X;
			const auto y2 = value1.Y;
			const auto z2 = value1.Z;
			const auto w2 = value1.W;
			const auto num1 = y1 * z2 - z1 * y2;
			const auto num2 = z1 * x2 - x1 * z2;
			const auto num3 = x1 * y2 - y1 * x2;
			const auto num4 = x1 * x2 + y1 * y2 + z1 * z2;
			Quaternion quaternion;
			quaternion.X = (x1 * w2 + x2 * w1) + num1;
			quaternion.Y = (y1 * w2 + y2 * w1) + num2;
			quaternion.Z = (z1 * w2 + z2 * w1) + num3;
			quaternion.W = w1 * w2 - num4;
			return quaternion;
		}

		static constexpr Quaternion Negate(Quaternion const& quaternion) {
			Quaternion quaternion1;
			quaternion1.X = -quaternion.X;
			quaternion1.Y = -quaternion.Y;
			quaternion1.Z = -quaternion.Z;
			quaternion1.W = -quaternion.W;
			return quaternion1;
		}

		static constexpr Quaternion Add(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			Quaternion quaternion;
			quaternion.X = quaternion1.X + quaternion2.X;
			quaternion.Y = quaternion1.Y + quaternion2.Y;
			quaternion.Z = quaternion1.Z + quaternion2.Z;
			quaternion.W = quaternion1.W + quaternion2.W;
			return quaternion;
		}

		static constexpr Quaternion Subtract(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			Quaternion quaternion;
			quaternion.X = quaternion1.X - quaternion2.X;
			quaternion.Y = quaternion1.Y - quaternion2.Y;
			quaternion.Z = quaternion1.Z - quaternion2.Z;
			quaternion.W = quaternion1.W - quaternion2.W;
			return quaternion;
		}

		static constexpr Quaternion Multiply(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			const auto x1 = quaternion1.X;
			const auto y1 = quaternion1.Y;
			const auto z1 = quaternion1.Z;
			const auto w1 = quaternion1.W;
			const auto x2 = quaternion2.X;
			const auto y2 = quaternion2.Y;
			const auto z2 = quaternion2.Z;
			const auto w2 = quaternion2.W;
			const auto num1 = y1 * z2 - z1 * y2;
			const auto num2 = z1 * x2 - x1 * z2;
			const auto num3 = x1 * y2 - y1 * x2;
			const auto num4 = x1 * x2 + y1 * y2 + z1 * z2;
			Quaternion quaternion;
			quaternion.X = (x1 * w2 + x2 * w1) + num1;
			quaternion.Y = (y1 * w2 + y2 * w1) + num2;
			quaternion.Z = (z1 * w2 + z2 * w1) + num3;
			quaternion.W = w1 * w2 - num4;
			return quaternion;
		}

		static constexpr Quaternion Multiply(Quaternion const& quaternion1, float scaleFactor) {
			Quaternion quaternion;
			quaternion.X = quaternion1.X * scaleFactor;
			quaternion.Y = quaternion1.Y * scaleFactor;
			quaternion.Z = quaternion1.Z * scaleFactor;
			quaternion.W = quaternion1.W * scaleFactor;
			return quaternion;
		}

		static constexpr Quaternion Divide(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			const auto x = quaternion1.X;
			const auto y = quaternion1.Y;
			const auto z = quaternion1.Z;
			const auto w = quaternion1.W;
			const auto num1 = 1.0f / (quaternion2.X * quaternion2.X + quaternion2.Y * quaternion2.Y + quaternion2.Z * quaternion2.Z + quaternion2.W * quaternion2.W);
			const auto num2 = -quaternion2.X * num1;
			const auto num3 = -quaternion2.Y * num1;
			const auto num4 = -quaternion2.Z * num1;
			const auto num5 = quaternion2.W * num1;
			const auto num6 = y * num4 - z * num3;
			const auto num7 = z * num2 - x * num4;
			const auto num8 = x * num3 - y * num2;
			const auto num9 = x * num2 + y * num3 + z * num4;
			Quaternion quaternion;
			quaternion.X = (x * num5 + num2 * w) + num6;
			quaternion.Y = (y * num5 + num3 * w) + num7;
			quaternion.Z = (z * num5 + num4 * w) + num8;
			quaternion.W = w * num5 - num9;
			return quaternion;
		}

		constexpr Quaternion operator-(Quaternion const& quaternion) const {
			return Quaternion::Negate(quaternion);
		}

		constexpr friend Quaternion operator+(Quaternion const& q1, Quaternion const& q2) {
			return Quaternion::Add(q1, q2);
		}

		constexpr friend Quaternion operator-(Quaternion const& q1, Quaternion const& q2) {
			return Quaternion::Add(q1, q2);
		}

		constexpr friend Quaternion operator*(Quaternion const& q1, Quaternion const& q2) {
			return Quaternion::Multiply(q1, q2);
		}

		constexpr friend Quaternion operator/(Quaternion const& q1, Quaternion const& q2) {
			return Quaternion::Divide(q1, q2);
		}

		constexpr friend Quaternion operator*(Quaternion const& q1, float scaleValue) {
			return Quaternion::Multiply(q1, scaleValue);
		}

		constexpr friend Quaternion operator*(float scaleValue, Quaternion const& q1) {
			return Quaternion::Multiply(q1, scaleValue);
		}
	};

	constexpr Vector2 Vector2::Transform(Vector2 const& position, Matrix const& matrix) {
		const auto posx = (position.X * matrix.M11 + position.Y * matrix.M21) + matrix.M41;
		const auto posy = (position.X * matrix.M12 + position.Y * matrix.M22) + matrix.M42;

		return{ posx, posy };
	}

	constexpr Vector2 Vector2::TransformNormal(Vector2 const& normal, Matrix const& matrix) {
		const auto posx = normal.X * matrix.M11 + normal.Y * matrix.M21;
		const auto posy = normal.X * matrix.M12 + normal.Y * matrix.M22;
		return { posx, posy };
	}

	constexpr Vector3 Vector3::Transform(Vector3 const& position, Matrix const& matrix) {
		const auto num1 = (position.X * matrix.M11 + position.Y * matrix.M21 + position.Z * matrix.M31) + matrix.M41;
		const auto num2 = (position.X * matrix.M12 + position.Y * matrix.M22 + position.Z * matrix.M32) + matrix.M42;
		const auto num3 = (position.X * matrix.M13 + position.Y * matrix.M23 + position.Z * matrix.M33) + matrix.M43;
		Vector3 vector3;
		vector3.X = num1;
		vector3.Y = num2;
		vector3.Z = num3;
		return vector3;
	}

	constexpr Vector3 Vector3::TransformNormal(Vector3 const& normal, Matrix const& matrix)
	{
		const auto num1 = normal.X * matrix.M11 + normal.Y * matrix.M21 + normal.Z * matrix.M31;
		const auto num2 = normal.X * matrix.M12 + normal.Y * matrix.M22 + normal.Z * matrix.M32;
		const auto num3 = normal.X * matrix.M13 + normal.Y * matrix.M23 + normal.Z * matrix.M33;
		Vector3 vector3;
		vector3.X = num1;
		vector3.Y = num2;
		vector3.Z = num3;
		return vector3;
	}

	constexpr Vector4 Vector4::Transform(Vector2 const& position, Matrix const& matrix)
	{
		const auto num1 = (position.X * matrix.M11 + position.Y * matrix.M21) + matrix.M41;
		const auto num2 = (position.X * matrix.M12 + position.Y * matrix.M22) + matrix.M42;
		const auto num3 = (position.X * matrix.M13 + position.Y * matrix.M23) + matrix.M43;
		const auto num4 = (position.X * matrix.M14 + position.Y * matrix.M24) + matrix.M44;
		Vector4 vector4;
		vector4.X = num1;
		vector4.Y = num2;
		vector4.Z = num3;
		vector4.W = num4;
		return vector4;
	}

	constexpr Vector4 Vector4::Transform(Vector3 const& position, Matrix const& matrix)
	{
		const auto num1 = (position.X * matrix.M11 + position.Y * matrix.M21 + position.Z * matrix.M31) + matrix.M41;
		const auto num2 = (position.X * matrix.M12 + position.Y * matrix.M22 + position.Z * matrix.M32) + matrix.M42;
		const auto num3 = (position.X * matrix.M13 + position.Y * matrix.M23 + position.Z * matrix.M33) + matrix.M43;
		const auto num4 = (position.X * matrix.M14 + position.Y * matrix.M24 + position.Z * matrix.M34) + matrix.M44;
		Vector4 vector4;
		vector4.X = num1;
		vector4.Y = num2;
		vector4.Z = num3;
		vector4.W = num4;
		return vector4;
	}

	constexpr Vector4 Vector4::Transform(Vector4 const& vector, Matrix const& matrix) {
		const auto num1 = (vector.X * matrix.M11 + vector.Y * matrix.M21 + vector.Z * matrix.M31 + vector.W * matrix.M41);
		const auto num2 = (vector.X * matrix.M12 + vector.Y * matrix.M22 + vector.Z * matrix.M32 + vector.W * matrix.M42);
		const auto num3 = (vector.X * matrix.M13 + vector.Y * matrix.M23 + vector.Z * matrix.M33 + vector.W * matrix.M43);
		const auto num4 = (vector.X * matrix.M14 + vector.Y * matrix.M24 + vector.Z * matrix.M34 + vector.W * matrix.M44);
		Vector4 vector4;
		vector4.X = num1;
		vector4.Y = num2;
		vector4.Z = num3;
		vector4.W = num4;
		return vector4;
	}

	constexpr Vector2 Vector2::Transform(Vector2 const& value, Quaternion const& rotation) {
		const auto rx = rotation.X + rotation.X;
		const auto ry = rotation.Y + rotation.Y;
		const auto rz = rotation.Z + rotation.Z;
		const auto rwz = rotation.W * rz;
		const auto rxx = rotation.X * rx;
		const auto rxy = rotation.X * ry;
		const auto ryy = rotation.Y * ry;
		const auto rzz = rotation.Z * rz;
		const auto x = value.X * (1.0F - ryy - rzz) + value.Y * (rxy - rwz);
		const auto y = value.X * (rxy + rwz) + value.Y * (1.0F - rxx - rzz);
		return{ x,y };
	}

	constexpr Vector3 Vector3::Transform(Vector3 const& value, Quaternion const& rotation)
	{
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
		const auto num13 = (value.X * (1.0F - num10 - num12) + value.Y * (num8 - num6) + value.Z * (num9 + num5));
		const auto num14 = (value.X * (num8 + num6) + value.Y * (1.0F - num7 - num12) + value.Z * (num11 - num4));
		const auto num15 = (value.X * (num9 - num5) + value.Y * (num11 + num4) + value.Z * (1.0F - num7 - num10));
		Vector3 vector3;
		vector3.X = num13;
		vector3.Y = num14;
		vector3.Z = num15;
		return vector3;
	}

	constexpr Vector4 Vector4::Transform(Vector2 const& value, Quaternion const& rotation)
	{
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
		const auto num13 = (value.X * (1.0F - num10 - num12) + value.Y * (num8 - num6));
		const auto num14 = (value.X * (num8 + num6) + value.Y * (1.0F - num7 - num12));
		const auto num15 = (value.X * (num9 - num5) + value.Y * (num11 + num4));
		Vector4 vector4;
		vector4.X = num13;
		vector4.Y = num14;
		vector4.Z = num15;
		vector4.W = 1.0f;
		return vector4;
	}

	constexpr Vector4 Vector4::Transform(Vector3 const& value, Quaternion const& rotation) {
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
		const auto num13 = (value.X * (1.0F - num10 - num12) + value.Y * (num8 - num6) + value.Z * (num9 + num5));
		const auto num14 = (value.X * (num8 + num6) + value.Y * (1.0F - num7 - num12) + value.Z * (num11 - num4));
		const auto num15 = (value.X * (num9 - num5) + value.Y * (num11 + num4) + value.Z * (1.0F - num7 - num10));
		Vector4 vector4;
		vector4.X = num13;
		vector4.Y = num14;
		vector4.Z = num15;
		vector4.W = 1.0f;
		return vector4;
	}
}

#endif