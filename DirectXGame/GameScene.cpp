#include "GameScene.h"
#include "Player.h"
#include "KamataEngine.h"
#include "MyMath.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	// ファイル名を指定してテクスチャを読み込む
	texturHandle_ = TextureManager::Load("mario.png");

	// 3Dモデルの生成
	model_ = Model::Create();
	modelBlock_ = Model::Create();


	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// カメラの初期化
	camera_.Initialize();

	// 自キャラの生成
	player_ = new Player();

	// 自キャラの初期化
	player_->Initialize(model_,texturHandle_,&camera_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 要素数
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

	}

}


void GameScene::Update() {

	// 自キャラの更新
	player_->Update();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock) 
				continue;

			// アフェン変換行列の作成
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			// 定数バッファ二に転送する
			worldTransformBlock->TransferMatrix();

		}
	}


	// デバッグカメラの更新
	debugCamera_->Update();
	
	#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_O)) {
	
	isDebugCameraActive_ = !isDebugCameraActive_;
	
	}
	#endif

	if (isDebugCameraActive_) {
	
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();

	} else {
	
		camera_.UpdateMatrix();

	}


}

void GameScene::Draw() {
	
	//========= 描画開始=========================================
	Model::PreDraw(dxCommon->GetCommandList());

	// player_->Draw();

	

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock)
				continue;

			modelBlock_->Draw(*worldTransformBlock, camera_);
		

		}
	}


	Model::PostDraw();
	//=========描画終了========================================

}

GameScene::~GameScene() { 
	delete model_;
	delete modelBlock_;
	delete player_;
	delete debugCamera_;
	

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
	
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
		
			delete worldTransformBlock;

		}

	}

	worldTransformBlocks_.clear();
}



