#ifndef XNA_DX_INIT_HPP
#define XNA_DX_INIT_HPP

#include "headers.hpp"
#include "csharp/type.hpp"

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
		static void insertRegisteredReader() {
			const auto reader = csharp::typeof<T>();
			csharp::RuntimeType::Add(reader.FullName(), reader);

		}

		template <typename T>
		static void insertRegisteredReader(String const& microsoftNameFullName) {
			const auto reader = csharp::typeof<T>();
			csharp::RuntimeType::Add(reader.FullName(), reader);
			csharp::RuntimeType::Add(microsoftNameFullName, reader);			
		}

		template <typename T>
		static void insertActivadorReader() {
			ContentTypeReaderActivador::SetActivador(std::make_shared<csharp::Type>(csharp::typeof<T>()), []() -> sptr<ContentTypeReader> {
				auto obj = snew<T>();
				return reinterpret_pointer_cast<ContentTypeReader>(obj);
				});
		}
	};
}

#endif