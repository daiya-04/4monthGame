#pragma once
#include <stdint.h>
#include <Vec2.h>

/// <summary>
/// プレイヤーの色々なパラメータを纏めたクラス
/// </summary>
class PlayerParameter
{
public:

	void Initialize();

	//ジャンプ関連の構造体
	struct WorkJump {
		//ジャンプ可能かどうかのフラグ
		bool canJump = false;
		//ギリジャンの最大猶予フレーム
		const uint32_t kMaxGraceTime = 6;
		//ギリジャンの猶予フレーム
		uint32_t graceTime = 0;
		//ジャンプ時のy速度
		float jumpVelocity = -25.0f;
	};

	//壁キック関連の構造体
	struct WorkWallJump {
		//壁キックできるかどうかのフラグ
		bool canWallJump = false;
		//ギリ壁キックの最大猶予フレーム
		const uint32_t kMaxGraceTime = 6;
		//ギリ壁キックの猶予フレーム
		uint32_t graceTime = 0;
		//壁キックの移動量
		Vector2 wallJumpVelocity = { 10.0f,-30.0f };
	};

	//ヒップドロップ関連の構造体
	struct WorkHipDrop {
		//ヒップドロップ可能かどうか
		bool canHipDrop = false;
		//ヒップドロップ可能になるまでの空中待機フレーム
		const uint32_t kNecessaryTime = 15;
	};

	//穴掘り関連の処理
	struct WorkDig {
		//穴掘りフラグ
		bool isDig = false;
		//穴堀りを行うポジション
		Vector2 digPosition{};
	};

	//掴み関連の処理
	struct WorkCatch {
		//現在掴める状態かどうか
		bool canCatch = true;

	};

	//溜めジャンプ関連の構造体
	struct WorkChargeJump {
		//現在チャージ可能かどうか
		bool isCharge = false;
		//チャージにかかる時間
		const uint32_t kMaxChargeTime = 60;
		//チャージ時間をカウントする変数
		uint32_t chargeTimer = 0;
		//溜めジャンプのy移動量
		float chargeJumpVelocity = -35.0f;
		//溜めジャンプによるブロック破壊フラグ
		bool canBreak = false;
		//破壊ができなくなる速度
		float unBreakVelocity = -2.0f;
	};

	//探索時間関連
	struct WorkMiningTimer {


	};

	//ジャンプ
	WorkJump Jump_;

	//壁キック
	WorkWallJump wallJump_;

	//ヒップドロップ
	WorkHipDrop hipDrop_;

	//掴み
	WorkCatch catch_;

	//溜めジャンプ
	WorkChargeJump chargeJump_;

	//穴掘り
	WorkDig dig_;

	//移動量
	float speed_ = 1.0f;

	//左右移動速度上限
	float maxMoveSpeed_ = 10.0f;

};
