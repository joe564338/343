#include "Player.h"
int Player::CreatePlayer(){
	if (!PlayerTexture.loadFromFile("testplayer.png")){
		return EXIT_FAILURE;
	}
	PlayerSprite.setTexture(PlayerTexture);
	PlayerSprite.setOrigin(16.f, 16.f);
	PlayerSprite.setPosition(mXPosition, mYPosition);
	return 1;
}
void Player::Fly(){
	if (mFuel > 0){
		mFuel--;
	}
}
void Player::UpdateScore(int addedScore){
	mScore += addedScore;
}
void Player::PlayerDeath(){
	mLives--;
}
int Player::GetX(){
	return mXPosition;
}
int Player::GetY(){
	return mYPosition;
}
int Player::GetFuel(){
	return mFuel;
}
int Player::GetLives(){
	return mLives;
}
int Player::GetScore(){
	return mScore;
}
