// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Engine/EngineInterface.h"

#include "Factory.h"
#include "GameObject.h"
#include "Base.h"
#include "City.h"
#include "AntiMissile.h"
#include "EnemyMissile.h"


// Initialize singleton instance
Factory* Singleton<Factory>::mMyInstance = nullptr;


// Create new object depending on the type
GameObject * Factory::CreateObject(exEngineInterface * pEngine, ObjectType objectType) {

	switch (objectType)
	{
		// Create a Base
		case ObjectType::TypeBase:
		{
			return new Base(pEngine);						
		}

		// Create a City
		case ObjectType::TypeCity:
		{
			return new City(pEngine);						
		}

		// Create a Anti Missile
		case ObjectType::TypeAntiMissile:
		{
			return new AntiMissile(pEngine);				
		}

		// Create a Enemy Missile
		case ObjectType::TypeEnemyMissile:
		{
			return new EnemyMissile(pEngine);				
		}
		default:
			return nullptr;
	}
}

