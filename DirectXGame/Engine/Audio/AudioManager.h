#pragma once
#include <xaudio2.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <wrl.h>
#include <vector>
#include <string>
#include <memory>

#include "Audio.h"


class AudioManager {
private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	static const size_t kMaxNumPlayHandles = 256;

public:

	static AudioManager* GetInstance();

	static Audio* Load(const std::string& filename);

private:

	Audio* LoadInternal(const std::string& filename);

public:

	void Init();

	void Update();

	void AllAudioStop();

	bool IsAllAudioStop();

private:

	std::vector<std::unique_ptr<Audio>> audios_;

private:

	AudioManager() = default;
	~AudioManager();
	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

};

