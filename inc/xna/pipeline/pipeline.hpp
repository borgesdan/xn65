#ifndef XNA_PIPELINE_PIPELINE_HPP
#define XNA_PIPELINE_PIPELINE_HPP

#include "../csharp/attribute.hpp"
#include "../default.hpp"
#include <any>
#include <filesystem>
#include <initializer_list>		
#include <map>
#include <stack>
#include <utility>

//Provides classes representing base types and building block functionality
//for use by more specialized object models, such as the Graphics DOM.

namespace xna {
	//Provides a collection of child objects for a content item.
	template <class TParent, class TChild>
	class ChildCollection {
	protected:
		ChildCollection(sptr<TParent> const& parent) : parentNode(parent)
		{
			if (!parent)
				Exception::Throw(ExMessage::ArgumentIsNull);
		}

		//Gets the parent of a child object.
		virtual sptr<TParent> GetParent(sptr<TChild> const& child) = 0;
		//Modifies the value of the parent object of the specified child object.
		virtual void SetParent(sptr<TChild> const&, sptr<TParent> const& parent) = 0;

		//Removes all children from the collection.
		void ClearItems() {
			for (size_t i = 0; i < data.size(); ++i)
				SetParent(data[i], nullptr);

			data.clear();
		}

		//Inserts a child object into the collection at the specified location.
		void InsertItem(size_t index, sptr<TChild> const& item) {
			if (!item)
				Exception::Throw(ExMessage::ArgumentIsNull);

			if (GetParent(item) != nullptr)
				Exception::Throw(ExMessage::InvalidOperation);

			data.insert(data.begin() + index, item);
			SetParent(item, parentNode);
		}

		//Removes a child object from the collection.
		void RemoveItem(size_t index) {
			const auto& child = data[index];
			data.erase(data.begin() + index);
			SetParent(child, nullptr);
		}

		//Modifies the value of the child object at the specified location.
		void SetItem(size_t index, sptr<TChild> const& item) {
			if (!item)
				Exception::Throw(ExMessage::ArgumentIsNull);

			const auto& child = data[index];

			if (child == item)
				return;

			if (GetParent(item) != nullptr)
				Exception::Throw(ExMessage::InvalidOperation);

			SetItem(index, item);
			SetParent(child, nullptr);
			SetParent(item, parentNode);
		}

	public:
		std::vector<sptr<TChild>> data;

	private:
		sptr<TParent> parentNode = nullptr;
	};

	//Base class for dictionaries that map string identifiers to data values.
	template <class T>
	class NamedValueDictionary {
	public:
		virtual ~NamedValueDictionary() {}

	protected:
		//Base class for dictionaries that map string identifiers to data values.
		virtual void AddItem(String const& key, T const& value) {
			if (!key.empty())
				Exception::Throw(ExMessage::ArgumentIsNullOrEmpty);

			if constexpr (XnaHelper::is_shared_ptr<T>::value) {
				if (!value)
					Exception::Throw(ExMessage::ArgumentIsNullOrEmpty);
			}

			items.emplace(key, value);
		}

		//Removes all elements from the dictionary.
		virtual void ClearItems() {
			items.clear();
		}

		//Removes the specified element from the dictionary.
		virtual bool RemoveItem(String const& key) {
			if (!key.empty())
				Exception::Throw(ExMessage::ArgumentIsNullOrEmpty);

			items.erase(key);
		}

		//Modifies the value of an existing element.
		virtual void SetItem(String const& key, T const& value) {
			if (!key.empty())
				Exception::Throw(ExMessage::ArgumentIsNullOrEmpty);

			if constexpr (XnaHelper::IsSmartPointer<T>()) {
				if (!value)
					Exception::Throw(ExMessage::ArgumentIsNullOrEmpty);
			}

			items[key] = value;
		}

	public:
		//Adds the specified key and value to the dictionary.
		void Add(String const& key, T const& value) {
			AddItem(key, value);
		}

		//Removes all keys and values from the dictionary.
		void Clear() {
			ClearItems();
		}

