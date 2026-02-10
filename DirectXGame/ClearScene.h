#pragma once
#include "Fade.h"
#include "KamataEngine.h"

using namespace KamataEngine;

class ClearScene {
public:
	~ClearScene();

	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	bool IsFinished() const { return finished_; }

private:
	// ビュープロジェクション
	Camera camera_;
	WorldTransform worldTransform_;

	// 表示用モデル（今は使わなくてもOK）
	Model* model_ = nullptr;

	// 終了フラグ
	bool finished_ = false;

	// フェード
	Fade* fade_ = nullptr;

	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;
};
