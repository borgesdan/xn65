#ifndef XNA_CONTENT_READERS_AUDIO_HPP
#define XNA_CONTENT_READERS_AUDIO_HPP

#include "../../audio/soundeffect.hpp"
#include "csharp/time.hpp"
#include "../../csharp/type.hpp"
#include "../manager.hpp"
#include "../reader.hpp"

namespace xna {
	class SoundEffectReader : public ContentTypeReaderT<PSoundEffect> {
	public:
		SoundEffectReader() : ContentTypeReaderT(typeof<PSoundEffect>()) {
			ContentTypeReader::TargetIsValueType = false;
		}

		PSoundEffect Read(ContentReader& input, PSoundEffect& existingInstance) override {
			const auto count1 = input.ReadInt32();
			const auto format = input.ReadBytes(count1);
			const auto count2 = input.ReadInt32();
			const auto data = input.ReadBytes(count2);
			const auto loopStart = input.ReadInt32();
			const auto loopLength = input.ReadInt32();
			const auto num = input.ReadInt32();

			auto sf = snew<SoundEffect>(format, data, loopStart, loopLength, csharp::TimeSpan::FromMilliseconds((double)num));
			return sf;
		}
	};
}

#endif