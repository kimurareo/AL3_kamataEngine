#include "ClearScene.h"
#include "MyMath.h"
#include <numbers>

using namespace KamataEngine;

// デストラクタ
ClearScene::~ClearScene() {
	// モデル
	delete model_;
	delete fade_;
}

// 初期化
void ClearScene::Initialize() {
	// モデル生成（CLEAR文字など）
	model_ = Model::CreateFromOBJ("clearFont");

	// カメラの初期化
	camera_.Initialize();

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = {2, 2, 2};
	worldTransform_.translation_ = {0, 0, 0};

	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	phase_ = Phase::kFadeIn;
	finished_ = false;
}

// 更新
void ClearScene::Update() {

	// アフィン変換行列の作成
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	switch (phase_) {
	case Phase::kMain:
		// Enterでタイトルへ
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;

	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;

	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
}

// 描画
void ClearScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	// CLEAR表示
	model_->Draw(worldTransform_, camera_);

	// フェード
	fade_->Draw();

	Model::PostDraw();
}
