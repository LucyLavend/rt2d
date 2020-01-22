/**
 * This file is part of the game Vixel in the RT2D framework.
 *
 * - Copyright 2019 Lucy Jongebloed
 */

#include <time.h>
#include "game.h"
#include <stdlib.h>
#include <fstream>
#include <string>
#include <algorithm>

Game::Game() : SuperScene()
{
	// audio
	Audio::init();
	this->loadAudio();
	music[0]->play();

	//add all materials
	materials.push_back(air);//0
	materials.push_back(dirt);//1
	materials.push_back(wood);//2
	materials.push_back(stone);//3
	materials.push_back(fire);//4
	materials.push_back(lava);//5
	materials.push_back(water);//6
	materials.push_back(acid);//7
	materials.push_back(chara);//8
	materials.push_back(grass);//9
	materials.push_back(homeInactive);//10
	materials.push_back(homeActive);//11

	currentMaterial = 1;
	useableMaterialsCap = 8;
	scrolledAmount = 0;
	frameCount = 0;

	level = 0;
	totalLevelCount = 5;

	srand((unsigned)time(nullptr));

	text[0]->message("Game");
	text[3]->message("Material number: " + std::to_string(currentMaterial));

	timer.start();

	// create Canvas
	pixelsize = 8;
	canvas = new Canvas(pixelsize);
	uiCanvas = new Canvas(pixelsize);
	layers[0]->addChild(canvas);
	layers[1]->addChild(uiCanvas);

	initLevel();
	drawUI();
}

Game::~Game()
{
	layers[0]->removeChild(canvas);
	layers[1]->removeChild(uiCanvas);
	delete canvas;
	delete uiCanvas;
}

void Game::drawUI() {
	for (int i = 0; i < useableMaterialsCap; i++)
	{
		//reverse order for a nicer look
		RGBAColor mat = materials[useableMaterialsCap - 1 - i];
		uiCanvas->setPixel(uiCanvas->width() - 2 * i - 4, uiCanvas->height() - 5, mat);
		uiCanvas->setPixel(uiCanvas->width() - 2 * i - 5, uiCanvas->height() - 5, mat);
		uiCanvas->setPixel(uiCanvas->width() - 2 * i - 4, uiCanvas->height() - 6, mat);
		uiCanvas->setPixel(uiCanvas->width() - 2 * i - 5, uiCanvas->height() - 6, mat);
		

		if (std::find(disabledMaterials.begin(), disabledMaterials.end(), i) != disabledMaterials.end()) {
			//draw disabled material underline
			uiCanvas->setPixel(uiCanvas->width() - 2 * (useableMaterialsCap - 1 - i) - 4, uiCanvas->height() - 7, RED);
			uiCanvas->setPixel(uiCanvas->width() - 2 * (useableMaterialsCap - 1 - i) - 5, uiCanvas->height() - 7, RED);
		}
		else {
			//clear selectet material underlining
			uiCanvas->clearPixel(uiCanvas->width() - 2 * (useableMaterialsCap - 1 - i) - 4, uiCanvas->height() - 7);
			uiCanvas->clearPixel(uiCanvas->width() - 2 * (useableMaterialsCap - 1 - i) - 5, uiCanvas->height() - 7);
		}
	}
	//draw selected material underline
	uiCanvas->setPixel(uiCanvas->width() - 2 * (useableMaterialsCap - 1 - currentMaterial) - 4, uiCanvas->height() - 7, WHITE);
	uiCanvas->setPixel(uiCanvas->width() - 2 * (useableMaterialsCap - 1 - currentMaterial) - 5, uiCanvas->height() - 7, WHITE);

	//draw home state of all characters
	for (int x = 0; x < characters.size(); x++) {

		if (characters[x].home) {
			uiCanvas->setPixel(uiCanvas->width() - x * 2 - 4, uiCanvas->height() - 3, WHITE);
		}
		else {
			uiCanvas->setPixel(uiCanvas->width() - x * 2 - 4, uiCanvas->height() - 3, RGBAColor(100, 100, 100, 255));
		}
	}
}

