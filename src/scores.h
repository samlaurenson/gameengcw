#include <SFML/Graphics.hpp>

//Object that will store the name of the player and their time taken to complete the dungeon
class Scores {
private:
	float time;
	sf::String playerName;
public:
	void setTime(float time);
	void setPlayerName(sf::String playerName);
	float getTime();
	sf::String getPlayerName();

	static std::vector<Scores> loadScoresFromFile(std::string path);
	static void saveScoresToFile(std::vector<Scores> scores, std::string path);
};