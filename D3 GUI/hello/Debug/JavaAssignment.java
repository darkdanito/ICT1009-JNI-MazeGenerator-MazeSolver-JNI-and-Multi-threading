import java.io.*;

// main file of the Assignment.
// Contain the main()
// Contain the method to read input from users
public class JavaAssignment extends Thread
{
	public static final char WALL = '0'; 						// 0 = Wall
	public static final char FREE = '1'; 						// 1 = Passage
	public static final char TARGET = 'T'; 						// Location of the TARGET
	public static final char AGENT = 'A'; 						// Location of the AGENT
	
	static 
	{
	       System.loadLibrary("hello");
	}
	
	private native void sayHello();
	private native String sayNecro(String msg);
	private native int[] steps(int[] numbers);
	
	private native boolean sayBoolean(boolean bool);
	
	public void run() 
    {
        System.out.println("Second Thread has started...... \n Init C++ GUI.");
        new JavaAssignment().sayHello();
    }
	
	public static void main(String[] args) throws IOException 
	{
	//	new JavaAssignment().sayHello();
		
		(new JavaAssignment()).start();
		System.out.println("Main Thread has started...... ");
		
		String result = new JavaAssignment().sayNecro(" ");
		
		Boolean testBool = true;
		Boolean javaBool = new JavaAssignment().sayBoolean(testBool);
		System.out.println("\nBoolean Value from C++: "+ javaBool);
		
		while (javaBool != true)
		{
			javaBool = new JavaAssignment().sayBoolean(testBool);
			System.out.println("CONDITONAL WAIT !!!! .... [Welcome to Dinosauar.....");
		}
		
		
		
//		while (result.equals("javaExit"))
//		{
			result = new JavaAssignment().sayNecro(" ");
			
//			if (result.equals("javaExit2"))
//			{
//				break;
//			}
			
			IO.NecroSaveInit();
			
			try
			{
				BufferedReader reader = IO.ReadFile();					// call IO to read Maze File
				
				MazeGenerator newMaze = new MazeGenerator();			// call create empty Maze
				
				newMaze.sizeMaze(reader);								// Check Maze size of text file
				
				BufferedReader readerD = IO.ReadFile();					// call IO to read Maze File

				newMaze.generateMaze(readerD);							// initialize the Maze
				
				inputAgent(newMaze);									// Update Agent X Y into Maze
				inputTarget(newMaze);									// Update Target X Y into Maze
				
				long startTime2 = System.currentTimeMillis();			// Get Start time 
				
				Agent AgentOne = new Agent();							// Create Agent Object
				AgentOne.setName('B');
				AgentOne.setNamecode('A');
				
				Target TargetGoal = new Target();						// Create Target Object
				TargetGoal.setName('C');
				TargetGoal.setNamecode('T');
				
				MazeSolver.catchTarget(newMaze, AgentOne, TargetGoal);
				MazeSolver.solve(newMaze, AgentOne, TargetGoal);

				long endTime2 = System.currentTimeMillis();				// Get End Time

				System.out.println("Program runtime : " + (endTime2 - startTime2) + " ms");
				
				IO.Save(newMaze);			
					
					// Passing of Steps to C++
				
					MazeSolver mazeSolver = new MazeSolver();
					int steps = mazeSolver.steps();
					int stepsActual = mazeSolver.stepsActual();
					
					int stepsArray[] = {steps, stepsActual};
					
				//	new JavaAssignment().steps(stepsArray);
					
				reader.close();
				readerD.close();

			} catch(FileNotFoundException e) 
			{
				System.out.println("\nError !!!!! Maze File cannot be found. Please check on file directory. [JAVA SIDE][JavaAssignment.JAVA]");
			}
//			new JavaAssignment().sayHello();
//		}
		
		
	}
	
