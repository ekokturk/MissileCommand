// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Engine/IRenderer.h>
#include <Game/AntiMissile.h>
#include <Game/Base.h>
#include <Game/City.h>
#include <Game/Enemy.h>
#include <Game/Game.h>
#include <Game/Player.h>
#include <Platform/SystemEvent.h>
#include <Render/Camera/Camera.h>

MissileCommand::MissileCommand()
	: mGameState(GameState::MainMenu)
	, mCamera(std::make_unique<Zeron::Render::Camera>(Zeron::Vec2{ static_cast<float>(kViewportWidth), static_cast<float>(kViewportHeight) }, 0.f, 1.f))
	, mTickCount(0)
{}

MissileCommand::~MissileCommand() {}

void MissileCommand::Initialize(const Zeron::Vec2i& viewSize)
{
	mViewSize = Zeron::Vec2(static_cast<float>(viewSize.X), static_cast<float>(viewSize.Y));
}

const char* MissileCommand::GetWindowName() const
{
	return "Missile Command";
}

Zeron::Color MissileCommand::GetClearColor() const
{
	return Zeron::Color::Black;
}

void MissileCommand::OnEvent(const Zeron::SystemEvent& evt)
{
	std::visit(
		Zeron::TypeTraits::Visitor{
			[&](const Zeron::SystemEvent::KeyDown& data) {
				switch (static_cast<Zeron::KeyCode::Type>(data.mCode)) {
					case Zeron::KeyCode::Enter: {
						if (mGameState == GameState::MainMenu) {
							_startGamePlay();
						}
						else if (mGameState == GameState::Defeat) {
							_startGamePlay();
						}
					} break;
					case Zeron::KeyCode::N1: {
						if (mGameState == GameState::Playing) {
							mPlayer->SelectBase(0);
						}
					} break;
					case Zeron::KeyCode::N2: {
						if (mGameState == GameState::Playing) {
							mPlayer->SelectBase(1);
						}
					} break;
					case Zeron::KeyCode::N3: {
						if (mGameState == GameState::Playing) {
							mPlayer->SelectBase(2);
						}
					} break;
					default: break;
				}
			},
			[&](const Zeron::SystemEvent::MouseMoved& data) {
				if (mGameState == GameState::Playing) {
					mPlayer->MoveCursor(_getAdjustedCursorPos(data.mPosX, data.mPosY));
				}
			},
			[&](const Zeron::SystemEvent::MouseButtonDown& data) {
				if (data.mCode == Zeron::MouseCode::LeftButton) {
					if (mGameState == GameState::Playing) {
						const Zeron::Vec2 cursorPos = _getAdjustedCursorPos(data.mPosX, data.mPosY);
						mPlayer->MoveCursor(cursorPos);

						if (!mPlayer->TrySelectBase(cursorPos)) {
							mPlayer->LaunchAntiMissile();
						}
					}
					else {
#if ZE_PLATFORM_ANDROID
						_startGamePlay();
#endif
					}
				}
			},
			[&](const Zeron::SystemEvent::WindowResized& data) {
				mViewSize = { static_cast<float>(data.mWidth), static_cast<float>(data.mHeight) };
			},
			[](const auto&) {},
		},
		evt.GetData()
	);
}

const Zeron::Render::Camera& MissileCommand::GetCamera() const
{
	return *mCamera;
}

void MissileCommand::Tick()
{
	switch (mGameState) {
		case GameState::MainMenu: {
		} break;
		case GameState::Playing: {
			_updateGamePlay();
		} break;
		case GameState::Defeat: {
		} break;
	}
	++mTickCount;
}

void MissileCommand::Render(IRenderer& renderer)
{
	_renderBackground(renderer);
	switch (mGameState) {
		case GameState::MainMenu: {
			_renderMainMenu(renderer);
		} break;
		case GameState::Playing: {
			_renderGamePlay(renderer);
		} break;
		case GameState::Defeat: {
			_renderDefeatScreen(renderer);
		} break;
	}
}

