// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Engine/IRenderer.h>
#include <Game/GameObject.h>
#include <Game/Missile.h>


Missile::Missile()
	: mInitialPosition({ 0, 0 })
	, mTarget({ 0, 0 })
	, mMissileSpeed(1)
	, mMinRadius(3)
	, mMaxRadius(35)
	, mRadiusChange(0.5f)
	, mIsLaunch(false)
	, mIsExploding(false)
	, mIsCollapsing(false)
{}

void Missile::SetTarget(const Zeron::Vec2& target)
{
	mTarget = target;
	mDirection = mTarget - mPos;
	// TODO: Use Zeron Normalize here
	mDirection = { mDirection.X / mDirection.Length(), mDirection.Y / mDirection.Length() };
}

void Missile::SetExploding()
{
	mIsExploding = true;
}

void Missile::Tick()
{
	if (mIsAlive) {
		if (Zeron::Vec2::Distance(mTarget, mPos) > mMinRadius && !mIsExploding) {
			SetTarget(mTarget);
			mPos.X += mDirection.X * mSpeed;
			mPos.Y += mDirection.Y * mSpeed;
		}
		else {
			mIsExploding = true;
		}
	}
}