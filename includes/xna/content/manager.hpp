#ifndef XNA_CONTENT_MANAGER_HPP
#define XNA_CONTENT_MANAGER_HPP

#include "../csharp/service.hpp"
#include "csharp/io/stream.hpp"
#include "../default.hpp"
#include "reader.hpp"

namespace xna {
	//The run-time component which loads managed objects from the binary files produced by the design time content pipeline.
	class ContentManager : public std::enable_shared_from_this<ContentManager> {
	public:
		ContentManager(std::shared_ptr<IServiceProvider> const& services) :
			rootDirectory("") {
			serviceProvider = services;
		};

		ContentManager(std::shared_ptr<IServiceProvider> const& services, std::string const& rootDirectory) :
			rootDirectory(rootDirectory){
			serviceProvider = services;
		};		

		//Gets the service provider associated with the ContentManager.
		std::shared_ptr<IServiceProvider> ServiceProvider() const {
			return serviceProvider;
		}

		//Gets or sets the root directory associated with this ContentManager.
		constexpr std::string RootDirectory() const {
			return rootDirectory;
		}

		//Gets or sets the root directory associated with this ContentManager.
		void RootDirectory(std::string const& value) {
			rootDirectory = value;
		}

		//Loads an asset that has been processed by the Content Pipeline.
		template <typename T>
		auto Load(std::string const& assetName) {
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
		static std::shared_ptr<IServiceProvider> GameServiceProvider() {
			return mainGameService;
		}

	protected:
		template <typename T>
		auto ReadAsset(std::string const& assetName) {
			auto input = OpenStream(assetName);

			if (!input)
				return misc::ReturnDefaultOrNull<T>();

			const auto _this = shared_from_this();
			auto contentReader = ContentReader::Create(_this, input, assetName);

			auto asset = contentReader->ReadAsset<T>();
			return asset;
		}

		std::shared_ptr<csharp::Stream> OpenStream(std::string const& assetName) const;

	private:
		friend class ContentReader;
		friend class Game;

		std::string rootDirectory;				
		std::shared_ptr<IServiceProvider> serviceProvider = nullptr;
		std::map<std::string, std::shared_ptr<void>> loadedAssets;
		
		inline static std::shared_ptr<IServiceProvider> mainGameService = nullptr;		
		inline const static std::string contentExtension = ".xnb";
	};
}

#endif