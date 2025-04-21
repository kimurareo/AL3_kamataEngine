#pragma once
#include "KamataEngine.h"

class Player {
public:

	// 初期化関数
	void Initialize(KamataEngine::Model* model, uint32_t texturHandle, KamataEngine::Camera* camera);

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


};