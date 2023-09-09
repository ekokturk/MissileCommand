// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Engine/IRenderer.h>
#include <Game/AntiMissile.h>
#include <Game/EnemyMissile.h>
#include <Game/GameObject.h>

AntiMissile::AntiMissile()
{
	mType = TypeAntiMissile;
	mPos = { 0, 0 };
	mSpeed = 0;
	mMissileSpeed = 4;
	mRadiusChange = 0.8f;
}

void AntiMissile::Tick()
{
	Missile::Tick();
	// Check if state of the missile is valid
	if (mIsAlive && mIsLaunch) {
		// Increase radius when it explodes
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

void AntiMissile::Render(IRenderer& renderer)
{
	if (mIsAlive && mIsLaunch) {
		if (!mIsExploding) {
			renderer.DrawLine(mInitialPosition, mPos, { 255, 255, 255, 255 }, 3);
			renderer.DrawCircle(mPos, mMinRadius, { 0, 0, 255, 255 }, 3);
			renderer.DrawBox(mTarget + Zeron::Vec2{ -1, -8 }, mTarget + Zeron::Vec2{ 1, 8 }, { 255, 255, 255, 255 }, 0);
			renderer.DrawBox(mTarget + Zeron::Vec2{ -8, -1 }, mTarget + Zeron::Vec2{ 8, 1 }, { 255, 255, 255, 255 }, 0);
		}
		else if (mIsExploding && !mIsCollapsing) {
			renderer.DrawCircle(mPos, mMinRadius, { 0, 0, 255, 255 }, 0);
		}
		else if (mIsCollapsing) {
			if (mMinRadius > 0) {
				renderer.DrawCircle(mPos, mMinRadius, { 0, 0, 255, 255 }, 0);
			}
		}
	}
}

void AntiMissile::Launch(const Zeron::Vec2& target)
{
	mInitialPosition = mPos;
	mTarget = target;
	mSpeed = mMissileSpeed;
	mIsLaunch = true;
}

bool AntiMissile::IsCollided(GameObject* object) const
{
	if (mIsExploding && mIsAlive) {
		if (object->Type() == TypeEnemyMissile)	 // If object is an enemy missile
		{
			EnemyMissile* enemy = dynamic_cast<EnemyMissile*>(object);
			if (Zeron::Vec2::Distance(enemy->Pos(), mPos) < enemy->Radius() + mMinRadius) {
				return true;
			}
		}
	}

	return false;
}