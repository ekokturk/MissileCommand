// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Engine/IRenderer.h>
#include <Game/City.h>
#include <Game/GameObject.h>


City::City()
	: mRadius(30)
	, mOutlineColor({ 70, 83, 104, 255 })
{
	mType = TypeCity;
	mLayer = 3;
	mObjectColor = { 128, 150, 186, 255 };
}


void City::Render(IRenderer& renderer)
{
	if (mIsAlive) {
		renderer.DrawCircle(mPos, mRadius + 4, mOutlineColor, mLayer + 1);
		renderer.DrawCircle(mPos, mRadius, mObjectColor, mLayer);
	}
}

void City::Tick() {}
