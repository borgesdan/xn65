#ifndef XNA_PIPELINE_COMPILER_HPP
#define XNA_PIPELINE_COMPILER_HPP

#include "../default.hpp"

namespace xna {
	class ContentCompiler {
	public:
		P_ContentTypeWriter GetTypeWriter(Type const& type, std::vector<P_Type> dependencies);
	};
}

#endif