#include "typereadermanager.hpp"
#include "reader.hpp"

namespace xna {
	std::vector<PContentTypeReader> ContentTypeReaderManager::ReadTypeManifest(Int typeCount, sptr<ContentReader>& contentReader, xna_error_ptr_arg)
	{
		initMaps();
		
		auto contentTypeReaderArray = std::vector<PContentTypeReader>(typeCount);
		std::vector<PContentTypeReader> newTypeReaders;

		for (size_t index = 0; index < typeCount; ++index)
		{
			auto typeReader = ContentTypeReaderManager::GetTypeReader(contentReader->ReadString(), contentReader, newTypeReaders);

			if (contentReader->ReadInt32() != typeReader->TypeVersion()) {
				xna_error_apply(err, XnaErrorCode::BAD_TYPE);
				ContentTypeReaderManager::RollbackAddReaders(newTypeReaders);
				return std::vector<PContentTypeReader>();
			}

			contentTypeReaderArray[index] = typeReader;

			if (!newTypeReaders.empty()) {
				auto manager = std::shared_ptr<ContentTypeReaderManager>(new ContentTypeReaderManager(contentReader));

				for (size_t i = 0; i < newTypeReaders.size(); ++i) {
					auto& contentTypeReader = newTypeReaders[i];
					contentTypeReader->Initialize(manager);
				}
			}
		}

		return contentTypeReaderArray;
	}

	sptr<ContentTypeReader> ContentTypeReaderManager::GetTypeReader(String const& targetType, sptr<ContentReader>& contentReader, xna_error_ptr_arg)
	{
		if (targetType.empty())
			return nullptr;

		sptr<ContentTypeReader> typeReader = nullptr;

		if (!ContentTypeReaderManager::targetTypeToReader.contains(targetType)) {
			xna_error_apply(err, XnaErrorCode::ARGUMENT_OUT_OF_RANGE);
			return nullptr;
		}		

		return ContentTypeReaderManager::targetTypeToReader[targetType];
	}

	ContentTypeReaderManager::ContentTypeReaderManager(sptr<ContentReader>& contentReader) {
		initMaps();
	}

	sptr<ContentTypeReader> ContentTypeReaderManager::GetTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, std::vector<PContentTypeReader>& newTypeReaders, xna_error_ptr_arg)
	{
		sptr<ContentTypeReader> reader = nullptr;

		if (ContentTypeReaderManager::nameToReader.contains(readerTypeName) || !ContentTypeReaderManager::InstantiateTypeReader(readerTypeName, contentReader, reader)) {
			return ContentTypeReaderManager::nameToReader[readerTypeName];
		}		

		ContentTypeReaderManager::AddTypeReader(readerTypeName, contentReader, reader, err);

		if (xna_error_haserros(err)) return nullptr;

		newTypeReaders.push_back(reader);

		return reader;
	}

	bool ContentTypeReaderManager::InstantiateTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, sptr<ContentTypeReader>& reader)
	{
		if (ContentTypeReaderManager::readerTypeToReader.contains(readerTypeName)) {
			reader = ContentTypeReaderManager::readerTypeToReader[readerTypeName];
			ContentTypeReaderManager::nameToReader.insert({ readerTypeName, reader });
			return false;
		}

		reader = ContentTypeReaderActivador::CreateInstance(readerTypeName);
	}

	void ContentTypeReaderManager::AddTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, sptr<ContentTypeReader>& reader, xna_error_ptr_arg)
	{
		auto targetType = reader->TargetType();

		if (ContentTypeReaderManager::targetTypeToReader.contains(targetType)) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		ContentTypeReaderManager::targetTypeToReader.insert({ targetType, reader });
		ContentTypeReaderManager::readerTypeToReader.insert({ reader->GetType(), reader });
		ContentTypeReaderManager::nameToReader.insert({ readerTypeName, reader });
	}

	void ContentTypeReaderManager::RollbackAddReaders(std::vector<sptr<ContentTypeReader>>& newTypeReaders)
	{
		if (newTypeReaders.empty())
			return;		

		for (size_t i = 0; i < newTypeReaders.size(); ++i) {
			auto newTypeReader = newTypeReaders[i];
			ContentTypeReaderManager::RollbackAddReader(ContentTypeReaderManager::nameToReader, newTypeReader);
			ContentTypeReaderManager::RollbackAddReader(ContentTypeReaderManager::targetTypeToReader, newTypeReader);
			ContentTypeReaderManager::RollbackAddReader(ContentTypeReaderManager::readerTypeToReader, newTypeReader);
		}
	}

	void ContentTypeReaderManager::RollbackAddReader(std::map<String, sptr<ContentTypeReader>>& dictionary, sptr<ContentTypeReader>& reader) {
		std::map<String, sptr<ContentTypeReader>>::iterator it;

		for (it = dictionary.begin(); it != dictionary.end(); it++) {
			if (it->second == reader) {
				dictionary.erase(it->first);
				it = dictionary.begin();
			}
		}
	}

	void ContentTypeReaderManager::initMaps()
	{
		if (targetTypeToReader.empty() && readerTypeToReader.empty()) {
			auto typeReader = New<ObjectReader>();
			auto contentTypeReader = reinterpret_pointer_cast<ContentTypeReader>(typeReader);

			targetTypeToReader.insert({ typeReader->TargetType(), contentTypeReader});
			readerTypeToReader.insert({ typeReader->GetType(), contentTypeReader});
		}
	}

	sptr<void> ObjectReader::Read(ContentReader input, sptr<void> existingInstance)
	{
		return nullptr;
	}
}