#ifndef XNA_CONTENT_MANAGER_HPP
#define XNA_CONTENT_MANAGER_HPP

#include "../csharp/stream.hpp"
#include "../default.hpp"
#include "../game/servicecontainer.hpp"
#include "reader.hpp"
#include <algorithm>
#include <filesystem>
#include <map>

namespace xna {
	class ContentManager {
	public:
		friend class ContentReader;

		ContentManager(String const& rootDirectory, sptr<GameServiceContainer> const& services) : 
			_rootDirectory(rootDirectory){
			_services = services;
		};		

		static sptr<GameServiceContainer> Services() {
			return _services;
		}

		constexpr String RootDirectory() const {
			return _rootDirectory;
		}

		void RootDirectory(String const& value) {
			_rootDirectory = value;
		}

		template <typename T>
		T Load(String const& assetName) {
			if (assetName.empty()) return T();			

			auto obj2 = ReadAsset<T>(assetName); 

			return obj2;
		}		

	protected:
		template <typename T>
		T ReadAsset(String const& assetName) {
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
		std::vector<Byte> byteBuffer;
		
		inline const static String contentExtension = ".xnb";
		inline static sptr<GameServiceContainer> _services = nullptr;
	};
}

#endif