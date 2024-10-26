#pragma once


#include "Graphics.h"
#include "vec2.h"

class board {

	static constexpr int dimension = 20;
	static constexpr int width = Graphics::ScreenWidth / dimension;
	static constexpr int height = Graphics::ScreenHeight / dimension;

	Graphics& gfx;

	loc2 fromEdge;

public:
	board(Graphics& ggg, const loc2& borderStartTile);

	void draw_tile(const loc2& pos, Color c);

	void draw_border();
	int getWidth()const;
	int getHeight()const;
	void setEdge(const loc2& newedge);

	bool isInGrid(const loc2& pos)const;
	bool isInBorder(const loc2& pos)const;
};