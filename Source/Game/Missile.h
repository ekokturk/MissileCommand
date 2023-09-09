// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Game/GameObject.h>

class Missile : public GameObject {
  public:
	virtual void Launch(const Zeron::Vec2& target) = 0;
	virtual bool IsCollided(GameObject* objects) const = 0;

	void Tick() override;
	float Radius() const { return mMinRadius; }
	bool IsExploding() const { return mIsExploding; }
	void SetExploding();
	void SetTarget(const Zeron::Vec2& target);

  protected:
	Missile();
	~Missile() = default;

	Zeron::Vec2 mInitialPosition;
	Zeron::Vec2 mTarget;
	float mMissileSpeed;

	float mMinRadius;
	float mMaxRadius;
	float mRadiusChange;

	bool mIsLaunch;
	bool mIsExploding;
	bool mIsCollapsing;
};
