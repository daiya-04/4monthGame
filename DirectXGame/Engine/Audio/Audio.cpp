#include "Audio.h"
#include "Log.h"
#include <cassert>

#include "WinApp.h"

#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"Mf.lib")
#pragma comment(lib,"mfplat.lib")
#pragma comment(lib,"Mfreadwrite.lib")
#pragma comment(lib,"mfuuid.lib")

using namespace Microsoft::WRL;

ComPtr<IXAudio2> Audio::xAudio2_;
IXAudio2MasteringVoice* Audio::masterVoice_ = nullptr;
float Audio::bgmVolume_ = 0.5f;
float Audio::seVolume_ = 0.5;

//Audio* Audio::GetInstance() {
//	static Audio instance;
//
//	return &instance;
//}

//Audio::~Audio() {
//	for (size_t index = 0; index < kMaxNumPlayHandles; ++index) {
//		DestroyPlayHandle(index);
//	}
//
//	if (masterVoice_) {
//		masterVoice_->DestroyVoice();
//		masterVoice_ = nullptr;
//	}
//
//	MFShutdown();
//}

//size_t Audio::LoadWave(const std::string& fileName) {
//	return Audio::GetInstance()->LoadWaveInternal(fileName);
//}

//size_t Audio::Load(const std::string& filename) {
//	return Audio::GetInstance()->LoadInternal(filename);
//}

void Audio::Init() {
	HRESULT hr;
	//XAudioエンジンのインスタンスを作成
	hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(hr));
	//マスターボイスを作成
	hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(hr));
}

void Audio::DstoroyVoice() {
	if (masterVoice_) {
		masterVoice_->DestroyVoice();
		masterVoice_ = nullptr;
	}
}

void Audio::Update() {
	if (sourceVoices_) {

		float sourceVolume = volume_;

		if (audioType_ == AudioType::BGM) {
			sourceVolume *= bgmVolume_;
			SetVolume(sourceVolume);
		}
		else if (audioType_ == AudioType::SE) {
			sourceVolume *= seVolume_;
			SetVolume(sourceVolume);
		}
		XAUDIO2_VOICE_STATE state{};
		sourceVoices_->GetState(&state);
		if (state.BuffersQueued == 0) {
			DestroyPlayHandle();
			isStop_ = true;
		}
		
	}
}

void Audio::Play() {

	if (IsValidPlayhandle() && audioType_ == Audio::AudioType::BGM) {
		return;
	}

	if (IsValidPlayhandle() && audioType_ == AudioType::SE) {
		StopSound();
	}

	float sourceVolume = volume_;

	HRESULT hr;

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = buffer_.data();
	buf.AudioBytes = bufferSize_;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (audioType_ == AudioType::BGM) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	//sourceVoiceの作成
	IXAudio2SourceVoice* pSourcVoice = nullptr;
	hr = xAudio2_->CreateSourceVoice(&pSourcVoice, &wfex_);
	assert(SUCCEEDED(hr));

	hr = pSourcVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(hr));

	hr = pSourcVoice->Start();
	assert(SUCCEEDED(hr));

	sourceVoices_ = pSourcVoice;
	isStop_ = false;

	if (audioType_ == AudioType::BGM) {
		sourceVolume *= bgmVolume_;
		SetVolume(sourceVolume);
	}
	else if (audioType_ == AudioType::SE) {
		sourceVolume *= seVolume_;
		SetVolume(sourceVolume);
	}
	
}

void Audio::SoundPlayLoopEnd() {
	if (IsValidPlayhandle()) {
		sourceVoices_->Stop();
		DestroyPlayHandle();
	}
}

//size_t Audio::LoadInternal(const std::string& filename) {
//	//読み込み済みのファイルの検索
//	auto iter = std::find_if(soundData_.begin(), soundData_.end(), [&](const SoundData& soundData) {
//		return soundData.filename == filename;
//		});
//	if (iter != soundData_.end()) {
//		return std::distance(soundData_.begin(), iter);
//	}
//
//	//SourceReaderの作成
//	ComPtr<IMFSourceReader> MFSourceReader;
//	std::string directoryPath = "Resources/sounds/";
//	MFCreateSourceReaderFromURL(ConvertString(directoryPath + filename).c_str(), NULL, &MFSourceReader);
//
//	//メディアタイプの取得
//	ComPtr<IMFMediaType> MFMediaType;
//	MFCreateMediaType(&MFMediaType);
//	MFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
//	MFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
//	MFSourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, MFMediaType.Get());
//
//	//MFMediaType->Release();
//	MFMediaType = nullptr;
//	MFSourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &MFMediaType);
//
//
//	//オーディオデータ形式の読み込み
//	WAVEFORMATEX* waveFormat{ nullptr };
//	MFCreateWaveFormatExFromMFMediaType(MFMediaType.Get(), &waveFormat, nullptr);
//
//	//データの読み込み
//	std::vector<BYTE> mediaData;
//	while (true) {
//
//		ComPtr<IMFSample> MFSample;
//		DWORD dwStreamFlags{ 0 };
//		MFSourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwStreamFlags, nullptr, &MFSample);
//
//		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM) { break; }
//
//		ComPtr<IMFMediaBuffer> MFMediaBuffer;
//		MFSample->ConvertToContiguousBuffer(&MFMediaBuffer);
//		
//		BYTE* buffer{ nullptr };
//		DWORD cbCurrentLength{ 0 };
//		MFMediaBuffer->Lock(&buffer, nullptr, &cbCurrentLength);
//
//		mediaData.resize(mediaData.size() + cbCurrentLength);
//		memcpy(mediaData.data() + mediaData.size() - cbCurrentLength, buffer, cbCurrentLength);
//
//		MFMediaBuffer->Unlock();
//	}
//
//	//returnする為の音声データ
//	SoundData soundData{};
//	soundData.filename = filename;
//	soundData.wfex = *waveFormat;
//	soundData.bufferSize = sizeof(BYTE) * static_cast<uint32_t>(mediaData.size());
//	soundData.pBuffer = std::move(mediaData);
//	soundData_.emplace_back(soundData);
//
//	return soundData_.size() - 1;
//}

