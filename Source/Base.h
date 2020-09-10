// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "GameObject.h"
#include "Engine/EngineTypes.h"

class AntiMissile;

class Base : public GameObject
{
public:
	Base(exEngineInterface * pEngine);
	virtual ~Base() = default;

	// Return current missile count
	int	MissileCount() const { return mCurrentMissiles; };

	// Set missiles to a certain amount (Both max and current)
	void SetMissileCount(int count);		

	// Reload missiles if it is not at max count
	void ReloadMissile();				

	// Indicate selected base
	void ShowSelection() const;				

	// Update and Render Base
	virtual void Run() override;			

	// Render Base
	virtual void Render() override;			

	// Create a new missile and return it
	AntiMissile * PrepareMissile();			

private:
	float	mWidth;							// Width of the base
	float	mHeight;						// Height of the base
	exColor	mSelectedColor;					// Color of selection indication

	int		mMaxMissiles;					// Max missiles that base can have
	int		mCurrentMissiles;				// Missiles that base currently has
	float	mMissileDelay;					// Delay between each missile launch
	float	mMissileTimer;					// Timer which counts for delay

};

