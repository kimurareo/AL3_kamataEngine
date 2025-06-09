#include "CameraController.h"
#include "Player.h"

void CameraController::Initialize() {

	// カメラの初期化
	camera_.Initialize();
}

void CameraController::Update() {

	// Getterを読み込み
	const Vector3& targetVelocity = target_->GetVelocity();

	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorladTransform();
	// 追従対象とオフセットからカメラの座標を計算
	targetCoordinates = targetWorldTransform.translation_ + targetOffset_ + targetVelocity * kVelocityBias;

	// 座標によりゆったり追従
	camera_.translation_.x = Lerp(camera_.translation_.x, targetCoordinates.x, kInterpolationRate);

	// 追従対象が画面外に出ないように補正
	camera_.translation_.x = max(camera_.translation_.x, targetCoordinates.x + targetMargin.left);
	camera_.translation_.x = min(camera_.translation_.x, targetCoordinates.x + targetMargin.right);
	camera_.translation_.y = max(camera_.translation_.x,targetCoordinates.y + targetMargin.bottom );
	camera_.translation_.y = min(camera_.translation_.x, targetCoordinates.y + targetMargin.top);


	// 移動範囲制限
	camera_.translation_.x = max(camera_.translation_.x, movableArea_.left);
	camera_.translation_.x = min(camera_.translation_.x, movableArea_.right);
	camera_.translation_.y = max(camera_.translation_.x, movableArea_.bottom);
	camera_.translation_.y = min(camera_.translation_.x, movableArea_.top);

	// 行列を更新する
	camera_.UpdateMatrix();

}

void CameraController::Reset() {

	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorladTransform();
	// 追従対象とオフセットからカメラの座標を計算
	camera_.translation_ = targetWorldTransform.translation_ + targetOffset_;

}
