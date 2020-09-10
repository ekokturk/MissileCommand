// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include <cmath>
#include <iostream>
#include <string>
#include <random>
#include "Engine/EngineInterface.h"
#include "GameObject.h"
#include "Base.h"
#include "AntiMissile.h"

Base::Base(exEngineInterface * pEngine)
	: GameObject(pEngine),
	mWidth (60),
	mHeight(40),
	mSelectedColor({ 0, 255, 0, 255 }),
	mMaxMissiles(5),
	mCurrentMissiles(mMaxMissiles),
	mMissileDelay(200),
	mMissileTimer(0)
{
	mEngine = pEngine;
	mType = TypeBase;

	mObjectColor = { 70, 70, 70, 255 };
}


void Base::SetMissileCount(int count)
{
	mMaxMissiles = count;
	mCurrentMissiles = count;
}


void Base::Render()
{
	const exVector2 leftSide = { mPos.x - mWidth / 2, mPos.y - mHeight / 2 };
	const exVector2 rightSide = { mPos.x + mWidth / 2, mPos.y + mHeight / 2 };

	mEngine->DrawBox(leftSide, rightSide, mObjectColor, 2);
	mEngine->DrawBox({leftSide.x + 10, leftSide.y - 20}, { rightSide.x - 10, rightSide.y - 20 }, mObjectColor, 2);
	mEngine->DrawText(0, { mPos.x, leftSide.y}, std::to_string(mCurrentMissiles).c_str(), { 255,255,255,255 }, 0);
}


void Base::Run() {
	ReloadMissile();
	Render();
}

void Base::ReloadMissile()
{
	if (mCurrentMissiles < mMaxMissiles)			// Check if missile is missing
	{
		mMissileTimer += 1.0f;						// Count time for reload
		if (mMissileTimer >= mMissileDelay)			// If the reload delay is reached
		{
			mMissileTimer = 0;						// Reset timer
			mCurrentMissiles++;						// increment missile count
		}
	}
}

void Base::ShowSelection() const
{
	const exVector2 leftSide = { mPos.x - mWidth / 2, mPos.y + mHeight / 2  - 10};
	const exVector2 rightSide = { mPos.x + mWidth / 2, mPos.y + mHeight / 2 };
	mEngine->DrawBox(leftSide, rightSide, mSelectedColor, 0);
}

// Create a new missile from base
AntiMissile* Base::PrepareMissile()
{
	AntiMissile * missile = new AntiMissile(mEngine);		// Create a new missile
	missile->SetPos(mPos);									// Set missile start position
	mCurrentMissiles--;										// Decrease missile count

	return missile;
}

