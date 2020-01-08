/**
 * This file is part of the game Vixel in the RT2D framework.
 *
 * - Copyright 2019 Lucy Jongebloed
 *     - Initial commit
 */

#include "character.h"

Character::Character(int posX, int posY)
{
	initPosition.y = posX;
	initPosition.y = posY;

	spriteW = 3;
	spriteH = 5;

	init();
}

void Character::init() {
	position = initPosition;
	awake = false;
	direction = 1;

	breath = 10;
	airTime = 0;
}

void Character::walk() {
	position.x += 1 * direction;
}

void Character::applyGravity() {
	position.y--;
	airTime++;
}

void Character::switchDirection() {
	if (direction == 1) {
		direction = -1;
	}
	else {
		direction = 1;
	}
}

void Character::die() {
	position = initPosition;
	init();
	std::cout << "DEATH" << std::endl;
}


Character::~Character()
{

}