void Game::update(float deltaTime)
{
	// Make SuperScene do what it needs to do (Escape key stops Scene)
	SuperScene::update(deltaTime);

	std::stringstream titletxt;
	titletxt << "Vixel";
	text[0]->message(titletxt.str());

	text[10]->message(""); // clear player click count message

	// Update and draw only when it's time
	float tsec = timer.seconds();
	if (tsec > 0.01 - deltaTime) { // Game update time
		
		//update stuff
		updateField();
		updateHomes();
		updateCharacters();
		drawLevel();
		drawUI();
		checkLevelProgress();

		// restart frametimer
		timer.start();
		frameCount++;
	}

	int mousex = floor(input()->getMouseX() / pixelsize);
	int mousey = floor(canvas->height() - input()->getMouseY() / pixelsize);
	text[2]->message("Pos: " + std::to_string(mousex) + ", " + std::to_string(mousey)); //display mouse position

	//place material
	if (input()->getMouse(0)) {
		this->placePixel(int(mousex), int(mousey), currentMaterial);
	}
	//reset key
	if (input()->getKeyDown(KeyCode('R'))) {
		initLevel();
	}
	//wakeup all characters
	if (input()->getKeyDown(KeyCode(32))) { //spacebar
		for (Character &c : characters) {
			c.awake = true;
		}
	}
	//increase or decrease level
	if (input()->getKeyDown(KeyCode(91))) { //left bracket
		if (level > 0) {
			level--;
			initLevel();
		}
	}
	else if (input()->getKeyDown(KeyCode(93))) { //right bracket
		if (level < totalLevelCount) {
			level++;
			initLevel();
		}
	}
	//fill key
	if (input()->getKeyDown(KeyCode('M'))) {
		for (int i = 0; i < current.size(); i++)
		{
			if (current[i] == 0) {
				current[i] = currentMaterial;
			}
		}
	}
	//change material on scroll
	if (input()->mouseScrollY != 0) {
		scrolledAmount += input()->mouseScrollY;

		moveToSelectableMat();
	}
	//select material on number key press
	for (int i = 0; i <= 9; i++)
	{
		if (input()->getKeyDown(KeyCode(49 + i)) && i < useableMaterialsCap) { // KeyCode 49 is Alpha1

			if (!(std::find(disabledMaterials.begin(), disabledMaterials.end(), i) != disabledMaterials.end())) {
				currentMaterial = i;
			}
		}
	}

	// MS Windows loop fix
	if (music[0]->state() == Sound::STATE_STOPPED) {
		music[0]->play();
	}
}

void Game::moveToSelectableMat() {
	while ((std::find(disabledMaterials.begin(), disabledMaterials.end(), scrolledAmount) != disabledMaterials.end())) {
		if (input()->mouseScrollY > 0) {
			scrolledAmount++;
		}
		else {
			scrolledAmount--;
		}
	}

	if (scrolledAmount < 0) {
		scrolledAmount = useableMaterialsCap - 1;
	}
	if (scrolledAmount > useableMaterialsCap - 1) {
		scrolledAmount = 0;
	}
	currentMaterial = scrolledAmount;
}

void Game::initLevel() {
	//reset level
	const int w = canvas->width();
	const int h = canvas->height();
	current = std::vector<int>(w * h, 0);

	disabledMaterials.clear();
	checkDisabledMaterials();
	moveToSelectableMat();

	characters.clear();
	homes.clear();
	for (Character &i : characters) {
		i.init();
	}
	current = createMapFromImage();
}

void Game::checkDisabledMaterials() {
	std::string line;
	std::ifstream levelInfo("assets/levels/disabled_materials.txt");

	if (levelInfo.is_open())
	{
		while (std::getline(levelInfo, line))
		{
			bool levelIndexFound = false;
			std::string levelIndexString = "";
			int levelIndex = 0;
			std::string disabledMatString = "";

			for (char& c : line) {
				//stop if : is found
				if (c == ':') {
					levelIndexFound = true;
				}
				//add character to level index
				if (!levelIndexFound) {
					levelIndexString += c;
				}
				//add characters to disabled materials list
				else if (c != ':' && c != ' ') {
					disabledMatString += c;
				}
			}
			std::istringstream(levelIndexString) >> levelIndex;

			if (levelIndex == level) {

				std::string matNumber;
				for (char& c : disabledMatString) {
					//split the numbers
					if (c != ',') {
						matNumber += c;
					}
					else {
						//add disabled material number to the array
						int number = 0;
						std::istringstream(matNumber) >> number;
						disabledMaterials.push_back(number);
						matNumber = "";
					}
				}
			}
		}
		levelInfo.close();
	}

	for (int aaaa = 0; aaaa < disabledMaterials.size(); aaaa++)
	{
		std::cout << disabledMaterials[aaaa] << std::endl;
	}
}

