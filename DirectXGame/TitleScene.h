#pragma once
#include "KamataEngine.h"
#include "Fade.h"

using namespace KamataEngine;

class TitleScene {
public:
	~TitleScene();

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
	WorldTransform worldTransformPlayer_;

	Model* modelPlayer_ = nullptr;
	Model* model_ = nullptr;

	float rotate_ = 0.0f;

	// 終了フラグ
	bool finished_ = false;

	Fade* fade_ = nullptr;
	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

};
