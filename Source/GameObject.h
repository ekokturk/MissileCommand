// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "Engine/EngineTypes.h"

class exEngineInterface;

// Game object type
enum ObjectType {
	None = 0,
	TypeCity,
	TypeBase,
	TypeEnemyMissile,
	TypeAntiMissile,
};

// Gameobject abstract class
class GameObject
{
public:
	// Update its position and visuals
	virtual void Run() = 0;								

	// Render object in viewport
	virtual void Render() = 0;							

	// Get type
	ObjectType Type() const { return mType; }

	// Get Position
	exVector2 Pos() const { return mPos; }
	
	// Get Velocity direction (Normalized)
	exVector2 Direction() const { return mDirection; }

	// Get Velocity magnitude
	float Speed() const { return mSpeed; };

	// Get alive state of the gameobject
	bool IsAlive() const { return mIsAlive; };

	// Set alive state of the game object
	void SetAlive(bool state);

	// Set position
	void SetPos(exVector2 nPos);

	// Set color
	void SetColor(exColor nColor1);

	// Set layer
	void SetLayer(int nLayer);

	// Set direction vector
	void SetDirection(exVector2 pVelocity);

	// Set velocity magnitude
	void SetSpeed(float pSpeed);					

protected:
	GameObject(exEngineInterface * pEngine);
	~GameObject() = default;
	exEngineInterface * mEngine;
	
	int					mLayer;			// Layer of the gameobject
	exColor				mObjectColor;	// Color of the gameobject
	ObjectType			mType;			// Type of the gameobject

	bool				mIsAlive;		// Check if gameobject is active

	exVector2			mPos;			// Position of the gameobject
	float				mSpeed;			// Speed Magnitude
	exVector2			mDirection;		// Velocity direction (This is a normalized vector)
};

