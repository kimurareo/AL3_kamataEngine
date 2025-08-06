#pragma once
#include "KamataEngine.h"
#include <array>
#define _USE_MATH_DEFINES
#include <math.h>


using namespace KamataEngine;


class DeathParticles {
public:

	void Initialize(Model* model, Camera* camera, const Vector3& position);
	void Update();
	void Draw();

	// モデル
	KamataEngine::Model* model_ = nullptr;
	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// パーティクルの個数
	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransforms_;

	// 存続時間
	static inline const float kDuration = 1.0f;

	// 移動の速さ
	static inline const float kSpeed = 0.1f;

	// 分割した1個分の角度
	static inline const float kAngleUnit = 2 * float(M_PI) / 8.0f;

	// 終了フラグ
	bool isFinished_ = false;
	// 経過時間カウント
	float counter_ = 0.0f;

	// 色変更オブジェクト
    ObjectColor objectColor_;
	// 色の数値
	Vector4 color_;


	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;


};