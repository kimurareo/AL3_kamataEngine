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


}


void GameScene::Update() {

	// 自キャラの更新
	player_->Update();

}

void GameScene::Draw() {
	
	// 自キャラの描画
	Model::PreDraw(dxCommon->GetCommandList());

	player_->Draw();

	Model::PostDraw();
}

GameScene::~GameScene() { 
	delete model_;
	delete player_;
}