void Game::checkLevelProgress() {

	if (characters.size() != 0) {
		int chraractersHome = 0;

		for (Character &c : characters) {
			if (c.home) {
				chraractersHome++;
			}
		}
		//next level if all character are home
		if (chraractersHome >= characters.size()) {
			if (level < totalLevelCount) {
				level++;
				initLevel();
			}
		}
	}
}

std::vector<int> Game::createMapFromImage() {

	const int w = canvas->width();
	const int h = canvas->height();
	std::vector<int> result = std::vector<int>(w * h, 0);

	levelImage = new Sprite();
	std::string levelDir = "assets/levels/level" + std::to_string(level) + ".tga";
	levelImage->setupSpriteTGAPixelBuffer(levelDir, 1, 0);

	PixelBuffer* pixels = levelImage->texture()->pixels();
	int counter = 0;
	for (int y = 0; y < canvas->height(); y++) {
		for (int x = 0; x < canvas->width(); x++) {
			if (getIdFromPos(x, y) != -1) {
				int r = pixels->data[counter + 0];
				int g = pixels->data[counter + 1];
				int b = pixels->data[counter + 2];
				//check if there a character via the red channel, then use green and blue to change the size of the character
				if (r == 1) {
					Character c(x, y);
					c.spriteW = g;
					c.spriteH = b;
					characters.push_back(c);
				}
				//check if there a home via the red channel, then use green and blue to change the size of the home
				else if (r == 222) {
					Home h(x, y);
					h.spriteW = g;
					h.spriteH = b;
					homes.push_back(h);
				}
				//loop over materials to find the right one
				else {
					for (int i = 0; i < materials.size(); i++)
					{
						//compare color
						if (r == materials[i].r && g == materials[i].g && b == materials[i].b) {
							result[getIdFromPos(x, y)] = i;
						}
					}
				}
				counter += pixels->bitdepth;
			}
		}
	}
	return result;
}

void Game::drawLevel() {

	const int w = canvas->width();
	const int h = canvas->height();

	//draw screen from array
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			canvas->setPixel(x, y, materials[current[getIdFromPos(x, y)]]);
		}
	}
}

