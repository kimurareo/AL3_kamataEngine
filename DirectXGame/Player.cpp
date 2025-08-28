#define NOMINMAX
#include "Player.h"
#include <numbers>
#include <algorithm>
#include "MyMath.h"
#include <algorithm>
#include "MapChipField.h"


using namespace KamataEngine;
using namespace MathUtility;


void Player::Initialize(Model* model,  Camera* camera,const Vector3& position) {

	// NULLポインタチェック
	//assert(model);

	model_ = model;
	worldTransform_.translation_ = position;

	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	
	// 初期回転
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;


}

// 移動入力
void Player::InputMove() {

	// 左右移動操作
	if (onGround_) {

		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				velocity_.x *= (1.0f - kAttenuation);

				acceleration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x > 0.0f) {

					velocity_.x *= (1.0f, -kAttenuation);
				}

				acceleration.x -= kAcceleration;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}

			// 加速・減速
			velocity_ += acceleration;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRumSpeed, kLimitRumSpeed);

		} else {

			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_ += Vector3(0, kJumpAccleration, 0);
		}

	} else {

		velocity_ += Vector3(0, -kGravityAcceleration, 0);

		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}


// 旋回制御
void Player::AnimateTurn() {

	if (turnTimer_ > 0.0f) {

		turnTimer_ = 1.0f / 60.0f;

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}

}


void Player::CheckMapCollision(CollisionMapInfo& info) {

	CheckMapCollisionUp(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);

}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0},
	};
	
	return center + offsetTable[static_cast<uint32_t>(corner)];
}

// マップチップ衝突判定上
void Player::CheckMapCollisionUp(CollisionMapInfo& info) {

	// 上昇あり?
	if (info.move.y <= 0) {
		return;
	}


	// 移動後の4つの核の座標
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;

	// 真上の当たり判定を行う
	bool hit = false;

	// 左上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	
	// 右上点の判定dd
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
	
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));
		// 現在座標が壁の外か判定
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));
		if (indexSetNow.yIndex != indexSet.yIndex) {
			// めり込み先ブロックの範囲矩形
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
			// 天井に当たったことを記録する
			info.ceiliing = true;
		}
	}

}

// マップ衝突判定下方向
void Player::CheckMapCollisionDown(CollisionMapInfo& info) {

	// 下降あり?
	if (info.move.y >= 0) {
		return;
	}

	// 移動後の4つの核の座標
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;


	// 真下の当たり判定を行う
	bool hit = false;

	// 左下の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {

		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0,-kHeight / 2.0f, 0));
		// 現在座標が壁の外か判定
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_+ Vector3(0,-kHeight / 2.0f, 0));
		if (indexSetNow.yIndex != indexSet.yIndex) {

			// めり込み先ブロックの範囲矩形
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
			// 天井に当たったことを記録する
			info.landing = true;
		}
	}


}

void Player::CheckMapCollisionRight(CollisionMapInfo& info) {

	// 右移動あり
	if (info.move.x <= 0) {
		return;
	}

	// 移動後の4つの核の座標
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;


	// 右の当たり判定を行う
	bool hit = false;

	// 右上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {

		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3( +kWidth / 2.0f, 0,0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
		// 天井に当たったことを記録する
		info.hitWall = true;
	}

}

// 衝突判定左
void Player::CheckMapCollisionLeft(CollisionMapInfo& info) {

	// 左移動あり
	if (info.move.x >= 0) {
		return;
	}

	// 移動後の4つの核の座標
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;


	//  左の当たり判定を行う
	bool hit = false;

	//  左上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	//  左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {

		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(-kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::min(0.0f, rect.right - worldTransform_.translation_.x + (kWidth / 2.0f + kBlank));
		// 天井に当たったことを記録する
		info.hitWall = true;
	}

}

void Player::CheckMapMove(const CollisionMapInfo& info) {

	// 移動
	worldTransform_.translation_ += info.move;

}

void Player::CheckMapCeiling(const CollisionMapInfo& info) {

	// 天井に当たった？
	if (info.ceiliing) {
	
		DebugText::GetInstance()->ConsolePrintf("hitceiling\n");

		velocity_.y = 0;

	}

}

// 壁に接触している時の処理
void Player::CheckMapWall(const CollisionMapInfo& info) {

	// 壁接触による減速
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}

}

// 接地状態の切り替え処理
void Player::CheckMapLanding(const CollisionMapInfo& info) {

	// 自キャラが設置状態？
	if (onGround_) {
	
		// 接地状態

		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {
		

			// 移動後の4つの核の座標
			std::array<Vector3, kNumCorner> positionNew;

			for (uint32_t i = 0; i < positionNew.size(); ++i) {
				positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			}

			MapChipType mapChipType;

			// 真下の当たり判定を行う
			bool hit = false;

			// 左下の判定
			MapChipField::IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom] + Vector3(0,-kGroundSearchHeight,0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 右下点の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom] + Vector3(0,-kGroundSearchHeight,0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 落下なら空中状態に切り替え
			if (!hit) {
				onGround_ = false;
			}

		
		}
	
	} else {
	
		
		// 着地フラグ
		if (info.landing) {
		
			// 着地状態に切り替える
			onGround_ = true;
			// 着地時にX速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			// Y速度をゼロにする
			velocity_.y = 0.0f;
		
		}

	}

}

Vector3 Player::GetWorldPosition() {
	
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Player::GetAABB() { 
	
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	
	return aabb; 
}

void Player::OnCollision(const Enemy* enemy) {
	(void)enemy;
	// デスフラグを立てる
	isDead_ = true;
}

void Player::Update() {

	// 移動入力
	InputMove();
	
	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo = {};
	// 移動量に速度尾値をコピー
	collisionMapInfo.move = velocity_;
	collisionMapInfo.landing = false;
	collisionMapInfo.hitWall = false;

	// 2マップ衝突チェック
	CheckMapCollision(collisionMapInfo);

	// 判定結果を反映して移動させる
	//worldTransform_.translation_ += velocity_;

	// 3
	CheckMapMove(collisionMapInfo);

	// 4 
	CheckMapCeiling(collisionMapInfo);

	// 5
	CheckMapWall(collisionMapInfo);

	// 6
	CheckMapLanding(collisionMapInfo);

	//// 着地フラグ
	//bool landing = false;

	//// 地面との当たり判定
	//if (velocity_.y < 0) {
	//	if (worldTransform_.translation_.y <= 1.0f) {
	//		landing = true;
	//	}
	//}

	//// 接地判定
	//if (onGround_) {
	//// ジャンプ開始
	//	if (velocity_.y > 0.0f) {
	//	onGround_ = false;
	//	}
	//} else {
	//// 着地
	//	if (landing) {
	//	// めり込み
	//		worldTransform_.translation_.y = 1.0f;
	//		// 摩擦で横方向速度が減速する
	//		velocity_.x *= (1.0f - kAttenuation);
	//		// 下方向速度をリセット
	//		velocity_.y = 0.0f;
	//		// 接地状態に移行
	//		onGround_ = true;
	//	}
	//}

	// 7旋回制御
	AnimateTurn();

	// アフェン変換行列の作成
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

}


void Player::Draw() {

	// プレイヤーを生きている時だけ表示させる
	if (isDead_ == false) {

		model_->Draw(worldTransform_, *camera_);
	}
}

