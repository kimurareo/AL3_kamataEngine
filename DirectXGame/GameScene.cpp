#include "GameScene.h"
#include "Player.h"
#include "Skydome.h"
#include "KamataEngine.h"
#include "MyMath.h"
#include "Enemy.h"
#include "DeathParticles.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	// ファイル名を指定してテクスチャを読み込む
	//texturHandle_ = TextureManager::Load("mario.png");

	// 3Dモデルの生成
	model_ = Model::Create();
	

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// カメラの初期化
	camera_.Initialize();

	//===================================================
	// マップチップの描画の初期化
	//===================================================
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenerateBlocks();


	//===============================================================
	// 自キャラ
	//===============================================================
	
	// モデル
	model_ = Model::CreateFromOBJ("player",true);

	// 生成
	player_ = new Player();
	
	player_->SetMapChipField(mapChipField_);

	// 初期化
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	player_->Initialize(model_, &camera_, playerPosition);

	//===============================================
	// 雑魚キャラ
	//===============================================
	// モデル
	model_ = Model::CreateFromOBJ("enemy");


	for (int32_t i = 0; i < 4; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(6 + i, 18);
		newEnemy->Initialize(model_, &camera_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(8, 18);
	//enemy_->Initialize(model_, &camera_, enemyPosition);

	
	// デスパーティクル
	// パーティクルの3Dモデルデータの生成
	modelParticle_ = Model::CreateFromOBJ("deathParticle", true);
	deathParticles_ = new DeathParticles;
	deathParticles_->Initialize(modelParticle_, &camera_, playerPosition);



	//================================================================
	// 天球
	//================================================================

	// モデル
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// 天球の生成
	skydome_ = new Skydome();

	// 初期化
	skydome_->Initialize(modelSkydome_, &camera_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// ゲームフェーズから開始
	phase_ = Phase::kPlay;

	/*// 要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;

	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockHorizontal);
	worldTransformBlocks_.resize(kNumBlockVirtical);

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);

	}

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
	
		for (uint32_t j = 0; j <  kNumBlockHorizontal; ++j) {

			if (j % 2 == 0) 	
				continue;
			

			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}

	}*/

	
	// カメラコントローラーの生成
	cameraController_ = new CameraController();
	// 初期化
	cameraController_->Initialize();
	// 追従対象をセット
	cameraController_->SetTarget(player_);
    // リセット
	cameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);

	
}


void GameScene::Update() {

	switch (phase_) {

		case Phase::kPlay:
			// ゲームプレイフェーズの処理

			// 全ての当たり判定
		    CheckAllCollision();

		    // ゲームプレイフェーズの処理
		    if (player_->IsDead() == true) {
			    // デス演出フェーズに切り替え
			    phase_ = Phase::kDeath;

			    // 自キャラの座標を取得
			    const KamataEngine::Vector3 deathParticlesPosition = player_->GetWorldPosition();

			    // パーティクル
			    deathParticles_ = new DeathParticles();
			    deathParticles_->Initialize(modelParticle_, &camera_, deathParticlesPosition);
		    }		

			break;

		case Phase::kDeath:
			// デス演出の処理
		    // デスパーティクルの更新
		    deathParticles_->Update();
		   

			break;


	}


	// 自キャラの更新
	player_->Update();

	// 天球の更新
	skydome_->Update();

	// 敵の更新
	// enemy_->Update();
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	// カメラコントローラーの更新
	cameraController_->Update();

	// 行列を定義バッファに転送
	// worldTransform_.TransferMatrix();

	// debugCamera_->Update();

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif // _DEBUG

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.matView = cameraController_->GetViewProjection().matView;
		camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の更新と転送
		camera_.TransferMatrix();
	}
	// ブロックの更新
	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock) {
				continue;
			}

			// アフィン変換行列の作成

			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			////定数バッファに転送する

			worldTransformBlock->TransferMatrix();
		}
	}
}

void GameScene::Draw() {
	
	//========= 描画開始=========================================
	Model::PreDraw(dxCommon->GetCommandList());

	
	skydome_->Draw();
	

	 for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock)
				continue;

			modelBlock_->Draw(*worldTransformBlock, camera_);
		

		}
	}

	 // プレイヤーの描画
	 player_->Draw();

	 // 雑魚キャラの描画
	 for (Enemy* enemy : enemies_) {
		 enemy->Draw();
	 }

	 // パーティクルの描画
	 if (deathParticles_) {
	 
	 deathParticles_->Draw();

	 }

	Model::PostDraw();
	//=========描画終了========================================

}

void GameScene::GenerateBlocks() {

	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定(縦方向ブロック数)
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {

		// 1列の要素数を設定
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {

				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
	
}

void GameScene::CheckAllCollision() {

	// 判定対象1と2の座標
	AABB aabb1, aabb2;

	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラ地敵弾全ての当たり判定
	for (Enemy* enemy : enemies_) {
	
		// 敵弾の座標
		aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {
		
			// 自キャラの衝突時関数を呼び出す
			player_->OnCollision(enemy);

			// 敵の衝突時関数を呼び出す
			enemy->OnCollision(player_);

		
		}

	}

}

void GameScene::ChangePhase() {

	switch (phase_) {
	case Phase::kPlay:
		// ゲームプレイフェーズの処理

		if (player_->IsDead() == true) {
			// デス演出フェーズに切り替え
			phase_ = Phase::kDeath;

			// 自キャラの座標を取得
			const KamataEngine::Vector3 deathParticlesPosition = player_->GetWorldPosition();

			// パーティクル
			deathParticles_ = new DeathParticles();
			deathParticles_->Initialize(modelParticle_, &camera_, deathParticlesPosition);
		}

		break;

	case Phase::kDeath:
		// デス演出フェーズの処理


		break;
	}


}

GameScene::~GameScene() { 
	delete model_;
	delete modelBlock_;
	delete player_;
	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;
	delete deathParticles_;


	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
	
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
		
			delete worldTransformBlock;

		}

	}

	worldTransformBlocks_.clear();
}



