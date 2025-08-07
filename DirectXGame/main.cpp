#include <Windows.h>
#include <KamataEngine.h>
#include "GameScene.h"
#include "TitleScene.h"

using namespace KamataEngine;

// シーンはグローバル変数で宣言
GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;

// シーン
enum class Scene { 
	kUnknown = 0,

	kTitle,
	kGame,
};


// 現在シーン
Scene scene = Scene::kUnknown;

// シーン切り替え処理
void ChangeScene() { 
	
	switch (scene) { case Scene::kTitle:
		if (titleScene->IsFinished()) {
		
			// シーン変更
			scene = Scene::kGame;
			// 旧シーンの開放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			gameScene  = new GameScene;
			gameScene->Initialize();
		}
		break;
		
		case Scene::kGame:
		if (gameScene->IsFinished()) {
		
			// シーン変更
			scene = Scene::kTitle;
			// 旧シーンの開放
			delete gameScene;
			gameScene = nullptr;

			// 新シーンの生成と初期化
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;

	}

}

// シーンの更新
void UpdateScene() {
	switch (scene) {
	
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	
	}
}

// シーンの描画
void DrawScene() {
	switch (scene) {
	
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	
	}
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	// エンジンの初期化
	KamataEngine::Initialize(L"LE2D_10_キムラ_レオ_AL3");
	
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// ゲームシーンのインスタンス生成
	//GameScene* gameScene = new GameScene();

	// ゲームシーンの初期化
	//gameScene->Initialize();

	// 最初のシーンの初期化
	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();

	// メインループ
	while (true) {

		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		// シーン切り替え
		ChangeScene();
		// 現在シーン更新
		UpdateScene();

		// ゲームシーンの更新
		//gameScene->Update();

		//===============================================================
		// 描画開始
		//===============================================================
		dxCommon->PreDraw();

		// 現在のシーン
		DrawScene();

		// ゲームシーンの描画
		//gameScene->Draw();


		//=================================================================
		// 描画終了
		//=================================================================
		dxCommon->PostDraw();

	}

	// ゲームシーンの開放
	delete gameScene;
	delete titleScene;

	// エンジン終了処理
	KamataEngine::Finalize();

	// ゲームシーンの開放
	delete gameScene;
	// nullptrの代入
	gameScene = nullptr;



	return 0;
}
