// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Game/GameObject.h>

class City : public GameObject {
  public:
	City();
	virtual ~City() = default;

	void Tick() override;
	void Render(IRenderer& renderer) override;

  private:
	float mRadius;
	Zeron::Color mOutlineColor;
};