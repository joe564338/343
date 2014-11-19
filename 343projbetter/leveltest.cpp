#include "level.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <Box2D.h>

#include "Player.h"
using namespace::std;
void RedirectIOToConsole();
void CreateBox(b2World& World, int MouseX, int MouseY);
int main()
{
	string filepath = "C:/Users/Joseph/Documents/Visual Studio 2013/Projects/343projbetter/testmap.tmx";
	RedirectIOToConsole();
	b2Vec2 Gravity(0.f, 9.8f);
	b2World World(Gravity);
	Level level;
	level.LoadFromFile(filepath);
	sf::Vector2i tileSize = level.GetTileSize();
	std::vector<Object> collision = level.GetObjects("collision");
	for (int i = 0; i < collision.size(); i++)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(collision[i].rect.left + tileSize.x / 2 * (collision[i].rect.width / tileSize.x - 1),
			collision[i].rect.top + tileSize.y / 2 * (collision[i].rect.height / tileSize.y - 1));
		b2Body* body = World.CreateBody(&bodyDef);
		b2PolygonShape shape;
		shape.SetAsBox(collision[i].rect.width / 2, collision[i].rect.height / 2);
		body->CreateFixture(&shape, 1.0f);
	}
	sf::RenderWindow window;
	sf::Vector2i screenSize(1360, 760);
	window.create(sf::VideoMode(screenSize.x, screenSize.y), "Level.h test");
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);
	sf::View view;
	view.reset(sf::FloatRect(0.0f, 0.0f, screenSize.x, screenSize.y));
	view.setViewport(sf::FloatRect(0.0f, 0.0f, 2.0f, 2.0f));
	//Player player1;
	//player1.CreatePlayer();
	sf::Texture PlayerTexture;
	sf::Sprite PlayerSprite;
	if (!PlayerTexture.loadFromFile("testplayer.png")){
		return EXIT_FAILURE;
	}
	PlayerSprite.setTexture(PlayerTexture);
	PlayerSprite.setOrigin(16.f, 16.f);
	PlayerSprite.setPosition(800/SCALE, 800/ SCALE);
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
	myBodyDef.position = b2Vec2( 800/ SCALE, 800 / SCALE); //set the starting position
	myBodyDef.angle = 0; //set the starting angle
	b2Body* dynamicBody = World.CreateBody(&myBodyDef);
	b2PolygonShape player;
	player.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &player;
	boxFixtureDef.density = 1.f;
	boxFixtureDef.friction = 2.f;
	dynamicBody->CreateFixture(&boxFixtureDef);
	dynamicBody->SetFixedRotation(true);
	sf::Texture BoxTexture;
	if (!BoxTexture.loadFromFile("box.png", sf::IntRect(10, 10, 32, 32))){
		return EXIT_FAILURE;
	}
	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		b2Vec2 vel = dynamicBody->GetLinearVelocity();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
			vel.x = 3.5;
			vel.y = dynamicBody->GetLinearVelocity().y;
			dynamicBody->SetLinearVelocity(vel);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
			vel.x = -3.5;
			vel.y = dynamicBody->GetLinearVelocity().y;
			dynamicBody->SetLinearVelocity(vel);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
			if (vel.y == 0)
				dynamicBody->ApplyForce(b2Vec2(0, -500), dynamicBody->GetWorldCenter(), 0);
		}
		World.Step(1 / 60.f, 8, 3);

		window.clear(sf::Color::White);

		int BodyCount = 0;
		for (b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
		{
			if (BodyIterator->GetType() == b2_dynamicBody)
			{
				
				++BodyCount;
			}

		}
		b2Vec2 pos = dynamicBody->GetPosition();
		PlayerSprite.setPosition(SCALE * pos.x, SCALE * pos.y);
		//view.setCenter(pos.x + screenSize.x/8 , pos.y + screenSize.y/8);
		//window.setView(view);
		window.draw(PlayerSprite);
		//level.Draw(window);
		//player1.getSprite().setPosition(SCALE * pos.x, SCALE * pos.y);
		
		window.draw(PlayerSprite);
		window.display();
	}

	return 0;
}
void CreateBox(b2World& World, int MouseX, int MouseY)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(MouseX / SCALE, MouseY / SCALE);
	BodyDef.type = b2_dynamicBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

static const WORD MAX_CONSOLE_LINES = 500;
void RedirectIOToConsole()
{
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;

	// allocate a console for this app
	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen(hConHandle, "w");

	*stdout = *fp;

	setvbuf(stdout, NULL, _IONBF, 0);

	// redirect unbuffered STDIN to the console

	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen(hConHandle, "w");

	*stderr = *fp;

	setvbuf(stderr, NULL, _IONBF, 0);

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well
	ios::sync_with_stdio();
}