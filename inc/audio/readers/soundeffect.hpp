#ifndef XNA_AUDIO_READERS_SOUNDEFFECT_HPP
#define XNA_AUDIO_READERS_SOUNDEFFECT_HPP

#include "content/manager.hpp"
#include "content/reader.hpp"
#include "csharp/type.hpp"
#include "audio/soundeffect.hpp"

namespace xna {
	class SoundEffectReader : public ContentTypeReaderT<PSoundEffect> {
	public:
		SoundEffectReader() : ContentTypeReaderT(typeof<SoundEffect>()) {}

		PSoundEffect Read(ContentReader& input, PSoundEffect& existingInstance) override {
			return nullptr;
		}
	};
}

#endif