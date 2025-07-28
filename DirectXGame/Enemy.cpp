#include "Enemy.h"
#include "KamataEngine.h"
#include "MyMath.h"
#include <numbers>



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

	// 敵の90ど回転
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;

	// タイマーを加算
	walkTimer_ += 1.0f / 60.0f;

	// 回転アニメーション
	worldTransform_.rotation_.x = std::sin(walkTimer_ * 5.0f);

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

Vector3 Enemy::GetWorldPosition() { 

	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}

void Enemy::OnCollision(const Player* player) { 
	(void)player;
}

AABB Enemy::GetAABB() { 

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb; 

}
