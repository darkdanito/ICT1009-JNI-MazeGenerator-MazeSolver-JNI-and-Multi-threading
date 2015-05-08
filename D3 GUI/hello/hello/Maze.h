#include "Mazecommon.h"
#include "RecursiveBack.h"
#include "HuntKill.h"
#include "RecursiveBonus.h"
#include "IOFile.h"


using namespace std;

#ifndef Maze_H
#define Maze_H

class Maze 
{
	private:
		size_t        height;
		size_t        width;
		
		COLORREF	  wallColor;
		COLORREF	  passColor;

	public:

		char		 **mazeGrid;			// is Private DE !!!!!

		Maze();
		size_t getHeight() { return height; };
		size_t getWidth() { return width; };

		COLORREF getWallColor() { return wallColor; };
		COLORREF getPassColor() { return passColor; };

		Algorithm*   algorithms[3];												// Functions Constructors
		ERR_ENUM Maze::init();
		ERR_ENUM Maze::generate(ALGORITHMS_ENUM algorithm);
		ERR_ENUM Maze::generateByFile(HWND hwnd, vector<string> filecontent);
		void Maze::renderMaze(HWND hwnd);
		void createMazeSize(size_t width, size_t height);

		void changeMazeWallColor(COLORREF currentWallColor);
		void changeMazePassColor(COLORREF currentPassColor);
};
#endif