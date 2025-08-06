#include "DeathParticles.h"
#include "GameScene.h"
#include "KamataEngine.h"
#include "Player.h"
#include "cassert"
#include <algorithm>
#include <numbers>

#define NOMINMAX
#include "MapChipField.h"
#include <algorithm>
using namespace KamataEngine;
using namespace MathUtility;


void DeathParticles::Initialize(Model* model, Camera* camera, const Vector3& position) {

	// モデルをメンバ変数に記録
	model_ = model;

	camera_ = camera;

	// ワールド変換の初期化
	for (WorldTransform& worldTransform : worldTransforms_) {
	
		worldTransform.Initialize();
		worldTransform.translation_ = position;

	}

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};

}

void DeathParticles::Update() {

	  for (WorldTransform& worldTransform : worldTransforms_) {

		  worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);

		worldTransform.TransferMatrix();
	}
	
	  for (uint32_t i = 0; i < 8; i++) {
		// 基本となる速度ベクトル
		KamataEngine::Vector3 velocity = {kSpeed, 0, 0};
		// 回転角を計算する
		float angle = kAngleUnit * i;
		// Z軸まわり回転行列
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		// 基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);
		// 移動処理
		worldTransforms_[i].translation_ += velocity;
	}
	// カウンターを1フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;

	if (counter_ >= kDuration) {
		counter_ = kDuration;
		// 終了扱いにする
		isFinished_ = true;
	}

	
	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	// 色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);
	
	

	
	// 終了なら何もしない
	if (isFinished_) {
		return;
	}


}

void DeathParticles::Draw() {


	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *camera_,&objectColor_);
	}

	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

}
