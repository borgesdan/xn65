#include "xna/content/typereadermanager.hpp"
#include "xna/content/reader.hpp"
#include "xna/content/readers/default.hpp"

namespace xna {

	sptr<ContentTypeReader> ContentTypeReaderActivador::CreateInstance(sptr<Type> const& type) {
		if (!type)
		{
			throw std::invalid_argument("ContentTypeReaderActivador: type is null.");
		}

		const auto hash = type->GetHashCode();

		if (!activators.contains(hash))
			return nullptr;

		auto activador = activators[hash];

		if (!activador) return nullptr;

		return activador();
	}

	void ContentTypeReaderActivador::SetActivador(sptr<Type> const& type, Activador activador) {
		if (!type) {
			throw std::invalid_argument("ContentTypeReaderActivador: type is null.");
		}

		const auto hash = type->GetHashCode();

		if (!activators.contains(hash))
			activators.insert({ hash, activador });
	}

	std::vector<PContentTypeReader> ContentTypeReaderManager::ReadTypeManifest(Int typeCount, sptr<ContentReader>& contentReader)
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
				ContentTypeReaderManager::RollbackAddReaders(newTypeReaders);
				return std::vector<PContentTypeReader>();
			}

			contentTypeReaderArray[index] = typeReader;			
		}

		if (!newTypeReaders.empty()) {
			auto manager = std::shared_ptr<ContentTypeReaderManager>(new ContentTypeReaderManager(contentReader));

			for (size_t i = 0; i < newTypeReaders.size(); ++i) {
				auto& contentTypeReader = newTypeReaders[i];
				contentTypeReader->Initialize(manager);
			}
		}

		return contentTypeReaderArray;
	}

	sptr<ContentTypeReader> ContentTypeReaderManager::GetTypeReader(sptr<Type> const& targetType)
	{
		if (!targetType) {
			throw std::invalid_argument("ContentTypeReaderManager::GetTypeReader: targetType is null.");
		}		

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

	sptr<ContentTypeReader> ContentTypeReaderManager::GetTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, std::vector<PContentTypeReader>& newTypeReaders)
	{
		sptr<ContentTypeReader> reader = nullptr;

		if (ContentTypeReaderManager::nameToReader.contains(readerTypeName)) {
			return ContentTypeReaderManager::nameToReader[readerTypeName];
		}
		else if (!ContentTypeReaderManager::InstantiateTypeReader(readerTypeName, contentReader, reader)) {
			return reader;
		}		

		ContentTypeReaderManager::AddTypeReader(readerTypeName, contentReader, reader);		

		newTypeReaders.push_back(reader);

		return reader;
	}

	bool ContentTypeReaderManager::InstantiateTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, sptr<ContentTypeReader>& reader)
	{
		sptr<Type> type = nullptr;

		if (Type::NameOfRegisteredTypes.contains(readerTypeName))
			type = Type::NameOfRegisteredTypes[readerTypeName];		

		if (!type) {

			std::string error("ContentTypeReaderManager::InstantiateTypeReader:  registered type is null. ");
			error.append("TypeName: " + readerTypeName);
			throw std::runtime_error(error);
		}

		if (ContentTypeReaderManager::readerTypeToReader.contains(type)) {
			reader = ContentTypeReaderManager::readerTypeToReader[type];
			ContentTypeReaderManager::nameToReader.insert({ readerTypeName, reader });
			return false;
		}

		reader = ContentTypeReaderActivador::CreateInstance(type);
		return true;
	}

	void ContentTypeReaderManager::AddTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, sptr<ContentTypeReader>& reader)
	{
		auto targetType = reader->TargetType();

		if (ContentTypeReaderManager::targetTypeToReader.contains(targetType)) {
			return;
		}

		ContentTypeReaderManager::targetTypeToReader.insert({ targetType, reader });
		ContentTypeReaderManager::readerTypeToReader.insert({ typeof(*reader), reader});
		ContentTypeReaderManager::nameToReader.insert({ readerTypeName, reader });
	}

	void ContentTypeReaderManager::RollbackAddReaders(std::vector<sptr<ContentTypeReader>>& newTypeReaders)
	{
		if (newTypeReaders.empty())
			return;		

		for (size_t i = 0; i < newTypeReaders.size(); ++i) {
			auto& newTypeReader = newTypeReaders[i];
			ContentTypeReaderManager::RollbackAddReader(ContentTypeReaderManager::nameToReader, newTypeReader);
			ContentTypeReaderManager::RollbackAddReader(ContentTypeReaderManager::targetTypeToReader, newTypeReader);
			ContentTypeReaderManager::RollbackAddReader(ContentTypeReaderManager::readerTypeToReader, newTypeReader);
		}
	}	

	void ContentTypeReaderManager::initMaps()
	{
		if (targetTypeToReader.empty() && readerTypeToReader.empty()) {
			auto typeReader = snew<ObjectReader>();
			auto contentTypeReader = reinterpret_pointer_cast<ContentTypeReader>(typeReader);
			
			targetTypeToReader.insert({ typeof<Object>(), contentTypeReader});
			readerTypeToReader.insert({ typeof<ObjectReader>(), contentTypeReader});
		}
	}	

	void ContentTypeReaderManager::RollbackAddReader(std::map<String, PContentTypeReader>& dictionary, sptr<ContentTypeReader>& reader) {
		std::map<String, sptr<ContentTypeReader>>::iterator it;

		for (it = dictionary.begin(); it != dictionary.end(); it++) {
			if (it->second == reader) {
				dictionary.erase(it->first);
				it = dictionary.begin();
			}
		}
	}

	void ContentTypeReaderManager::RollbackAddReader(std::map<PType, PContentTypeReader>& dictionary, sptr<ContentTypeReader>& reader) {
		std::map<PType, sptr<ContentTypeReader>>::iterator it;

		for (it = dictionary.begin(); it != dictionary.end(); it++) {
			if (it->second == reader) {
				dictionary.erase(it->first);
				it = dictionary.begin();
			}
		}
	}
}