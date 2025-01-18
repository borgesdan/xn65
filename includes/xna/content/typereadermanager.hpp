#ifndef XNA_CONTENT_TYPEREADER_HPP
#define XNA_CONTENT_TYPEREADER_HPP

#include "csharp/type.hpp"
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
		sptr<csharp::Type> TargetType() { return _targetType; }

		//Reads a strongly typed object from the current stream.
		virtual std::any Read(ContentReader& input, std::any& existingInstance) = 0;

	protected:
		ContentTypeReader(sptr<csharp::Type> const& targetType) : _targetType(targetType)
		{
		}

	public:
		//Let's admit that the target is primarily of type value, if not it must be manually set to false
		bool TargetIsValueType{ true };

	private:
		sptr<csharp::Type> _targetType = nullptr;
	};

	//Worker for reading a specific managed type from a binary format. 
	//Derive from this class to add new data types to the content pipeline system. 
	template <class T>
	class ContentTypeReaderT : public ContentTypeReader {
	public:
		//For some reason ListReader<T> needs a default constructor
		ContentTypeReaderT() : ContentTypeReader(std::make_shared<csharp::Type>(csharp::typeof<T>())) {}
	protected:
		ContentTypeReaderT(sptr<csharp::Type> const& targetType) : ContentTypeReader(targetType) {}

	public:
		//Reads a strongly typed object from the current stream.
		std::any Read(ContentReader& input, std::any& existingInstance) override {
			if (existingInstance.has_value() && !(existingInstance.type() == typeid(T)))
				throw std::runtime_error("ContentTypeReader<T>::Read: bad xbn, wrong type.");

			auto existingInstance1 = misc::ReturnDefaultOrNull<T>();
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

		static sptr<ContentTypeReader> CreateInstance(sptr<csharp::Type> const& type);
		static void SetActivador(sptr<csharp::Type> const& type, Activador activador);

	private:
		inline static auto activators = std::map<size_t, Activador>();

		ContentTypeReaderActivador();
		ContentTypeReaderActivador(ContentTypeReaderActivador&&);
		ContentTypeReaderActivador(ContentTypeReaderActivador&);
	};

	using PContentTypeReader = sptr<ContentTypeReader>;
	using PType = sptr<csharp::Type>;

	//-------------------------------------------------------//
	// 				ContentTypeReaderManager				 //
	//-------------------------------------------------------//
	
	//A manager that constructs and keeps track of type reader objects. 
	class ContentTypeReaderManager {
	public:
		static std::vector<PContentTypeReader> ReadTypeManifest(Int typeCount, sptr<ContentReader>& contentReader);
		
		//Looks up a reader for the specified type.
		static sptr<ContentTypeReader> GetTypeReader(sptr<csharp::Type> const& targetType);		

		inline static bool ContainsTypeReader(sptr<csharp::Type> const& targetType) {
			return ContentTypeReaderManager::targetTypeToReader.contains(targetType);
		}

	private:
		ContentTypeReaderManager(sptr<ContentReader>& contentReader);
		static sptr<ContentTypeReader> GetTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, std::vector<PContentTypeReader>& newTypeReaders);
		static bool InstantiateTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, sptr<ContentTypeReader>& reader);
		static void AddTypeReader(String const& readerTypeName, sptr<ContentReader>& contentReader, sptr<ContentTypeReader>& reader);
		static void RollbackAddReaders(std::vector<sptr<ContentTypeReader>>& newTypeReaders);

		static void RollbackAddReader(std::map<String, PContentTypeReader>& dictionary, sptr<ContentTypeReader>& reader);
		static void RollbackAddReader(std::map<PType, PContentTypeReader>& dictionary, sptr<ContentTypeReader>& reader);

	private:
		sptr<ContentReader> contentReader = nullptr;

		inline static auto nameToReader = std::map<String, PContentTypeReader>();
		inline static auto targetTypeToReader = std::map<PType, PContentTypeReader>();
		inline static auto readerTypeToReader = std::map<PType, PContentTypeReader>();

		static void initMaps();
	};
}

#endif