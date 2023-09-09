// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

class Enemy;
class City;
class Base;
class AntiMissile;

class Player {

  public:
	Player();
	~Player();

	void Update(size_t tickCount, Enemy* enemy);
	void Render(IRenderer& renderer) const;

	int Score() const { return mScore; }
	const Zeron::Vec2& GetCursorPos() const { return mCursorPos; }
	void SetScore(int pScore);
	Zeron::Vec2 GetRandomCity() const;
	bool IsLost() const;
	void SetupBases(int missileCount);
	void SetupCities();
	void LaunchAntiMissile();
	void SelectBase(int baseIndex);
	bool TrySelectBase(const Zeron::Vec2& pos);
	void MoveCursor(const Zeron::Vec2& pos);
	void AttackCities(const Missile* missile);

  protected:
	Zeron::Vec2 mCursorPos;

	int mScore;
	int mScoreIncrement;
	int mSelectedBase;

	float mAttackDelay;
	float mAttackDelayCounter;
	bool mIsReload;

	std::vector<std::unique_ptr<City>> mCities;
	std::vector<std::unique_ptr<Base>> mBases;
	std::vector<std::unique_ptr<AntiMissile>> mMissiles;
};
