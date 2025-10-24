#ifndef XNA_FRAMEWORK_IPACKEDVECTOR_HPP
#define XNA_FRAMEWORK_IPACKEDVECTOR_HPP

#include "Vector4.hpp"

namespace Xna {
	class IPackedVector {
	public:
		virtual Vector4 ToVector4() const = 0;
		virtual void PackFromVector4(Vector4 const& vector) = 0;
	};

	template <typename T>
	class IPackedVectorT {
	public:
		virtual T PackedValue() const = 0;
		virtual void PackedValue(T const& value) = 0;
	};
}

#endif