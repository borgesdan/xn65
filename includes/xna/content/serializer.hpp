#ifndef XNA_CONTENT_SERIALIZER_HPP
#define XNA_CONTENT_SERIALIZER_HPP

#include <string>

namespace xna {
	//A custom Attribute that marks a field or property to control how it is serialized or to indicate that protected or private data should be included in serialization.
	struct ContentSerializerAttribute {
		//Gets or sets the XML element name (default=name of the managed type member).
		std::string ElementName;
		//Gets or sets the XML element name for each item in a collection (default = "Item"). 
		std::string CollectionItemName;
		//Gets or sets a value idicating whether to write member contents directly into the current XML context rather than wrapping the member in a new XML element (default=false). 
		bool FlattenContent{ false };
		//Indicates whether to write this element if the member is null and skip past it if not found when deserializing XML (default=false). 
		bool Optional{ false };
		//Get or set a value indicating whether this member can have a null value (default=true).
		bool AllowNull{ true };
		//Indicates whether this member is referenced from multiple parents and should be serialized as a unique ID reference (default=false). 
		bool SharedResource{ false };
	};
}

#endif