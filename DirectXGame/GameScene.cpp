#include "GameScene.h"
#include "Player.h"
#include "KamataEngine.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	// ファイル名を指定してテクスチャを読み込む
	texturHandle_ = TextureManager::Load("mario.png");

	// 3Dモデルの生成
	model_ = Model::Create();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// カメラの初期化
	Camera_.Initialize();

	// 自キャラの生成
	player_ = new Player();

	// 自キャラの初期化
	player_->Initialize(model_,texturHandle_,&Camera_);

	// 要素数
	const uint32_t kNumBlockHorizontal = 20;

	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;

	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockHorizontal);

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockHorizontal; i++) {
	
		worldTransformBlocks_[i] = new WorldTransform();
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockWidth * i; 
		worldTransformBlocks_[i]->translation_.y = 0.0f;


	}

}


void GameScene::Update() {

	// 自キャラの更新
	player_->Update();

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
	
		// アフェン変換行列の作成
		

	}

}

void GameScene::Draw() {
	
	//========= 描画開始=========================================
	Model::PreDraw(dxCommon->GetCommandList());

	player_->Draw();


	Model::PostDraw();
	//========= 描画終了========================================

}

GameScene::~GameScene() { 
	delete model_;
	delete player_;

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock;
	}
	worldTransformBlocks_.clear();
}



