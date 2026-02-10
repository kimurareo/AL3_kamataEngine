#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

using namespace KamataEngine;

class MapChipField;

class Enemy;

class Player {

	private:
	// プレイヤー当たり判定サイズ
	Vector3 playerSize_ = {1.0f, 2.0f, 1.0f};

	// ゴール情報
	Vector3 goalPos_ = {95.0f, 0.0f, 0.0f};
	Vector3 goalSize_ = {1.0f, 2.0f, 1.0f};

	bool isGoal_ = false;


public:

	// 初期化関数
	void Initialize(Model* model,Camera* camera,const Vector3& position);

	// 更新関数
	void Update();

	// 描画関数
	void Draw();

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t texturHandle_ = 0u;

	KamataEngine::Camera* camera_ = nullptr;

	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.01f;
	static inline const float kLimitRumSpeed = 1.0f;

	enum class LRDirection { 
		kRight,
		kLeft,

	};

	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;

	// 旋回開始＜秒＞
	static inline const float kTimeTurn = 0.3f;

	// 設置状態フラグ
	bool onGround_ = true;
	// 重力加速
	static inline const float kGravityAcceleration = 0.1f;
	// 最大落下速度
	static inline const float kLimitFallSpeed = 1.0f;
	// ジャンプ初速
	static inline const float kJumpAccleration = 1.0f;

	const WorldTransform& GetWorladTransform() const { return worldTransform_; }

	const Vector3& GetVelocity() const { return velocity_; }

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	
	// キャラクターの当たり判定のサイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// 移動入力
	void InputMove();
	
	// 旋回制御
	void AnimateTurn();


	// マップとの当たり判定情報
	struct CollisionMapInfo {
		// 天井衝突フラグ
		bool ceiliing = false;
		// 着地フラグ
		bool landing = false;
		// 壁接触フラグ
		bool hitWall = false;
		// 移動量
		Vector3 move;
	};

	
	
	// 角
	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,

		kNumCorner
	};


	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	static inline const float kBlank = 1.0f;
	
	void CheckMapCollision(CollisionMapInfo& info);

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	void CheckMapMove(const CollisionMapInfo& info);

	void CheckMapCeiling(const CollisionMapInfo& info);

	// 5壁に接触しているときの判定
	void CheckMapWall(const CollisionMapInfo& info);

	// 接地状態の切り替え処理
	void CheckMapLanding(const CollisionMapInfo& info);

	static inline const float kAttenuationLanding = 1;

	// 微小な数値
	static inline const float kGroundSearchHeight = 0.1f;

	static inline const float kAttenuationWall = 0.5f;

	// ワールド座標を入れる変数
	Vector3 GetWorldPosition();

	// AABBを取得
	AABB GetAABB();

	// 衝突応答
	void OnCollision(const Enemy* enemy);

	// デスフラグ
	bool isDead_ = false;

	// デスフラグのgetter
	bool IsDead() const { return isDead_; }

	// ジャンプカウント
	int jumpCount_ = 0;
	// 最大ジャンプ回数
	int maxJump_ = 2;

	bool IsGoal() const { return isGoal_; }


};