#pragma once
#include <stdint.h>

/// <summary>
/// プレイヤーの色々なパラメータを纏めたクラス
/// </summary>
class PlayerParameter
{
public:

	//ジャンプ関連の構造体
	struct WorkJump {
		//ジャンプ可能かどうかのフラグ
		bool canJump = false;
		//ギリジャンの最大猶予フレーム
		const uint32_t kMaxGraceTime = 6;
		//ギリジャンの猶予フレーム
		uint32_t graceTime = 0;
		//ジャンプ時のy速度
		float jumpVelocity = 5.0f;
	};

	//壁キック関連の構造体
	struct WorkWallJump {
		//壁キックできるかどうかのフラグ
		bool canWallJump = false;
		//ギリ壁キックの最大猶予フレーム
		const uint32_t kMaxGraceTime = 6;
		//ギリ壁キックの猶予フレーム
		uint32_t graceTime = 0;
	};

	//ヒップドロップ関連の構造体
	struct WorkHipDrop {
		//ヒップドロップ可能かどうか
		bool canHipDrop = false;
		//ヒップドロップ可能になるまでの空中待機フレーム
		const uint32_t kNecessaryTime = 15;
	};

	//掴み関連の処理
	struct WorkCatch {
		//現在掴める状態かどうか
		bool canCatch = true;

	};

	//ジャンプ
	WorkJump Jump_;

	//壁キック
	WorkWallJump wallJump_;

	//ヒップドロップ
	WorkHipDrop hipDrop_;

	//掴み
	WorkCatch catch_;

	//移動量
	float speed_ = 3.0f;

};
