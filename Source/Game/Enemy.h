// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

class AntiMissile;
class Player;
class EnemyMissile;
class IEngine;

class Enemy {
  public:
	Enemy();
	~Enemy();

	void SpawnEnemy(const Zeron::Vec2& targetPos);
	void Update(size_t tickCount, Player* player);
	void Render(IRenderer& renderer) const;

	size_t DestroyEnemyMissile(AntiMissile* antiMissile);

  private:
	std::vector<std::unique_ptr<EnemyMissile>> mMissiles;
	float mSpawnCounter;
	float mSpawnDelay;
};
