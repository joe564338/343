#ifndef __LEVEL_H
#define __LEVEL_H
#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
static const float SCALE = 30.f;
struct Object
{
	int GetPropertyInt(std::string name);
	float GetPropertyFloat(std::string name);
	std::string GetPropertyString(std::string name);

	std::string name;
	std::string type;
	sf::Rect<int> rect;
	std::map<std::string, std::string> properties;

	sf::Sprite sprite;
};

struct Layer
{
	int opacity;
	std::vector<sf::Sprite> tiles;
};

class Level
{
public:
	bool LoadFromFile(std::string filename);
	Object GetObjectPlayer(std::string name);
	std::vector<Object> GetObjects(std::string name);
	void Draw(sf::RenderWindow &window);
	sf::Vector2i GetTileSize();

private:
	int width, height, tileWidth, tileHeight;
	int firstTileID;
	sf::Rect<float> drawingBounds;
	sf::Texture tilesetImage;
	std::vector<Object> objects;
	std::vector<Layer> layers;
};

#endif


