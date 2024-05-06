#ifndef XNA_CONTENT_MANAGER_HPP
#define XNA_CONTENT_MANAGER_HPP

#include "../csharp/stream.hpp"
#include "../default.hpp"
#include "reader.hpp"
#include <algorithm>
#include <filesystem>
#include <map>
#include "../game/servicecontainer.hpp"

namespace xna {
	class ContentManager {
	public:
		friend class ContentReader;

		ContentManager(String const& rootDirectory, sptr<GameServiceContainer> const& services) : 
			_rootDirectory(rootDirectory){
			_services = services;
		};

		virtual ~ContentManager(){
			Unload();
		}

		static sptr<GameServiceContainer> Services() {
			return _services;
		}

		constexpr String RootDirectory() const {
			return _rootDirectory;
		}

		void RootDirectory(String const& value) {
			_rootDirectory = value;
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
				auto& ptr = _loadedAssets[assetName];
				auto obj1 = reinterpret_pointer_cast<T>(ptr);

				return obj1;
			}

			auto obj2 = ReadAsset<T>(assetName); 
			//auto voidAsset = reinterpret_pointer_cast<void>(obj2);
			_loadedAssets.insert({ assetName , obj2 });

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
		std::map<String, sptr<void>> _loadedAssets;
		inline const static String contentExtension = ".xnb";
		std::vector<Byte> byteBuffer;
		
		inline static sptr<GameServiceContainer> _services = nullptr;
	};
}

#endif