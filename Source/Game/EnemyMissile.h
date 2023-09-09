// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Game/Missile.h>

class GameObject;

class EnemyMissile : public Missile {

  public:
	EnemyMissile();
	virtual ~EnemyMissile() = default;

	void Tick() override;
	void Render(IRenderer& renderer) override;
	void Launch(const Zeron::Vec2& target) override;
	bool IsCollided(GameObject* objects) const override;
};
