#include "Enemy.h"
#include "KamataEngine.h"
#include "MyMath.h"



using namespace KamataEngine;

// 初期化処理
void Enemy::Initialize(Model* model, const Vector3& position) {

	// モデルをメンバ変数に記録
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// 初期座標の設定
	worldTransform_.translation_ = position;


}

// 更新処理
void Enemy::Update() {

	// アフェン変換行列の作成
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	

}

// 描画処理
void Enemy::Draw() {
	
	model_->Draw(worldTransform_, *camera_);

}
