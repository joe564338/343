#ifndef __ENEMYROBOT_H
#define _ENEMYROBOT_H
#include <string>
#include "Enemy.h"
#include <SFML/Graphics.hpp>
using namespace::std;
class EnemyRobot : public Enemy{
public:
	EnemyRobot();
	virtual void AIFunction();//inherritted from Enemy
	virtual void CreateEnemy();//inherritted from Enemy
};
#endif