import java.awt.Point;

// Target Class 
// Mainly for bonus 
public class Target extends Walker 
{														// Target 
	
	public static final char WALL = '0'; 					// 0 = Wall
	public static final char TARGET = 'T'; 					// Location of the TARGET
	public static final char AGENT = 'A'; 
	public static final char PATH = ' ';				// Path that the AGENT Traveled 
	
	public Target(Point Location, char Name, char Namecode) 
	{
		super(Location,Name,Namecode);
	}
	
	public Target() 
	{
	}
	
	public static Point[] getSidePoint(Point toc) 			// Method for getting AGENT movement
	{
		Point[] sidePoint = new Point[4]; 					// to move is UP DOWN LEFT RIGHT
		
		sidePoint[0] = new Point(toc.x + 1, toc.y); 		// Down
		sidePoint[1] = new Point(toc.x, toc.y + 1); 		// Right
		sidePoint[2] = new Point(toc.x - 1, toc.y); 		// Up
		sidePoint[3] = new Point(toc.x, toc.y - 1); 		// Left
		
		return sidePoint;
	}
	
	public static Point[] getPossibleRoute(Point[] toc, MazeGenerator newMaze)
	{ 														// Method for getting possible								   
															// As AGENT cannot walk into WALLS
		int i = 0;
		Point[] possiblePath = new Point[4];				

		for (Point potentialMove : toc) 
		{													// To validate the possible moves
															// As to prevent the Agent from moving
															// out of the array bounds
			if (potentialMove.x >= 0 && potentialMove.y >= 0 && 				
				potentialMove.x < newMaze.getRow() && 
				potentialMove.y < newMaze.getColumn()) 
			{
				// Convert Point to Array Value
				char movementObject = newMaze.getMazeArray()[potentialMove.x][potentialMove.y];
				
				// Check is the Array Value is a WALL or not
				if (movementObject != WALL && movementObject != AGENT) 
				{
					possiblePath[i] = potentialMove; 		// If it is not a wall than								
					i++;									// it is a possible move
				}
			}
		}
		return possiblePath;
	}	
}