		//Determines whether the specified key is present in the dictionary.
		bool ContainsKey(String const& key) {
			return items.find(key) != items.end();
		}

		//Gets the number of items in the dictionary.
		Int Count() {
			return static_cast<Int>(items.size());
		}

		//Removes the specified key and value from the dictionary.
		bool Remove(String const& key) {
			auto i = items.find(key);

			if (i == items.end())
				return false;

			items.erase(i);
		}

		//Gets the value associated with the specified key.
		bool TryGetValue(String const& key, T& value) {
			if (!ContainsKey(key))
				return false;

			value = items[key];
			return true;
		}

	public:
		std::map<String, T> items;
	};

	class OpaqueDataDictionary : public NamedValueDictionary<std::any> {
	public:
		template <typename T>
		T GetValue(String const& key, T defaultValue) {
			std::any obj1;
			auto result = TryGetValue(key, obj1);

			if (result && obj1.type() == typeid(T)) {
				auto value = std::any_cast<T>(obj1);
				return value;
			}

			return defaultValue;
		}

		String GetContentAsXml() {
			//TODO: implementar com tratamento de XML
			Exception::Throw(ExMessage::NotImplemented);

			return contentAsXml;
		}

	protected:
		void AddItem(String const& key, std::any const& value) override {
			contentAsXml.empty();
			NamedValueDictionary::AddItem(key, value);
		}

		void ClearItems() override {
			contentAsXml.empty();
			NamedValueDictionary::ClearItems();
		}

		bool RemoveItem(String const& key) override {
			contentAsXml.empty();
			NamedValueDictionary::RemoveItem(key);
		}

		void SetItem(String const& key, std::any const& value) override {
			contentAsXml.empty();
			NamedValueDictionary::SetItem(key, value);
		}

	private:
		String contentAsXml;
	};

	//Provides properties describing the origin of the game asset, such as the original source file
	//and creation tool. This information is used for error reporting, and by processors that need to determine from what directory the asset was originally loaded. 
	class ContentIdentity {
	public:
		constexpr ContentIdentity() {}
		constexpr ContentIdentity(String const& sourceFilename)
			: SourceFilename(sourceFilename) {}
		constexpr ContentIdentity(String const& sourceFilename, String sourceTool)
			: SourceFilename(sourceFilename), SourceTool(sourceTool) {}
		constexpr ContentIdentity(String const& sourceFilename, String sourceTool, String fragmentIdentifier)
			: SourceFilename(sourceFilename), SourceTool(sourceTool), FragmentIdentifier(fragmentIdentifier) {}

	public:
		//Gets or sets the file name of the asset source.
		String SourceFilename;
		//Gets or sets the creation tool of the asset.
		String SourceTool;
		//Gets or sets the specific location of the content item within the larger source file.
		String FragmentIdentifier;
	};

	//Provides methods for reporting informational messages or warnings from content importers and processors.
	class ContentBuilderLogger {
		//Outputs a low priority status message from a content importer or processor.
		virtual void LogMessage(String const& message, std::initializer_list<std::any> messageArgs) = 0;
		//Outputs a high-priority status message from a content importer or processor.
		virtual void LogImportantMessage(String const& message, std::initializer_list<std::any> messageArgs) = 0;

		//Outputs a warning message from a content importer or processor.
		virtual void LogWarning(
			String const& helpLink,
			sptr<ContentIdentity> const& contentIdentity,
			String const& message,
			std::initializer_list<std::any> messageArgs) = 0;

		//Outputs a message indicating that a content asset has begun processing.
		inline void PushFile(String const& filename) {
			fileStack.push(filename);
		}

		//Outputs a message indicating that a content asset has completed processing.
		inline void PopFile() {
			fileStack.pop();
		}

		//Gets the filename currently being processed, for use in warning and error messages.
		String GetCurrentFilename(sptr<ContentIdentity> const& contentIdentity);

	public:
		String LoggerRootDirectory;

	private:
		std::stack<String> fileStack;

