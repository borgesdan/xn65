#ifndef XNA_CONTENT_READERS_AUDIO_HPP
#define XNA_CONTENT_READERS_AUDIO_HPP

#include "content/manager.hpp"
#include "content/reader.hpp"
#include "csharp/type.hpp"
#include "audio/soundeffect.hpp"
#include "csharp/timespan.hpp"

namespace xna {
	class SoundEffectReader : public ContentTypeReaderT<PSoundEffect> {
	public:
		SoundEffectReader() : ContentTypeReaderT(typeof<PSoundEffect>()) {
			ContentTypeReader::TargetIsValueType = false;
		}

		PSoundEffect Read(ContentReader& input, PSoundEffect& existingInstance) override {
			const auto count1 = input.ReadInt32();
			auto format = input.ReadBytes(count1);
			auto count2 = input.ReadInt32();
			auto data = input.ReadBytes(count2);
			auto loopStart = input.ReadInt32();
			auto loopLength = input.ReadInt32();
			auto num = input.ReadInt32();

			auto sf = snew<SoundEffect>(format, data, loopStart, loopLength, TimeSpan::FromMilliseconds((double)num));
			return sf;
		}
	};
}

#endif