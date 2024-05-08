#ifndef XNA_CONTENT_DEFAULTREADERS_ARRAY_HPP
#define XNA_CONTENT_DEFAULTREADERS_ARRAY_HPP

#include "reader.hpp"
#include "../default.hpp"

namespace xna {
	template <typename T>
	class ArrayReader : public ContentTypeReaderT<std::vector<T>> {
	public:
		void Initialize(sptr<ContentTypeReaderManager> const& manager) {
			elementReader = manager->GetTypeReader(typeof<T>());
		}

		std::vector<T> Read(ContentReader& input, std::vector<T>& existingInstance) override {
			const auto length = input.ReadInt32();
			std::vector<T> objArray(length);

			for (size_t index = 0; index < length; ++index)
				objArray[index] = input.ReadObject<T>(elementReader);

			return objArray;
		}

	private:
		sptr<ContentTypeReaderManager> elementReader = nullptr;
	};

	class BooleanReader : public ContentTypeReaderT<bool> {
		bool Read(ContentReader& input, bool& existingInstance) override {
			auto value = input.ReadBoolean();
			auto b = New<bool>(value);
			return b;
		}
	};
}

#endif