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
	static void Init();
	static void DstoroyVoice();
	void Update();
	//音声データの解放
	void SoundUnload();
	//音声再生
	void Play(float volume = 1.0f,bool loop = false);

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
	//再生しているか
	bool IsValidPlayhandle();

	void DestroyPlayHandle();

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

};

