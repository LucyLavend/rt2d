/**
 * This file is part of the game Vixel in the RT2D framework.
 *
 * - Copyright 2019 Lucy Jongebloed
 *     - Initial commit
 */

#include "basicentity.h"
#include "character.h"

Character::Character(int posX, int posY) : BasicEntity()
{
	initPosition.x = posX;
	initPosition.y = posY;

	home = false;
	is_controlled = false;
	jumping = false;

	spriteW = 3;
	spriteH = 5;

	init();
}

void Character::init() {
	position = initPosition;
	awake = false;
	direction = 1;

	breath = 16;
	airTime = 0;
}

void Character::walk() {
	if (is_controlled) {
		if (input()->getJoyButton(1)) {
			direction = 1;
			position.x++;
		}
		else if (input()->getJoyButton(2)) {
			direction = -1;
			position.x--;
		}
		if (input()->getJoyButtonDown(3)) {
			jumping = true;
			position.y++;
		}
		if (input()->getJoyButtonUp(3)) {
			jumping = false;
		}
	}
	else {
		position.x += 1 * direction;
	}
}

void Character::applyGravity() {
	if (!jumping) {
		position.y--;
		airTime++;
	}
}

void Character::switchDirection() {
	if (is_controlled) {
		return;
	}
	else {
		if (direction == 1) {
			direction = -1;
		}
		else {
			direction = 1;
		}
	}
}

void Character::die() {
	if (awake) {
		position = initPosition;
		init();
	}
}


Character::~Character()
{

}
