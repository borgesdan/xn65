#ifndef XNA_PLATFORM_SOUNDENGINE_DX_HPP
#define XNA_PLATFORM_SOUNDENGINE_DX_HPP

#include "../sound/audioengine.hpp"


namespace xna {
	class AudioEngine : public IAudioEngine {
	public:
		AudioEngine() {
			
		}

	public:
		//static inline uptr<DirectX::AudioEngine> _dxAudioEngine = uNew<DirectX::AudioEngine>();
	};
}

#endif