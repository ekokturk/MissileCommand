// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include <vector>
#include "Engine/EngineTypes.h"

class Player;
class EnemyMissile;
class exEngineInterface;

class Enemy
{
public:
	Enemy(exEngineInterface * pEngine);
	virtual ~Enemy() = default;

	// Spawn an enemy missile with a target at a random position
	void SpawnEnemy(exVector2 targetPos);

	// Update enemy depending on the player
	void Update(Player * player);						

	// Set engine for the player
	void SetEngine(exEngineInterface * pEngine);		

	std::vector<EnemyMissile*> mMissiles;				// Missiles container of the enemy
private:
	
	exEngineInterface * mEngine;
	float mSpawnCounter;								// Count for spawn delay
	float mSpawnDelay;									// Delay time for spawn

};
