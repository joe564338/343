#ifndef __ENEMY_H
#define __ENEMY_H
#include <string>
#include <SFML/Graphics.hpp>
using namespace::std;
class Enemy{
protected:
	static string mName;
	sf::Texture EnemyTexture;
	sf::Sprite EnemySprite;
public:
	virtual ~Enemy(){}
	virtual void AIFunction();
	virtual void CreateEnemy();
};
#endif