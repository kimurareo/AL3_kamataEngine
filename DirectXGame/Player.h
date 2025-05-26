#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;


class Player {
public:

	// 初期化関数
	void Initialize(Model* model,Camera* camera,const Vector3& position);

	// 更新関数
	void Update();

	// 描画関数
	void Draw();

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t texturHandle_ = 0u;

	KamataEngine::Camera* camera_ = nullptr;

	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.01f;
	static inline const float kLimitRumSpeed = 5.0f;

	enum class LRDirection { 
		kRight,
		kLeft,

	};

	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;

	// 旋回開始＜秒＞
	static inline const float kTimeTurn = 0.3f;

	// 設置状態フラグ
	bool onGround_ = true;
	// 重力加速
	static inline const float kGravityAcceleration = 0.1f;
	// 最大落下速度
	static inline const float kLimitFallSpeed = 10.0f;
	// ジャンプ初速
	static inline const float kJumpAccleration = 1.0f;

};