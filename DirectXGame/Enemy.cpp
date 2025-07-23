#include "Enemy.h"
#include "KamataEngine.h"
#include "MyMath.h"



using namespace KamataEngine;

// 初期化処理
void Enemy::Initialize(Model* model,Camera* camera, const Vector3& position) {

	// モデルをメンバ変数に記録
	model_ = model;

	camera_ = camera;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// 初期座標の設定
	worldTransform_.translation_ = position;

	// 速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;

}

// 更新処理
void Enemy::Update() {

	// タイマーを加算
	walkTimer_ += 1.0f / 60.0f;

	// 回転アニメーション


	// アフェン変換行列の作成
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	// 移動
	worldTransform_.translation_.x += velocity_.x;


}

// 描画処理
void Enemy::Draw() {
	
	model_->Draw(worldTransform_, *camera_);

}
