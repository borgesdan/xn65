#ifndef XNA_CONTENT_MANAGER_HPP
#define XNA_CONTENT_MANAGER_HPP

#include "../csharp/service.hpp"
#include "../csharp/stream.hpp"
#include "../default.hpp"
#include "reader.hpp"
#include <map>

namespace xna {
	//The run-time component which loads managed objects from the binary files produced by the design time content pipeline.
	class ContentManager : public std::enable_shared_from_this<ContentManager> {
	public:
		ContentManager(sptr<IServiceProvider> const& services) :
			_rootDirectory("") {
			_services = services;
		};

		ContentManager(sptr<IServiceProvider> const& services, String const& rootDirectory) :
			_rootDirectory(rootDirectory){
			_services = services;
		};		

		//Gets the service provider associated with the ContentManager.
		sptr<IServiceProvider> ServiceProvider() const {
			return _services;
		}

		//Gets or sets the root directory associated with this ContentManager.
		constexpr String RootDirectory() const {
			return _rootDirectory;
		}

		//Gets or sets the root directory associated with this ContentManager.
		void RootDirectory(String const& value) {
			_rootDirectory = value;
		}

		//Loads an asset that has been processed by the Content Pipeline.
		template <typename T>
		auto Load(String const& assetName) {
			if (assetName.empty()) {
				return XnaHelper::ReturnDefaultOrNull<T>();
			}
			
			if constexpr (XnaHelper::is_shared_ptr<T>::value) {				

				if (_loadedAssets.contains(assetName)) {
					auto& voidAsset = _loadedAssets[assetName];					
					using TYPE = T::element_type;
					auto asset = reinterpret_pointer_cast<TYPE>(voidAsset);
					return asset;
				}
			}

			const auto obj2 = ReadAsset<T>(assetName); 

			if constexpr (XnaHelper::is_shared_ptr<T>::value) {

				if(obj2)
					_loadedAssets.emplace( assetName, obj2 );
			}

			return obj2;
		}		

		//Disposes all data that was loaded by this ContentManager.
		void Unload() {
			_loadedAssets.clear();
		}

		//Gets the service provider associated with the main Game.
		static sptr<IServiceProvider> GameServiceProvider() {
			return _gameServices;
		}

	protected:
		template <typename T>
		auto ReadAsset(String const& assetName) {
			auto input = OpenStream(assetName);

			if (!input)
				return XnaHelper::ReturnDefaultOrNull<T>();

			const auto _this = shared_from_this();
			auto contentReader = ContentReader::Create(_this, input, assetName);

			auto asset = contentReader->ReadAsset<T>();
			return asset;
		}

		sptr<Stream> OpenStream(String const& assetName) const;

	private:
		friend class ContentReader;
		friend class Game;

		String _rootDirectory;				
		sptr<IServiceProvider> _services = nullptr;
		std::map<String, sptr<void>> _loadedAssets;
		
		inline static sptr<IServiceProvider> _gameServices = nullptr;		
		inline const static String contentExtension = ".xnb";
	};
}

#endif