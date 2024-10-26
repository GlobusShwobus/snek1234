#pragma once

#include "board.h"
#include <random>


class snake {

	class segment {
		Color c;
		loc2 pos;
	public:

		segment() = default;
		void initHead(const loc2& pos);
		void initBody(std::mt19937& _rng);

		void moveHead(const loc2& delta_pos);
		void follow(const segment& next);
		void draw(board& brd)const;

		loc2 getLoc()const;
		void setBlack() { c = { 0,0,0 }; }
	};

	static constexpr int sizeMax = 100;
	segment tiles[sizeMax];
	int currentSizeCount = 1;

	std::mt19937& rng;
public:

	snake(const loc2& pos, std::mt19937& _rng);

	void move(const loc2& delta_pos);
	void grow();
	void draw(board& brd)const;

	loc2 getNextHeadPos(const loc2& delta_loc)const;
	bool selfCollision(const loc2& delta_loc)const;
	bool isSpawnInSnek(const loc2& delta_loc)const;


	void reset(const loc2& pos);
};

