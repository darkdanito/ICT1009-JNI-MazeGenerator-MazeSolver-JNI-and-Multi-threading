import java.io.*;

// IO Class
// Contain the method to read the maze file.
// Update the location of the Maze File below.

public class IO 
{
	public static BufferedReader ReadFile() throws FileNotFoundException
	{														// Read maze from text file
		// Update the location of the Maze File below: 
		String fileName = "C:\\Users\\darkdanito\\Desktop\\Mulithread_D3 GUI\\D3 GUI\\hello\\Debug\\test.txt";
		FileReader inputFile = new FileReader(fileName);
		BufferedReader reader = new BufferedReader(inputFile);
		
		return reader;
	}
	
	public static BufferedReader ReadInput()
	{														// ReadInput	
		InputStreamReader inputValue = new InputStreamReader(System.in);
		BufferedReader reader = new BufferedReader(inputValue);
		
		return reader;
	}


	public static void Save (MazeGenerator newMaze) 
	{
		Writer writer = null;
		int height = newMaze.getRow();
		
		try {
		    writer = new BufferedWriter(new OutputStreamWriter(
		          new FileOutputStream(("C:\\Users\\darkdanito\\Desktop\\Mulithread_D3 GUI\\D3 GUI\\hello\\Debug\\javaTest.txt"), false)));
		    
		    for(int count =0;count <height;count++) 
		    {
		    writer.write(newMaze.getMazeArray()[count]);
		    if (count < height-1)
		    {
 			writer.write(System.getProperty( "line.separator" ));
		    }
		   
		    }
		} catch (IOException ex) 
		{
		  System.out.println("\nError !!!!! Please check on file directory. [JAVA SIDE][IO.JAVA]");
		} finally 
		{
		   try {writer.close();} catch (Exception ex) {}
		}
	}

	
	
	
	public static void NecroSave (char player, char moveType, int xPos, int yPos) 
	{
		Writer writer = null;

		try 
		{
		    writer = new BufferedWriter(new OutputStreamWriter(
		          new FileOutputStream(("C:\\Users\\darkdanito\\Desktop\\Mulithread_D3 GUI\\D3 GUI\\hello\\Debug\\necroTest.txt"), true)));
		    
		    writer.write(player);
		    writer.append(',');
		    writer.write(moveType);
		    writer.append(',');
		    writer.write(String.valueOf(xPos));
		    writer.append(',');
		    writer.write(String.valueOf(yPos));
		    writer.write(System.getProperty( "line.separator" ));
		} catch (IOException ex) 
		{
		  System.out.println("\nError !!!!! Please check on file directory. [JAVA SIDE][IO.JAVA]");
		} finally 
		{
		   try {writer.close();} catch (Exception ex) {}
		}
	}

	public static void NecroSaveInit () 
	{
		Writer writer = null;

		try 
		{
		    writer = new BufferedWriter(new OutputStreamWriter(
		          new FileOutputStream(("C:\\Users\\darkdanito\\Desktop\\Mulithread_D3 GUI\\D3 GUI\\hello\\Debug\\necroTest.txt"), false)));
		    
		} catch (IOException ex) 
		{
		  System.out.println("\nError !!!!! Please check on file directory. [JAVA SIDE][IO.JAVA]");
		} finally 
		{
		   try {writer.close();} catch (Exception ex) {}
		}
	}

}
