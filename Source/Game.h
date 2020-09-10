//
// * ENGINE-X
// * SAMPLE GAME
//
// + Game.h
// definition of MyGame, an implementation of exGameInterface
//

#pragma once

#include "GameInterface.h"
#include "Engine/EngineTypes.h"

class Player;
class Enemy;


//-----------------------------------------------------------------
//-----------------------------------------------------------------

class MyGame : public exGameInterface
{
public:

	MyGame();
	virtual	~MyGame();

	virtual void Initialize( exEngineInterface* pEngine ) override;

	virtual const char*	GetWindowName() const override;
	virtual void GetClearColor( exColor& color ) const override;

	virtual void OnEvent( SDL_Event* pEvent ) override;
	virtual void OnEventsConsumed() override;;

	virtual void Run( float fDeltaT ) override;

private:
	
	// Setup game world
	void Setup()  const;

	// Print background visual
	void Background() const;

	// Draw mouse cursor on viewport
	void DrawCursor(exVector2 pos, exColor color)  const;

	// Main menu 
	void MainMenu();

	// Show screen after player is defeated
	void DefeatScreen();

	// Reinitialize current state of the game
	void ClearGameState();								

	bool mIsMenu;			// Checks if it is main menu loop
	bool mIsGamePlay;		// Checks if it is gameplay loop
	bool mIsDefeat;			// Checks if player is defeated

	exEngineInterface* mEngine;

	Player* 	mPlayer;
	Enemy*		mEnemy;


};
