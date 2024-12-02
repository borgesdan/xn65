#ifndef XNA_CONTENT_MANAGER_HPP
#define XNA_CONTENT_MANAGER_HPP

#include "../csharp/service.hpp"
#include "../csharp/stream.hpp"
#include "../default.hpp"
#include "reader.hpp"

namespace xna {
	//The run-time component which loads managed objects from the binary files produced by the design time content pipeline.
	class ContentManager : public std::enable_shared_from_this<ContentManager> {
	public:
		ContentManager(sptr<IServiceProvider> const& services) :
			rootDirectory("") {
			serviceProvider = services;
		};

		ContentManager(sptr<IServiceProvider> const& services, String const& rootDirectory) :
			rootDirectory(rootDirectory){
			serviceProvider = services;
		};		

		//Gets the service provider associated with the ContentManager.
		sptr<IServiceProvider> ServiceProvider() const {
			return serviceProvider;
		}

		//Gets or sets the root directory associated with this ContentManager.
		constexpr String RootDirectory() const {
			return rootDirectory;
		}

		//Gets or sets the root directory associated with this ContentManager.
		void RootDirectory(String const& value) {
			rootDirectory = value;
		}

		//Loads an asset that has been processed by the Content Pipeline.
		template <typename T>
		auto Load(String const& assetName) {
			if (assetName.empty()) {
				return misc::ReturnDefaultOrNull<T>();
			}
			
			if constexpr (misc::is_shared_ptr<T>::value) {

				if (loadedAssets.contains(assetName)) {
					auto& voidAsset = loadedAssets[assetName];
					using TYPE = T::element_type;
					auto asset = reinterpret_pointer_cast<TYPE>(voidAsset);
					return asset;
				}
			}

			const auto obj2 = ReadAsset<T>(assetName); 

			if constexpr (misc::is_shared_ptr<T>::value) {

				if(obj2)
					loadedAssets.emplace( assetName, obj2 );
			}

			return obj2;
		}		

		//Disposes all data that was loaded by this ContentManager.
		void Unload() {
			loadedAssets.clear();
		}

		//Gets the service provider associated with the main Game.
		static sptr<IServiceProvider> GameServiceProvider() {
			return mainGameService;
		}

	protected:
		template <typename T>
		auto ReadAsset(String const& assetName) {
			auto input = OpenStream(assetName);

			if (!input)
				return misc::ReturnDefaultOrNull<T>();

			const auto _this = shared_from_this();
			auto contentReader = ContentReader::Create(_this, input, assetName);

			auto asset = contentReader->ReadAsset<T>();
			return asset;
		}

		sptr<Stream> OpenStream(String const& assetName) const;

	private:
		friend class ContentReader;
		friend class Game;

		String rootDirectory;				
		sptr<IServiceProvider> serviceProvider = nullptr;
		std::map<String, sptr<void>> loadedAssets;
		
		inline static sptr<IServiceProvider> mainGameService = nullptr;		
		inline const static String contentExtension = ".xnb";
	};
}

#endif