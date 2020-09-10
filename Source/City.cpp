// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Engine/EngineInterface.h"
#include "GameObject.h"
#include "City.h"


City::City(exEngineInterface * pEngine)
	: GameObject(pEngine),
	mRadius(30),
	mOutlineColor({ 70, 83, 104, 255 })
{
	mEngine = pEngine;
	mType = TypeCity;
	mLayer = 3;
	mObjectColor = { 128, 150, 186, 255 };
}


void City::Render()
{
	if (mIsAlive) 
	{
		mEngine->DrawCircle(mPos, mRadius+4, mOutlineColor, mLayer+1);		
		mEngine->DrawCircle(mPos, mRadius,	 mObjectColor,  mLayer);		
	}
}

void City::Run()
{
	Render();
}