void Game::updateCharacters() {
	for (Character &i : characters) {

		//character vars
		Pointi oldPosition = i.position;
		int highestCollision = -1;
		int floorCollisions = 0;
		int amountOfWater = 0;
		int homeAmount = 0;

		if (i.awake) {
			if (frameCount % 12 == 0) {

				//fall audio
				if (i.airTime == 6) {
					sfx[1]->play();
				}

				//loop through height
				for (int y = 0; y < i.spriteH; y++) //check on the side of the character if there are collisions
				{
					int leftId = getIdFromPos(i.position.x - 1, i.position.y + y);
					int rightId = getIdFromPos(i.position.x + i.spriteW, i.position.y + y);

					int blockToCheck;
					if (i.direction == 1) {
						if (rightId != -1) {
							blockToCheck = current[rightId];
						}
						else {
							blockToCheck = 100;
						}
					}
					else {
						if (leftId != -1) {
							blockToCheck = current[leftId];
						}
						else {
							blockToCheck = 100;
						}
					}
					if (blockToCheck != 0 && blockToCheck != 10) { //check if there's air in front of the character

						if (blockToCheck == 6) { //water
							amountOfWater++;
						}
						else if (blockToCheck == 11) { //home
							homeAmount++;
						}
						else if (blockToCheck == 5) { //die in lava
							i.die();
							drawCharacter(i, oldPosition);
						}

						highestCollision = y;
					}
				} //done checking vertically

				if (highestCollision == -1 && i.awake) { //walking
					i.walk();
					i.breath = 16;
				}
				else if (highestCollision == 0 && i.spriteH > 1 && i.awake) { //walk up one block slope
					i.position.y += 1;
					i.walk();
				}
				else if (highestCollision == 1 && i.spriteH > 2 && i.awake) { //walk up two block slope
					i.position.y += 2;
					i.walk();
				}
				else { //turn around
					i.switchDirection();
				}
			}
			if (frameCount % 4 == 0 && i.awake) {
				//gravity
				for (int x = 0; x < i.spriteW; x++) //check if there's air under character
				{
					int belowId = getIdFromPos(i.position.x + x, i.position.y - 1);
					if (belowId != -1) {
						int blockToCheck = current[belowId];
						if (blockToCheck != 0 && blockToCheck != 10 && blockToCheck != 11) { //check if there's air in front of the character
							if (blockToCheck == 5) { //die in lava
								i.die();
								drawCharacter(i, oldPosition);
							}
							else if (blockToCheck == 6) {
								i.airTime = 0;
								amountOfWater++;
							}
							if (blockToCheck != 6 || belowId == -1) {
								floorCollisions++;
							}
						}
					}
					else {
						floorCollisions++;
					}
				}
				if (floorCollisions == 0) { //fall if there are no collisions below the character
					i.applyGravity();
				}
				else {
					//kill character when falling for too long and hits the ground
					if (i.airTime > 40) { //amount of blocks to fall before applying falldamage
						sfx[1]->stop();
						sfx[0]->play();
						i.die();
						drawCharacter(i, oldPosition);
					}
					else {
						//std::cout << "hahah" << std::endl;
						//sfx[1]->pause(); //stop falling sound
						i.airTime = 0;
					}
				}
			}

			if (frameCount % 6 == 0) {
				if (amountOfWater >= i.spriteH) { //check if the character is submerged in water and remove some breath
					if (i.breath == 7) { //play drowning sound
						sfx[2]->play();
					}
					i.breath--;
					if (i.breath <= 0) { //drown.
						sfx[3]->play();
						i.die();
						drawCharacter(i, oldPosition);
					}
				}
				else {
					//i.breath = 16;
				}
			}
		}
		//home check
		if (homeAmount >= i.spriteH) {
			sfx[4]->play();
			clearCharacter(i, oldPosition);
			i.awake = false;
			i.home = true;
		}
		//draw character
		if (!i.home) {
			clearCharacter(i, oldPosition);
			drawCharacter(i, oldPosition);
		}
	}
}

void Game::clearCharacter(Character c, Pointi op) {

	for (int x = 0; x < c.spriteW; x++) //clear the character
	{
		for (int y = 0; y < c.spriteH; y++)
		{
			current[getIdFromPos(op.x + x, op.y + y)] = 0;
		}
	}
}

void Game::drawCharacter(Character c, Pointi op) {

	for (int x = 0; x < c.spriteW; x++) //draw the character
	{
		for (int y = 0; y < c.spriteH; y++)
		{
			current[getIdFromPos(c.position.x + x, c.position.y + y)] = 8;
		}
	}
}

void Game::updateHomes() {
	for (Home &h : homes) {

		int woodAmount = 0;
		bool active = false;

		//loop through top and bottom row to find wood
		for (int x = 0; x < h.spriteW; x++)
		{
			int blockAbove = getIdFromPos(h.position.x + x, h.position.y - 1);
			int blockBelow = getIdFromPos(h.position.x + x, h.position.y + h.spriteH + 1);

			if (blockAbove != -1 && current[blockAbove] == 2) {
				woodAmount++;
			}
			if (blockBelow == -1 || current[blockBelow] == 2) {
				woodAmount++;
			}
		}
		//loop through sides to find wood
		for (int y = 0; y < h.spriteH; y++)
		{
			int blockLeft = getIdFromPos(h.position.x - 1, h.position.y + y);
			int blockRight = getIdFromPos(h.position.x + h.spriteW + 1, h.position.y + y);

			if (blockLeft != -1 && current[blockLeft] == 2) {
				woodAmount++;
			}
			if (blockRight != -1 && current[blockRight] == 2) {
				woodAmount++;
			}
		}
		//------
		if (woodAmount >= (h.spriteH + h.spriteW * 2)) {
			active = true;
		}

		drawHome(h, active);
	}
}