		static void GetRelativeFilename(String const& filename, String const& rootDirectory, String& out);
	};

	//Provides properties that define various aspects of content stored using the intermediate file format of the XNA Framework.
	struct ContentItem {
		//[ContentSerializer(Optional = true)]
		//Gets or sets the name of the content item.
		String Name;

		//[ContentSerializer(Optional = true)]
		//Gets or sets the identity of the content item.
		sptr<ContentIdentity> Identity = nullptr;

		//[ContentSerializer(Optional = true)]
		//Gets the opaque data of the content item.
		sptr<OpaqueDataDictionary> OpaqueData = snew<OpaqueDataDictionary>();
	};

	//Specifies external references to a data file for the content item. 
	//While the object model is instantiated, reference file names are absolute.
	//When the file containing the external reference is serialized to disk, file names are relative to the file.
	//This allows movement of the content tree to a different location without breaking internal links.
	template <typename T>
	class ExternalReference {};

	//Provides properties that define logging behavior for the importer. 
	class ContentImporterContext {
	public:
		virtual ~ContentImporterContext() {}

		//Gets the logger for an importer.
		virtual uptr<ContentBuilderLogger> Logger() const = 0;
		//Adds a dependency to the specified file.
		virtual void AddDependecy(String const& fileName) = 0;
		//The absolute path to the root of the build output (binaries) directory.
		virtual String OutputDirectory() const = 0;
		//he absolute path to the root of the build intermediate (object) directory.
		virtual String IntermediateDirectory() const = 0;
	};

	//Accesses a statically typed ContentImporter instance from generic code using dynamic typing.
	class IContentImporter {
	public:
		virtual ~IContentImporter() {}

		//Imports an asset from the specified file.
		virtual std::any Import(String const& filename, ContentImporterContext& context) = 0;
	};

	//Implements a file format importer for use with game assets.
	template <class T>
	class ContentImporter : public IContentImporter {
	public:
		virtual ~ContentImporter() {}

		//Imports an asset from the specified file.
		virtual T Import(String const& filename, ContentImporterContext& context) = 0;

		virtual std::any Import(String const& filename, ContentImporterContext& context) override {
			std::any obj = Import<T>(filename, context);
			return obj;
		}
	};

	//Provides properties that identify and provide metadata about the importer, such as supported file extensions and caching information.
	class ContentImporterAttribute : public Attribute {
	public:
		ContentImporterAttribute(String const& fileExtension) : fileExtensions(1) {
			fileExtensions[0] = fileExtension;
		}

		ContentImporterAttribute(std::initializer_list<String> extensions) : fileExtensions(extensions)
		{
			for (size_t i = 0; i < fileExtensions.size(); ++i) {
				if (!fileExtensions[i].starts_with('.'))
					Exception::Throw(ExMessage::ArgumentFormat);
			}
		}

		//Gets the supported file name extensions of the importer.
		constexpr std::vector<String> FileExtensions() const {
			return fileExtensions;
		}

	public:
		//Gets and sets the caching of the content during importation.
		bool CacheImportedData{ false };
		//Gets or sets the string representing the importer in a user interface.
		String DisplayName;
		//Gets or sets the name of the default processor for content read by this importer.
		String DefaultProcessor;

	private:
		std::vector<String> fileExtensions;
	};

	//Provides access to custom processor parameters, methods for converting member data, and triggering nested builds.
	class ContentProcessorContext {
		//Gets the logger interface used for status messages or warnings.
		virtual sptr<ContentBuilderLogger> Logger() = 0;
		//Gets the collection of parameters used by the content processor.
		virtual sptr<OpaqueDataDictionary> Parameters() = 0;
		//Gets the current content build target platform.
		virtual xna::TargetPlatform TargetPlatform() = 0;
		//Gets the current content build target profile.
		virtual GraphicsProfile TargetProfile() = 0;
		//Gets the name of the current content build configuration.
		virtual String BuildConfiguration() = 0;
		//Gets the output file name of the content processor.
		virtual String OutputFilename() = 0;
		//Gets the output path of the content processor.
		virtual String OutputDirectory() = 0;
		//Gets the path of the directory that will contain any intermediate files generated by the content processor.
		virtual String IntermediateDirectory() = 0;
		//Adds a dependency to the specified file.
		virtual void AddDependency(String const& filename) = 0;
		//Add a file name to the list of related output files maintained by the build item.
		virtual void AddOutputFile(String const& filename) = 0;

