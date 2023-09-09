// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Engine/IRenderer.h>
#include <Game/AntiMissile.h>
#include <Game/Base.h>
#include <Game/GameObject.h>

Base::Base()
	: mWidth(60)
	, mHeight(40)
	, mSelectedColor({ 0, 255, 0, 255 })
	, mMaxMissiles(5)
	, mCurrentMissiles(mMaxMissiles)
	, mMissileDelay(200)
	, mMissileTimer(0)
{
	mType = TypeBase;

	mObjectColor = { 70, 70, 70, 255 };
}

void Base::SetMissileCount(int count)
{
	mMaxMissiles = count;
	mCurrentMissiles = count;
}

void Base::Render(IRenderer& renderer)
{
	const Zeron::Vec2 leftSide = { mPos.X - mWidth / 2, mPos.Y - mHeight / 2 };
	const Zeron::Vec2 rightSide = { mPos.X + mWidth / 2, mPos.Y + mHeight / 2 };

	renderer.DrawBox({ leftSide.X + 10, leftSide.Y - 20 }, { rightSide.X - 10, rightSide.Y - 20 }, { 100, 100, 100, 255 }, 2);
	renderer.DrawBox(leftSide, rightSide, mObjectColor, 2);
	renderer.DrawText(0, { mPos.X, leftSide.Y }, std::to_string(mCurrentMissiles).c_str(), { 255, 255, 255, 255 }, 0);
}

void Base::Tick()
{
	ReloadMissile();
}

void Base::ReloadMissile()
{
	if (mCurrentMissiles < mMaxMissiles) {
		mMissileTimer += 1.0f;
		if (mMissileTimer >= mMissileDelay) {
			mMissileTimer = 0;
			mCurrentMissiles++;
		}
	}
}

void Base::ShowSelection(IRenderer& renderer) const
{
	const Zeron::Vec2 leftSide = { mPos.X - mWidth / 2, mPos.Y + mHeight / 2 - 10 };
	const Zeron::Vec2 rightSide = { mPos.X + mWidth / 2, mPos.Y + mHeight / 2 };
	renderer.DrawBox(leftSide, rightSide, mSelectedColor, 0);
}

bool Base::IsInside(const Zeron::Vec2& pos) const
{
	const Zeron::Vec2 leftSide = { mPos.X - mWidth / 2, mPos.Y - mHeight / 2 };
	const Zeron::Vec2 rightSide = { mPos.X + mWidth / 2, mPos.Y + mHeight / 2 };
	return leftSide.X < pos.X && pos.X <= rightSide.X && leftSide.Y < pos.Y && pos.Y <= rightSide.Y;
}

// Create a new missile from base
std::unique_ptr<AntiMissile> Base::PrepareMissile()
{
	std::unique_ptr<AntiMissile> missile = std::make_unique<AntiMissile>();
	missile->SetPos(mPos);
	mCurrentMissiles--;
	return missile;
}
