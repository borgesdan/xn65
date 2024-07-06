#ifndef XNA_EXCEPTION_HPP
#define XNA_EXCEPTION_HPP

#include <stdexcept>
#include <string>
#include <source_location>

namespace xna {
	//Structure for throwing exceptions with a message and information from the source file
	struct Exception {
		//Raises an exception with a message. Source file information is automatically captured.
		static void Throw(std::string const& message = "", const std::source_location location = std::source_location::current()) {
			std::string error;

			error.append("Exception in: ");
#if _DEBUG
			error.append(location.file_name());
			error.append("(");
			error.append(std::to_string(location.line()));
			error.append(":");
			error.append(std::to_string(location.column()));
			error.append(") ");
#endif
			error.append("'");
			error.append(location.function_name());
			error.append("': ");
			error.append(message);
			error.append("\n");

			throw std::runtime_error(error);
		}

		inline static const std::string FAILED_TO_CREATE = "Failed to create component.";
		inline static const std::string FAILED_TO_APPLY = "Failed to apply component.";		
		inline static const std::string FAILED_TO_MAKE_WINDOW_ASSOCIATION = "Failed to create association with window.";
		inline static const std::string UNABLE_TO_INITIALIZE = "Unable to initialize component.";
		inline static const std::string UNABLE_TO_BUILD_OBJECT = "Unable to build object.";
		inline static const std::string NOT_IMPLEMENTED = "Not Implemented.";
		inline static const std::string ARGUMENT_IS_NULL = "The argument is null or one of its values.";
		inline static const std::string INVALID_OPERATION = "An invalid operation occurred.";
	};
}

#endif