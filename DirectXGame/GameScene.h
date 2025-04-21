#pragma once
#include "KamataEngine.h"
#include "Player.h"

using namespace KamataEngine;

// ゲームシーン
class GameScene {
public:


	// 初期化
	void Initialize();

	// 更新
	void Update();

	//　描画
	void Draw();

	// テクスチャハンドル
	uint32_t texturHandle_ = 0;

	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	// カメラ
	KamataEngine::Camera Camera_;
	
	// 自キャラ
	Player* player_ = nullptr;

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();


	// デストラクタ
	~GameScene();



};
