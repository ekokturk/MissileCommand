// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "GameObject.h"

class Missile : public GameObject
{
public:

	virtual void Launch(exVector2 target) = 0;
	virtual bool IsCollided(GameObject* objects) = 0;	

	// Update and draw missile
	virtual void Run() override;									

	// Return the radius of the missile
	float Radius() const { return mMinRadius; };

	// Check if missile is exploding
	bool IsExploding() const { return mIsExploding; };

	// Set missile to explode
	void SetExploding();

	// Set target of the missile and direction vector
	void SetTarget(exVector2 target);			

protected:
	Missile(exEngineInterface * pEngine);
	~Missile() = default;

	exVector2	mInitialPosition;	// Initial position of the missile before launch
	exVector2	mTarget;			// Target which missile is going for
	float		mMissileSpeed;		// Speed of the missile

	float		mMinRadius;			// This is the radius that will change but it is also the starting radius
	float		mMaxRadius;			// Maximum size that a missile can reach
	float		mRadiusChange;		// How fast the radius will change
	
	bool		mIsLaunch;			// Check if missile is launched
	bool		mIsExploding;		// Check if missile is exploding
	bool		mIsCollapsing;		// Check if missile reached max size

};

