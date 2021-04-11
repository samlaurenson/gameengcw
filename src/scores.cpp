#include "scores.h"
#include <iostream>
#include <fstream>
#include <sstream>

void Scores::setTime(float _time)
{
	time = _time;
}

float Scores::getTime()
{
	return time;
}

void Scores::setPlayerName(sf::String _playerName)
{
	playerName = _playerName;
}

sf::String Scores::getPlayerName()
{
	return playerName;
}

std::vector<Scores> Scores::loadScoresFromFile(std::string _path)
{
	std::vector<Scores> returnScores;
	std::string line;
	std::ifstream path(_path);

	while (getline(path, line))
	{
		Scores newScore;
		int increment = 0; //0 = time, 1 = player name
		std::stringstream ss(line);
		while (getline(ss, line, ','))
		{
			std::cout << line << std::endl;
			if (increment == 1)
			{
				newScore.setPlayerName(line);
			}
			else {
				newScore.setTime(std::stof(line));
			}

			increment++;
		}

		returnScores.push_back(newScore);
	}
	//std::cout << returnScores[0].getPlayerName().toAnsiString() << std::endl;
	//std::cout << returnScores[0].getTime() << std::endl;

	return returnScores;
}

void Scores::saveScoresToFile(std::vector<Scores> _scores)
{
	//Save each entry in leaderboard to the scores file
	//Will need to sort times before writing to file
}