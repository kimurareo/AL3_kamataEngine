#include "DeathParticles.h"
#include "MyMath.h"

void DeathParticles::Initialize(Model* model, Camera* camera, const Vector3& position) {

	// モデルをメンバ変数に記録
	model_ = model;

	camera_ = camera;

	// ワールド変換の初期化
	for (WorldTransform& worldTransform : worldTransforms_) {
	
		worldTransform.Initialize();
		worldTransform.translation_ = position;

	}

}

void DeathParticles::Update() {

	

}

void DeathParticles::Draw() {

	


}
