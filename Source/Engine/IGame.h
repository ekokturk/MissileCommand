// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Platform/SystemEvent.h>

namespace Zeron
{
	namespace Render
	{
		class Camera;
	}
}

class IGame {
  public:
	virtual void Initialize(const Zeron::Vec2i& viewSize) = 0;
	virtual const char* GetWindowName() const = 0;
	virtual Zeron::Color GetClearColor() const = 0;
	virtual void OnEvent(const Zeron::SystemEvent& evt) = 0;
	virtual void Tick() = 0;
	virtual void Render(IRenderer& renderer) = 0;
	virtual const Zeron::Render::Camera& GetCamera() const = 0;
};
