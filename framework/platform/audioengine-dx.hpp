#ifndef XNA_PLATFORM_SOUNDENGINE_DX_HPP
#define XNA_PLATFORM_SOUNDENGINE_DX_HPP

#include "../audio/audioengine.hpp"
#include <Audio.h>

namespace xna {
	struct WaveFormat {
		Ushort FormatTag{ 0 };
		Ushort Channels{ 0 };
		Ulong SamplesPerSecond{ 0 };
		Ulong Buffer{ 0 };
		Ushort BlockAlign{ 0 };
		Ushort BitsPerSamples{ 0 };
		Ushort ByteSize{ 0 };
	};

	class AudioEngine : public IAudioEngine {
	public:
		AudioEngine() {
			_dxAudioEngine = New<DirectX::AudioEngine>(
#ifdef _DEBUG
				DirectX::AudioEngine_Debug
#endif
			);			
		}		

		virtual ~AudioEngine() {
			if (_dxAudioEngine) {
				_dxAudioEngine->Suspend();
			}
		}

		virtual bool Reset() override {
			if (!_dxAudioEngine)
				return false;

			return _dxAudioEngine->Reset();
		}

		virtual bool Update() override {
			if (!_dxAudioEngine)
				return false;

			return _dxAudioEngine->Update();
		}

		virtual bool Resume() override {
			if (!_dxAudioEngine)
				return false;

			_dxAudioEngine->Resume();			

			return true;
		}

		virtual bool Suspend() override {
			if (!_dxAudioEngine)
				return false;

			_dxAudioEngine->Resume();			

			return true;
		}

		virtual void DefaultSampleRate(int value) override {
			if (!_dxAudioEngine) return;

			_dxAudioEngine->SetDefaultSampleRate(value);
		}

		virtual void MasteringLimit(int limit, int loudness) override {
			if (!_dxAudioEngine) return;

			_dxAudioEngine->SetMasteringLimit(limit, loudness);
		}

		virtual void MasterVolume(float value) override {
			if (!_dxAudioEngine) return;

			_dxAudioEngine->SetMasterVolume(value);			
		}

		virtual void Reverb(AudioReverb value) override {
			if (!_dxAudioEngine) return;

			const auto reverb = static_cast<DirectX::AUDIO_ENGINE_REVERB>(value);
			_dxAudioEngine->SetReverb(reverb);
		}

	public:
		sptr<DirectX::AudioEngine> _dxAudioEngine = nullptr;
	};
}

#endif