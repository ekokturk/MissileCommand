// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include <random>
#include "Engine/EngineInterface.h"
#include "Engine/EngineTypes.h"
#include "SDL.h"
#include "Enemy.h"
#include "Player.h"


#include "AntiMissile.h"
#include "Base.h"
#include "City.h"
#include "EnemyMissile.h"


Player::Player():
	mouseX(0),
	mouseY(0),
	mScore(0),
	mScoreIncrement(10),
	mSelectedBase(1),
	mAttackDelay(1),
	mAttackDelayCounter(0),
	mIsReload(false)
{
}


// Set player Score
void Player::SetScore(int pScore)
{
	mScore = pScore;
}


// Return a random city
exVector2 Player::RandomCity()
{
	if (!IsLost())																// Check if player is lost
	{
		std::random_device rd;													// Get a random city
		std::mt19937 mt(rd());
		std::uniform_int_distribution<int> city(0, mCities.size() - 1);
		int randCity = city(mt);
		while (!mCities[randCity]->IsAlive())									// Find an alive city if it is already destroyed
		{
			randCity = city(mt);
		}
		return mCities[randCity]->Pos();										// Return the position of the random city
	}
	else{
		return { 0, 0 };
	}

}

// Check if player is lost (If cities got destroyed)
bool Player::IsLost()
{
	for (City * city : mCities)								
	{
		if (city->IsAlive()){
			return false;
		}
	}
	return true;
}

void Player::Update(Enemy * enemy)
{
	// Update bases
	for (unsigned int i = 0; i < mBases.size(); i++)
	{
		mBases[i]->Run();
		if (i == static_cast<unsigned int>(mSelectedBase)){
			mBases[i]->ShowSelection();
		}
	}

	// Update cities
	for (City * city : mCities)
	{
		city->Run();
	}

	// Player missile collision
	for (AntiMissile * missile : mMissiles)									
	{
		missile->Run();
		// Check for collision between enemy missiles
		for (EnemyMissile * e : enemy->mMissiles)						
		{
			if (missile->IsCollided(dynamic_cast<GameObject*>(e)))
			{
				// Check if enemy is already exploding
				if(!e->IsExploding()){
					// Add score otherwise
					mScore += mScoreIncrement;								
				}
				// Explode enemy missile
				e->SetExploding();										
			}
		}
	}

	// Set attack delay
	if (mIsReload)
	{
		mAttackDelayCounter += 0.05f;										
		if (mAttackDelayCounter >= mAttackDelay)
		{
			mAttackDelayCounter = 0;
			mIsReload = false;
		}
	}
}

// Launch missile from base
void Player::LaunchAntiMissile(exVector2 target)
{
	if (mBases[mSelectedBase]->MissileCount() > 0 && !mIsReload)
	{
		AntiMissile * missile = mBases[mSelectedBase]->PrepareMissile();
		missile->Launch(target);
		mMissiles.push_back(missile);
		exVector2 base = mBases[mSelectedBase]->Pos();
		mIsReload = true;
	}
}


void Player::SetupBases(int missileCount)
{
	// Offset from window
	const float sideOffset = 40;
	// Space between bases
	const float incrementX = (kViewportWidth - 2 * sideOffset)/2;	
	const float posY = kViewportHeight-20;						
	float posX = sideOffset;										
	
	for (auto *base : mBases)
	{
		// Set base position
		base->SetPos({ posX,  posY });						
		base->SetMissileCount(missileCount);
		// For next base
		posX += incrementX;										
	}
}

void Player::SetupCities()
{
	// Offset from window
	const float sideOffset = 100;
	// Space between bases
	const float incrementX = (kViewportWidth - 2 * sideOffset) / 5;	
	float posX = sideOffset;									
	const float posY = kViewportHeight + 5;							

	for (auto* city : mCities)
	{
		// Set city position
		city->SetPos({ posX,  posY });
		// For next base
		posX += incrementX;										
	}
}

void Player::MouseEvents()
{
	// Set mouse location
	SDL_GetMouseState(&mouseX, &mouseY);		

	// Attack Input
	if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT))	
	{
		// Launch a missile
		LaunchAntiMissile({ static_cast<float>(mouseX), static_cast<float>(mouseY) });			
	}
}

void Player::SelectBase()
{
	if (SDL_GetKeyboardState(0)[SDL_SCANCODE_1])					// Number 1 for base 1
		mSelectedBase = 0;
	else if (SDL_GetKeyboardState(0)[SDL_SCANCODE_2])				// Number 2 for base 2
		mSelectedBase = 1;
	if (SDL_GetKeyboardState(0)[SDL_SCANCODE_3])					// Number 3 for base 3
		mSelectedBase = 2;
}
