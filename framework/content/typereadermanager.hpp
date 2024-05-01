#ifndef XNA_CONTENT_TYPEREADER_HPP
#define XNA_CONTENT_TYPEREADER_HPP

#include "../default.hpp"
#include <map>
#include <algorithm>

namespace xna {
	//ContentTypeReader
	class ContentTypeReader {
	public:
		virtual Int TypeVersion() { return 0; }
		virtual bool CanDeserializeIntoExistingObject() { return 0; }
		virtual void Initialize(sptr<ContentTypeReaderManager>& manager) {}
		
		constexpr String TargetType() { return _targetType; }

		virtual String GetType() { return "ContentTypeReader"; }

	protected:
		ContentTypeReader(String targetType) : _targetType(targetType) {
		}

		virtual sptr<void> Read(ContentReader input, sptr<void> existingInstance) = 0;

	private:
		String _targetType = "contentTypeReader";
	};

	//ContentTypeReaderActivador
	class ContentTypeReaderActivador {
	public:
		using Activador = sptr<ContentTypeReader>(*)();

		static sptr<ContentTypeReader> CreateInstance(String const& readerTypeName) {
			if (!activators.contains(readerTypeName))
				return nullptr;

			auto activador = activators[readerTypeName];

			if (!activador) return nullptr;

			return activador();
		}

		static void SetActivador(String const& readerTypeName, Activador activador) {
			if (!activators.contains(readerTypeName))
				activators.insert({ readerTypeName, activador });
		}

	private:
		inline static std::map<String, Activador> activators =
			std::map<String, Activador>();

		ContentTypeReaderActivador();
		ContentTypeReaderActivador(ContentTypeReaderActivador&&);
		ContentTypeReaderActivador(ContentTypeReaderActivador&);
	};

	using PContentTypeReader = sptr<ContentTypeReader>;

	//ContentTypeReaderManager
	class ContentTypeReaderManager {
	public:
		static std::vector<PContentTypeReader> ReadTypeManifest(Int typeCount, sptr<ContentReader>& contentReader, xna_error_nullarg);
		static sptr<ContentTypeReader> GetTypeReader(String const& targetType, sptr<ContentReader>& contentReader, xna_error_nullarg);
		
		inline sptr<ContentTypeReader> GetTypeReader(String const& targetType, xna_error_nullarg) {
			return ContentTypeReaderManager::GetTypeReader(targetType, this->contentReader, err);
		}

		inline static bool ContainsTypeReader(String const& targetType) {
			return ContentTypeReaderManager::targetTypeToReader.contains(targetType);
		}

	private:
		ContentTypeReaderManager(sptr<ContentReader>& contentReader);
		static sptr<ContentTypeReader> GetTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, std::vector<PContentTypeReader>& newTypeReaders, xna_error_nullarg);
		static bool InstantiateTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, sptr<ContentTypeReader>& reader);
		static void AddTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, sptr<ContentTypeReader>& reader, xna_error_nullarg);
		static void RollbackAddReaders(std::vector<sptr<ContentTypeReader>>& newTypeReaders);
		static void RollbackAddReader(std::map<String, sptr<ContentTypeReader>>& dictionary, sptr<ContentTypeReader>& reader);
		

	private:
		sptr<ContentReader> contentReader = nullptr;

		inline static std::map<String, PContentTypeReader> nameToReader = std::map<String, PContentTypeReader>();
		inline static std::map<String, PContentTypeReader> targetTypeToReader = std::map<String, PContentTypeReader>();
		inline static std::map<String, PContentTypeReader> readerTypeToReader = std::map<String, PContentTypeReader>();
		
		static void initMaps();
	};

	//ObjectReader
	class ObjectReader : public ContentTypeReader {
	public:
		ObjectReader() : ContentTypeReader("object"){
			ContentTypeReaderActivador::SetActivador("object", []() -> sptr<ContentTypeReader> {
				auto obj = New <ObjectReader>();
				return reinterpret_pointer_cast<ContentTypeReader>(obj);
				});
		}

		// Inherited via ContentTypeReader
		sptr<void> Read(ContentReader input, sptr<void> existingInstance) override;

		String GetType() override {
			return "ObjectReader";
		}
	};
}

#endif