#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;


class Enemy {

	public:

	// 基本的な関数
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw();

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデルのポインタ
	Model* model_ = nullptr;

	// カメラのポインタ
	Camera* camera_ = nullptr;



};
