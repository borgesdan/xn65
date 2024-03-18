#ifndef XNA_COMMON_MATRIX_HPP
#define XNA_COMMON_MATRIX_HPP

namespace xna {
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

		constexpr Matrix(float M11, float M12, float M13, float M14, float M21, float M22, float M23, float M24, float M31, float M32, float M33, float M34, float M41, float M42, float M43, float M44)
			: 
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
	};
}

#endif