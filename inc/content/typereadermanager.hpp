#ifndef XNA_CONTENT_TYPEREADER_HPP
#define XNA_CONTENT_TYPEREADER_HPP

#include "../csharp/type.hpp"
#include "../default.hpp"
#include <algorithm>
#include <map>
#include <any>

namespace xna {
	//-------------------------------------------------------//
	// 					 ContentTypeReader					 //
	//-------------------------------------------------------//

	//Worker for reading a specific managed type from a binary format. 
	class ContentTypeReader {
	public:
		virtual Int TypeVersion() { return 0; }
		virtual bool CanDeserializeIntoExistingObject() { return false; }

		//Retrieves and caches nested type readers. Called by the framework at creation time.
		virtual void Initialize(sptr<ContentTypeReaderManager> const& manager) {}

		//Gets the type handled by this reader component.
		sptr<Type> TargetType() { return _targetType; }

		//Reads a strongly typed object from the current stream.
		virtual std::any Read(ContentReader& input, std::any& existingInstance) = 0;

	protected:
		ContentTypeReader(sptr<Type> const& targetType) : _targetType(targetType)
		{
		}

	public:
		//Let's admit that the target is primarily of type value, if not it must be manually set to false
		bool TargetIsValueType{ true };

	private:
		sptr<Type> _targetType = nullptr;
	};

	//Worker for reading a specific managed type from a binary format. 
	//Derive from this class to add new data types to the content pipeline system. 
	template <class T>
	class ContentTypeReaderT : public ContentTypeReader {
	public:
		//For some reason ListReader<T> needs a default constructor
		ContentTypeReaderT() : ContentTypeReader(typeof<T>()) {}
	protected:
		ContentTypeReaderT(sptr<Type> const& targetType) : ContentTypeReader(targetType) {}

	public:
		//Reads a strongly typed object from the current stream.
		std::any Read(ContentReader& input, std::any& existingInstance) override {
			if (existingInstance.has_value() && !(existingInstance.type() == typeid(T)))
				throw std::runtime_error("ContentTypeReader<T>::Read: bad xbn wrong type.");

			auto existingInstance1 = XnaHelper::ReturnDefaultOrNull<T>();
			auto obj = Read(input, existingInstance1);
			return obj;
		}

		//Reads a strongly typed object from the current stream.
		virtual T Read(ContentReader& input, T& existingInstance) = 0;
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
		inline static auto targetTypeToReader = std::map<PType, PContentTypeReader>();
		inline static auto readerTypeToReader = std::map<PType, PContentTypeReader>();

		static void initMaps();
	};
}

#endif