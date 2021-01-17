#pragma once
#include <string>

enum Position { 
	N, 
	E, 
	S,
	W 
};

class Player {
	static int nextID;

	std::string name = "Player";
	std::string color = "blue";
	int id = -1;

	int setId();
public:
	int score = 0;
	bool firstTurn = true;
	Position position = Position::N;

	Player(std::string name="Player", Position position=Position::N, std::string color="blue");
	std::string getColor();
	int getID();
};