/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	rng(std::random_device()()),
	bord(gfx, borderPos),
	snek({ 10,10 }, rng),
	gol(rng, bord, snek)
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if (!isStarted && !gameOver) {
		if (wnd.kbd.KeyIsPressed(VK_RETURN)) {
			delta_loc = { 1,0 };
			borderPos = { 1,1 };
			snek.reset(delta_loc);
			isStarted = true;
		}
	}


	if (!gameOver && isStarted) {
		if (wnd.kbd.KeyIsPressed(VK_UP)) {

			delta_loc = { 0,-1 };
		}
		else if (wnd.kbd.KeyIsPressed(VK_DOWN)) {

			delta_loc = { 0, 1 };
		}
		else if (wnd.kbd.KeyIsPressed(VK_LEFT)) {

			delta_loc = { -1, 0 };
		}
		else if (wnd.kbd.KeyIsPressed(VK_RIGHT)) {

			delta_loc = { 1, 0 };
		}
	}

	if (gameOver && !isStarted) {
		if (wnd.kbd.KeyIsPressed('F')) {
			gameOver = false;
		}
	}
}

void Game::ComposeFrame()
{
	if (isStarted) {
		currentFrame++;
		if (currentFrame >= movePerFrame) {
			currentFrame = 0;

			const loc2 nextLoc = snek.getNextHeadPos(delta_loc);
			if (!bord.isInBorder(nextLoc) || snek.selfCollision(nextLoc)) {
				gameOver = true;
				isStarted = false;
			}
			else {

				bool eating = nextLoc == gol.getLoc();
				if (eating) {

					snek.grow();
				}
				snek.move(delta_loc);
				if (eating) {
					eatCounter++;
					gol.respawn(rng, bord, snek);
				}
			}
			if (obstc.obstacleCollision(nextLoc)) {
				borderPos.x++;
				borderPos.y++;

				bord.setEdge(borderPos);
			}
		}

		if (eatCounter >= spawnObstacle) {
			obstc.spawn(rng, bord, snek);
			eatCounter = 0;
		}


		bord.draw_border();
		gol.draw(bord);
		snek.draw(bord);
		obstc.draw(bord);
	}

	if (gameOver) {
		SpriteCodex::DrawGameOver(400, 300, gfx);
	}
	if (!gameOver && !isStarted) {
		SpriteCodex::DrawTitle(400, 300, gfx);
	}
}
