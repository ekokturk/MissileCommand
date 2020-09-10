// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "Missile.h"

class GameObject;

class EnemyMissile	: public Missile
{

public:
	EnemyMissile(exEngineInterface * pEngine);
	virtual ~EnemyMissile() = default;

	// Render Missile
	virtual void Render() override;

	// Launch Missile
	virtual void Launch(exVector2 target) override;

	// Check for collision
	virtual bool IsCollided(GameObject* objects) override;				


};
