#ifndef XNA_CONTENT_MANAGER_HPP
#define XNA_CONTENT_MANAGER_HPP

#include "../default.hpp"
#include <filesystem>
#include <map>
#include <algorithm>
#include "../csharp/stream.hpp"

namespace xna {
	class ContentManager {
	public:
		ContentManager(String const& rootDirectory) : 
			_rootDirectory(rootDirectory),
			_path(rootDirectory){};

		virtual ~ContentManager(){
			Unload();
		}

		constexpr String RootDirectory() const {
			return _rootDirectory;
		}

		void RootDirectory(String const& value) {
			_rootDirectory = value;
			_path = value;
		}

		virtual void Unload() {
			if (_loadedAssets.empty())
				return;	
			
			_loadedAssets.clear();
		}

		template <typename T>
		sptr<T> Load(String const& assetName) {
			if (assetName.empty()) return nullptr;		

			if (_loadedAssets.contains(assetName)) {
				const auto& ptr = _loadedAssets[assetName];
				const auto obj1 = reinterpret_pointer_cast<T>(ptr);

				return obj1;
			}

			const auto obj2 = ReadAsset<T>(assetName);
			return obj2;
		}

	protected:
		template <typename T>
		sptr<T> ReadAsset(String const& assetName) {
			auto input = OpenStream(assetName);
			auto contentReader = ContentReader::Create(this, input, assetName);

			return contentReader->ReadAsset<T>();
		}

		sptr<Stream> OpenStream(String const& assetName) {
			String filePath = _rootDirectory + "\\" + assetName + contentExtension;
			const auto stream = New<FileStream>(filePath);
			return reinterpret_pointer_cast<Stream>(stream);
		}

	private:
		String _rootDirectory;
		std::filesystem::path _path;
		std::map<String, sptr<void>> _loadedAssets;
		inline const static String contentExtension = ".xnb";
	};
}

#endif