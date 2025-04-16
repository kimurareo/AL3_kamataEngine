#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	// ファイル名を指定してテクスチャを読み来む
	texturehandle_ = TextureManager::Load("mario.png");

	// 3Dモデルの生成
	model_ = Model::Create();

	worldTransform_.Initialize();

	camera_.Initialize();



}

void GameScene::Update() {

}

void GameScene::Draw() {

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	model_->Draw(worldTransform_, camera_, texturehandle_);

	// 3Dモデル描画後処理
	Model::PostDraw();

}

GameScene::~GameScene() {
    
	delete model_;

}



