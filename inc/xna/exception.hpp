#ifndef XNA_EXCEPTION_HPP
#define XNA_EXCEPTION_HPP

#include <stdexcept>
#include <string>
#include <source_location>
#include <memory>

namespace xna {
	//Structure for throwing exceptions with a message and information from the source file
	struct Exception {
		//Raises an exception with a message. Source file information is automatically captured.
		static void Throw(std::string const& message = "", std::source_location const& location = std::source_location::current());

		inline static void ThrowIfNull(std::shared_ptr<void> const& argument, std::string const& argumentName, std::source_location const& location = std::source_location::current()) {
			ThrowIfNull(&argument, argumentName, location);
		}

		inline static void ThrowIfNull(std::unique_ptr<void> const& argument, std::string const& argumentName, std::source_location const& location = std::source_location::current()) {
			ThrowIfNull(&argument, argumentName, location);
		}

		static void ThrowIfNull(void const* argument, std::string const& argumentName, std::source_location const& location = std::source_location::current());

		inline static const std::string FAILED_TO_CREATE = "Failed to create component.";
		inline static const std::string FAILED_TO_APPLY = "Failed to apply component.";		
		inline static const std::string FAILED_TO_MAKE_WINDOW_ASSOCIATION = "Failed to create association with window.";
		inline static const std::string UNABLE_TO_INITIALIZE = "Unable to initialize component.";
		inline static const std::string UNABLE_TO_BUILD_OBJECT = "Unable to build object.";
		inline static const std::string NOT_IMPLEMENTED = "Not Implemented.";
		inline static const std::string ARGUMENT_IS_NULL = "The argument is null or one of its values.";
		inline static const std::string INVALID_OPERATION = "An invalid operation occurred.";
		inline static const std::string BAD_XNB = "Bad xnb file";
		inline static const std::string OUT_OF_BOUNDS = "Out of bounds.";
		inline static const std::string END_OF_FILE = "End of file.";
	};	
}

#endif