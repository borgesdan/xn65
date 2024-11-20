#ifndef XNA_PIPELINE_SERIALIZER_HPP
#define XNA_PIPELINE_SERIALIZER_HPP

#include <string>
#include <any>
#include "xna/csharp/xml.hpp"
#include "xna/content/serializer.hpp"
#include "xna/helpers.hpp"

namespace xna {	
	class IntermediateWriter;	
	class IntermediateReader;	

	class IntermediateSerializer {
	public:
		template <typename T>
		static T Deserialize(XmlReader& input, std::string referenceRelocationPath) {

		}
	};

	//Provides methods for serializing and deserializing a specific managed type.
	class ContentTypeSerializer {
	public:
		//Retrieves and caches any nested type serializers and allows reflection over the target data type.
		virtual void Initialize(IntermediateSerializer serializer) {};
		//Serializes an object to intermediate XML format.
		virtual void Serialize(IntermediateWriter output, std::any const& value, ContentSerializerAttribute& format);
		//Serializes an object to intermediate XML format.
		virtual std::any Deserialize(IntermediateWriter input, ContentSerializerAttribute& format, std::any const& existingInstance);

		//Gets a value indicating whether this component may load data into an existing object or if it must it construct a new instance of the object before loading the data.
		virtual bool CanDeserializeIntoExistingObject() const { return false; }
		//Queries whether an object contains data to be serialized.
		virtual bool ObjectIsEmpty(std::any const& value) const { return false; }
		//Gets a short-form XML name for the target type, or null if there is none.
		constexpr std::string XmlTypeName() const { return xmlTypeName; }

	protected:
		ContentTypeSerializer() {}
		ContentTypeSerializer(std::string const& xmlTypeName) : xmlTypeName(xmlTypeName) {}

	private:
		std::string xmlTypeName;
	};

	struct ISerialization {
		virtual void Serialize(ContentTypeSerializer& serializer, bool serializeOptional = true) = 0;
	};

	template <typename T>
	class ContentTypeSerializer_T : public ContentTypeSerializer {
	public:
		virtual void Serialize(IntermediateWriter output, T const& value, ContentSerializerAttribute& format) = 0;

		void Serialize(IntermediateWriter output, std::any const& value, ContentSerializerAttribute& format) override {				
			Serializer<T>(output, CastType(value), format);
		}

		virtual T Deserialize(IntermediateWriter input, ContentSerializerAttribute& format, T& existingInstance) = 0;

		std::any Deserialize(IntermediateWriter input, ContentSerializerAttribute& format, std::any const& existingInstance) override {			 

			if constexpr (XnaHelper::IsSmartPoint<T>() && !existingInstance.has_value())
				return T();
			
			T existingInstance1 = CastType(existingInstance);

			std::any obj = Deserialize<T>(input, format, existingInstance1);

			return obj;
		}

	protected:
		ContentTypeSerializer_T() {}
		ContentTypeSerializer_T(std::string const& xmlTypeName) : ContentTypeSerializer(xmlTypeName){}	

	private:
		static T CastType(std::any const& value) {
			try {
				return std::any_cast<T>(value);
			}
			catch (std::exception& ex) {
				Exception::Throw(Exception::FAILED_TO_CREATE);
			}
		}
	};
}

#endif