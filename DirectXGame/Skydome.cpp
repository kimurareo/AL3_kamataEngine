#include "Skydome.h"

void Skydome::Initialize(Model* model, Camera* camera) {

	// NULLポインタチェック
	assert(model);

	model_ = model;


	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	worldTransform_.Initialize();

}

void Skydome::Update() {

}

void Skydome::Draw() {

	model_->Draw(worldTransform_, *camera_);

}
