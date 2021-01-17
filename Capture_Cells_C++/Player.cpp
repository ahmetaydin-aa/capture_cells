#include "Player.h"

int Player::nextID = 0;

Player::Player(std::string name, Position position, std::string color) {
	this->name = name;
	this->color = color;
	this->id = this->setId();
	this->position = position;
	this->firstTurn = true;
	this->score = 0;
}

int Player::setId() {
	this->nextID += 1;
	return this->nextID;
}

std::string Player::getColor() {
	return this->color;
}

int Player::getID() {
	return this->id;
}