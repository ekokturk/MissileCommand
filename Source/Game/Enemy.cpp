// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Engine/IRenderer.h>
#include <Game/AntiMissile.h>
#include <Game/City.h>
#include <Game/Enemy.h>
#include <Game/EnemyMissile.h>
#include <Game/Player.h>


Enemy::Enemy()
	: mSpawnCounter(0)
	, mSpawnDelay(10)
{}

Enemy::~Enemy() {}

void Enemy::Update(size_t tickCount, Player* player)
{
	for (auto&& missile : mMissiles) {
		missile->Tick();
		player->AttackCities(missile.get());
	}

	if (tickCount % 1000 == 0) {
		mMissiles.erase(
			std::remove_if(
				mMissiles.begin(),
				mMissiles.end(),
				[](const std::unique_ptr<EnemyMissile>& missile) {
					return !missile->IsAlive();
				}
			),
			mMissiles.end()
		);
	}
}

void Enemy::Render(IRenderer& renderer) const
{
	for (auto&& missile : mMissiles) {
		missile->Render(renderer);
	}
}

size_t Enemy::DestroyEnemyMissile(AntiMissile* antiMissile)
{
	size_t count = 0;
	for (auto&& e : mMissiles) {
		if (antiMissile->IsCollided(e.get())) {
			if (!e->IsExploding()) {
				++count;
			}
			e->SetExploding();
		}
	}
	return count;
}

void Enemy::SpawnEnemy(const Zeron::Vec2& targetPos)
{
	if (!Zeron::Math::EqualsNear(targetPos.X, 0.f) && !Zeron::Math::EqualsNear(targetPos.Y, 0.f)) {
		mSpawnCounter += 0.1f;
		if (mSpawnCounter >= mSpawnDelay) {
			mSpawnCounter = 0;
			std::unique_ptr<EnemyMissile> missile = std::make_unique<EnemyMissile>();
			missile->Launch(targetPos);
			mMissiles.push_back(std::move(missile));
		}
	}
}