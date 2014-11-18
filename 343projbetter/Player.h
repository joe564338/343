#ifndef __PLAYER_H
#define __PLAYER_H
#include <iostream>
#include <SFML\Graphics.hpp> 
using namespace::std;
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
	Player() : mFuel(0), mXPosition(400), mYPosition(400), mLives(3), mScore(0) {
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
};
#endif