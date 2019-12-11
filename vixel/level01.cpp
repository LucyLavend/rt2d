/**
 * This is level one of the game "Vixel"
 *
 * 
 */

#include <time.h>
#include "level01.h"

Level01::Level01() : SuperScene()
{
	materials.push_back(air);
	materials.push_back(dirt);
	materials.push_back(wood);
	materials.push_back(stone);
	materials.push_back(fire);
	materials.push_back(lava);
	materials.push_back(water);
	materials.push_back(acid);

	currentMaterial = 0;


	srand((unsigned)time(nullptr));

	text[0]->message("Level01");
	text[3]->message("Material number: " + std::to_string(currentMaterial));

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

	text[10]->message(""); // clear player click count message

	// ###############################################################
	// Update and draw only when it's time
	// ###############################################################
	float tsec = timer.seconds();
	if (tsec > 0.01 - deltaTime) { // 0.01 is 100 fps
		static int counter=0;
		

		//update stuff
		updateField();
		updateDefenseGrid();

		// restart frametimer
		counter++;
		timer.start();
	}

	int mousex = floor(input()->getMouseX() / pixelsize);
	int mousey = floor(canvas->height() - input()->getMouseY() / pixelsize);
	text[2]->message("Pos: " + std::to_string(mousex) + ", " + std::to_string(mousey));

	if (input()->getMouse(0)) {
		this->placePixel(int(mousex), int(mousey), currentMaterial);
	}

	if (input()->getKeyDown(KeyCode('R'))) {
		std::cout << "Button test" << std::endl;
		restart();
	}

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

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			
			canvas->setPixel(x, y, materials[getIdFromPos(x, y)]);

		}
	}
}

void Level01::updateField() {

	const int w = canvas->width();
	const int h = canvas->height();
	std::vector<int> next = std::vector<int>(w * h, 0);


}

bool Level01::placePixel(int x, int y, int mat) {
	canvas->setPixel(x, y, materials[mat]);
	canvas->setPixel(x - 1, y, materials[mat]);
	canvas->setPixel(x + 1, y, materials[mat]);
	canvas->setPixel(x, y - 1, materials[mat]);
	canvas->setPixel(x, y + 1, materials[mat]);
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