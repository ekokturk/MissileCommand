// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

class IRenderer;

enum ObjectType
{
	None = 0,
	TypeCity,
	TypeBase,
	TypeEnemyMissile,
	TypeAntiMissile,
};

class GameObject {
  public:
	virtual void Tick() = 0;
	virtual void Render(IRenderer& renderer) = 0;
	ObjectType Type() const { return mType; }
	const Zeron::Vec2& Pos() const { return mPos; }
	const Zeron::Vec2& Direction() const { return mDirection; }
	float Speed() const { return mSpeed; };
	bool IsAlive() const { return mIsAlive; };
	void SetAlive(bool state);
	void SetPos(const Zeron::Vec2& nPos);
	void SetColor(Zeron::Color nColor1);
	void SetLayer(int nLayer);
	void SetDirection(const Zeron::Vec2& pVelocity);
	void SetSpeed(float pSpeed);

  protected:
	GameObject();
	~GameObject() = default;

	int mLayer;
	Zeron::Color mObjectColor;
	ObjectType mType;

	bool mIsAlive;

	Zeron::Vec2 mPos;
	Zeron::Vec2 mDirection;
	float mSpeed;
};
