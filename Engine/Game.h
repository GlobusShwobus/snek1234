/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <random>
#include "board.h"
#include "snake.h"
#include "Goal.h"
#include "SpriteCodex.h"

//draw a border for the wall, adjust boards pos to start from whereever, my idea is to subtract from edges and add from 0 edges
//add title screen, add press enter, start game, add restart game
//add padding between sneak tiles
//add more colorful snake pattern
//snake speeds up over time
//add special goals that reduce snek speed


class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	std::mt19937 rng;
	loc2 delta_loc{ 1,0 };
	loc2 borderPos{ 1,1 };

	board bord;
	snake snek;
	goal gol;
	obstacle obstc;

	
	static constexpr int movePerFrame = 10;
	int currentFrame = 0;

	static constexpr int spawnObstacle = 3;
	int eatCounter = 0;

	bool gameOver = false;
	bool isStarted = false;


	/********************************/
};