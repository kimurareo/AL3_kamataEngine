#include "Player.h"
using namespace KamataEngine;


void Player::Initialize(KamataEngine::Model* model, uint32_t texturHandle, KamataEngine::Camera* camera) {

	// NULLポインタチェック
	assert(model);

	model_ = model;

	texturHandle_ = texturHandle;

	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	worldTransform_.Initialize();

}

void Player::Update() {

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw() {

	model_->Draw(worldTransform_,*camera_,texturHandle_);

}
