#ifndef XNA_SOUND_SOUNDENGINE_HPP
#define XNA_SOUND_SOUNDENGINE_HPP

#include "../default.hpp"
//#include <Audio.h>

namespace xna {
	class IAudioEngine {
	public:
		virtual ~IAudioEngine(){}

	public:
		//uptr<DirectX::AudioEngine> _dxAudioEngine = nullptr;
	};
}

#endif