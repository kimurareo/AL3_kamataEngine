#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;


class Enemy {

	public:

	// 基本的な関数
	void Initialize(Model* model, Camera* camera,const Vector3& position);
	void Update();
	void Draw();

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデルのポインタ
	Model* model_ = nullptr;

	// カメラのポインタ
	Camera* camera_ = nullptr;

	// 歩行の速さ
	static inline const float kWalkSpeed = 0.01f;

	// 速度
	Vector3 velocity_ = {};

	// 最初の角度
	//static inline const float kwalkMotionAngleState = -45.0f;

	// 最後の角度
	//static inline const float kWalkMotionAngleEnd = 45.0f;

	// アニメーションの周期をなる時間
	//static inline const float kWalkMotinTime = 1.0f;


	// 経過時間
	float walkTimer_ = 0.0f;


};