	public static void inputAgent(MazeGenerator newMaze) throws IOException
	{														// User Input of AGENT X Y Position
		int agentX = 0;
		int agentY = 0;
		
		String inputAgentX = null;
		String inputAgentY = null;

		
		String control = "None";
		
		for (int i = 0; i < newMaze.getRow() ; i++)
		{
			for (int j = 0; j < newMaze.getColumn(); j++) 
			{
				if (newMaze.getMazeArray()[i][j] == AGENT)
				{
					control = "Exist";
				}
			}
		}
		
		if (control == "None")
		{
			BufferedReader readerDD = IO.ReadInput();
			
			do 
			{
				newMaze.displayMaze();								
				System.out.print("\nPlease enter the starting X position of Agent: ");
				
				inputAgentX = readerDD.readLine();	
				if (!inputAgentX.isEmpty() && inputAgentX.matches("[0-9]+")) 
				{													
					agentX = Integer.parseInt(inputAgentX);
					if (agentX >= 0 && agentX < newMaze.getRow()) 	// Bigger than 0 and less than
					{												// array size
						System.out.print("Please enter the starting Y position of Agent: ");
						
						inputAgentY = readerDD.readLine();
						if (!inputAgentY.isEmpty() && inputAgentY.matches("[0-9]+")) 
						{
							agentY = Integer.parseInt(inputAgentY);
							if (agentY >= 0 && agentY < newMaze.getColumn()) 
							{						
								if (newMaze.getMazeArray()[agentX][agentY] != WALL) 
								{									// Update to Array Agent Location
									newMaze.getMazeArray()[agentX][agentY] = AGENT;
									newMaze.displayMaze();
								} else 
								{
									System.out.println("Cannot place on Wall!!!");
								}
							} else 
							{
								System.out.println("Enter only positive numbers of Y coordinate and within range!!!");
							}
						} else 
						{
							System.out.println("Only numbers are allowed for Y coordinate!!! ");
						}
					} else 
					{
						System.out.println("Enter only positive numbers of X coordinate and within range!!!");
					}
				} else 
				{
					System.out.println("Only numbers are allowed for X coordinate!!!");
				}
			} while (inputAgentX.isEmpty() || !inputAgentX.matches("[0-9]+") ||
					agentX < 0 || agentX >= newMaze.getRow()|| 
					inputAgentY.isEmpty() || !inputAgentY.matches("[0-9]+") ||
					agentY < 0 || agentY >= newMaze.getColumn() ||
					newMaze.getMazeArray()[agentX][agentY] == WALL);
		}
	}
	
	public static void inputTarget(MazeGenerator newMaze) throws IOException
	{														// User Input of Target X Y Position
		int targetX = 0;
		int targetY = 0;

		String inputTargetX = null;
		String inputTargetY = null;
		
		String control = "None";
		
		for (int i = 0; i < newMaze.getRow() ; i++)
		{
			for (int j = 0; j < newMaze.getColumn(); j++) 
			{
				if (newMaze.getMazeArray()[i][j] == TARGET)
				{
					control = "Exist";
				}
			}
		}
		
		if (control == "None")
		{
			BufferedReader readerDD = IO.ReadInput();
			
			do 
			{
				System.out.print("\nPlease enter the starting X position of Target: ");
				inputTargetX = readerDD.readLine();

				if (!inputTargetX.isEmpty() && inputTargetX.matches("[0-9]+")) 
				{
					targetX = Integer.parseInt(inputTargetX);	
					if (targetX >= 0 && targetX < newMaze.getRow()) 
					{
						System.out.print("Please enter the starting Y position of Target: ");
						inputTargetY = readerDD.readLine();
						if (!inputTargetY.isEmpty() && inputTargetY.matches("[0-9]+")) 
						{
							targetY = Integer.parseInt(inputTargetY);
							if (targetY >= 0 && targetY < newMaze.getColumn()) 
							{
								if (newMaze.getMazeArray()[targetX][targetY] != WALL && 
									newMaze.getMazeArray()[targetX][targetY] != AGENT ) 
								{								// Update to Array Target Location
									newMaze.getMazeArray()[targetX][targetY] = TARGET;
									newMaze.displayMaze();
								} else 
								{
									System.out.println("Cannot place on Wall or Agent!!!");
								}
							} else 
							{
								System.out.println("Enter only positive numbers of Y coordinate and within range!!!");
							}
						} else 
						{
							System.out.println("Only numbers are allowed for Y coordinate!!! ");
						}
					} else 
					{
						System.out.println("Enter only positive numbers of X coordinate and within range"
								+ "!!!");
					}
				} else 
				{
					System.out.println("Only numbers are allowed for X coordinate!!!");
				}
			} while (inputTargetX.isEmpty() || !inputTargetX.matches("[0-9]+") || 
					targetX < 0 || targetX >= newMaze.getRow()||
					inputTargetY.isEmpty() || !inputTargetY.matches("[0-9]+") || 
					targetY < 0 || targetY >= newMaze.getColumn() ||
					newMaze.getMazeArray()[targetX][targetY] == WALL || 
					newMaze.getMazeArray()[targetX][targetY] == AGENT);			
		}
	}
}