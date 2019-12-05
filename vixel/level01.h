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

#define POINT_OF_NO_RETURN Pointi(-1000,-1000)

struct SI_AnimatedSprite {
	Pointi position;
	Pointi velocity;
	std::vector<PixelSprite> frames;
	void addPixelSprite(PixelSprite ps) {
		frames.push_back(ps);
	}
};

class Level01: public SuperScene
{
public:
	Level01();
	virtual ~Level01();

	virtual void update(float deltaTime);

private:
	size_t pixelsize;
	std::vector<PixelSprite> defense_blocks;

	int currentMaterial;

	void setupDefenseGrid();
	void updateDefenseGrid();
	bool placePixel(int x, int y);
	void restart();

	void setupDefenseBlock();

	PixelSprite defense_block;

	Canvas* canvas;
	Timer timer;

	RGBAColor* sand = new RGBAColor(116, 63, 57, 1);
	RGBAColor* rock = new RGBAColor(230, 177, 133, 1);
	RGBAColor wood;
	RGBAColor fire;
	RGBAColor water;
	RGBAColor lava;
};

#endif /* SCENE13_H */
