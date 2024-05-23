#pragma once
#include "Vec2.h"
#include "Sprite.h"
#include <memory>
#include <optional>

class Option{
public:

	Option();

	void Init();
	void Update();
	void Draw();

	bool IsWindow() const { return isWindow_; }

private:

	bool isWindow_ = false;
	bool isFullScreen_;

	uint32_t windowTex_ = 0;
	std::unique_ptr<Sprite> window_;

	uint32_t gaugeTex_ = 0;
	std::unique_ptr<Sprite> bgmGauge_;
	std::unique_ptr<Sprite> seGauge_;

	uint32_t gaugeFrameTex_ = 0;
	std::unique_ptr<Sprite> gaugeFrame_;

	uint32_t bgmTextImage_ = 0;
	uint32_t seTextImage_ = 0;
	std::unique_ptr<Sprite> bgmText_;
	std::unique_ptr<Sprite> seText_;

	uint32_t fullScreenImage_ = 0;
	std::unique_ptr<Sprite> fullScreenText_;

	uint32_t checkBoxTex_ = 0;
	uint32_t checkBoxFrameTex_ = 0;
	uint32_t checkTex_ = 0;

	std::unique_ptr<Sprite> checkBox_;
	std::unique_ptr<Sprite> checkBoxFrame_;
	std::unique_ptr<Sprite> check_;

	uint32_t bButtonTex_ = 0;
	std::unique_ptr<Sprite> bButton_;

	float param_ = 0.0f;

	static uint32_t bgmVolume_;
	static uint32_t seVolume_;

private:

	enum class Mode {
		Root,
		Open,
		Close,
	};

	void RootInit();
	void RootUpdate();

	void OpenInit();
	void OpenUpdate();

	void CloseInit();
	void CloseUpdate();

	enum class Item {
		BGM,
		SE,
		FullScreen,
	};

	void BGMInit();
	void BGMUpdate();

	void SEInit();
	void SEUpdate();

	void FSInit();
	void FSUpdate();

private:

	Mode mode_ = Mode::Open;
	std::optional<Mode> modeRequest_ = Mode::Open;

	Item item_ = Item::BGM;
	std::optional<Item> itemRequest_ = Item::BGM;

};

