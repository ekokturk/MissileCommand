// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Engine/Engine.h>

#include <Engine/IGame.h>
#include <Engine/Renderer2D.h>
#include <Graphics/CommandBuffer.h>
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsContext.h>
#include <Graphics/GraphicsTypes.h>
#include <Platform/Platform.h>
#include <Platform/Window.h>


Engine::Engine(Zeron::Platform& platform)
	: mPlatform(platform)
	, mWindow(nullptr)
{}

Engine::~Engine() = default;

void Engine::Run(IGame* game)
{
	Zeron::WindowConfig config;
	config.mName = game->GetWindowName();
	config.mPreferredAPI = Zeron::WindowAPI::GLFW;
	config.mWidth = kViewportWidth;
	config.mHeight = kViewportHeight;
	mWindow = mPlatform.CreatePlatformWindow(config);

	mGraphics = Zeron::Gfx::Graphics::CreateGraphics(Zeron::Gfx::GraphicsType::Vulkan);
	mGraphics->Init();
	mGraphicsContext = mGraphics->CreateGraphicsContext();
	mGraphicsContext->Init(mWindow->GetSystemHandle(), mWindow->GetSize());
	mRenderer = std::make_unique<Renderer2D>(*mGraphics, *mGraphicsContext->GetSwapChainRenderPass());

	game->Initialize(mGraphicsContext->GetSwapChainSize());
	Zeron::Time::TickTimer ticker(Zeron::Time::Seconds(1.L / 60));


	bool isRunning = true;
	bool isSuspended = false;
	while (isRunning) {
		const uint64_t tickCount = ticker.Tick();
		mPlatform.Update();
		while (mWindow->HasSystemEvents()) {
			Zeron::SystemEvent e = mWindow->GetNextSystemEvent();
			std::visit(
				Zeron::TypeTraits::Visitor{
					[&](const Zeron::SystemEvent::WindowClosed&) {
						isRunning = false;
					},
					[&](const Zeron::SystemEvent::WindowMinimized&) {
						isSuspended = true;
					},
					[&](const Zeron::SystemEvent::WindowRestored&) {
						isSuspended = false;
					},
					[&](const Zeron::SystemEvent::WindowResized& data) {
						mGraphicsContext->ResizeSwapChain(Zeron::Vec2i(data.mWidth, data.mHeight));
					},
					[](const auto&) {},
				},
				e.GetData()
			);
			game->OnEvent(e);
		}

		for (uint64_t i = 0; i < tickCount; ++i) {
			game->Tick();
		}

		if (!isSuspended && isRunning) {
			game->Render(*mRenderer);
			mRenderer->PrepareBatch();

			const Zeron::Vec2i& viewportSize = mGraphicsContext->GetSwapChainSize();
			Zeron::Gfx::CommandBuffer& cmd = mGraphicsContext->BeginCommands();
			{
				cmd.Clear(game->GetClearColor());
				cmd.SetViewport(viewportSize);
				cmd.SetScissor(viewportSize);

				mGraphicsContext->BeginSwapChainRenderPass(cmd);
				{
					mRenderer->RenderBatched(cmd, game->GetCamera());
				}
				mGraphicsContext->EndSwapChainRenderPass(cmd);
			}
			mGraphicsContext->EndCommands();

			mGraphicsContext->Submit(cmd);
			mGraphicsContext->Present();
		}
	}
}
