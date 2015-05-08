import java.awt.Point;
import java.util.Random;

// MazeSolver Class
// Mainly contain the Algorithm for the HSA A * Search
// Contain the Recursive function for the Agent when it reach the dead end.

public class MazeSolver 
{
	public static final char FREE = '1';				// 1 = Passage 
	public static final char TARGET = 'T';				// Location of the TARGET
	public static final char AGENT = 'A';				// Location of the AGENT
	public static final char PATH = ' ';				// Path that the AGENT Traveled 
	
	public static int steps;			// Steps taken for AGENT to reach TARGET with backtrack
	public static int actualSteps;		// Steps taken for AGENT to reach without backtrack
	
	public static Point[] catchTarget(MazeGenerator newMaze, Agent AgentOne, Target TargetGoal) // Method for HSA A * Search
	{
		// Update the location of Agent and Target
		AgentOne.setLocation(AgentOne.findLocation(AgentOne.Namecode(), newMaze));
		TargetGoal.setLocation(TargetGoal.findLocation(TargetGoal.Namecode(), newMaze));
		
		Point[] sidePoint = Agent.getSidePoint(AgentOne.getLocation());
		Point[] possibleRoute = Agent.getPossibleRoute(sidePoint,newMaze);

		int[] costArray = { 0, 0, 0, 0 }; 				// Store the cost of route
		int i = 0;					
		
//		System.out.println("\nCost of Route: ");
		
		for (Point routeCalc : possibleRoute) 
		{
			if (routeCalc != null) 
			{	
				// Based on the Method of getSidePoint() and getPossibleRoute
				// the results are a Point Array that contain the possible Route that
				// the Agent can move
				
				// Variable cost is the calculation for the route cost.
				// Based on the A * Search Algorithm, the AGENT routes are based on the cost.
				// The cheapest route will be chosen.
				
				// By using Math.abs is to convert the number to a positive one for calculation
				
				// Formula: f(n) = g(n) + h(n)
				// f(n) is the route cost : total
				// g(n) Starting to Node
				// h(n) Node to target
				
				// Based on Manhattan distance which is based on the sum of horizontal and 
				// vertical distance.

				int gCost = Math.abs(routeCalc.getLocation().x - AgentOne.getLocation().x)+ 
						Math.abs(routeCalc.getLocation().y - AgentOne.getLocation().y) + 1;
				
				int hCost = Math.abs(TargetGoal.getLocation().x - routeCalc.x)+ 
						Math.abs(TargetGoal.getLocation().y - routeCalc.y) + 1;
				
				int fCost = gCost + hCost;
				
//				System.out.println("Route: [" + routeCalc.x + "," + routeCalc.y+ "] = g(N): "+gCost+ 
//						" h(N) :"+hCost+" f(N): "+fCost);
		
				costArray[i] = fCost;
				i++;
			}
		}

		Point[] actualRoute = possibleRoute;
		
		// Sorting the route based on cost.
		// As for the For Loops for the Point.
		// After the nested For Loop are completed,
		// The First route in the Point Array will the cheapest.
		// The route cost are sorted by Ascending order
		
		for (int loopCounter1 = 0; loopCounter1 < (4 - 1); loopCounter1++) 
		{
			for (int j = 0; j < 4 - loopCounter1 - 1; j++) 
			{
				if (costArray[j] > costArray[j + 1]) 
				{
					// Sort the values in the array
					int temp = costArray[j];
					costArray[j] = costArray[j + 1];
					costArray[j + 1] = temp;

					// Sort the values in the Point Array
					Point temp2 = actualRoute[j];
					actualRoute[j] = actualRoute[j + 1];
					actualRoute[j + 1] = temp2;
				}
			}
		}
		// Sorted actualRoute Pointer array with shortest distance on the top.
		
		return actualRoute;
	}
	
	public static void solve(MazeGenerator newMaze, Agent AgentOne, Target TargetGoal)
	{ 
		steps = 0;
		
		if (findPath(AgentOne.getLocation(), newMaze, AgentOne, TargetGoal)) 
		{												// If Maze has solution
//			newMaze.displayMaze();
			System.out.println("\nAgent has reach the Target in: " + steps+" steps. [Including Backtrack]");
			System.out.println("Agent actual Steps: "+actualSteps+" steps.");
		} else 	
		{												// If Maze no solution	
			System.out.println("Agent unable to reach the Target!");
//			newMaze.displayMaze();
		}
	}
	
