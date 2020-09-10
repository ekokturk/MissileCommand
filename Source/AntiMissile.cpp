// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Engine/EngineInterface.h"
#include "GameObject.h"
#include "MathLibrary.h"
#include "AntiMissile.h"
#include "EnemyMissile.h"

AntiMissile::AntiMissile(exEngineInterface * pEngine)
	: Missile(pEngine)
{
	mEngine = pEngine;
	mType = TypeAntiMissile;

	mPos = { 0, 0 };							// Initialize position
	mSpeed = 0;									// Initialize current speed speed
	mMissileSpeed = 4;							// Initialize launch speed
	mRadiusChange = 0.8f;						// Set change in radius
}

void AntiMissile::Render()
{
	// Check if state of the missile is valid
	if (mIsAlive && mIsLaunch)														
	{
		// Move until it explodes, draw a line
		if (!mIsExploding)															
		{
			mEngine->DrawLine(mInitialPosition, mPos, { 255,255,255,255 }, 2);
			mEngine->DrawCircle(mPos, mMinRadius, { 0,0,255,255 }, 2);
			mEngine->DrawBox(Math::Add(mTarget, { -1,-8 }), Math::Add(mTarget, { 1,8 }), {255,255,255,255}, 0);
			mEngine->DrawBox(Math::Add(mTarget, { -8,-1 }), Math::Add(mTarget, { 8,1 }), { 255,255,255,255 }, 0);
		}
		// Increase radius when it explodes
		else if (mIsExploding && !mIsCollapsing)									
		{		
			mMinRadius += mRadiusChange;
			mEngine->DrawCircle(mPos, mMinRadius, { 0,0,255,255 }, 0);
			if (mMinRadius >= mMaxRadius){
				mIsCollapsing = true;

			}
		}
		// Decrease radius when it contracts
		else if (mIsCollapsing)														
		{
			mMinRadius -= mRadiusChange;
			if (mMinRadius <= 0){
				mIsAlive = false;
			}
			else{
				mEngine->DrawCircle(mPos, mMinRadius, { 0,0,255,255 }, 0);
			}
		}
	}


}

void AntiMissile::Launch(exVector2 target)
{
	mInitialPosition = mPos;
	mTarget = target;
	mSpeed = mMissileSpeed;
	mIsLaunch = true;
}

bool AntiMissile::IsCollided(GameObject* object)
{
	if (mIsExploding && mIsAlive)
	{
		if (object->Type() == TypeEnemyMissile)											// If object is an enemy missile
		{
			EnemyMissile * enemy = dynamic_cast<EnemyMissile*>(object);					// Cast object to enemy missile
			if (Math::Distance(enemy->Pos(), mPos) < enemy->Radius() + mMinRadius)		// Check if collision is valid
			{
				return true;
			}
		}
	}

	return false;
}