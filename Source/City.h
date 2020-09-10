// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "GameObject.h"
#include "Engine/EngineTypes.h"

class City : public GameObject
{
public:
	City(exEngineInterface * pEngine);
	virtual ~City() = default;

	// Update and Render City
	virtual void Run() override;

	// Render City
	virtual void Render() override;							

private:
	float		mRadius;			// Radius of a city
	exColor		mOutlineColor;		// Secondary color of the city

};