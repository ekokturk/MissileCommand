// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Engine/IRenderer.h>
#include <Game/AntiMissile.h>
#include <Game/Base.h>
#include <Game/City.h>
#include <Game/Enemy.h>
#include <Game/EnemyMissile.h>
#include <Game/Player.h>
#include <random>

Player::Player()
	: mScore(0)
	, mScoreIncrement(10)
	, mSelectedBase(1)
	, mAttackDelay(1)
	, mAttackDelayCounter(0)
	, mIsReload(false)
{
	SetupBases(4);
	SetupCities();
}

Player::~Player() {}

void Player::SetScore(int pScore)
{
	mScore = pScore;
}


Zeron::Vec2 Player::GetRandomCity() const
{
	if (!IsLost()) {
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<int> city(0, mCities.size() - 1);
		int randCity = city(mt);
		while (!mCities[randCity]->IsAlive()) {
			randCity = city(mt);
		}
		return mCities[randCity]->Pos();
	}
	return { 0, 0 };
}

bool Player::IsLost() const
{
	for (auto&& city : mCities) {
		if (city->IsAlive()) {
			return false;
		}
	}
	return true;
}

void Player::Update(size_t tickCount, Enemy* enemy)
{
	for (auto&& mBase : mBases) {
		mBase->Tick();
	}

	for (auto&& city : mCities) {
		city->Tick();
	}

	for (auto&& missile : mMissiles) {
		missile->Tick();
		const size_t count = enemy->DestroyEnemyMissile(missile.get());
		mScore += mScoreIncrement * count;
	}

	if (mIsReload) {
		mAttackDelayCounter += 0.05f;
		if (mAttackDelayCounter >= mAttackDelay) {
			mAttackDelayCounter = 0;
			mIsReload = false;
		}
	}

	if (tickCount % 1000 == 0) {
		mMissiles.erase(
			std::remove_if(
				mMissiles.begin(),
				mMissiles.end(),
				[](const std::unique_ptr<AntiMissile>& missile) {
					return !missile->IsAlive();
				}
			),
			mMissiles.end()
		);
	}
}

void Player::Render(IRenderer& renderer) const
{
	for (unsigned int i = 0; i < mBases.size(); i++) {
		mBases[i]->Render(renderer);
		if (i == static_cast<unsigned int>(mSelectedBase)) {
			mBases[i]->ShowSelection(renderer);
		}
	}

	for (auto&& city : mCities) {
		city->Render(renderer);
	}

	for (auto&& missile : mMissiles) {
		missile->Render(renderer);
	}
}

void Player::LaunchAntiMissile()
{
	if (mBases[mSelectedBase]->MissileCount() > 0 && !mIsReload) {
		std::unique_ptr<AntiMissile> missile = mBases[mSelectedBase]->PrepareMissile();
		missile->Launch({ mCursorPos.X, mCursorPos.Y });
		mMissiles.push_back(std::move(missile));
		mIsReload = true;
	}
}


void Player::SetupBases(int missileCount)
{
	const float sideOffset = 60;
	const float incrementX = (kViewportWidth - 2 * sideOffset) / 2;
	const float posY = kViewportHeight - 20;
	float posX = sideOffset;

	for (int i = 0; i < 3; i++) {
		mBases.push_back(std::make_unique<Base>());
		mBases.back()->SetPos({ posX, posY });
		mBases.back()->SetMissileCount(missileCount);
		posX += incrementX;
	}
}

void Player::SetupCities()
{
	const float sideOffset = 120;
	const float incrementX = (kViewportWidth - 2 * sideOffset) / 5;
	float posX = sideOffset;
	const float posY = kViewportHeight + 5;

	for (int i = 0; i < 6; i++) {
		mCities.push_back(std::make_unique<City>());
		mCities.back()->SetPos({ posX, posY });
		posX += incrementX;
	}
}

void Player::MoveCursor(const Zeron::Vec2& pos)
{
	mCursorPos = pos;
}

void Player::AttackCities(const Missile* missile)
{
	for (auto&& city : mCities) {
		if (missile->IsCollided(city.get())) {
			city->SetAlive(false);
		}
	}
}

void Player::SelectBase(int baseIndex)
{
	ZE_ASSERT(baseIndex < static_cast<int>(mBases.size()) && baseIndex >= 0, "Invalid index");
	mSelectedBase = baseIndex;
}

bool Player::TrySelectBase(const Zeron::Vec2& pos)
{
	bool selected = false;
	for (size_t i = 0; i < mBases.size(); ++i) {
		if (mBases[i]->IsInside(pos)) {
			SelectBase(i);
			selected = true;
		}
	}
	return selected;
}