void MissileCommand::_drawCursor(IRenderer& renderer, const Zeron::Vec2& pos, const Zeron::Color& color) const
{
	renderer.DrawCircle(pos, 7, color, 0);
	renderer.DrawBox(pos + Zeron::Vec2{ -1, -15 }, pos + Zeron::Vec2{ 1, 15 }, color, 0);
	renderer.DrawBox(pos + Zeron::Vec2{ -15, -1 }, pos + Zeron::Vec2{ 15, 1 }, color, 0);
	renderer.DrawCircle(pos, 3, { 255, 255, 255, 255 }, 0);
}

void MissileCommand::_renderBackground(IRenderer& renderer) const
{
	renderer.DrawBox({ 0, kViewportHeight - 50 }, { kViewportWidth, kViewportHeight }, { 183, 112, 62, 255 }, 10);
}

void MissileCommand::_renderMainMenu(IRenderer& renderer) const
{
	renderer.DrawText(1, { 75, 200 }, "MISSILE COMMAND", { 0, 0, 255, 255 }, 1);
	renderer.DrawText(0, { 220, 300 }, "PRESS ENTER TO PLAY", { 255, 255, 255, 255 }, 1);
#if ZE_PLATFORM_ANDROID
	renderer.DrawText(2, { 245, 400 }, "Tap To Launch Missile", { 80, 80, 80, 255 }, 1);
	renderer.DrawText(2, { 255, 420 }, "Tap Base To Activate It", { 80, 80, 80, 255 }, 1);
#else
	renderer.DrawText(2, { 245, 400 }, "Left Click To Launch Missile", { 80, 80, 80, 255 }, 1);
	renderer.DrawText(2, { 255, 420 }, "1-2-3 Keys To Change Bases", { 80, 80, 80, 255 }, 1);
#endif
	renderer.DrawText(2, { 160, 440 }, "Available Missiles Shown As Numbers On Bases", { 80, 80, 80, 255 }, 1);
}

void MissileCommand::_renderGamePlay(IRenderer& renderer) const
{
	mPlayer->Render(renderer);
	mEnemy->Render(renderer);

	const auto& cursor = mPlayer->GetCursorPos();
	_drawCursor(renderer, { cursor.X, cursor.Y }, { 0, 0, 255, 255 });
	renderer.DrawText(0, { 350, 50 }, "SCORE", { 80, 80, 80, 255 }, -10);
	renderer.DrawText(0, { 360, 80 }, std::to_string(mPlayer->Score()).c_str(), { 80, 80, 80, 255 }, 0);
}

void MissileCommand::_renderDefeatScreen(IRenderer& renderer) const
{
	renderer.DrawText(1, { 55, 200 }, "YOU ARE DEFEATED", { 255, 0, 0, 255 }, 1);
	renderer.DrawText(0, { 330, 300 }, ("SCORE " + std::to_string(mPlayer->Score())).c_str(), { 180, 180, 180, 255 }, 1);
	renderer.DrawText(0, { 190, 400 }, "PRESS ENTER TO PLAY AGAIN", { 255, 255, 255, 255 }, 1);
}

void MissileCommand::_startGamePlay()
{
	mGameState = GameState::Playing;
	mPlayer = std::make_unique<Player>();
	mEnemy = std::make_unique<Enemy>();
}
Zeron::Vec2 MissileCommand::_getAdjustedCursorPos(int mouseX, int mouseY) const
{
	return { static_cast<float>(mouseX) * kViewportWidth / mViewSize.X, static_cast<float>(mouseY) * kViewportHeight / mViewSize.Y };
}

void MissileCommand::_updateGamePlay()
{
	mPlayer->Update(mTickCount, mEnemy.get());
	mEnemy->Update(mTickCount, mPlayer.get());
	mEnemy->SpawnEnemy(mPlayer->GetRandomCity());
	if (mPlayer->IsLost()) {
		mGameState = GameState::Defeat;
	}
}