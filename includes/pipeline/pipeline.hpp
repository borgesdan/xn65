#ifndef XNA_PIPELINE_PIPELINE_HPP
#define XNA_PIPELINE_PIPELINE_HPP

#include <string>
#include <memory>
#include <map>
#include "xna/exception.hpp"
#include "xna/helpers.hpp"


namespace xna {
	class ContentBuilderLogger {

	};

	//Base class for dictionaries that map string identifiers to data values.
	template <typename T>
	class NamedValueDictionary {
	public:
		void Add(std::string const& key, T const& value) {
			return AddItem(key, value);
		}

		void Clear() {
			ClearItems();
		}

		bool ContainsKey(std::string const& key) {
			return items.contains(key);
		}

		size_t Count() const {
			return items.size();
		}

		bool Remove(std::string const& key) {
			return RemoveItem(key);
		}

		bool TryGetValue(std::string const& key, T& value) {
			if (items.contains(key))
			{
				value = items[key];
				return true;
			}

			return false;
		}

		T& operator[](std::string const& key) {
			return *items[key];
		}

	protected:
		virtual void AddItem(std::string const& key, T const& value) {
			if (key.empty()) {
				Exception::Throw(Exception::INVALID_OPERATION);
			}

			if constexpr (XnaHelper::IsSmartPoint<T>()) {
				if (value == nullptr)
					Exception::Throw(Exception::INVALID_OPERATION);
			}

			items.insert({ key, value });
		}

		virtual void ClearItems() {
			items.clear();
		}

		virtual bool RemoveItem(std::string const& key) {
			if (key.empty()) {
				return false;
			}

			const auto& search = items.find(key);

			if (search == items.end())
				return false;

			items.erase(key);

			return true;
		}

		virtual void SetItem(std::string const& key, T const& value) {
			if (key.empty()) {
				Exception::Throw(Exception::INVALID_OPERATION);
			}

			if constexpr (XnaHelper::IsSmartPoint<T>()) {
				if (value == nullptr)
					Exception::Throw(Exception::INVALID_OPERATION);
			}

			items[key] = value;
		}

	private:
		std::map<std::string, T> items;
	};

	//Provides properties describing the origin of the game asset, such as the original source file and creation tool. 
	//This information is used for error reporting, and by processors that need to determine from what directory the asset was originally loaded.
	struct ContentIdentity {
		constexpr ContentIdentity() = default;
		constexpr ContentIdentity(std::string sourceFilename) :
			SourceFilename(sourceFilename) {}
		constexpr ContentIdentity(std::string sourceFilename, std::string sourceTool) :
			SourceFilename(sourceFilename), SourceTool(sourceTool) {}
		constexpr ContentIdentity(std::string sourceFilename, std::string sourceTool, std::string fragmentIdentifier) :
			SourceFilename(sourceFilename), SourceTool(sourceTool), FragmentIdentifier(fragmentIdentifier) {}

		//Gets or sets the file name of the asset source.
		//Optional = true
		std::string SourceFilename;
		//Gets or sets the creation tool of the asset.
		//Optional = true
		std::string SourceTool;
		//Gets or sets the specific location of the content item within the larger source file.
		//Optional = true
		std::string FragmentIdentifier;
	};

	//Provides properties that define opaque data for a game asset.
	class OpaqueDataDictionary : NamedValueDictionary<std::any> {

	};

	//Provides properties that define various aspects of content stored using the intermediate file format of the XNA Framework.
	class ContentItem {
	public:
		ContentItem() {
			opaqueData = std::make_shared<OpaqueDataDictionary>();
		}

		//Gets or sets the name of the content item.
		std::string Name() const { return name; }
		//Gets or sets the identity of the content item.
		std::shared_ptr<ContentIdentity> Identity() const { return identity; }
		//Gets the opaque data of the content item.
		std::shared_ptr<OpaqueDataDictionary> OpaqueData() const { return opaqueData; }

		//Gets or sets the name of the content item.
		void Name(std::string const& value) { name = value; }
		//Gets or sets the identity of the content item.
		void Identity(std::shared_ptr<ContentIdentity> const& value) { identity = value; }

	private:
		//Optional = true
		std::string name;
		//Optional = true
		std::shared_ptr<ContentIdentity> identity;
		//Optional = true
		std::shared_ptr<OpaqueDataDictionary> opaqueData;
	};
}

#endif