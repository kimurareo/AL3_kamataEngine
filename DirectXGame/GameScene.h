#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>

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
	Model* model_ = nullptr;

	// ブロックの3Dモデルデータ
	Model* modelBlock_ = Model::CreateFromOBJ("cube");

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// カメラ
    Camera camera_;
	
	// 自キャラ
	Player* player_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// デバッグカメラの生成
	DebugCamera* debugCamera_ = nullptr;


	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// デストラクタ
	~GameScene();



};
