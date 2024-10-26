#include "board.h"
#include "snake.h"
#include "Goal.h"
#include "assert.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////    G   R   I   D    /////////////////////////////////////////////////////
board::board(Graphics& ggg, const loc2& borderStartTile) :gfx(ggg),fromEdge(borderStartTile){}

void board::draw_tile(const loc2& pos, Color c) {

	assert(pos.x >= 0);
	assert(pos.y >= 0);
	assert(pos.x < width);
	assert(pos.y < height);


	gfx.DrawRectDim(pos.x * dimension, pos.y * dimension, dimension - 5, dimension - 5, c);
}

void board::draw_border() {

	gfx.DrawRect(fromEdge.x * dimension, fromEdge.y * dimension, (width - fromEdge.x) * dimension, (height - fromEdge.y) * dimension, {0,255,0});

	gfx.DrawRect((fromEdge.x * dimension) + dimension, (fromEdge.y * dimension) + dimension, ((width - fromEdge.x) * dimension) - dimension, ((height - fromEdge.y) * dimension) - dimension, {0,0,0});
}


int board::getWidth()const {
	return width;
}
int board::getHeight()const {
	return height;
}
void board::setEdge(const loc2& edge) {
	fromEdge = edge;
}

bool board::isInGrid(const loc2& pos)const {
	return (pos.x >= 0 && pos.x < Graphics::ScreenWidth) && (pos.y >= 0 && pos.y < Graphics::ScreenHeight);
}

bool board::isInBorder(const loc2& pos)const {


	return (pos.x >= fromEdge.x + 1 && pos.x < width - fromEdge.x - 1) &&
		   (pos.y >= fromEdge.y + 1 && pos.y < height - fromEdge.y - 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////    S   N   E   K    /////////////////////////////////////////////////////


snake::snake(const loc2& startPos, std::mt19937& _rng):rng(_rng) {
	tiles[0].initHead(startPos);
}

void snake::move(const loc2& delta_pos) {
	for (int i = currentSizeCount; i > 0; --i) {
		tiles[i].follow(tiles[i - 1]);
	}
	tiles[0].moveHead(delta_pos);
}
void snake::grow() {
	if (currentSizeCount < sizeMax) {
		tiles[currentSizeCount].initBody(rng);
		currentSizeCount++;
	}
}
void snake::draw(board& brd)const {
	for (int i = 0; i < currentSizeCount; i++) {
		tiles[i].draw(brd);
	}
}
loc2 snake::getNextHeadPos(const loc2& delta_loc)const {
	return tiles[0].getLoc() += delta_loc;
}
bool snake::selfCollision(const loc2& delta_loc )const {
	for (int i = 0; i < currentSizeCount - 1; ++i) {
		if (tiles[i].getLoc() == delta_loc) {
			return true;
		}
	}
	return false;
}
bool snake::isSpawnInSnek(const loc2& delta_loc)const {
	for (int i = 0; i < currentSizeCount; ++i) {
		if (tiles[i].getLoc() == delta_loc) {
			return true;
		}
	}
	return false;
}

void snake::reset(const loc2& pos) {
	const int first = 1;
	int index = 1;
	for (auto& each : tiles) {
		if (index == first) {
			index++;
			continue;
		}

		each.setBlack();
	}
	tiles[0].initHead({10,10});
	currentSizeCount = 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////    S N A K E  T I L E S    //////////////////////////////////////////////

void snake::segment::initHead(const loc2& _pos) {
	pos = _pos;
	c = Color{ 200,150,0 };
}
void snake::segment::initBody(std::mt19937& _rng) {
	std::uniform_int_distribution<int> red(150,170);
	std::uniform_int_distribution<int> green(200, 255);
	std::uniform_int_distribution<int> blue(25, 50);
	c = Color{ (unsigned char)red(_rng),(unsigned char)green(_rng),(unsigned char)blue(_rng)};
}
void snake::segment::moveHead(const loc2& delta_pos) {

	assert(abs(delta_pos.x) + abs(delta_pos.y) == 1);
	pos += delta_pos;
}
void snake::segment::follow(const segment& next) {

	pos = next.pos;
}
void snake::segment::draw(board& brd)const {

	brd.draw_tile(pos, c);
}
loc2 snake::segment::getLoc()const {
	return pos;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////    G  O  A  L    ////////////////////////////////////////////////////

goal::goal(std::mt19937& _rng, const board& brd, const snake& snek) {
	respawn(_rng, brd, snek);
}

void goal::respawn(std::mt19937& _rng, const board& brd, const snake& snek) {
	std::uniform_int_distribution<int> xDist(0, brd.getWidth() - 1);
	std::uniform_int_distribution<int> yDist(0, brd.getHeight() - 1);


	loc2 newLoc;
	bool breakLoop = false;
	while (!breakLoop) {
		newLoc.x = xDist(_rng);
		newLoc.y = yDist(_rng);
		if (!snek.isSpawnInSnek(newLoc)) {
			if (brd.isInBorder(newLoc)) {
				breakLoop = true;
			}
		}
	}

	pos = newLoc;
}
void goal::draw(board& brd)const {
	brd.draw_tile(pos, col);
}
const loc2& goal::getLoc()const {
	return pos;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////    O B S T A C L E   ////////////////////////////////////////////////

obstacle::obstacle() {
	for (auto& each : smegs) {
		each.set({ 0,0 }, { 0,0,0 });
	}
}

void obstacle::spawn(std::mt19937& _rng, const board& brd, const snake& snek) {


	std::uniform_int_distribution<int> xDist(0, brd.getWidth() - 1);
	std::uniform_int_distribution<int> yDist(0, brd.getHeight() - 1);
	
	loc2 newLoc;
	bool breakLoop = false;
	while (!breakLoop) {
		newLoc.x = xDist(_rng);
		newLoc.y = yDist(_rng);
		if (!snek.isSpawnInSnek(newLoc)) {
			if (brd.isInBorder(newLoc)) {
				breakLoop = true;
			}
		}
	}
	if (currentSize < maxSize) {
		smegs[currentSize].set(newLoc, { 0,0,255 });
		currentSize++;
	}
}
void obstacle::draw(board& brd)const {

	for (int i = 0; i < currentSize; i++) {
		smegs[i].draw(brd);
	}
}
bool obstacle::obstacleCollision(const loc2& pos) {
	for (int i = 0; i < currentSize; i++) {
		if (smegs[i].getPos() == pos) {

			currentSize--;
			smegs[i].set({ 0,0 }, { 0,0,0 });

			return true;
		}
	}
	return false;
}


void obstacle::smegment::draw(board& bord)const {
	bord.draw_tile(pos, c);
}
void obstacle::smegment::set(const loc2& newPos, Color col) {
	pos = newPos;
	c = col;
}
const loc2& obstacle::smegment::getPos()const {
	return pos;
}