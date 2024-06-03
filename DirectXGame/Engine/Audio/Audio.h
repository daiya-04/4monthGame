#pragma once
#include <xaudio2.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <wrl.h>

#include <fstream>
#include <vector>

class AudioManager;

class Audio{
private:

	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	friend class AudioManager;

public:

	enum class AudioType {
		BGM,
		SE,
	};

public:
	static void Init();
	static void DstoroyVoice();
	void Update();
	//音声データの解放
	void SoundUnload();
	//音声再生
	void Play();

	//ループ終了
	void SoundPlayLoopEnd();
	//音声ロード
	//size_t LoadWaveInternal(const std::string& filename);

	//再生停止
	void StopSound();
	//ピッチの設定
	void SetPitch(float pitch);
	//音量設定
	void SetVolume(float volume);
	//インスタンス毎の音量設定
	void SetUniqueVolume(float volume) { volume_ = volume; }
	//再生しているか
	bool IsValidPlayhandle();

	void DestroyPlayHandle();

	bool IsStop()const { return isStop_; }

public:

	static float bgmVolume_;
	static float seVolume_;

private:

	static ComPtr<IXAudio2> xAudio2_;
	static IXAudio2MasteringVoice* masterVoice_;
	//std::vector<SoundData> soundData_;
	IXAudio2SourceVoice* sourceVoices_ = nullptr;

	std::string filename_;
	//波形フォーマット
	WAVEFORMATEX wfex_;
	//バッファの先頭アドレス
	std::vector<BYTE> buffer_;
	//バッファのサイズ
	uint32_t bufferSize_;
	AudioType audioType_{};

	bool isStop_ = true;

	float volume_ = 1.0f;

};

