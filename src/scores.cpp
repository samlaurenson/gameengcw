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
	std::vector<Scores> returnScores(10);
	std::string line;
	std::ifstream path(_path);
	int iterator = 0; //Using this iterator to ensure that no more than 10 scores are loaded in

	while (getline(path, line) && iterator < 10)
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

		//returnScores.push_back(newScore);
		returnScores[iterator] = newScore;
		iterator++;
	}
	//std::cout << returnScores[0].getPlayerName().toAnsiString() << std::endl;
	//std::cout << returnScores[0].getTime() << std::endl;

	return returnScores;
}

void Scores::saveScoresToFile(std::vector<Scores> _scores, std::string path)
{
	std::string resultString = "";

	//Building the string which is what will be written to the file
	for (auto& s : _scores)
	{
		resultString.append(std::to_string(s.getTime()) + "," + s.getPlayerName() + "\n");
	}

	std::ofstream outputFile;
	outputFile.open(path);
	outputFile << resultString;
	outputFile.close();
}