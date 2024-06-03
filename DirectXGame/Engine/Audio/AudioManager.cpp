#include "AudioManager.h"
#include "Log.h"
#include <cassert>

#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"Mf.lib")
#pragma comment(lib,"mfplat.lib")
#pragma comment(lib,"Mfreadwrite.lib")
#pragma comment(lib,"mfuuid.lib")

using namespace Microsoft::WRL;

AudioManager* AudioManager::GetInstance() {
	static AudioManager instance;

	return &instance;
}

AudioManager::~AudioManager() {

	for (const auto& audio : audios_) {
		audio->DestroyPlayHandle();
		audio->SoundUnload();
	}

	audios_.clear();

	Audio::DstoroyVoice();

	MFShutdown();

}

Audio* AudioManager::Load(const std::string& filename) {
	return AudioManager::GetInstance()->LoadInternal(filename);
}
void AudioManager::Init() {
	Audio::Init();
	MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
}

void AudioManager::Update() {

	for (const auto& audio : audios_) {
		audio->Update();
	}

}

void AudioManager::AllAudioStop() {
	for (const auto& audio : audios_) {
		audio->StopSound();
	}
}

bool AudioManager::IsAllAudioStop() {
	for (const auto& audio : audios_) {
		if (!audio->IsStop()) {
			return false;
		}
	}
	
	return true;
}

Audio* AudioManager::LoadInternal(const std::string& filename) {

	//読み込み済みのファイルの検索
	for (const auto& audio : audios_) {
		if (audio->filename_ == filename) {
			return audio.get();
		}
	}
	
	std::string audioTypeName;
	for (const char name : filename) {
		if (name == '/') {
			break;
		}
		audioTypeName += name;
	}

	//SourceReaderの作成
	ComPtr<IMFSourceReader> MFSourceReader;
	std::string directoryPath = "Resources/sounds/";
	MFCreateSourceReaderFromURL(ConvertString(directoryPath + filename).c_str(), NULL, &MFSourceReader);

	//メディアタイプの取得
	ComPtr<IMFMediaType> MFMediaType;
	MFCreateMediaType(&MFMediaType);
	MFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	MFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	MFSourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, MFMediaType.Get());

	//MFMediaType->Release();
	MFMediaType = nullptr;
	MFSourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &MFMediaType);


	//オーディオデータ形式の読み込み
	WAVEFORMATEX* waveFormat{ nullptr };
	MFCreateWaveFormatExFromMFMediaType(MFMediaType.Get(), &waveFormat, nullptr);

	//データの読み込み
	std::vector<BYTE> mediaData;
	while (true) {

		ComPtr<IMFSample> MFSample;
		DWORD dwStreamFlags{ 0 };
		MFSourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwStreamFlags, nullptr, &MFSample);

		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM) { break; }

		ComPtr<IMFMediaBuffer> MFMediaBuffer;
		MFSample->ConvertToContiguousBuffer(&MFMediaBuffer);

		BYTE* buffer{ nullptr };
		DWORD cbCurrentLength{ 0 };
		MFMediaBuffer->Lock(&buffer, nullptr, &cbCurrentLength);

		mediaData.resize(mediaData.size() + cbCurrentLength);
		memcpy(mediaData.data() + mediaData.size() - cbCurrentLength, buffer, cbCurrentLength);

		MFMediaBuffer->Unlock();
	}

	//returnする為の音声データ
	Audio audio;
	audio.wfex_ = *waveFormat;
	audio.bufferSize_ = sizeof(BYTE) * static_cast<uint32_t>(mediaData.size());
	audio.buffer_ = std::move(mediaData);
	audio.filename_ = filename;

	if (audioTypeName == "SE") { audio.audioType_ = Audio::AudioType::SE; }
	else if (audioTypeName == "BGM") { audio.audioType_ = Audio::AudioType::BGM; }

	audios_.emplace_back(std::make_unique<Audio>(std::move(audio)));

	return audios_.back().get();
}