//size_t Audio::LoadWaveInternal(const std::string& filename) {
//	auto iter = std::find_if(soundData_.begin(), soundData_.end(), [&](const SoundData& soundData) {
//		return soundData.filename == filename;
//		});
//	if (iter != soundData_.end()) {
//		return std::distance(soundData_.begin(), iter);
//	}
//
//#pragma region ファイルオープン
//	//directoryPath
//	std::string directoryPath = "Resources/sounds/";
//	//ファイル出力ストリームのインスタンス
//	std::ifstream file;
//	//wavファイルをバイナリモードで開く
//	file.open(directoryPath + filename, std::ios_base::binary);
//	assert(file.is_open());
//
//#pragma endregion
//#pragma region wavデータの読み込み
//	//RIFFヘッダーの読み込み
//	RiffHeader riff;
//	file.read((char*)&riff, sizeof(riff));
//	//ファイルがRiffかチェック
//	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
//		assert(0);
//	}
//	//タイプがWAVEかチェック
//	if (strncmp(riff.type, "WAVE", 4) != 0) {
//		assert(0);
//	}
//	//Formatチャンクの読み込み
//	FormatChunk format{};
//	//チャンクヘッダーの確認
//	file.read((char*)&format, sizeof(ChunkHeader));
//	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
//		assert(0);
//	}
//	//チャンク本体の読み込み
//	assert(format.chunk.size <= sizeof(format.fmt));
//	file.read((char*)&format.fmt, format.chunk.size);
//	//Dataチャンクの読み込み
//	ChunkHeader data;
//	file.read((char*)&data, sizeof(data));
//	//JUNKチャンクを検出した場合
//	if (strncmp(data.id, "JUNK", 4) == 0) {
//		//読み取りチャンクを検出した場合	
//		file.seekg(data.size, std::ios_base::cur);
//		//再読み込み
//		file.read((char*)&data, sizeof(data));
//	}
//	//LISTチャンクを検出した場合
//	if (strncmp(data.id, "LIST", 4) == 0) {
//		//読み取りチャンクを検出した場合	
//		file.seekg(data.size, std::ios_base::cur);
//		//再読み込み
//		file.read((char*)&data, sizeof(data));
//	}
//	// bextチャンクを検出した場合
//	if (strncmp(data.id, "bext", 4) == 0) {
//		// 読み取りチャンクを検出した場合
//		file.seekg(data.size, std::ios_base::cur);
//		// 再読み込み
//		file.read((char*)&data, sizeof(data));
//	}
//	//INFOチャンクを検出した場合
//	if (strncmp(data.id, "INFO", 4) == 0) {
//		// 読み取りチャンクを検出した場合
//		file.seekg(data.size, std::ios_base::cur);
//		// 再読み込み
//		file.read((char*)&data, sizeof(data));
//	}
//	//REAPERチャンクを検出した場合
//	if (strncmp(data.id, "REAPER", 6) == 0) {
//		// 読み取りチャンクを検出した場合
//		file.seekg(data.size, std::ios_base::cur);
//		// 再読み込み
//		file.read((char*)&data, sizeof(data));
//	}
//	if (strncmp(data.id, "junk", 4) == 0) {
//		// 読み取りチャンクを検出した場合
//		file.seekg(data.size, std::ios_base::cur);
//		// 再読み込み
//		file.read((char*)&data, sizeof(data));
//	}
//	if (strncmp(data.id, "data", 4) != 0) {
//		assert(0);
//	}
//	//Dataチャンクのデータ部(波形データ)の読み込み
//	std::vector<BYTE> pBuffer(data.size);
//	file.read(reinterpret_cast<char*>(pBuffer.data()), data.size);
//
//	//Waveファイルを閉じる
//	file.close();
//#pragma endregion
//#pragma region 読み込んだ音声データのreturn
//	//returnする為の音声データ
//	SoundData soundData{};
//	soundData.filename = filename;
//	soundData.wfex = format.fmt;
//	soundData.pBuffer = std::move(pBuffer);
//	soundData.bufferSize = data.size;
//#pragma endregion
//	soundData_.emplace_back(soundData);
//
//	return soundData_.size() - 1;
//}

void Audio::StopSound() {
	if (sourceVoices_) {
		sourceVoices_->Stop();
		sourceVoices_->FlushSourceBuffers();
		DestroyPlayHandle();
		isStop_ = true;
	}
}

void Audio::SetPitch(float pitch) {
	sourceVoices_->SetFrequencyRatio(pitch);
}

void Audio::SetVolume(float volume) {
	sourceVoices_->SetVolume(volume);
}

bool Audio::IsValidPlayhandle() {
	return sourceVoices_ != nullptr;
}

void Audio::DestroyPlayHandle() {
	if (sourceVoices_) {
		sourceVoices_->DestroyVoice();
		sourceVoices_ = nullptr;
	}
}

void Audio::SoundUnload() {
	//バッファのメモリの解放
	buffer_.clear();
	bufferSize_ = 0;
	wfex_ = {};
}