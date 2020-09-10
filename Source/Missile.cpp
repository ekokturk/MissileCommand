// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "MathLibrary.h"
#include "Engine/EngineInterface.h"
#include "GameObject.h"
#include "Missile.h"


Missile::Missile(exEngineInterface * pEngine)
	: GameObject(pEngine),
	mInitialPosition({0,0}),
	mTarget({ 0,0 }),
	mMissileSpeed(1),
	mMinRadius(3),
	mMaxRadius(35),
	mRadiusChange(0.5f),
	mIsLaunch(false),
	mIsExploding(false),
	mIsCollapsing(false)
{
}

// Set target for the missile
void Missile::SetTarget(exVector2 target)
{
	mTarget = target;													// Set target position
	mDirection = Math::Normalize(Math::Substract(mTarget, mPos));		// Set direction
}

// Set missile to explode
void Missile::SetExploding()
{
	mIsExploding = true;
}

void Missile::Run()
{
	if (mIsAlive)
	{
		if (Math::Distance(mTarget, mPos) > mMinRadius && !mIsExploding)
		{
			SetTarget(mTarget);
			mPos.x += mDirection.x * mSpeed;
			mPos.y += mDirection.y * mSpeed;
		}
		else{
			mIsExploding = true;
		}
	}
	Render();
}