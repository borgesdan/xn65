#ifndef XNA_EXCEPTION_HPP
#define XNA_EXCEPTION_HPP

#include <stdexcept>
#include <string>
#include <source_location>

namespace xna {
	struct ExMessage {
		inline static const std::string InvalidOperation = "An invalid operation occurred.";
		inline static const std::string InitializeComponent = "Unable to initialize component";
		inline static const std::string CreateComponent = "Failed to create component";
		inline static const std::string ApplyComponent = "Failed to apply component";
		inline static const std::string UnintializedComponent = "Component is not initialized";
		inline static const std::string MakeWindowAssociation = "Failed to create association with window";
	};

	struct Exception {
		static void Throw(std::string const& message, const std::source_location location = std::source_location::current()) {
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
	};
}

#endif