	private static boolean findPath(Point toc, MazeGenerator newMaze, Agent AgentOne, Target TargetGoal) 
	{													// Method for the recursive function
		// Update the possible movement of Agent based on the cheapest Route
		// Available, which is based on HSA A*

		Point[] sortedRoute = catchTarget(newMaze, AgentOne, TargetGoal);

		for (Point potentialMove : sortedRoute) 
		{
			if (potentialMove != null) 
			{
				if (squareIsFree(potentialMove, newMaze)) 
				{
					enterSquare(potentialMove,newMaze,AgentOne);
					
					AgentOne.setLocation(AgentOne.findLocation(AgentOne.Namecode(), newMaze));
					
					if (mazeFinished(AgentOne.getLocation(),TargetGoal)) 
					{
						return true;
					}
					
					//	Bonus Portion
					if (steps%2 == 0)
					{						
						Point[] targetSidePoint = Target.getSidePoint(TargetGoal.getLocation());
						Point[] targetPossibleRoute = Target.getPossibleRoute(targetSidePoint, newMaze);
						
//						System.out.println("\nTarget Possible Route: ");
						int targetPossibleRouteCounter = 0;
						for (Point routeCalc : targetPossibleRoute) 
							{
								if (routeCalc != null)
								{
									targetPossibleRouteCounter ++;
//									System.out.println(routeCalc.x+","+routeCalc.y);
								}
							}
						
						Random randomGenerator = new Random();
						int targetRandomIndex = randomGenerator.nextInt(targetPossibleRouteCounter);
						
//						System.out.println("############");
//						
//						System.out.println("Random Number: "+targetRandomIndex);
//						System.out.println("Target Route choosen: "+targetPossibleRoute[targetRandomIndex].x+","+targetPossibleRoute[targetRandomIndex].y);
						enterSquare2(targetPossibleRoute[targetRandomIndex],newMaze,TargetGoal);
						
//						System.out.println("############");
						
//						newMaze.displayMaze();	
					}
					if (findPath(potentialMove,newMaze,AgentOne,TargetGoal)) 
					{									// If no available moves 
						return true;					// backtracking begins
					}
					exitSquare(potentialMove,newMaze, AgentOne);
				}
			}
		}
		return false;
	}

	private static boolean mazeFinished(Point location, Target TargetGoal) 
	{													// Check has the Agent reach the Target
		return location.equals(TargetGoal.getLocation());
	}												
	
	private static boolean squareIsFree(Point square , MazeGenerator newMaze) 
	{													// Method to check Movement
		if (square.x < 0 || square.x >= newMaze.getRow() || 
			square.y < 0 || square.y >= newMaze.getColumn()) 		
		{
			return false;
		}
		return (newMaze.getMazeArray()[square.x][square.y] == FREE || 
				newMaze.getMazeArray()[square.x][square.y] == TARGET);
	}
	
	private static void enterSquare(Point square, MazeGenerator newMaze, Agent AgentOne)
	{													// Update new location of Agent	

//		System.out.println("Route Chosen: "+square.x+","+square.y);
		
		newMaze.getMazeArray()[square.x][square.y] = AGENT;
		IO.NecroSave(AgentOne.getName(), 'N', square.x, square.y);
		
		if 												// To check Agent and Actual location
		(												// Update only when they are correct
		square.x == AgentOne.getLocation().x+1 && square.y == AgentOne.getLocation().y ||
		square.x == AgentOne.getLocation().x && square.y == AgentOne.getLocation().y+1 ||
		square.x == AgentOne.getLocation().x-1 && square.y == AgentOne.getLocation().y ||
		square.x == AgentOne.getLocation().x && square.y == AgentOne.getLocation().y-1 
		)
		{
			newMaze.getMazeArray()[AgentOne.getLocation().x][AgentOne.getLocation().y] = PATH;
			IO.NecroSave(AgentOne.getName(), 'P', AgentOne.getLocation().x, AgentOne.getLocation().y);
		}
		steps++;										// Steps taken including recursive 
		actualSteps++;									// Steps for shortest route
	}
	
	private static void enterSquare2(Point square, MazeGenerator newMaze, Target Target)
	{													// Update new location of Agent	
		newMaze.getMazeArray()[square.x][square.y] = TARGET;
		IO.NecroSave(Target.getName(), 'N', square.x, square.y);
		
		if 												// To check Agent and Actual location
		(												// Update only when they are correct
		square.x == Target.getLocation().x+1 && square.y == Target.getLocation().y ||
		square.x == Target.getLocation().x && square.y == Target.getLocation().y+1 ||
		square.x == Target.getLocation().x-1 && square.y == Target.getLocation().y ||
		square.x == Target.getLocation().x && square.y == Target.getLocation().y-1 
		)
		{
			newMaze.getMazeArray()[Target.getLocation().x][Target.getLocation().y] = FREE;
			IO.NecroSave(Target.getName(), 'R', Target.getLocation().x, Target.getLocation().y);
		}
	}
	
	private static void exitSquare(Point square, MazeGenerator newMaze, Agent AgentOne) 
	{													// Method to update Array for backtrack
		newMaze.getMazeArray()[square.x][square.y] = '@';
		IO.NecroSave(AgentOne.getName(), 'R', square.x, square.y);
		
		steps++;										// Steps taken including recursive
		actualSteps--;									// Steps for shortest route
	}
	
	public int steps()
	{
		return steps;
	}
	public int stepsActual()
	{
		return actualSteps;
	}
}
