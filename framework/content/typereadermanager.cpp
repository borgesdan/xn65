#include "content/typereadermanager.hpp"
#include "content/reader.hpp"
#include "content/readers/default.hpp"

namespace xna {
	std::vector<PContentTypeReader> ContentTypeReaderManager::ReadTypeManifest(Int typeCount, sptr<ContentReader>& contentReader, xna_error_ptr_arg)
	{
		initMaps();
		
		auto contentTypeReaderArray = std::vector<PContentTypeReader>(typeCount);
		std::vector<PContentTypeReader> newTypeReaders;

		for (size_t index = 0; index < typeCount; ++index)
		{
			const auto readerTypeName = contentReader->ReadString();
			const auto xnaType = readerTypeName.substr(0, readerTypeName.find(","));


			auto typeReader = ContentTypeReaderManager::GetTypeReader(xnaType.empty() ? readerTypeName : xnaType, contentReader, newTypeReaders);

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

	sptr<ContentTypeReader> ContentTypeReaderManager::GetTypeReader(sptr<Type> const& targetType, sptr<ContentReader>& contentReader, xna_error_ptr_arg)
	{
		if (!targetType) {
			xna_error_apply(err, XnaErrorCode::ARGUMENT_IS_NULL);
			return nullptr;
		}

		//sptr<ContentTypeReader> typeReader = nullptr;

		/*if (!ContentTypeReaderManager::targetTypeToReader.contains(targetType)) {
			xna_error_apply(err, XnaErrorCode::ARGUMENT_OUT_OF_RANGE);
			return nullptr;
		}	*/	

		for (auto const& item : ContentTypeReaderManager::targetTypeToReader) {
			auto firstHashCode = item.first->GetHashCode();
			auto targetHashCode = targetType->GetHashCode();

			if (firstHashCode == targetHashCode)
				return item.second;
		}

		throw std::runtime_error("ContentTypeReaderManager::GetTypeReade: targetType not found.");
	}

	ContentTypeReaderManager::ContentTypeReaderManager(sptr<ContentReader>& contentReader) {
		initMaps();
	}

	sptr<ContentTypeReader> ContentTypeReaderManager::GetTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, std::vector<PContentTypeReader>& newTypeReaders, xna_error_ptr_arg)
	{
		sptr<ContentTypeReader> reader = nullptr;

		if (ContentTypeReaderManager::nameToReader.contains(readerTypeName)) {
			return ContentTypeReaderManager::nameToReader[readerTypeName];
		}
		else if (!ContentTypeReaderManager::InstantiateTypeReader(readerTypeName, contentReader, reader, err)) {
			return reader;
		}

		if (xna_error_haserros(err))
			return nullptr;

		ContentTypeReaderManager::AddTypeReader(readerTypeName, contentReader, reader, err);

		if (xna_error_haserros(err)) return nullptr;

		newTypeReaders.push_back(reader);

		return reader;
	}

	bool ContentTypeReaderManager::InstantiateTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, sptr<ContentTypeReader>& reader, xna_error_ptr_arg)
	{
		sptr<Type> type = nullptr;

		if (Type::NameOfRegisteredTypes.contains(readerTypeName))
			type = Type::NameOfRegisteredTypes[readerTypeName];		

		if (!type) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (ContentTypeReaderManager::readerTypeToReader.contains(type)) {
			reader = ContentTypeReaderManager::readerTypeToReader[type];
			ContentTypeReaderManager::nameToReader.insert({ readerTypeName, reader });
			return false;
		}

		reader = ContentTypeReaderActivador::CreateInstance(type, err);
		return true;
	}

	void ContentTypeReaderManager::AddTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, sptr<ContentTypeReader>& reader, xna_error_ptr_arg)
	{
		auto targetType = reader->TargetType();

		if (ContentTypeReaderManager::targetTypeToReader.contains(targetType)) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		ContentTypeReaderManager::targetTypeToReader.insert({ targetType, reader });
		//ContentTypeReaderManager::readerTypeToReader.insert({ reader->GetType(), reader });
		ContentTypeReaderManager::readerTypeToReader.insert({ typeof(*reader), reader});
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

	void ContentTypeReaderManager::initMaps()
	{
		if (targetTypeToReader.empty() && readerTypeToReader.empty()) {
			auto typeReader = New<ObjectReader>();
			auto contentTypeReader = reinterpret_pointer_cast<ContentTypeReader>(typeReader);

			//targetTypeToReader.insert({ typeReader->TargetType(), contentTypeReader});
			//readerTypeToReader.insert({ typeReader->GetType(), contentTypeReader});
			targetTypeToReader.insert({ typeof<Object>(), contentTypeReader});
			readerTypeToReader.insert({ typeof<ObjectReader>(), contentTypeReader});
		}
	}	
}