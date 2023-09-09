// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Game/GameObject.h>

class AntiMissile;

class Base : public GameObject {
  public:
	Base();
	virtual ~Base() = default;

	void Tick() override;
	void Render(IRenderer& renderer) override;

	std::unique_ptr<AntiMissile> PrepareMissile();
	void SetMissileCount(int count);
	void ReloadMissile();
	void ShowSelection(IRenderer& renderer) const;

	int MissileCount() const { return mCurrentMissiles; }
	bool IsInside(const Zeron::Vec2& pos) const;

  private:
	float mWidth;
	float mHeight;
	Zeron::Color mSelectedColor;

	int mMaxMissiles;
	int mCurrentMissiles;
	float mMissileDelay;
	float mMissileTimer;
};
