// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once


#include "GameObject.h"
#include "Singleton.h"

class exEngineInterface;

// Factory Class
class Factory : public Singleton<Factory>
{
public:
	friend class Singleton<Factory>;

	// Create a new object
	static GameObject * CreateObject(exEngineInterface * pEngine, ObjectType objectType);	

private:
	Factory() = default;
	~Factory() = default;
};

