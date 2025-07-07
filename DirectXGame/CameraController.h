#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;
using namespace MathUtility;


// プレイヤーの呼び出し
class Player;


class CameraController {
public:

	// 矩形
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};



	// 初期化関数
	void Initialize();

	// 更新関数
	void Update();

	// カメラ
	Camera camera_;

	Player* target_ = nullptr;

	// セッター
	void SetTarget(Player* target) { target_ = target;}
	
	// リセット関数
	void Reset();
	
	// 追従対象とカメラの座標の差
	Vector3 targetOffset_ = {0, 0, -15.0f};

	// Getter
	const KamataEngine::Camera& GetViewProjection() const { return camera_; }

	// カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};

	void SetMovableArea(Rect area) { movableArea_ = area; }

	// カメラの目標座標
	Vector3 targetCoordinates;

	// 座標補間割合
	static inline const float kInterpolationRate = 0.01f;

	// 速度掛け率
	static inline const float kVelocityBias = 30.0f;

	// 追従対象の各方向へのカメラ移動範囲
	static inline const Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};
	

};
