#ifndef XNA_PIPELINE_SERIALIZER_HPP
#define XNA_PIPELINE_SERIALIZER_HPP

#include <string>
#include <any>
#include <memory>
#include "xna/csharp/xml.hpp"
#include "xna/content/serializer.hpp"
#include "xna/helpers.hpp"

namespace xna {	
	class IntermediateWriter;	
	class IntermediateReader;	

	class IntermediateSerializer {
	public:
		static std::shared_ptr<IntermediateSerializer> SingletonInstance();

		template <typename T>
		static void Serialize(XmlWriter& output, T& value, std::string const& referenceRelocationPath);

		template <typename T>
		static T Deserialize(XmlReader& input, std::string referenceRelocationPath) {

		}		

	private:
		IntermediateSerializer() {}

	private:
		static inline std::shared_ptr<IntermediateSerializer> singletonInstance = nullptr;
	};

	//Provides methods for serializing and deserializing a specific managed type.
	class ContentTypeSerializer {
	public:
		ContentTypeSerializer() {}
		ContentTypeSerializer(std::string const& xmlTypeName) : xmlTypeName(xmlTypeName) {}

		//Retrieves and caches any nested type serializers and allows reflection over the target data type.
		virtual void Initialize(IntermediateSerializer& serializer) {};
		//Serializes an object to intermediate XML format.
		virtual void Serialize(IntermediateWriter& output, std::any const& value, ContentSerializerAttribute& format);
		//Serializes an object to intermediate XML format.
		virtual std::any Deserialize(IntermediateWriter& input, ContentSerializerAttribute& format, std::any const& existingInstance);

		//Gets a value indicating whether this component may load data into an existing object or if it must it construct a new instance of the object before loading the data.
		virtual bool CanDeserializeIntoExistingObject() const { return false; }
		//Queries whether an object contains data to be serialized.
		virtual bool ObjectIsEmpty(std::any const& value) const { return false; }
		//Gets a short-form XML name for the target type, or null if there is none.
		constexpr std::string XmlTypeName() const { return xmlTypeName; }

	private:
		std::string xmlTypeName;
	};

	//Provides an implementation of many of the methods of IntermediateSerializer.
	//Deserializes and tracks state for shared resources and external references.
	class IntermediateReader {
	public:
		IntermediateReader(
			std::shared_ptr<IntermediateSerializer> const& serializer,
			std::shared_ptr<XmlReader> const& xmlReader)
			: serializer(serializer), xmlReader(xmlReader){}

		//Gets the parent serializer.
		std::shared_ptr<IntermediateSerializer> Serializer() const { return serializer; }

		//Gets the XML input stream.
		std::shared_ptr<XmlReader> Xml() const { return xmlReader; }

		//Reads a single object from the input XML stream.
		template <class T>
		T ReadObject(ContentSerializerAttribute& format) {
			
		}

		//Reads a single object from the input XML stream.
		template <class T>
		T ReadObject(ContentSerializerAttribute& format, T& existingInstance);

		//Reads a single object from the input XML stream.
		template <class T>
		T ReadObject(ContentSerializerAttribute& format, ContentTypeSerializer& typeSerializer);

		//Reads a single object from the input XML stream.
		template <class T>
		T ReadObject(ContentSerializerAttribute& format, ContentTypeSerializer& typeSerializer, T& existingInstance);

		//Reads a single object from the input XML stream.
		template <class T>
		T ReadRawObject(ContentSerializerAttribute& format);

		//Reads a single object from the input XML stream.
		template <class T>
		T ReadRawObject(ContentSerializerAttribute& format, T& existingInstance);

		//Reads a single object from the input XML stream.
		template <class T>
		T ReadRawObject(ContentSerializerAttribute& format, ContentTypeSerializer& typeSerializer);

		//Reads a single object from the input XML stream.
		template <class T>
		T ReadRawObject(ContentSerializerAttribute& format, ContentTypeSerializer& typeSerializer, T& existingInstance);		

	private:
		//Reads a single object from the input XML stream.
		template <class T>
		T ReadObjectInternal(ContentSerializerAttribute& format, ContentTypeSerializer& typeSerializer, T& existingInstance);
		
		//Reads a single object from the input XML stream.
		template <class T>
		T ReadRawObjectInternal(ContentSerializerAttribute& format, ContentTypeSerializer& typeSerializer, T& existingInstance);

	private:
		std::shared_ptr<IntermediateSerializer> serializer;
		std::shared_ptr<XmlReader> xmlReader;
	};	

	struct ISerialization {
		virtual void Serialize(ContentTypeSerializer& serializer, bool serializeOptional = true) = 0;
	};

	//Provides a generic implementation of ContentTypeSerializer methods and properties for serializing and deserializing a specific managed type.
	template <typename T>
	class ContentTypeSerializer_T : public ContentTypeSerializer {
	public:
		ContentTypeSerializer_T() {}
		ContentTypeSerializer_T(std::string const& xmlTypeName) : ContentTypeSerializer(xmlTypeName) {}

		//Serializes an object to intermediate XML format.
		virtual void Serialize(IntermediateWriter& output, T const& value, ContentSerializerAttribute& format) = 0;

		//Serializes an object to intermediate XML format.
		void Serialize(IntermediateWriter& output, std::any const& value, ContentSerializerAttribute& format) override {
			Serializer<T>(output, CastType(value), format);
		}

		//Deserializes a strongly typed object from intermediate XML format.
		virtual T Deserialize(IntermediateWriter& input, ContentSerializerAttribute& format, T& existingInstance) = 0;

		//Deserializes a strongly typed object from intermediate XML format.
		std::any Deserialize(IntermediateWriter& input, ContentSerializerAttribute& format, std::any const& existingInstance) override {

			if constexpr (XnaHelper::IsSmartPoint<T>() && !existingInstance.has_value())
				return T();
			
			T existingInstance1 = CastType(existingInstance);

			std::any obj = Deserialize<T>(input, format, existingInstance1);

			return obj;
		}

	private:
		static T CastType(std::any const& value) {
			try {
				return std::any_cast<T>(value);
			}
			catch (std::exception& ex) {
				throw csharp::InvalidOperationException();
			}
		}
	};
}

#endif