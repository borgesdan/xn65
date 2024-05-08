#include "init-dx.hpp"
#include "../csharp/type.hpp"
#include "texture-dx.hpp"
#include "content-readers/texture2Dreader-dx.hpp"
#include "../content/typereadermanager.hpp"

namespace xna {
	void InitPlatform::InitRegisteredTypes()
	{
		Type::NameOfRegisteredTypes.insert({ "Texture2D", typeof<Texture2D>() });

		//Texture2DReader
		const auto textureReader = typeof<Texture2DReader>();
		Type::NameOfRegisteredTypes.insert({ "xna::Texture2DReader", textureReader });
		Type::NameOfRegisteredTypes.insert({ "Microsoft.Xna.Framework.Content.Texture2DReader", textureReader });		
	}

	void InitPlatform::InitActivadors()
	{
		ContentTypeReaderActivador::SetActivador(typeof<ObjectReader>(), []() -> sptr<ContentTypeReader> {
			auto obj = New <ObjectReader>();
			return reinterpret_pointer_cast<ContentTypeReader>(obj);
			});

		ContentTypeReaderActivador::SetActivador(typeof<Texture2DReader>(), []() -> sptr<ContentTypeReader> {
			auto obj = New <Texture2DReader>();
			return reinterpret_pointer_cast<ContentTypeReader>(obj);
			});		
	}
}