void Game::drawHome(Home h, bool active) {

	for (int x = 0; x < h.spriteW; x++) //draw the home
	{
		for (int y = 0; y < h.spriteH; y++)
		{
			int pos = getIdFromPos(h.position.x + x, h.position.y + y);
			if (pos != -1 && current[pos] != 2 && current[pos] != 4) {
				if (active) {
					current[pos] = 11;
				}
				else {
					current[pos] = 10;
				}
			}
		}
	}
}

void Game::updateField() {

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
			if (frameCount % 2 == 0 && current[pixel] == 1) {
				if (pixelBelow > -1 && (current[pixelBelow] == 0 || current[pixelBelow] == 6 || current[pixelBelow] == 5)) { //ignore air, water and lava
					next[pixel] = 0;
					next[pixelBelow] = 1;
				}
				else if ((pixelBelow == -1 || current[pixelBelow] == 1) && pixelAbove != -1 && current[pixelAbove] == 0 && (rand() % 50) == 1) { //create grass ontop
					next[pixel] = 9;
				}
				else {
					next[pixel] = 1;
				}
			}

			//grass logic
			else if (frameCount % 2 == 0 && current[pixel] == 9) {
				if (pixelBelow > -1 && (current[pixelBelow] == 0)) { //ignore air, water and lava
					next[pixel] = 0;
					next[pixelBelow] = 1;
				}
				else if (pixelAbove == -1 || current[pixelAbove] != 0 && current[pixelAbove] != 8) { //ignore air, water and lava
					next[pixel] = 1;
				}
				else {
					next[pixel] = 9;
				}
			}

			//fire logic
			else if (frameCount % 4 == 0 && current[pixel] == 4) {
				if (pixelAbove > -1 && current[pixelAbove] == 2) { //find wood
					next[pixelAbove] = 4;
				}
				if (pixelBelow > -1 && current[pixelBelow] == 2) {
					next[pixelBelow] = 4;
				}
				if (pixelLeft > -1 && current[pixelLeft] == 2) {
					next[pixelLeft] = 4;
				}
				if (pixelRight > -1 && current[pixelRight] == 2) {
					next[pixelRight] = 4;
				}
				if (rand() % 10 == 1) {
					next[pixel] = 0;
				}
				else {
					next[pixel] = 4;
				}
			}

			//stone logic
			else if (current[pixel] == 3) {
				if (pixelBelow > -1 && (current[pixelBelow] == 0 || current[pixelBelow] == 6) && pixelLeft > -1 && current[pixelLeft] == 0 && pixelRight > -1 && current[pixelRight] == 0) { //
					next[pixel] = 0;
					next[pixelBelow] = 3;
				}
				else if ((rand() % 90000 == 1) && pixelBelow > -1 && (current[pixelBelow] == 0 || current[pixelBelow] == 6)) {
					next[pixel] = 0;
					next[pixelBelow] = 3;
				}
				else {
					next[pixel] = 3;
				}
			}

			//water logic
			else if (current[pixel] == 6) {
				float dir = rand() % 3;
				bool left = false;
				bool right = false;
				bool down = false;

				if (dir == 1) {
					if (pixelLeft > -1 && (current[pixelLeft] == 0)) { //find air
						next[pixel] = 0;
						next[pixelLeft] = 6;
						left = true;
					}
					else if (pixelLeft > -1 && (current[pixelLeft] == 5)) { //find lava
						next[pixel] = 0;
						next[pixelLeft] = 3;
						left = true;
					}
					else if (pixelLeft > -1 && (current[pixelLeft] == 4)) { //find fire
						next[pixel] = 0;
						next[pixelLeft] = 0;
						left = true;
					}
				}
				else if(dir == 2){
					if (pixelRight > -1 && (current[pixelRight] == 0)) { //find air
						next[pixel] = 0;
						next[pixelRight] = 6;
						right = true;
					}
					else if (pixelRight > -1 && (current[pixelRight] == 5)) { //find lava
						next[pixel] = 0;
						next[pixelRight] = 3;
						right = true;
					}
					else if (pixelRight > -1 && (current[pixelRight] == 4)) { //find fire
						next[pixel] = 0;
						next[pixelRight] = 0;
						right = true;
					}
				}
				if (pixelBelow > -1 && (current[pixelBelow] == 0)) { //find air
					next[pixel] = 0;
					next[pixelBelow] = 6;
					down = true;
				}
				else if (pixelBelow > -1 && (current[pixelBelow] == 5)) { //find air
					next[pixel] = 0;
					next[pixelBelow] = 3;
					down = true;
				}
				else if (pixelBelow > -1 && (current[pixelBelow] == 4)) { //find air
					next[pixel] = 0;
					next[pixelBelow] = 0;
					down = true;
				}
				if(!left && !right && !down) { //check if there was no movement, then keep the pixel the same place as before
					next[pixel] = 6;
				}
			}

			//lava logic
			else if (frameCount % 2 == 0 && current[pixel] == 5) {
				float dir = rand() % 3;
				bool left = false;
				bool right = false;
				bool down = false;

				if (dir == 1) {
					if (pixelLeft > -1 && (current[pixelLeft] == 0)) { //find air
						next[pixel] = 0;
						next[pixelLeft] = 5;
						left = true;
					}
					else if (pixelLeft > -1 && (current[pixelLeft] == 2)) { //find wood
						next[pixel] = 0;
						next[pixelLeft] = 4;
						left = true;
					}
					else if (pixelLeft > -1 && (current[pixelLeft] == 6)) { //find water
						next[pixel] = 0;
						next[pixelLeft] = 3;
						left = true;
					}
				}
				else if (dir == 2) {
					if (pixelRight > -1 && (current[pixelRight] == 0)) { //find air
						next[pixel] = 0;
						next[pixelRight] = 5;
						right = true;
					}
					else if (pixelRight > -1 && (current[pixelRight] == 2)) { //find wood
						next[pixel] = 0;
						next[pixelRight] = 4;
						right = true;
					}
					else if (pixelRight > -1 && (current[pixelRight] == 6)) { //find water
						next[pixel] = 0;
						next[pixelRight] = 3;
						right = true;
					}
				}
				if (pixelBelow > -1 && (current[pixelBelow] == 0)) { //find air
					next[pixel] = 0;
					next[pixelBelow] = 5;
					down = true;
				}
				else if (pixelBelow > -1 && (current[pixelBelow] == 2)) { //find wood
					next[pixel] = 0;
					next[pixelBelow] = 4;
					down = true;
				}
				else if (pixelBelow > -1 && (current[pixelBelow] == 6)) { //find water
					next[pixel] = 0;
					next[pixelBelow] = 3;
					down = true;
				}
				if (!left && !right && !down) { //check if there was no movement, then keep the pixel the same place as before
					next[pixel] = 5;
				}
			}

			//acid logic
			else if (frameCount % 4 == 0 && current[pixel] == 7) {
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

bool Game::placePixel(int x, int y, int mat) {
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

void Game::loadAudio()
{
	Sound* f = new Sound("assets/audio/land_die.wav");//0
	f->loop(false);
	f->gain(1.0f);
	sfx.push_back(f);

	Sound* a = new Sound("assets/audio/fall.wav");//1
	a->loop(false);
	a->gain(0.8f);
	sfx.push_back(a);

	Sound* b = new Sound("assets/audio/drowning.wav");//2
	b->loop(false);
	b->gain(1.3f);
	sfx.push_back(b);

	Sound* c = new Sound("assets/audio/drown.wav");//3
	c->loop(false);
	c->gain(1.3f);
	sfx.push_back(c);

	Sound* d = new Sound("assets/audio/enter_home.wav");//4
	d->loop(false);
	d->gain(1.3f);
	sfx.push_back(d);

	Sound* m = new Sound("assets/audio/music_test.wav");//0
	m->loop(true);
	c->gain(0.5f);
	music.push_back(m);
}