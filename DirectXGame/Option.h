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
	bool isFullScreen_ = true;

	uint32_t windowTex_ = 0;
	std::unique_ptr<Sprite> window_;

	float param_ = 0.0f;

	uint32_t bgmVolume_ = 5;
	uint32_t seVolume_ = 5;

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

