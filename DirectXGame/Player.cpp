#define NOMINMAX
#include "Player.h"
#include <numbers>
#include <algorithm>
#include "MyMath.h"
#include <algorithm>

using namespace KamataEngine;
using namespace MathUtility;


void Player::Initialize(Model* model,  Camera* camera,const Vector3& position) {

	// NULLポインタチェック
	assert(model);

	model_ = model;


	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;


}

void Player::Update() {

	// 移動入力
	// 左右移動操作
	if (onGround_) {

		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				velocity_.x *= (1.0f - kAttenuation);

				acceleration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x > 0.0f) {

					velocity_.x *= (1.0f, -kAttenuation);
				}

				acceleration.x -= kAcceleration;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}

			// 加速・減速
			velocity_ += acceleration;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRumSpeed, kLimitRumSpeed);

		} else {

			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_ += Vector3(0, kJumpAccleration, 0);
		}

	} else {
	
		velocity_ += Vector3(0, -kGravityAcceleration, 0);

		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);

	}

	worldTransform_.translation_ += velocity_;

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_,worldTransform_.rotation_,worldTransform_.translation_);

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	if (turnTimer_ > 0.0f) {

		turnTimer_ = 1.0f / 60.0f;

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}

	// 着地フラグ
	bool landing = false;

	// 地面との当たり判定
	if (velocity_.y < 0) {
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}

	// 接地判定
	if (onGround_) {
	// ジャンプ開始
		if (velocity_.y > 0.0f) {
		onGround_ = false;
		}
	} else {
	// 着地
		if (landing) {
		// めり込み
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減速する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}

}

void Player::Draw() {

	model_->Draw(worldTransform_,*camera_);

}
