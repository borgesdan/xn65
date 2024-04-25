#ifndef XNA_XNAERROR_HPP
#define XNA_XNAERROR_HPP

namespace xna {
	enum class XnaErrorCode {
		NONE,
		ARGUMENT_OUT_OF_RANGE,
		ARGUMENT_IS_NULL,
		INVALID_OPERATION,
		FAILED_OPERATION,
		OVERFLOW_OPERATION,
		NULL_CAST,
		BAD_CAST,
		STREAM_ERROR,
		UNINTIALIZED_RESOURCE
	};

	inline void xna_error_apply(XnaErrorCode* source, XnaErrorCode const& value) {
		if (source != nullptr)
			*source = value;
	}

	inline bool xna_error_haserros(XnaErrorCode* source) {
		return source != nullptr;
	}

#define xna_error_nullarg	XnaErrorCode* err = nullptr	
#define xna_error_ptr_arg	XnaErrorCode* err
}

#endif