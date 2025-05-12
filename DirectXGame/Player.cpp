#include "Player.h"
using namespace KamataEngine;


void Player::Initialize(Model* model,  Camera* camera) {

	// NULLポインタチェック
	assert(model);

	model_ = model;


	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	worldTransform_.Initialize();

}

void Player::Update() {

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw() {

	model_->Draw(worldTransform_,*camera_);

}
