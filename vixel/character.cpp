/**
 * This file is part of the game Vixel in the RT2D framework.
 *
 * - Copyright 2019 Lucy Jongebloed
 *     - Initial commit
 */

#include "character.h"

Character::Character(int posX, int posY)
{
	position.x = posX;
	position.y = posY;

	direction = 1;

	spriteW = 3;
	spriteH = 5;

	characterSprite = {
		2,2,2,2,
		2,2,2,2,
		2,2,2,2,
		2,2,2,2,
		2,2,2,2
	};
}

void Character::walk() {
	position.x += 1 * direction;
}

void Character::applyGravity() {
	position.y--;
}

void Character::switchDirection() {
	if (direction == 1) {
		direction = -1;
	}
	else {
		direction = 1;
	}
}


Character::~Character()
{

}
