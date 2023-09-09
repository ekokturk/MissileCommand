// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Engine/Renderer2D.h>


class IGame;
class Renderer2D;

namespace Zeron
{
	class Window;
	class Platform;

	namespace Gfx
	{
		class Graphics;
		class GraphicsContext;
	}
}


class Engine {
  public:
	Engine(Zeron::Platform& platform);
	virtual ~Engine();

	void Run(IGame* game);


  private:
	Zeron::Platform& mPlatform;
	Zeron::Window* mWindow;
	std::unique_ptr<Zeron::Gfx::Graphics> mGraphics;
	std::unique_ptr<Zeron::Gfx::GraphicsContext> mGraphicsContext;
	std::unique_ptr<Renderer2D> mRenderer;
};
