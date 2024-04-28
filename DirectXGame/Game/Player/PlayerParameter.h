#pragma once
#include <stdint.h>
#include <Vec2.h>

//ジャンプ関連の構造体
struct WorkJump {
	//ジャンプ可能かどうかのフラグ
	bool canJump = false;
	//ギリジャンの最大猶予フレーム
	int32_t kMaxGraceTime = 6;
	//ギリジャンの猶予フレーム
	int32_t graceTime = 0;
	//ジャンプ時のy速度
	float jumpVelocity = -25.0f;

};

//壁キック関連の構造体
struct WorkWallJump {
	//壁キックできるかどうかのフラグ
	bool canWallJump = false;
	//ギリ壁キックの最大猶予フレーム
	int32_t kMaxGraceTime = 6;
	//ギリ壁キックの猶予フレーム
	int32_t graceTime = 0;
	//壁キックの移動量
	Vector2 wallJumpVelocity = { 10.0f,-30.0f };
};

//ヒップドロップ関連の構造体
//struct WorkHipDrop {
//	//ヒップドロップ可能かどうか
//	bool canHipDrop = false;
//	//ヒップドロップ可能になるまでの空中待機フレーム
//	int32_t kNecessaryTime = 15;
//};

//穴掘り関連の処理
struct WorkDig {
	//穴掘りフラグ
	bool isDig = false;
	//穴堀りを行うポジション
	Vector2 digPosition{};
	//採掘インターバル
	int32_t digInterval = 42;
	//採掘カウント
	int32_t digCount = 0;
	//耐久減少量の最高値
	int32_t maxDigPower = 20;
	//採掘時の耐久減少量
	int32_t digPower = 1;
};

//掴み関連の処理
//struct WorkCatch {
//	//現在掴める状態かどうか
//	bool canCatch = true;
//
//};

//溜めジャンプ関連の構造体
struct WorkChargeJump {
	//チャージジャンプ中かどうか
	bool isChargeJumping = false;
	//現在チャージ可能かどうか
	bool isCharge = false;
	//チャージにかかる時間
	int32_t maxChargeTime = 40;
	//チャージ時間をカウントする変数
	int32_t chargeTimer = 0;
	//溜めジャンプのy移動量
	float chargeJumpVelocity = -35.0f;
	//何マス分ジャンプするか
	int32_t jumpValue = 5;
	//ジャンプを止めるライン
	float stopLine = 0.0f;
	//溜めジャンプによるブロック破壊フラグ
	bool canBreak = false;
};

//耐熱時間関連
//struct WorkSaunaTimer {
//	//サウナにいられる時間(制限時間)
//	int32_t maxSaunaTime = 3600;
//	//実際にカウントする時間
//	int32_t countSaunaTimer = maxSaunaTime;
//};

/// <summary>
/// プレイヤーの色々なパラメータを纏めたクラス
/// </summary>
class PlayerParameter
{
public:

	void Initialize();

	/// <summary>
	/// 必要な変数をコピーする関数
	/// </summary>
	/// <param name="origin">コピー元</param>
	void CopyParameter(const PlayerParameter& origin);

	//ジャンプ
	WorkJump Jump_;

	//壁キック
	WorkWallJump wallJump_;

	//ヒップドロップ
	/*WorkHipDrop hipDrop_;*/

	//掴み
	/*WorkCatch catch_;*/

	//溜めジャンプ
	WorkChargeJump chargeJump_;

	//穴掘り
	WorkDig dig_;

	//!!!サウナタイム!!!
	/*WorkSaunaTimer saunaTimer_;*/

	//移動量
	float speed_ = 1.0f;

	//左右移動速度上限
	float maxMoveSpeed_ = 10.0f;

};
