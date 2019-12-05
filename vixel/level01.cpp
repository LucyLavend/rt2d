/**
 * This is level one of the game "Vixel"
 *
 * 
 */

#include <time.h>
#include "level01.h"

Level01::Level01() : SuperScene()
{
	materials.push_back(RGBAColor(116, 63, 57, 1));
	materials.push_back(RGBAColor(230, 177, 133, 1));
	materials.push_back(RGBAColor(255, 0, 0, 1));

	currentMaterial = 0;

	srand((unsigned)time(nullptr));

	text[0]->message("Level01");

	timer.start();

	// create Canvas
	pixelsize = 8;
	canvas = new Canvas(pixelsize); // pixelsize
	layers[0]->addChild(canvas);

	setupDefenseBlock();
	
	restart();
}


Level01::~Level01()
{
	layers[0]->removeChild(canvas);
	delete canvas;
}

void Level01::update(float deltaTime)
{
	// ###############################################################
	// Make SuperScene do what it needs to do (Escape key stops Scene)
	// ###############################################################
	SuperScene::update(deltaTime);

	std::stringstream titletxt;
	titletxt << "Vixel";
	text[0]->message(titletxt.str());

	//text[0]->message(""); // clear title
	//text[1]->message(""); // clear fps message
	//text[2]->message(""); // clear [/] next scene
	//text[3]->message(""); // clear <esc> to quit
	text[10]->message(""); // clear player click count message

	// ###############################################################
	// Update and draw only when it's time
	// ###############################################################
	float tsec = timer.seconds();
	if (tsec > 0.01 - deltaTime) { // 0.01 is 100 fps
		static int counter=0;

		int mousex = floor(input()->getMouseX() / pixelsize);
		int mousey = floor(canvas->height() - input()->getMouseY() / pixelsize);
		text[2]->message("Pos: " + std::to_string(mousex) + ", " + std::to_string(mousey));

		if (input()->getMouse(0)) {
			this->placePixel(int(mousex), int(mousey), currentMaterial);
		}

		if (input()->getKeyDown(KeyCode('R'))) {
			restart();
		}

		for (int i = 0; i <= 9; i++) 
		{
			if (input()->getKey(KeyCode(49 + i))) { // KeyCode 49 is Alpha1
				currentMaterial = i;
				text[3]->message("Material number: " + std::to_string(currentMaterial));
			}
		}

		// every timer update
		updateDefenseGrid();

		// restart frametimer
		counter++;
		timer.start();
	}
}

bool Level01::placePixel(int x, int y, int mat) {
	
	canvas->setPixel(x, y, materials[mat]);
	return true;
}

void Level01::restart()
{
	canvas->fill(canvas->backgroundcolor);

	setupDefenseGrid();
}

void Level01::updateDefenseGrid()
{
	size_t s = defense_blocks.size();
	for (size_t i = 0; i < s; i++) {
		canvas->drawSprite(defense_blocks[i]);
	}
}

void Level01::setupDefenseGrid()
{
	defense_blocks.clear();
	size_t num = 16;
	int spacing = 6;
	for (size_t x = 0; x < num; x++) {
		PixelSprite d = defense_block; // copy sprites etc
		d.position = Pointi((x*spacing)+16, 16);
		defense_blocks.push_back(d);
	}
}

// ###########################################################################
// setup all sprites

void Level01::setupDefenseBlock()
{
	char defenseBlockSprite[16] = {
		1,1,1,1,
		1,0,0,1,
		1,0,0,1,
		1,1,1,1,
	};

	defense_block.init(defenseBlockSprite, 4, 4);
	defense_block.position = Pointi(canvas->width() / 2, canvas->height() / 3);
}