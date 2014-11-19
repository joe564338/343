#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream> 
#include <random>
#include "level.h"
using namespace::std;
/** We need this to easily convert between pixel and real-world coordinates*/


/** Create the base for the boxes to land */
void CreateGround(b2World& World, float X, float Y);

/** Create the boxes */
void CreateBox(b2World& World, int MouseX, int MouseY);
void RedirectIOToConsole();

int main()
{
	int playerx = 200;
	int playery = 200;
	string filepath = "C:/Users/Joseph/Documents/Visual Studio 2013/Projects/343projbetter/testmap.tmx";
	RedirectIOToConsole();
	/** Prepare the world */
	b2Vec2 Gravity(0.f, 9.8f);
	b2World World(Gravity);
	/** Prepare the window */
	sf::RenderWindow Window(sf::VideoMode(1360, 760, 32), "Test");
	Window.setFramerateLimit(60);
	Window.setVerticalSyncEnabled(true);
	sf::View view;
	view.reset(sf::FloatRect(0.0f, 0.0f, 1360, 1360));
	view.setViewport(sf::FloatRect(0.0f, 0.0f, 2.0f, 2.0f));
	Level level;
	level.LoadFromFile(filepath);
	sf::Vector2i tileSize = level.GetTileSize();
	std::vector<Object> collision = level.GetObjects("Collision");
	for (int i = 0; i < collision.size(); i++)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(collision[i].rect.left + tileSize.x / 2 * (collision[i].rect.width / tileSize.x - 1),
			collision[i].rect.top + tileSize.y / 2 * (collision[i].rect.height / tileSize.y - 1));
		b2Body* body = World.CreateBody(&bodyDef);
		b2PolygonShape shape;
		shape.SetAsBox(collision[i].rect.width /2, collision[i].rect.height /2);
		body->CreateFixture(&shape, 1.0f);
	}
	
	/** Prepare textures */
	sf::Texture GroundTexture;
	sf::Texture BoxTexture;
	sf::Texture PlayerTexture;
	if (!GroundTexture.loadFromFile("ground.png")){
		return EXIT_FAILURE;
	}
	if (!BoxTexture.loadFromFile("box.png", sf::IntRect(10,10,32,32))){
		return EXIT_FAILURE;
	}
	if (!PlayerTexture.loadFromFile("testplayer.png")){
		return EXIT_FAILURE;
	}
	sf::Sprite PlayerSprite;
	PlayerSprite.setTexture(PlayerTexture);
	PlayerSprite.setOrigin(16.f, 16.f);
	PlayerSprite.setPosition(playerx, playery);
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
	myBodyDef.position = b2Vec2(playerx / SCALE, playery / SCALE); //set the starting position
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
	while (Window.isOpen())
	{
		sf::Event event;
		while (Window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				Window.close();
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			int MouseX = sf::Mouse::getPosition(Window).x;
			int MouseY = sf::Mouse::getPosition(Window).y;
			CreateBox(World, MouseX, MouseY);
		}
		b2Vec2 vel = dynamicBody->GetLinearVelocity();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
			vel.x = 5;
			vel.y = dynamicBody->GetLinearVelocity().y;
			dynamicBody->SetLinearVelocity(vel);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
			vel.x = -5;
			vel.y = dynamicBody->GetLinearVelocity().y;
			dynamicBody->SetLinearVelocity(vel);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
			//if (vel.y == 0)
			if (dynamicBody->GetLinearVelocity().y > -10){
				dynamicBody->ApplyForce(b2Vec2(0, -200), dynamicBody->GetWorldCenter(), 0);
			}
		}
		World.Step(1 / 60.f, 8, 3);
		
		Window.clear(sf::Color::White);
		level.Draw(Window);
		int BodyCount = 0;
		for (b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
		{
			if (BodyIterator->GetType() == b2_dynamicBody)
			{
				sf::Sprite Sprite;
				Sprite.setTexture(BoxTexture);
				Sprite.setOrigin(16.f, 16.f);
				Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
				Sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
				Window.draw(Sprite);
				
				++BodyCount;
			}
			else
			{
				sf::Sprite GroundSprite;
				GroundSprite.setTexture(GroundTexture);
				GroundSprite.setOrigin(400.f, 8.f);
				GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
				GroundSprite.setRotation(180 / b2_pi * BodyIterator->GetAngle());
				Window.draw(GroundSprite);
			}
		}
		b2Vec2 pos = dynamicBody->GetPosition();
		PlayerSprite.setPosition(SCALE * pos.x, SCALE * pos.y);

		Window.draw(PlayerSprite);
		//view.setCenter(pos.x, pos.y);
		//Window.setView(view);
		Window.display();
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

void CreateGround(b2World& World, float X, float Y)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.type = b2_staticBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((800.f / 2) / SCALE, (16.f / 2) / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.f;
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