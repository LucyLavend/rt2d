/**
 * This file is part of the game Vixel in the RT2D framework.
 *
 * - Copyright 2019 Lucy Jongebloed
 *     - Initial commit
 */

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <rt2d/timer.h>
#include <rt2d/vectorx.h>
#include <rt2d/canvas.h>
#include "superscene.h"
#include "character.h"
#include "home.h"

class Game: public SuperScene
{
public:
	Game();
	virtual ~Game();

	virtual void update(float deltaTime);

private:
	inline int getIdFromPos(int x, int y) { 
		//check if x and y are inside the canvas bounds
		if (x > -1 && x < canvas->width() && y > -1 && y < canvas->height()) {
			return (y * canvas->width()) + x; 
		}
		else {
			return -1;
		}
	};

	int frameCount;
	int currentMaterial;
	int scrolledAmount;
	int level;
	int totalLevelCount;
	size_t pixelsize;
	std::vector<Character> characters;
	std::vector<Home> homes;
	std::vector<int> current;
	std::vector<int> ui;

	Sprite* levelImage;

	void drawUI();
	void updateField();
	void initLevel();
	void drawLevel();
	void updateCharacters();
	void drawCharacter(Character c, Pointi op);
	void updateHomes();
	void drawHome(Home h);
	bool placePixel(int x, int y, int mat);
	std::vector<int> createMapFromImage();

	PixelSprite defense_block;

	Canvas* canvas;
	Canvas* uiCanvas;
	Timer timer;

	std::vector<RGBAColor> materials;
	RGBAColor air = RGBAColor(0, 0, 0, 0);
	RGBAColor dirt = RGBAColor(116, 63, 57, 255);
	RGBAColor wood = RGBAColor(230, 177, 133, 255);
	RGBAColor stone = RGBAColor(100, 100, 100, 255);
	RGBAColor fire = RGBAColor(228, 59, 68, 255);
	RGBAColor lava = RGBAColor(247, 118, 34, 255);
	RGBAColor water = RGBAColor(0, 149, 233, 255);
	RGBAColor acid = RGBAColor(99, 199, 77, 255);
	RGBAColor chara = RGBAColor(182, 83, 212, 255);
	RGBAColor grass = RGBAColor(62, 137, 72, 255);
	RGBAColor home = RGBAColor(102, 11, 111, 255);
	
};

#endif /* GAME_H */
