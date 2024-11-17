#ifndef XNA_PIPELINE_SERIALIZER_HPP
#define XNA_PIPELINE_SERIALIZER_HPP

#include "xna/csharp/xml.hpp"

namespace xna {
	class IntermediateSerializer {
		template <typename T>
		static T Deserialize<T>(XmlReader& input, std::string referenceRelocationPath) {

		}
	};
}

#endif