#pragma once
#include "KamataEngine.h"
#include <array>

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

};