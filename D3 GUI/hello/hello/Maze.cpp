#include <Windows.h>
#include "Maze.h"
#include<iostream>
#include<cstdlib>
#include<stack>
#include<ctime>

Maze::Maze()																	// Constructor initialization
{
	init();
}

/*Generate*/
ERR_ENUM Maze::generate(ALGORITHMS_ENUM algorithm)								// Generate the 4 available Algorithm - by calling Algorithm class
{
	if (algorithms[algorithm]->generate() == SUCCESS) 
	{
		return SUCCESS;
	}
	else 
	{
		return FAILURE;
	}
}

/*Generate Maze by File*/
ERR_ENUM Maze::generateByFile(HWND hwnd, vector<string> filecontent)			// Generate the Maze using the File
{
	if (height <= 1 || width <= 1 || mazeGrid == 0)
		return FAILURE;

	if (width < height || width > height)
	{
		for (size_t i = 0; i < height; i++)										// init the grid Array
		{
			for (size_t j = 0; j < width; j++)
			{
				mazeGrid[i][j] = '0';											// Initialize to 0
			}
		}
	}
	else
	{
		for (size_t i = 0; i < width; i++)										// init the grid Array
		{
			for (size_t j = 0; j < height; j++)
			{
				mazeGrid[i][j] = '0';											// Initialize to 0
			}
		}
	}

	for (size_t i = 0; i < filecontent.size(); i++)								// Set the WALL AND PASSENGE in the mazeGrid for the file maze
	{
		string row = filecontent.at(i);
		for (size_t j = 0; j < filecontent.at(i).size(); j++)
		{
			if (row.at(j) == '1')												// Path = 1
			{ 
				mazeGrid[i][j] = '1';											// Set all the value as Path
			}
			else if (row.at(j) == '0')											// Wall = 0
			{ 
				mazeGrid[i][j] = '0';											//Set all the value as WALL
			}
			else if (row.at(j) == '@')											// Wall = 0
			{
				mazeGrid[i][j] = '@';											//Set all the value as WALL
			}
			else if (row.at(j) == 'A')											// Wall = 0
			{
				mazeGrid[i][j] = 'A';											//Set all the value as WALL
			}
			else if (row.at(j) == 'T')											// Wall = 0
			{
				mazeGrid[i][j] = 'T';											//Set all the value as WALL
			}
			else
			{
				mazeGrid[i][j] = 'X';											// For invalid values
			}
		}
	}
	return SUCCESS;
}

/*Initialize empty*/
ERR_ENUM Maze::init()															// Initialization empty values for the attributes
{	
	height = 0;
	width = 0;
	mazeGrid = 0;

	wallColor = RGB(255, 0, 0);
	passColor = RGB(0, 0, 255);

	algorithms[0] = new RecursiveBack;											// Create new Algorithms Recursive
	algorithms[1] = new HuntKill;												// Create new Algorithms Hunt and Kill
	algorithms[2] = new RecursiveBonus;											// Create new Algorithms Recursive with Loop [Hybid]
	
	return SUCCESS;
}

/*Create Maze Size*/
void Maze::createMazeSize(size_t width, size_t height)							// Create Maze Size based on user input
{
	this->width = width;
	this->height = height;

	if (width < height || width > height)
	{
		mazeGrid = new char*[height + 1];										// Generate the X Size of Maze
		
		for (size_t i = 0; i < height + 1; ++i)
		{
			mazeGrid[i] = new char[width + 1];									// Generate the Y Size of Maze
		}

		for (size_t i = 0; i < 3; i++) 
		{
			algorithms[i]->hookMaze(mazeGrid, height, width);					// Each algorithms stated can have different maze attributes
		}
	}
	else if (width = height)
	{
		mazeGrid = new char*[width + 1];										// Generate the X Size of Maze
		
		for (size_t i = 0; i < width + 1; ++i)
		{
			mazeGrid[i] = new char[height + 1];									// Generate the Y Size of Maze
		}

		for (size_t i = 0; i < 3; i++) 
		{
			algorithms[i]->hookMaze(mazeGrid, width, height);					// Each algorithms stated can have different maze attributes
		}
	}
}

