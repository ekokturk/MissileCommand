// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Engine/IGame.h>

namespace Zeron
{
	namespace Render
	{
		class Camera;
	}
	class SystemEvent;
}
class Player;
class Enemy;

enum class GameState
{
	MainMenu,
	Playing,
	Defeat
};

class MissileCommand : public IGame {
  public:
	MissileCommand();
	virtual ~MissileCommand();

	void Initialize(const Zeron::Vec2i& viewSize) override;
	void Tick() override;
	void Render(IRenderer& renderer) override;
	void OnEvent(const Zeron::SystemEvent& evt) override;
	const char* GetWindowName() const override;
	Zeron::Color GetClearColor() const override;
	const Zeron::Render::Camera& GetCamera() const override;

  private:
	void _startGamePlay();
	Zeron::Vec2 _getAdjustedCursorPos(int mouseX, int mouseY) const;
	void _updateGamePlay();

	void _renderBackground(IRenderer& renderer) const;
	void _renderMainMenu(IRenderer& renderer) const;
	void _renderGamePlay(IRenderer& renderer) const;
	void _drawCursor(IRenderer& renderer, const Zeron::Vec2& pos, const Zeron::Color& color) const;
	void _renderDefeatScreen(IRenderer& renderer) const;

	GameState mGameState;
	Zeron::Vec2 mViewSize;
	std::unique_ptr<Zeron::Render::Camera> mCamera;
	std::unique_ptr<Player> mPlayer;
	std::unique_ptr<Enemy> mEnemy;
	size_t mTickCount;
};
