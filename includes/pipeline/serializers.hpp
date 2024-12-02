#ifndef XNA_PIPELINE_SERIALIZERS_HPP
#define XNA_PIPELINE_SERIALIZERS_HPP

#include "serialization.hpp"
#include "csharp/exception.hpp"
#include <any>
#include <memory>
#include <string>
#include <vector>

namespace xna {
	class XmlListReader {
	public:
		XmlListReader(std::shared_ptr<IntermediateReader> const& reader) : reader(reader) {
			if (this->reader == nullptr)
				throw csharp::InvalidOperationException();

			//this.enumerator = ((IEnumerable<string>) reader.Xml.ReadContentAsString().Split(XmlListReader.listSeparators, StringSplitOptions.RemoveEmptyEntries)).GetEnumerator();
		}

		constexpr bool AtEnd() const { return atEnd; }

	private:
		std::shared_ptr<IntermediateReader> reader;
		std::vector<std::string> enumerator;
		bool atEnd{ false };
	};

	struct IXmlListItemSerializer {
		virtual std::any Deserialize(XmlListReader const& list) = 0;
	};

	template <typename T>
	class XmlListItemSerializer : public ContentTypeSerializer_T<T>, public IXmlListItemSerializer {
	public:
		XmlListItemSerializer(){}
		XmlListItemSerializer(std::string const& xmlTypeName) : ContentTypeSerializer_T(xmlTypeName) {}

		virtual T Deserialize(XmlListReader& input) = 0;

		T Deserialize(IntermediateReader& input, ContentSerializerAttribute& format, T& existingInstance) override {		

			
		}
	};
}

#endif