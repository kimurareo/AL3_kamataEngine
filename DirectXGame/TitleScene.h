#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class TitleScene {
public:
	~TitleScene();
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
};
