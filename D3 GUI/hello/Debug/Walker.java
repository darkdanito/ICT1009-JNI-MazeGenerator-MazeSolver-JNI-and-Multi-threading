import java.awt.Point;

// Walker class
// Used to find the Agent / Target location in the array.

public class Walker 
{
	private Point location;
	private char name;
	private char namecode;
	
	public Walker(Point Location, char Name, char Namecode)
	{													// Constructor Method
		location = Location;
		name = Name;
		namecode = Namecode;
	}	
	
	public Walker()
	{
	}	
	
	public void setLocation(Point Location)				// Update location
	{
		this.location = Location;
	}

	public Point getLocation()							// Get current location
	{
		return location;
	}
	
	public char getName()
	{
		return name;
	}
	
	public void setName(char Name)
	{
		this.name = Name;
	}
	
	public char Namecode()
	{
		return namecode;
	}
	
	public void setNamecode(char Namecode)
	{
		this.namecode = Namecode;
	}
	
	public Point findLocation(char c, MazeGenerator newMaze)
	{													// Find Walker current location in Array 
		for (int i = 0; i < newMaze.getMazeArray().length; i++) 
		{
			for (int j = 0; j < newMaze.getMazeArray()[i].length; j++) 
			{
				if (newMaze.getMazeArray()[i][j] == c) 
				{
					Point location = new Point(i,j);
					return location;
				}
			}
		}
		return null;
	}

}
