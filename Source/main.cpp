// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Engine/Engine.h>
#include <Game/Game.h>
#include <Platform/EntryPoint.h>
#include <Platform/Platform.h>

bool Main(Zeron::Platform& platform, const Zeron::CommandLineArgs& args)
{
	Engine engine(platform);
	MissileCommand game;

	engine.Run(&game);

	return true;
}

ZERON_DECLARE_ENTRY_POINT(Main)