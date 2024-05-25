#ifndef XNA_PLATFORM_INIT_HPP
#define XNA_PLATFORM_INIT_HPP

#include "../default.hpp"
#include "../csharp/type.hpp"
#include "../content/typereadermanager.hpp"
#include "../platforminit.hpp"

namespace xna {
	struct PlatformInit {
		static void Init() {
			InitRegisteredTypes();
			InitActivadors();
		}

		static void InitRegisteredTypes();
		static void InitActivadors();

	private:
		template <typename T>
		static void insertRegisteredReader(String const& readerName) {
			const auto reader = typeof<T>();
			Type::NameOfRegisteredTypes.insert({ "xna::" + readerName, reader });
			Type::NameOfRegisteredTypes.insert({ "Microsoft.Xna.Framework.Content." + readerName, reader });
		}

		template <typename T>
		static void insertActivadorReader() {
			ContentTypeReaderActivador::SetActivador(typeof<T>(), []() -> sptr<ContentTypeReader> {
				auto obj = New <T>();
				return reinterpret_pointer_cast<ContentTypeReader>(obj);
				});
		}
	};		
}

#endif