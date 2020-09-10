//
// * ENGINE-X
// * SAMPLE GAME
//
// + Game.cpp
// implementation of MyGame, an implementation of exGameInterface
//

#include "Game.h"
#include "Engine/EngineInterface.h"

#include <string>

#include "AntiMissile.h"
#include "SDL.h"
#include "MathLibrary.h"
#include "Base.h"
#include "City.h"
#include "Enemy.h"
#include "EnemyMissile.h"
#include "Player.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------

const char* gWindowName = "Missile Command";

//-----------------------------------------------------------------
//-----------------------------------------------------------------

MyGame::MyGame()
	: mIsMenu(true)
	  , mIsGamePlay(false)
	  , mIsDefeat(false)
	  , mEngine(nullptr)
	  , mPlayer(nullptr)
	  , mEnemy(nullptr)

{
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

MyGame::~MyGame()
{
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

void MyGame::Initialize( exEngineInterface* pEngine )
{
	mEngine = pEngine;
	mPlayer = new Player();
	mEnemy = new Enemy(mEngine);
	mEngine->LoadFont("GALACTIC.ttf", 25);
	mEngine->LoadFont("GALACTIC.ttf", 60);
	mEngine->LoadFont("GALACTIC.ttf", 14);
	Setup();

}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

const char* MyGame::GetWindowName() const
{
	return gWindowName;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

void MyGame::GetClearColor( exColor& color ) const
{
	color.mColor[0] = 0;
	color.mColor[1] = 0;
	color.mColor[2] = 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

void MyGame::OnEvent( SDL_Event* pEvent )
{


}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

void MyGame::OnEventsConsumed()
{
	int nKeys = 0;
	const Uint8 *pState = SDL_GetKeyboardState( &nKeys );


}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

void MyGame::Run( float fDeltaT )
{

	
	Background();

	if (mIsMenu){
		MainMenu();
	}

	else if (mIsGamePlay)											
	{
		mPlayer->Update(mEnemy);
		mEnemy->Update(mPlayer);
		mEnemy->SpawnEnemy(mPlayer->RandomCity());
		// Inputs
		mPlayer->MouseEvents();
		mPlayer->SelectBase();										// Base select Input
		DrawCursor(mPlayer->MousePos(), { 0,0,255,255 });
		mEngine->DrawText(0, { 350,50 }, "SCORE", { 80,80,80,255 }, -10);
		mEngine->DrawText(0, { 360,80 } , std::to_string(mPlayer->Score()).c_str(), { 80,80,80,255 }, 0);

		if (mPlayer->IsLost())
		{
			mIsDefeat = true;
			mIsGamePlay = false;
		}
	}
	else if (mIsDefeat)
	{
		DefeatScreen();
	}


}

// Initialize game 
void MyGame::Setup()  const
{
	for (int i = 0; i < 6; i++)								// Create cities
	{
		mPlayer->mCities.push_back(new City(mEngine));
	}

	for (int i = 0; i < 3; i++)								// Create bases
	{
		mPlayer->mBases.push_back(new Base(mEngine));
	}

	mPlayer->SetupBases(4);									// Initialize bases with 4 missile capacity
	mPlayer->SetupCities();									// Initialize cities
}

// Draw game background
void MyGame::Background()  const
{
	mEngine->DrawBox({ 0, kViewportHeight - 50 }, { kViewportWidth, kViewportHeight }, { 183, 112, 62, 255 }, 10);
}

// Draw cursor target
void MyGame::DrawCursor(exVector2 pos, exColor color)  const
{
	mEngine->DrawCircle(pos, 7, color, 0);
	mEngine->DrawBox(Math::Add(pos, { -1,-15 }), Math::Add(pos, { 1,15 }), color, 0);
	mEngine->DrawBox(Math::Add(pos, {-15,-1}), Math::Add(pos, { 15,1 }), color, 0);
	mEngine->DrawCircle(pos, 3, {255, 255, 255, 255}, 0);


}

// Show main menu screen
void MyGame::MainMenu()
{
	mEngine->DrawText(1, { 75,200 }, "MISSILE COMMAND", { 0,0,255,255 }, 1);
	mEngine->DrawText(0, { 220,300 }, "PRESS ENTER TO PLAY", { 255,255,255,255 }, 1);
	mEngine->DrawText(2, { 245,400 }, "Left Click To Launch Missile", { 80,80,80,255 }, 1);
	mEngine->DrawText(2, { 255,420 }, "1-2-3 Keys To Change Bases", { 80,80,80,255 }, 1);
	mEngine->DrawText(2, { 160,440 }, "Available Missiles Shown As Numbers On Bases", { 80,80,80,255 }, 1);
	if (SDL_GetKeyboardState(0)[SDL_SCANCODE_RETURN])
	{
		mIsMenu = false;
		mIsGamePlay = true;
	}
}

// Show main menu screen
void MyGame::DefeatScreen()
{
	mEngine->DrawText(1, { 55,200 }, "YOU ARE DEFEATED", { 255,0,0,255 }, 1);
	mEngine->DrawText(0, { 330,300 }, ("SCORE " + std::to_string(mPlayer->Score())).c_str(), { 180,180,180, 255 }, 1);
	mEngine->DrawText(0, { 190,400 }, "PRESS ENTER TO PLAY AGAIN", { 255,255,255,255 }, 1);
	if (SDL_GetKeyboardState(0)[SDL_SCANCODE_RETURN])
	{
		mIsDefeat = false;
		mIsGamePlay = true;
		ClearGameState();
	}
}

void MyGame::ClearGameState()
{
	// Clear bases that player has from memory
	for (auto* mBase : mPlayer->mBases)
	{
		delete mBase;
	}
	mPlayer->mBases.clear();

	// Clear cities that player has from memory
	for (auto* mCitie : mPlayer->mCities)
	{
		delete mCitie;
	}
	mPlayer->mCities.clear();

	// Clear missiles that player has from memory
	for (auto* mMissile : mPlayer->mMissiles)
	{
		delete mMissile;
	}
	mPlayer->mMissiles.clear();

	// Clear missiles that enemy has from memory
	for (auto* mMissile : mEnemy->mMissiles)
	{
		delete mMissile;
	}
	mEnemy->mMissiles.clear();

	delete mPlayer;
	delete mEnemy;
	
	mPlayer = new Player();
	mEnemy = new Enemy(mEngine);

	Setup();
}