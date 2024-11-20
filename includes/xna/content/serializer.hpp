#ifndef XNA_CONTENT_SERIALIZER_HPP
#define XNA_CONTENT_SERIALIZER_HPP

#include <string>

namespace xna {
	struct ContentSerializerAttribute {
		std::string ElementName;
		std::string CollectionItemName;
		bool FlatternContent{ false };
		bool Optional{ false };
		bool AllowNull{ true };
		bool SharedResource{ false };
	};
}

#endif