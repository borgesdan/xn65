#ifndef XNA_CONTENT_TYPEREADER_HPP
#define XNA_CONTENT_TYPEREADER_HPP

#include "../csharp/type.hpp"
#include "../default.hpp"
#include <algorithm>
#include <map>

namespace xna {
	//-------------------------------------------------------//
	// 					 ContentTypeReader					 //
	//-------------------------------------------------------//
	class ContentTypeReader : public Object {
	public:
		ContentTypeReader(){}

	public:
		virtual Int TypeVersion() { return 0; }
		virtual bool CanDeserializeIntoExistingObject() { return false; }
		virtual void Initialize(sptr<ContentTypeReaderManager>& manager) {}
		
		sptr<Type> TargetType() { return _targetType; }		

		virtual sptr<Type> GetType() const override {
			auto type = New<Type>();
			type->FullName = "xna::ContentTypeReader";
			type->Namespace = "xna";
			type->IsClass = true;
			return type;
		}

	protected:
		ContentTypeReader(sptr<Type> const& targetType) : _targetType(targetType) 
		{
		}

		virtual sptr<void> Read(ContentReader input, sptr<void> existingInstance) = 0;

	public:
		bool TargetIsValueType{ false };

	private:
		sptr<Type> _targetType = nullptr;
	};

	//-------------------------------------------------------//
	// 						TypeComparator					 //
	//-------------------------------------------------------//
	struct TypeComparator
	{
		bool operator()(sptr<Type> t1, sptr<Type> t2) const
		{
			return t1->GetHashCode() < t2->GetHashCode();
		}		
	};

	//-------------------------------------------------------//
	// 				ContentTypeReaderActivador				 //
	//-------------------------------------------------------//
	class ContentTypeReaderActivador {
	public:
		using Activador = sptr<ContentTypeReader>(*)();

		static sptr<ContentTypeReader> CreateInstance(sptr<Type> const& type, xna_error_nullarg) {
			if (!type)
			{
				xna_error_apply(err, XnaErrorCode::ARGUMENT_IS_NULL);
				return nullptr;
			}

			const auto hash = type->GetHashCode();

			if (!activators.contains(hash))
				return nullptr;

			auto activador = activators[hash];

			if (!activador) return nullptr;

			return activador();
		}

		static void SetActivador(sptr<Type> const& type, Activador activador, xna_error_nullarg) {
			if (!type) {
				xna_error_apply(err, XnaErrorCode::ARGUMENT_IS_NULL);
				return;
			}

			const auto hash = type->GetHashCode();

			if (!activators.contains(hash))
				activators.insert({ hash, activador });
		}

	private:
		//inline static auto activators = std::map<size_t, Activador, TypeComparator>();
		inline static auto activators = std::map<size_t, Activador>();

		ContentTypeReaderActivador();
		ContentTypeReaderActivador(ContentTypeReaderActivador&&);
		ContentTypeReaderActivador(ContentTypeReaderActivador&);
	};

	using PContentTypeReader = sptr<ContentTypeReader>;
	using PType = sptr<Type>;

	//-------------------------------------------------------//
	// 				ContentTypeReaderManager				 //
	//-------------------------------------------------------//
	class ContentTypeReaderManager {
	public:
		static std::vector<PContentTypeReader> ReadTypeManifest(Int typeCount, sptr<ContentReader>& contentReader, xna_error_nullarg);
		static sptr<ContentTypeReader> GetTypeReader(sptr<Type> const& targetType, sptr<ContentReader>& contentReader, xna_error_nullarg);
		
		inline sptr<ContentTypeReader> GetTypeReader(sptr<Type> const& targetType, xna_error_nullarg) {
			return ContentTypeReaderManager::GetTypeReader(targetType, this->contentReader, err);
		}

		inline static bool ContainsTypeReader(sptr<Type> const& targetType) {
			return ContentTypeReaderManager::targetTypeToReader.contains(targetType);
		}

	private:
		ContentTypeReaderManager(sptr<ContentReader>& contentReader);
		static sptr<ContentTypeReader> GetTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, std::vector<PContentTypeReader>& newTypeReaders, xna_error_nullarg);
		static bool InstantiateTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, sptr<ContentTypeReader>& reader, xna_error_nullarg);
		static void AddTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, sptr<ContentTypeReader>& reader, xna_error_nullarg);
		static void RollbackAddReaders(std::vector<sptr<ContentTypeReader>>& newTypeReaders);		
		
		static void RollbackAddReader(std::map<String, PContentTypeReader>& dictionary, sptr<ContentTypeReader>& reader) {
			std::map<String, sptr<ContentTypeReader>>::iterator it;

			for (it = dictionary.begin(); it != dictionary.end(); it++) {
				if (it->second == reader) {
					dictionary.erase(it->first);
					it = dictionary.begin();
				}
			}
		}
		
		static void RollbackAddReader(std::map<PType, PContentTypeReader>& dictionary, sptr<ContentTypeReader>& reader) {
			std::map<PType, sptr<ContentTypeReader>>::iterator it;

			for (it = dictionary.begin(); it != dictionary.end(); it++) {
				if (it->second == reader) {
					dictionary.erase(it->first);
					it = dictionary.begin();
				}
			}
		}
		

	private:
		sptr<ContentReader> contentReader = nullptr;

		inline static auto nameToReader = std::map<String, PContentTypeReader>();
		//inline static auto targetTypeToReader = std::map<PType, PContentTypeReader, TypeComparator>();
		//inline static auto readerTypeToReader = std::map<PType, PContentTypeReader, TypeComparator>();
		inline static auto targetTypeToReader = std::map<PType, PContentTypeReader>();
		inline static auto readerTypeToReader = std::map<PType, PContentTypeReader>();
		
		static void initMaps();
	};

	//-------------------------------------------------------//
	//	 					ObjectReader					 //
	//-------------------------------------------------------//
	class ObjectReader : public ContentTypeReader {
	public:
		ObjectReader() : ContentTypeReader(typeof(this)){
			ContentTypeReaderActivador::SetActivador(typeof(this), []() -> sptr<ContentTypeReader> {
				auto obj = New <ObjectReader>();
				return reinterpret_pointer_cast<ContentTypeReader>(obj);
				});
		}

		// Inherited via ContentTypeReader
		sptr<void> Read(ContentReader input, sptr<void> existingInstance) override;		

		sptr<Type> GetType() const override{
			auto type = New<Type>();
			type->FullName = "xna::ObjectReader";
			type->Namespace = "xna";
			type->IsClass = true;
			return type;
		}
	};
}

#endif