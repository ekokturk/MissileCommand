// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Game/Missile.h>

class GameObject;
class IEngine;

class AntiMissile : public Missile {

  public:
	AntiMissile();
	virtual ~AntiMissile() = default;

	void Tick() override;
	void Render(IRenderer& renderer) override;
	void Launch(const Zeron::Vec2& target) override;
	bool IsCollided(GameObject* objects) const override;
};