		//Initiates a nested build of an asset.
		template <class TInput, class TOutput>
		sptr<ExternalReference<TInput>> BuildAsset(
			sptr<ExternalReference<TInput>>& sourceAsset,
			String const& processorName,
			sptr<OpaqueDataDictionary>& processorParameters,
			String const& importerName,
			String const& assetName) {
			return nullptr;
		}

		//Initiates a nested build of an asset.
		template <class TInput, class TOutput>
		sptr<ExternalReference<TInput>> BuildAsset(
			sptr<ExternalReference<TInput>>& sourceAsset,
			String const& processorName) {
			return BuildAsset<TInput, TOutput>(sourceAsset, processorName, nullptr, "", "");
		}

		//Initiates a nested build of the specified asset and then loads the result into memory.
		template <class TInput, class TOutput>
		sptr<ExternalReference<TInput>> BuildAndLoadAsset(
			sptr<ExternalReference<TInput>>& sourceAsset,
			String const& processorName,
			sptr<OpaqueDataDictionary>& processorParameters,
			String const& importerName) {
			return nullptr;
		}

		//Initiates a nested build of the specified asset and then loads the result into memory.
		template <class TInput, class TOutput>
		sptr<ExternalReference<TInput>> BuildAndLoadAsset(
			sptr<ExternalReference<TInput>>& sourceAsset,
			String const& processorName) {
			return BuildAndLoadAsset<TInput, TOutput>(sourceAsset, processorName, nullptr, "");
		}

		//Converts a content item object using the specified content processor.
		template <class TInput, class TOutput>
		TOutput Convert(
			TInput& input,
			String const& processorName,
			sptr<OpaqueDataDictionary>& processorParameters) {
			return {};
		}

		//Converts a content item object using the specified content processor.
		template <class TInput, class TOutput>
		TOutput Convert(
			TInput& input,
			String const& processorName) {
			auto params = snew<OpaqueDataDictionary>();
			return Convert(input, processorName, params);
		}
	};

	//Provides methods and properties for accessing a statically typed ContentProcessor subclass, using dynamically typed object data.
	struct IContentProcessor {
		//Processes the specified input data and returns the result.
		virtual std::any Process(std::any& input, ContentProcessorContext& context) = 0;
		//Gets the expected object type of the input parameter to IContentProcessor.Process.
		virtual sptr<Type> InputType() = 0;
		//Gets the object type returned by IContentProcessor.Process.
		virtual sptr<Type> OutputType() = 0;
	};

	//Provides a base class to use when developing custom processor components. All processors must derive from this class.
	template <typename TInput, typename TOutput>
	class ContentProcessor : IContentProcessor {
	public:
		//Processes the specified input data and returns the result.
		virtual TOutput Process(TInput& input, ContentProcessorContext& context) = 0;

		//Processes the specified input data and returns the result.
		std::any Process(std::any& input, ContentProcessorContext& context) override {
			if (!input.has_value())
				Exception::Throw(ExMessage::ArgumentIsNullOrEmpty);

			if (input.type() != typeid(TInput))
			{
				Exception::Throw(ExMessage::ArgumentFormat);
			}

			auto input1 = std::any_cast<TInput>(input);
			auto obj1 = Process(input1, context);

			return obj1;
		}

		//Gets the expected object type of the input parameter to IContentProcessor.Process. 
		sptr<Type> InputType() override {
			return typeof<TInput>();
		}

		//Gets the object type returned by IContentProcessor.Process.
		sptr<Type> OutputType() override {
			return typeof<TOutput>();
		}
	};
}

#endif