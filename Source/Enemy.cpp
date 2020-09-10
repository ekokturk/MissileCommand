// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Enemy.h"
#include "Player.h"
#include "City.h"
#include "EnemyMissile.h"
#include "Engine/EngineInterface.h"


Enemy::Enemy(exEngineInterface * pEngine)
	: mEngine(pEngine),
	mSpawnCounter(0),
	mSpawnDelay(10)
{
}


// Set engine 
void Enemy::SetEngine(exEngineInterface * pEngine)
{
	mEngine = pEngine;
}

// Update enemy objects with respected to the player
void Enemy::Update(Player * player)
{
	for (EnemyMissile * missile : mMissiles)							// Do every enemy missile 
	{
		missile->Run();													// Update and draw missile
		for (City * city : player->mCities)								// If it is collided with any cities
		{
			missile->IsCollided(dynamic_cast<GameObject*>(city));		// Set collision response
		}
	}
}


// Spawn enemy directed to a city
void Enemy::SpawnEnemy(exVector2 targetPos)
{
	if (targetPos.x != 0 && targetPos.y != 0)							// Check if there are available cities
	{
		mSpawnCounter += 0.1f;											// Count for spawn delay
		if (mSpawnCounter >= mSpawnDelay)								// Wait for the delay
		{
			mSpawnCounter = 0;											// Reset spawn counter
			EnemyMissile * missile = new EnemyMissile(mEngine);			// Create a new missile
			missile->Launch(targetPos);									// Move missile towards the target
			mMissiles.push_back(missile);								// Add missiles to container
		}
	}
}