/**
 * This file is part of the game Vixel in the RT2D framework.
 *
 * - Copyright 2019 Lucy Jongebloed
 *     - Initial commit
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include <vectorx.h>

class Character
{
public:
	Character(int posX, int posY);
	virtual ~Character();

	Pointi position;
	int direction;
	int spriteW;
	int spriteH;
	std::vector<int> characterSprite;

	void walk();
	void applyGravity();
	void switchDirection();

private:
	
};

#endif /* CHARACTER_H */
