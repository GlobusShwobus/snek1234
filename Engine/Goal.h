#pragma once
#include "snake.h"
#include "board.h"

class goal {
	loc2 pos;
	static constexpr Color col = Colors::Red;

public:

	goal(std::mt19937& _rng, const board& brd, const snake& snek);

	void respawn(std::mt19937& _rng, const board& brd, const snake& snek);
	void draw(board& brd)const;
	const loc2& getLoc()const;
};

class obstacle {

	class smegment {
		Color c;
		loc2 pos;
	public:
		void draw(board& bord)const;
		void set(const loc2& newPos, Color col);
		const loc2& getPos()const;
	};

	static constexpr int maxSize = 10;
	smegment smegs[maxSize];
	int currentSize = 0;


public:
	obstacle();

	void spawn(std::mt19937& _rng, const board& brd, const snake& snek);

	void draw(board& brd)const;

	bool obstacleCollision(const loc2& pos);
};
