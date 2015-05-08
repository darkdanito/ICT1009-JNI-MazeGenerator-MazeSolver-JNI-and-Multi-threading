import java.io.BufferedReader;
import java.io.IOException;

// MazeGenerator class
// Mainly contain the method related to the mazeArray.
// Contain method to read the maze from text file and update into array.
// Contain the method to print the Maze out in console.


public class MazeGenerator 
{
	protected  int cols;
	protected  int rows;
	private char[][] mazeArray;
	
	public MazeGenerator(int mazeColumns, int mazeRows, char[][] mazearray)
	{													// Constructor Method
		cols = mazeColumns;
	    rows = mazeRows;
	    mazeArray = mazearray;
	}

	public MazeGenerator(int mazeColumns, int mazeRows)
	{
		cols = mazeColumns;
	    rows = mazeRows;
	}	
	
	public MazeGenerator()								// Constructor Method
	{
		cols = 0;
	    rows = 0;
	}
	
	public int getColumn()								// get Length of Column
	{
		return mazeArray[0].length;	
	}
	
	public int getRow()									// get Length of Row
	{
		return mazeArray.length;
	}
	
	public char[][] getMazeArray()
	{
		return mazeArray;
	}
	
	public void setMazeArray(char[][] mazearray)
	{
		this.mazeArray = mazearray;
	}

	public void displayMaze()							// Print out the Maze out
	{													
		System.out.println();

		for (int i = 0; i < mazeArray.length; i++) 
		{
			for (int j = 0; j < mazeArray[i].length; j++) 
			{
				System.out.print(mazeArray[i][j]);
			}
			System.out.println();
		}
		System.out.println();
	}
	
	public void sizeMaze(BufferedReader reader) throws IOException
	{													// Check for Maze size
		String cursor;
		String content = "";
		int chars = 0;
		
		while ((cursor = reader.readLine()) != null) 
		{
			rows += 1; 									// Count rows
			content += cursor;

			String[] _words = cursor.split(""); 		// Count columns
			for (String w : _words) 
			{
				cols++;
			}
		}
	}
	
	public void generateMaze(BufferedReader readerD) throws IOException
	{
		mazeArray = new char[rows][rows]; 				//Declare array size
		
		for (int i = 0; i < rows; i++) 
		{
			String line = readerD.readLine();
			if (line == null) 
			{
				System.out.println("Invalid file !!!!!");
			}
			mazeArray[i] = line.toCharArray();
		}
		readerD.close();
	}
}
