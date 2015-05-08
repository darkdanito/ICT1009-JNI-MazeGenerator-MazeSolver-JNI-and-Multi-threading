
=========================================================================
V3.19

Added Multi-threaded code.
C++ GUI is running with java at the same time.
There is SHITTY load of bugs. Not sober as of typing this....

=========================================================================
V3.18

Added Solve Button [Click and it will auto close the GUI]
Added some conditions for appearing for certain buttons.

=========================================================================
V3.17

Refactor code.
Added comments
Removed unwanted codes 

=========================================================================
V3.16.1 

Fixed default black color for maze.
Fixed Render issues.

=========================================================================
V3.16

C++ solve button and solve step by step will be disabled when mazegrid is 0
Renamed variables
Added  Steps method to get it in C++

=========================================================================
V3.15

whatever updates.

=========================================================================
V3.14

Java open WinMain is INFININE now.
Only way to exit is by exit Button from menu.

=========================================================================
V3.13.1

Fixed TL's Change color dialog box error. As the wall and path are opposite.
Fixed path color. it can change color now. As earlier both are wall.
Made the wall and path box smaller. so i can code agent and target color as well.

=========================================================================
V3.13

Fixed Bug 1: Function to change color still not working
Fixed Bug 12: Target Intermittently can move into wall.
C++ code cleanup.

=========================================================================
V3.12

Resolved Flickering of GUI.
Optimizations on Java Code and the Error Message.
Fixed Bug 4.Rendering of Agent chasing Target in GUI issues. [Not tally or having some missing path].
Fixed Bug 6.Cannot Tell the differences if the Target is walking around the same path or path he visited before
Fixed Bug 10.Target got a lot of render issues in GUI

=========================================================================
V3.11

Solve Step by Step is working. But got bug as usual. 

=========================================================================
V3.10

Added another buttons for the step by step.
Added code on Java end to update txt of the steps taken.


=========================================================================
V3.9.1

Fix the mouseover input for the GUI.

=========================================================================
V3.9

Added 2 Buttons to the code. Functionality wise not really working.
Removed the Menu button for the user input for Agent/Target from earlier attempts.
Removed invalidate command cheat code for VM_PAINT. Relocated the code somewhere else.

=========================================================================
V3.8

C++ user input is validated. Min 4. Max 100.
C++ GUI is Maximized on load.
Message box when user choose a wall.

=========================================================================
V3.7

C++ GUI can take in Agent and Target Position by mouseclicks.
Java Input validation will only run when Agent or Target position is not found.
C++ Agent and Target validate wall before updating.

=========================================================================
V3.6

Removed unnecessary commented codes.
Rename some resource.rc variables.

=========================================================================
V3.5

Added maze out to txt --> to all algorithm.
All Maze 1 and 0 are Synced.
Fixed M x N to file issues.
Fix the .ico Issues.

=========================================================================
V3.4

Removed the need for C Code. Java --> C++ straight.
SYNC theh walls and path for Recursive, Recursive Bonus, file. Left HK.
Java Files can be open as a project in Eclipse now. [Need 32 Bit]

=========================================================================
V3.3

Removed the renderHK. All Mazes and Openfile uses the same Render ().
Added function to convert mazeGrid to txt for D3. 
Intergrated D1 code into the D3 folder.
All codes are now run from D1 code and D2 codes.

To DO: [YY]

Need to SYNC the wall and path.
WALL = 0
Path = 1

=========================================================================
V3.2

Applied Fixes for Bug 1: Convert String to Int for user input
Applied Fixes for Bug 2: Update the GUI without resizing
Added Validation for user Input for numbers only
Reordered the code for the flow

=========================================================================
V3.1

Replicated the D2 GUI 
Minor optimization of code

=========================================================================
V3.0

Added Dialog Box and Menu Button
