#ifndef XNA_PIPELINE_IMPORTER_HPP
#define XNA_PIPELINE_IMPORTER_HPP

#include <any>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include "logger.hpp"

namespace xna {
	//Provides properties that define logging behavior for the importer.
	struct ContentImporterContext {
		//Gets the logger for an importer.
		virtual std::shared_ptr<ContentBuilderLogger> Logger() = 0;
		//Name of an asset file.
		virtual void AddDependency(std::string filename) = 0;
		//The absolute path to the root of the build output (binaries) directory.
		virtual std::string OutputDirectory() = 0;
		//The absolute path to the root of the build intermediate (object) directory.
		virtual std::string IntermediateDirectory() = 0;
	};

	//Accesses a statically typed ContentImporter instance from generic code using dynamic typing.
	struct IContentImporter {
		//Imports an asset from the specified file.
		virtual std::any Import(std::string const& filename, ContentImporterContext& context) = 0;
	};

	//Implements a file format importer for use with game assets.
	template <typename T>
	struct ContentImporter_T : public IContentImporter {
		//Imports an asset from the specified file.
		virtual T Import(std::string const& filename, ContentImporterContext& context) = 0;

		//Imports an asset from the specified file.
		std::any Import(std::string const& filename, ContentImporterContext& context) override {
			std::any obj = Import<T>(filename, context);
			return obj;
		}
	};

	//rovides properties that identify and provide metadata about the importer, such as supported file extensions and caching information.
	struct ContentImporterAttribute {
		bool CacheImportedData;
		std::string DisplayName;
		std::string DefaultProcessor;

		ContentImporterAttribute(std::string const& fileExtension) {
			fileExtensions.push_back(fileExtension);
		}

		ContentImporterAttribute(std::vector<std::string> const& fileExtensions) {
			this->fileExtensions = fileExtensions;
			//TODO: check extensions
		}

		const std::vector<std::string>& FileExtensions() const {
			return fileExtensions;
		}

	private:
		std::vector<std::string> fileExtensions;
	};
}

#endif