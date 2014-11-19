#ifndef __PLAYER_H
#define __PLAYER_H
#include <iostream>
#include <SFML\Graphics.hpp> 
using namespace::std;
const int FUEL_CAN = 50;
const int MAX_FUEL = 100;
class Player{
private:
	int mLives;
	int mXPosition;
	int mYPosition;
	int mFuel;
	int mScore;
	sf::Texture PlayerTexture;
	sf::Sprite PlayerSprite;
public:
	Player() : mFuel(0), mXPosition(800), mYPosition(800), mLives(3), mScore(0) {
		cout << "Game Start" << endl;
	}
	virtual ~Player() {
		cout << "Game Over" << endl;
	}
	int CreatePlayer();
	void Fly();
	void UpdateScore(int addedScore);
	void PlayerDeath();
	int GetX();
	int GetY();
	int GetFuel();
	int GetScore();
	int GetLives();
	void AddFuel();
	sf::Sprite getSprite();
};
#endif