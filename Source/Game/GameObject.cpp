// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Game/GameObject.h>

GameObject::GameObject()
	: mLayer(0)
	, mObjectColor(Zeron::Color::White)
	, mType(ObjectType::TypeCity)
	, mIsAlive(true)
	, mPos({ 0, 0 })
	, mDirection({ 0, 0 })
	, mSpeed(0)
{}


void GameObject::SetAlive(bool state)
{
	mIsAlive = state;
}

void GameObject::SetPos(const Zeron::Vec2& pPos)
{
	mPos = pPos;
}

void GameObject::SetLayer(int nLayer)
{
	mLayer = nLayer;
}

void GameObject::SetColor(Zeron::Color pColor)
{
	mObjectColor = pColor;
}

void GameObject::SetSpeed(float pSpeed)
{
	mSpeed = pSpeed;
}

void GameObject::SetDirection(const Zeron::Vec2& pVelocity)
{
	// Get velocity direction magnitude
	const float magnitude = sqrt(pow(pVelocity.X, 2) + pow(pVelocity.Y, 2));
	mDirection = Zeron::Math::EqualsNear(magnitude, 0.f) ? Zeron::Vec2{ 0, 0 } : Zeron::Vec2{ pVelocity.X / magnitude, pVelocity.Y / magnitude };
}
