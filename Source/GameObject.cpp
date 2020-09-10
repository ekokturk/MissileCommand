// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "GameObject.h"
#include <cmath>

GameObject::GameObject(exEngineInterface * pEngine) 
	: mEngine(pEngine),
	mLayer(0),
	mObjectColor({ 0, 0, 0, 255 }),
	mType(ObjectType::TypeCity),
	mIsAlive(true),
	mPos({0,0}),
	mSpeed(0),
	mDirection({0,0})
{
}


void GameObject::SetAlive(bool state)
{
	mIsAlive = state;
}

void GameObject::SetPos(exVector2 pPos)
{
	mPos = pPos;
}

void GameObject::SetLayer(int nLayer)
{
	mLayer = nLayer;
}

void GameObject::SetColor(exColor pColor)
{
	mObjectColor = pColor;
}

void GameObject::SetSpeed(float pSpeed)
{
	mSpeed = pSpeed;
}

void GameObject::SetDirection(exVector2 pVelocity)
{
	// Get velocity direction magnitude
	const float magnitude = sqrt(pow(pVelocity.x, 2) + pow(pVelocity.y, 2));
	mDirection = magnitude == 0 ? exVector2{ 0, 0 } : exVector2{ pVelocity.x / magnitude, pVelocity.y / magnitude };
}