void Maze::renderMaze(HWND hwnd)												// Render Maze GUI or Recursive and Recursive with Loop
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hwnd, &ps);

	int sizeConstant = 30;														// Maze wall thickness constant

	if (width > 20 && width <= 50 || height > 20 && height <= 50)
	{
		sizeConstant = sizeConstant / 2;										// Shrink the wall thickness if > 20 and <= 50
	}
	else if (width > 50 || height > 50)
	{
		sizeConstant = sizeConstant / 4;										// Shrink the wall thickness if > 50
	}

	/*
	Reason required for the constant:
	As a rectangle () take in 5 parameters to be called.

	Our maze is make up of small little square.
	Thus each rectangle will need some values to offset it.
	*/
	int xLen = sizeConstant;													// x axies constant for rectangle
	int yLen = sizeConstant;													// y axies constant for rectangle

	if (width < height || width > height)
	{
		for (size_t i = 0; i < height; i++)										// Nested Loop to print the maze Array out
		{
			int y = yLen + sizeConstant;

			for (size_t j = 0; j < width; j++)
			{
				int z = xLen + sizeConstant;

				if (mazeGrid[i][j] == '0' )										// If it is a WALL
				{
					HBRUSH hbr = CreateSolidBrush(wallColor);				// BLUE
					SelectObject(hdc, hbr);
					Rectangle(hdc, xLen, yLen, z, y);
					DeleteObject(hbr);
				}
				else if (mazeGrid[i][j] == '1')									// It is a PATH
				{
					HBRUSH hbr = CreateSolidBrush(passColor);					// RED
					SelectObject(hdc, hbr);
					Rectangle(hdc, xLen, yLen, z, y);
					DeleteObject(hbr);
				}
				else if (mazeGrid[i][j] == '@')
				{
					HBRUSH hbr = CreateSolidBrush(RGB(0, 0, 0));				// RED
					SelectObject(hdc, hbr);
					Rectangle(hdc, xLen, yLen, z, y);
					DeleteObject(hbr);
				}
				else if (mazeGrid[i][j] == ' ')
				{
					HBRUSH hbr = CreateSolidBrush(RGB(255, 255, 255));				// RED
					SelectObject(hdc, hbr);
					Rectangle(hdc, xLen, yLen, z, y);
					DeleteObject(hbr);
				}
				else if (mazeGrid[i][j] == 'A')
				{
					HBRUSH hbr = CreateSolidBrush(RGB(0, 255, 255));				// RED
					SelectObject(hdc, hbr);
					Rectangle(hdc, xLen, yLen, z, y);
					DeleteObject(hbr);
				}
				else if (mazeGrid[i][j] == 'T')
				{
					HBRUSH hbr = CreateSolidBrush(RGB(255, 255, 0));				// RED
					SelectObject(hdc, hbr);
					Rectangle(hdc, xLen, yLen, z, y);
					DeleteObject(hbr);
				}
				else															// Others
				{
					HBRUSH hbr = CreateSolidBrush(RGB(0, 255, 0));				// Green
					SelectObject(hdc, hbr);
					Rectangle(hdc, xLen, yLen, z, y);
					DeleteObject(hbr);
				}

				xLen += sizeConstant;
			}

			yLen += sizeConstant;
			xLen = sizeConstant;												// Reset the x to print at the start for next row
		}
	}
	else
	{
		for (size_t i = 0; i < width; i++)										// Nested Loop to print the maze Array out
		{
			int y = yLen + sizeConstant;

			for (size_t j = 0; j < height; j++)
			{
				int z = xLen + sizeConstant;

				if (mazeGrid[i][j] == '0')										// If it is a WALL
				{
					HBRUSH hbr = CreateSolidBrush(wallColor);				// BLUE
					SelectObject(hdc, hbr);
					Rectangle(hdc, xLen, yLen, z, y);
					DeleteObject(hbr);
				}
				else if (mazeGrid[i][j] == '1')									// It is a PATH
				{
					HBRUSH hbr = CreateSolidBrush(passColor);				// RED
					SelectObject(hdc, hbr);
					Rectangle(hdc, xLen, yLen, z, y);
					DeleteObject(hbr);
				}
				else if (mazeGrid[i][j] == '@')
				{
					HBRUSH hbr = CreateSolidBrush(RGB(0, 0, 0));				// RED
					SelectObject(hdc, hbr);
					Rectangle(hdc, xLen, yLen, z, y);
					DeleteObject(hbr);
				}
				else if (mazeGrid[i][j] == ' ')
				{
					HBRUSH hbr = CreateSolidBrush(RGB(255, 255, 255));				// RED
					SelectObject(hdc, hbr);
					Rectangle(hdc, xLen, yLen, z, y);
					DeleteObject(hbr);
				}
				else if (mazeGrid[i][j] == 'A')
				{
					HBRUSH hbr = CreateSolidBrush(RGB(0, 255, 255));				// RED
					SelectObject(hdc, hbr);
					Rectangle(hdc, xLen, yLen, z, y);
					DeleteObject(hbr);
				}
				else if (mazeGrid[i][j] == 'T')
				{
					HBRUSH hbr = CreateSolidBrush(RGB(255, 255, 0));				// RED
					SelectObject(hdc, hbr);
					Rectangle(hdc, xLen, yLen, z, y);
					DeleteObject(hbr);
				}
				else															// Others
				{
					HBRUSH hbr = CreateSolidBrush(RGB(0, 255, 0));				// RED
					SelectObject(hdc, hbr);
					Rectangle(hdc, xLen, yLen, z, y);
					DeleteObject(hbr);
				}

				xLen += sizeConstant;
			}

			yLen += sizeConstant;
			xLen = sizeConstant;												// Reset the x to print at the start for next row
		}
	}
	EndPaint(hwnd, &ps);
}

void Maze::changeMazeWallColor(COLORREF currentWallColor)
{
	this->wallColor = currentWallColor;
}

void Maze::changeMazePassColor(COLORREF currentPassColor)
{
	this->passColor = currentPassColor;
}