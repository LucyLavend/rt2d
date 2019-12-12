/**
 * This file is part of a demo that shows how to use RT2D, a 2D OpenGL framework.
 *
 * - Copyright 2016 Rik Teerling <rik@onandoffables.com>
 *     - Initial commit
 */

#ifndef SCENE13_H
#define SCENE13_H

#include <vector>
#include <rt2d/timer.h>
#include <rt2d/vectorx.h>
#include <rt2d/canvas.h>
#include "superscene.h"

class Level01: public SuperScene
{
public:
	Level01();
	virtual ~Level01();

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

	size_t pixelsize;
	std::vector<PixelSprite> defense_blocks;
	std::vector<int> current;

	void updateField();
	void setupDefenseGrid();
	void updateDefenseGrid();
	void initLevel();
	void drawLevel();
	void setupDefenseBlock();
	bool placePixel(int x, int y, int mat);

	PixelSprite defense_block;

	Canvas* canvas;
	Timer timer;

	int currentMaterial;
	std::vector<RGBAColor> materials;
	RGBAColor air = RGBAColor(0, 0, 0, 0);
	RGBAColor dirt = RGBAColor(116, 63, 57, 255);
	RGBAColor wood = RGBAColor(230, 177, 133, 255);
	RGBAColor stone = RGBAColor(100, 100, 100, 255);
	RGBAColor fire = RGBAColor(228, 59, 68, 255);
	RGBAColor lava = RGBAColor(247, 118, 34, 255);
	RGBAColor water = RGBAColor(0, 149, 233, 255);
	RGBAColor acid = RGBAColor(99, 199, 77, 255);
	
};

#endif /* SCENE13_H */
