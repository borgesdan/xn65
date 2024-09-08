#include "xna/exception.hpp"

namespace xna {
	void Exception::Throw(std::string const& message, std::source_location const& location) {
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
		error.append("': ");
		error.append(location.function_name());
		error.append("': ");
		error.append(message);
		error.append("\n");

		throw std::runtime_error(error);
	}

	void Exception::ThrowIfNull(void const* argument, std::string const& argumentName, std::source_location const& location) {
		if (argument)
			return;

		std::string error;

		error.append("The value of ");
		error.append(argumentName);
		error.append(" is null.");

		Throw(error);
	}
}