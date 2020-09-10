// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "Missile.h"
#include "Engine/EngineTypes.h"

class GameObject;
class exEngineInterface;

class AntiMissile : public Missile
{
	
public:
	
	AntiMissile(exEngineInterface * pEngine);
	virtual ~AntiMissile() = default;

	// Render AntiMissile
	virtual void Render() override;

	// Give missile a speed and target
	virtual void Launch(exVector2 target) override;

	// Check for collision
	virtual bool IsCollided(GameObject* objects) override;

};
