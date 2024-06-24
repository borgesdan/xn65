#include "xna/platform-dx/dx.hpp"
#include "xna/csharp/stream.hpp"

using DxSoundEffect = DirectX::SoundEffect;

namespace xna {
	SoundEffectInstance::SoundEffectInstance() {
		impl = unew<PlatformImplementation>();
	}

	SoundEffectInstance::~SoundEffectInstance() {
		impl = nullptr;
	}	

	SoundEffect::SoundEffect(String const& fileName) {
		if (!AudioEngine::impl || !AudioEngine::impl->_dxAudioEngine)
			return;

		const auto file = XnaHelper::ToWString(fileName);
		impl->_dxSoundEffect = unew<DxSoundEffect>(AudioEngine::impl->_dxAudioEngine.get(), file.c_str());
	}

	SoundEffect::~SoundEffect() {
		impl = nullptr;
	}

	SoundEffect::SoundEffect(
		std::vector<Byte> const& format,
		std::vector<Byte> const& data,
		Int loopStart,
		Int loopLength,
		//We must evaluate how to use the time duration
		TimeSpan const& duration) {
		if (!AudioEngine::impl || !AudioEngine::impl->_dxAudioEngine)
			return;			
		
		//We expect 'format' to always be 16 bytes
		MemoryStream stream(format);
		WORD word = 0;
		DWORD dword = 0;

		auto bWord = reinterpret_cast<Byte*>(&word);
		auto bDword = reinterpret_cast<Byte*>(&dword);

		stream.Read(bWord, 2, 0, 2);
		auto tag = word;
		stream.Read(bWord, 2, 0, 2);
		auto channels = word;
		stream.Read(bDword, 4, 0, 4);
		auto samplesPerSec = dword;
		stream.Read(bDword, 4, 0, 4);
		auto bytesPerSec = dword;
		stream.Read(bWord, 2, 0, 2);
		auto blockAlign = word;
		stream.Read(bWord, 2, 0, 2);
		auto bitsPerSample = word;
		stream.Read(bWord, 2, 0, 2);
		auto cbSize = word;

		auto wavData = unew<Byte[]>(data.size());
		for (size_t i = 0; i < data.size(); ++i)
			wavData[i] = data[i];

		auto wfx = reinterpret_cast<WAVEFORMATEX*>(wavData.get());
		wfx->wFormatTag = tag;
		wfx->nChannels = channels;
		wfx->nSamplesPerSec = samplesPerSec;
		wfx->nAvgBytesPerSec = bytesPerSec;
		wfx->nBlockAlign = blockAlign;
		wfx->wBitsPerSample = bitsPerSample;
		wfx->cbSize = cbSize;

		auto startAudio = wavData.get();
		
		auto se = unew<DxSoundEffect>(
			AudioEngine::impl->_dxAudioEngine.get(),
			wavData,
			wfx,
			startAudio,
			data.size(),
			loopStart,
			loopLength);

		impl = unew<PlatformImplementation>();
		impl->_dxSoundEffect = std::move(se);
	}

	void SoundEffect::Play() {
		if (!impl->_dxSoundEffect)
			return;
		
		impl->_dxSoundEffect->Play();		
	}

	void SoundEffect::Play(float volume, float pitch, float pan) {
		if (!impl->_dxSoundEffect)
			return;		
		
		impl->_dxSoundEffect->Play(volume, pitch, pan);		
	}

	uptr<SoundEffectInstance> SoundEffect::CreateInstance() {
		if (!impl->_dxSoundEffect)
			return unew<SoundEffectInstance>();

		auto instance = unew<SoundEffectInstance>();
		instance->impl->_dxInstance = impl->_dxSoundEffect->CreateInstance();

		return instance;
	}

	void SoundEffectInstance::Play(bool loop) {
		if (!impl->_dxInstance)
			return;

		impl->_dxInstance->Play(loop);
	}

	void SoundEffectInstance::Stop(bool immediate) {
		if (!impl->_dxInstance)
			return;

		impl->_dxInstance->Stop(immediate);
	}

	void SoundEffectInstance::Pause() {
		if (!impl->_dxInstance)
			return;

		impl->_dxInstance->Pause();
	}

	void SoundEffectInstance::Resume() {
		if (!impl->_dxInstance)
			return;

		impl->_dxInstance->Resume();
	}

	void SoundEffectInstance::Volume(float volume) {
		if (!impl->_dxInstance)
			return;

		impl->_dxInstance->SetVolume(volume);
	}

	void SoundEffectInstance::Pitch(float pitch) {
		if (!impl->_dxInstance)
			return;

		impl->_dxInstance->SetPitch(pitch);
	}

	void SoundEffectInstance::Pan(float pan) {
		if (!impl->_dxInstance)
			return;

		impl->_dxInstance->SetPan(pan);
	}

	bool SoundEffectInstance::IsLooped() {
		if (!impl->_dxInstance)
			return false;

		return impl->_dxInstance->IsLooped();
	}
}