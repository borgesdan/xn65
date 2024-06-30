#ifndef XNA_CONTENT_SERIALIZER_HPP
#define XNA_CONTENT_SERIALIZER_HPP

#include "../default.hpp"
#include "../csharp/attribute.hpp"

namespace xna {
	class ContentSerializerAttribute : public Attribute {
	public:
        String ElementName;
        bool FlattenContent{false};
        bool Optional{ false };
        bool AllowNull{ true };
        bool SharedResource{ false };
        
        constexpr String CollectionItemName() const {
            if (collectionItemName.empty())
                return "Item";

            return collectionItemName;
        }

        void CollectionItemName(String const& value) {
            if (value.empty())
                Exception::Throw(ExMessage::ArgumentIsNullOrEmpty);

            collectionItemName = value;
        }

        constexpr bool HasCollectionItemName() {
            return !CollectionItemName.empty();
        }

    private:
        String collectionItemName;
	};
}

#endif