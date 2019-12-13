/**
 * This is level one of the game "Vixel"
 *
 * 
 */

#include <time.h>
#include "level01.h"

Level01::Level01() : SuperScene()
{
	materials.push_back(air);//0
	materials.push_back(dirt);//1
	materials.push_back(wood);//2
	materials.push_back(stone);//3
	materials.push_back(fire);//4
	materials.push_back(lava);//5
	materials.push_back(water);//6
	materials.push_back(acid);//7

	currentMaterial = 1;

	srand((unsigned)time(nullptr));

	text[0]->message("Level01");
	text[3]->message("Material number: " + std::to_string(currentMaterial));

	timer.start();

	// create Canvas
	pixelsize = 8;
	canvas = new Canvas(pixelsize); // pixelsize
	layers[0]->addChild(canvas);

	initLevel();
	setupDefenseBlock();
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

	text[10]->message(""); // clear player click count message

	// ###############################################################
	// Update and draw only when it's time
	// ###############################################################
	float tsec = timer.seconds();
	if (tsec > 0.04 - deltaTime) { // Game update time
		
		//update stuff
		updateField();
		drawLevel();
		updateDefenseGrid();

		// restart frametimer
		timer.start();
	}

	int mousex = floor(input()->getMouseX() / pixelsize);
	int mousey = floor(canvas->height() - input()->getMouseY() / pixelsize);
	text[2]->message("Pos: " + std::to_string(mousex) + ", " + std::to_string(mousey));
	//place material
	if (input()->getMouse(0)) {
		this->placePixel(int(mousex), int(mousey), currentMaterial);
	}
	//reset key
	if (input()->getKeyDown(KeyCode('R'))) {
		initLevel();
	}
	//fill key
	if (input()->getKeyDown(KeyCode('M'))) {
		for (int i = 0; i < current.size(); i++)
		{
			if (current[i] == 0) {
				current[i] = 2;
			}
		}
	}
	//select material
	for (int i = 0; i <= 9; i++)
	{
		if (input()->getKeyDown(KeyCode(49 + i)) && i < materials.size()) { // KeyCode 49 is Alpha1
			currentMaterial = i;
			text[3]->message("Material: " + std::to_string(currentMaterial));
			text[4]->message("Material color: " + std::to_string(materials[currentMaterial].r) + ", " + std::to_string(materials[currentMaterial].g) + ", " + std::to_string(materials[currentMaterial].b) + ", " + std::to_string(materials[currentMaterial].a));
		}
	}
}

void Level01::initLevel() {
	const int w = canvas->width();
	const int h = canvas->height();
	current = std::vector<int>(w * h, 0);
}

void Level01::drawLevel() {

	const int w = canvas->width();
	const int h = canvas->height();

	//draw screen from array
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			
			canvas->setPixel(x, y, materials[current[getIdFromPos(x, y)]]);

		}
	}
}

void Level01::updateField() {

	const int w = canvas->width();
	const int h = canvas->height();
	std::vector<int> next = std::vector<int>(w * h, 0);

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {

			int pixel = getIdFromPos(x, y);
			int pixelAbove = getIdFromPos(x, y + 1);
			int pixelBelow = getIdFromPos(x, y - 1);
			int pixelLeft = getIdFromPos(x - 1, y);
			int pixelRight = getIdFromPos(x + 1, y);

			//dirt logic
			if (current[pixel] == 1) {
				if (pixelBelow > -1 && (current[pixelBelow] == 0 || current[pixelBelow] == 7)) { //ignore air, acid
					next[pixel] = 0;
					next[pixelBelow] = 1;
				}
				else {
					next[pixel] = 1;
				}
			}

			//fire logic
			else if (current[pixel] == 4) {
				if (pixelAbove > -1 && current[pixelAbove] == 2) { //find wood
					next[pixel] = 0;
					next[pixelAbove] = 4;
				}
				if (pixelBelow > -1 && current[pixelBelow] == 2) {
					next[pixel] = 0;
					next[pixelBelow] = 4;
				}
				if (pixelLeft > -1 && current[pixelLeft] == 2) {
					next[pixel] = 0;
					next[pixelLeft] = 4;
				}
				if (pixelRight > -1 && current[pixelRight] == 2) {
					next[pixel] = 0;
					next[pixelRight] = 4;
				}
			}

			}

			//acid logic
			else if (current[pixel] == 7) {
				if (pixelAbove > -1 && current[pixelAbove] != 0 && current[pixelAbove] != 7) { //ignore air and self
					next[pixel] = 0;
					next[pixelAbove] = 7;
				}
				if (pixelBelow > -1 && current[pixelBelow] != 0 && current[pixelBelow] != 7) {
					next[pixel] = 0;
					next[pixelBelow] = 7;
				}
				if (pixelLeft > -1 && current[pixelLeft] != 0 && current[pixelLeft] != 7) {
					next[pixel] = 0;
					next[pixelLeft] = 7;
				}
				if (pixelRight > -1 && current[pixelRight] != 0 && current[pixelRight] != 7) {
					next[pixel] = 0;
					next[pixelRight] = 7;
				}

			}
			else {
				if (next[getIdFromPos(x, y)] == 0) {
					next[getIdFromPos(x, y)] = current[getIdFromPos(x, y)];
				}
			}
		}
	}

	current = next;
}

bool Level01::placePixel(int x, int y, int mat) {
	if (getIdFromPos(x, y) != -1) {
		current[getIdFromPos(x, y)] = mat;

		if(getIdFromPos(x - 1, y) != -1){
			current[getIdFromPos(x -1, y)] = mat;
		}
		if (getIdFromPos(x + 1, y) != -1) {
			current[getIdFromPos(x + 1, y)] = mat;
		}
		if (getIdFromPos(x, y - 1) != -1) {
			current[getIdFromPos(x, y - 1)] = mat;
		}
		if (getIdFromPos(x, y + 1) != -1) {
			current[getIdFromPos(x, y + 1)] = mat;
		}
	}
	return true;
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