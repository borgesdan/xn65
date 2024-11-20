#ifndef XNA_PIPELINE_SERIALIZER_HPP
#define XNA_PIPELINE_SERIALIZER_HPP

#include <string>
#include "xna/csharp/xml.hpp"

namespace xna {
	class ContentTypeSerializer {

	};

	struct ISerialization {
		virtual void Serialize(ContentTypeSerializer& serializer) = 0;
	};

	class IntermediateSerializer {
	public:
		template <typename T>
		static T Deserialize(XmlReader& input, std::string referenceRelocationPath) {

		}
	};
}

#endif