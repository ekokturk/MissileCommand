// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Engine/IRenderer.h>
#include <Game/City.h>
#include <Game/EnemyMissile.h>
#include <Game/Missile.h>
#include <random>

EnemyMissile::EnemyMissile()
{
	mType = TypeEnemyMissile;
	mMissileSpeed = 0.8f;
}


void EnemyMissile::Tick()
{
	Missile::Tick();

	if (mIsAlive && mIsLaunch) {

		if (mIsExploding && !mIsCollapsing) {
			mMinRadius += mRadiusChange;
			if (mMinRadius >= mMaxRadius) {
				mIsCollapsing = true;
			}
		}
		else if (mIsCollapsing) {
			mMinRadius -= mRadiusChange;
			if (mMinRadius <= 0) {
				mIsAlive = false;
			}
		}
	}
}

void EnemyMissile::Render(IRenderer& renderer)
{
	if (mIsAlive && mIsLaunch) {
		if (!mIsExploding) {
			renderer.DrawLine(mInitialPosition, mPos, { 255, 0, 0, 255 }, 2);
			renderer.DrawCircle(mPos, mMinRadius, { 0, 255, 0, 255 }, 1);
		}
		else if (mIsExploding && !mIsCollapsing) {
			renderer.DrawCircle(mPos, mMinRadius, { 0, 255, 0, 255 }, 0);
		}
		else if (mIsCollapsing) {
			if (mMinRadius > 0) {
				renderer.DrawCircle(mPos, mMinRadius, { 0, 255, 0, 255 }, 0);
			}
		}
	}
}

void EnemyMissile::Launch(const Zeron::Vec2& target)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> randX(0, kViewportWidth);
	const float posX = static_cast<float>(randX(mt));
	mPos = { posX, -5 };
	mInitialPosition = mPos;
	mTarget = target;
	mSpeed = mMissileSpeed;
	mIsLaunch = true;
}

bool EnemyMissile::IsCollided(GameObject* object) const
{
	if (object->Type() == TypeCity) {
		if (Zeron::Vec2::Distance(object->Pos(), mPos) < mMinRadius) {
			return true;
		}
	}

	return false;
}
