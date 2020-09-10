// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include <random>
#include "Engine/EngineInterface.h"
#include "Missile.h"
#include "EnemyMissile.h"
#include "MathLibrary.h"
#include "City.h"

EnemyMissile::EnemyMissile(exEngineInterface * pEngine)
	: Missile(pEngine)
{
	mEngine = pEngine;						// Set Engine
	mType = TypeEnemyMissile;				// Set object type
	mMissileSpeed = 0.8f;					// Set missile speed at launch
}


// Draw missile
void EnemyMissile::Render()
{
	if (mIsAlive && mIsLaunch)													// Check if state of the missile is valid
	{
		if (!mIsExploding)														// Move until it explodes, draw a line
		{
			mEngine->DrawLine(mInitialPosition, mPos, { 255,0,0,255 }, 2);
			mEngine->DrawCircle(mPos, mMinRadius, { 0,255,0,255 }, 2);
		}
		else if (mIsExploding && !mIsCollapsing)								// Increase radius when it explodes
		{
			mMinRadius += mRadiusChange;
			mEngine->DrawCircle(mPos, mMinRadius, { 0,255,0,255 }, 0);
			if (mMinRadius >= mMaxRadius){
				mIsCollapsing = true;
			}
		}
		else if (mIsCollapsing)													// Decrease radius when it contracts
		{
			mMinRadius -= mRadiusChange;
			mMinRadius <= 0 ? mIsAlive = false :
				mEngine->DrawCircle(mPos, mMinRadius, { 0,255,0,255 }, 0);
		}
	}
}

// Lauch an enemy missile in random direction moving towards a target
void EnemyMissile::Launch(exVector2 target)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> randX(0, kViewportWidth);
	const float posX = static_cast<float>(randX(mt));
	mPos = { posX, -5 };										// Start at a random X location
	mInitialPosition = mPos;									// Set initial position
	mTarget = target;											// Set target
	mSpeed = mMissileSpeed;										// Set missile speed
	mIsLaunch = true;											// Launch missile

}

// Check if missile is collided
bool EnemyMissile::IsCollided(GameObject* object)
{
	if (object->Type() == TypeCity)								// If object is a city
	{
		City * city = dynamic_cast<City*>(object);				// Cast object to city
		if (Math::Distance(city->Pos(), mPos) < mMinRadius)		// Check if collision is valid
		{
			city->SetAlive(false);								// Destroy city
			return true;
		}
	}

	return false;
}
