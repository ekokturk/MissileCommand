// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include <iostream>
#include <vector>
#include "Engine/EngineTypes.h"

class City;
class Base;
class AntiMissile;

class Player
{

public:
	Player();
	~Player() = default;

	// Get player score
	int Score() const { return mScore; };

	// Get mouse position
	exVector2 MousePos() const { return { static_cast<float>(mouseX), static_cast<float>(mouseY) }; };

	// Set score of the player
	void SetScore(int pScore);

	// Return the location of a random city which is still alive
	exVector2 RandomCity();

	// Check if player is lost
	bool IsLost();								

	// Initialize base locations
	void SetupBases(int missileCount);

	// Initialize city locations
	void SetupCities();

	// Update player with relation to enemy (collisions)
	void Update(Enemy * enemy);

	// Launch a missile
	void LaunchAntiMissile(exVector2 target);	

	// Inputs
	void SelectBase();							// Select base to attack
	void MouseEvents();							// Mouse inputs (cursor movement and attack click)


	std::vector<City*>			mCities;		// Cities player is required to defend
	std::vector<Base*>			mBases;			// Bases where the anti missiles are launched
	std::vector<AntiMissile*>	mMissiles;		// Missiles launched by the player

protected:
	int mouseX;						// Cursor X position
	int mouseY;						// Cursor Y position
	
	int mScore;						// Player score
	int mScoreIncrement;			// How much that the score will increase
	int mSelectedBase;				// Currently selected base to attack

	float	mAttackDelay;			// Delay for each attack
	float	mAttackDelayCounter;	// Counter for the delay
	bool	mIsReload;				// Checks if ammo amount is max or not to reaload
	



};

