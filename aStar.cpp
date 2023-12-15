//Program to Explore a string Maze using A Star Search for CIS 479
#include <iostream>
#include <string>
#include <vector>
#include<queue>
#include <iomanip>
#include <list>
#include <algorithm>
#include <unordered_set>
#include <map>
#include <set>
#include <sstream>

using namespace std;

//Global Definitions and initializations
//Direction costs
#define WEST 2
#define NORTH 1
#define EAST 2
#define SOUTH 3
//Global Variable for maintaining H Costs
int goalROW = 0;
int goalCOLUMN = 0;
//Starting Node Label! 
string current = "00";

//Function Declarations
string exploredNodes(string);
void iterateMaze(int&, int&, string, vector<vector<string>>);
void printToScreen(vector<vector<string>>);


struct Node {
public:
	string nodeNumber;
	bool explored;
//Path Costs, Manhattan Distance: f(n) = g(n) + h(n)
	int costF;
//Cost to move to current node
	int costG;
//Cost to get to Goal Node
	int costH;
//Vector of children a parent has
	vector < Node*> child;
//Pointer to Parent
	Node* parent;


// Node Constructor
	Node(string node, int costF = 0, int costG = 0, int costH = 0, bool explored = false, Node* parent = nullptr){
		this->nodeNumber = node;
		this->costF = costF;
		this->costG = costG;
		this->costH = costH;
		this->parent = parent;
		this->explored = explored;
	}

//Adding Children
	void addChild(string n, int costF = 0, int costG = 0, int costH = 0, bool explored = false, vector<vector<string>> maze = {}){
		Node* node = new Node(n, costF, costG, costH, explored, this);
		node->calculateF(maze);
		this->child.push_back(node);
	}


//Calculating the Modified Manhattan Distance (Calculation of f(n) = g(n) + h(n)
	void calculateF(vector<vector<string>> maze){
//Initialize Variables
		int parentROW = 0;
		int parentCOLUMN = 0;
		int childROW = 0;
		int childCOLUMN = 0;
		int ROW = 0;
		int COLUMN = 0;

// Determining F Values in West, North, East, and South Order, Initialize First Parent
		if(this->parent != NULL){ iterateMaze(parentROW, parentCOLUMN, this->parent->nodeNumber, maze); }
//Function Call to get Adjacent Cells after First one is initialized
		iterateMaze(childROW, childCOLUMN, this->nodeNumber, maze);
		if(this->parent != NULL){
// Get Children of Parent
			if(parentROW != childROW){
// If subtracting child row value from parent equals 1 then child row is 1 DOWN (SOUTH) than Parent
				if(childROW - parentROW == 1){
					if(this->parent != NULL){ this->costG = this->parent->costG + SOUTH; }
// Initial South Move
					else this->costG = SOUTH;
				}
// Or Else it is 1 UP (NORTH) than Parent (= -1)
				else {
					if(this->parent != NULL){ this->costG = this->parent->costG + NORTH; }
// Initial NORTH Move
					else this->costG = NORTH;
				}
			}
// If Columns are not equal, then moved in either direction
			if(parentCOLUMN != childCOLUMN){
// If Child is 1 to RIGHT of parent (EAST) (=1)
				if(childCOLUMN - parentCOLUMN == 1){
					if(this->parent != NULL){ this->costG = this->parent->costG + EAST; }
					// Initial EAST Move
					else this->costG = EAST;
				}
// Else child moved to LEFT (WEST) (=-1)
				else {
					if(this->parent != NULL){ this->costG = this->parent->costG + WEST; }
// Initial WEST Move
					else this->costG = WEST;
				}
			}
		}
//H Value Calculated here
//First Subtract Goal's Location 
		ROW = childROW - goalROW;
		COLUMN = childCOLUMN - goalCOLUMN;

//If ROW=-1, Then We Need to go SOUTH to Reach Goal
		if(ROW < 0){
			ROW *= -1;
			this->costH = ROW * SOUTH;
		}
//Or else we Need to go NORTH to reach Goal
		else this->costH = ROW * NORTH;
//If COLUMN = -1, we need 1 to go EAST to reach Goal
		if(COLUMN < 0){
			COLUMN *= -1;
			this->costH += COLUMN * EAST;
		}
//Or else we Need to go WEST to reach Goal
		else this->costH += COLUMN * WEST;
		this->costF = this->costG + this->costH;
	}
};


//Function to determine the Lowest Cost between Child Nodes (Picking Smallest f(n)
void lowestCost(vector<Node*>& path){
	Node* temp = NULL;
//Get Lowest F Value as next node to expand
	for(int i = 0; i < path.size(); i++){
		for(int j = i + 1; j < path.size(); j++){
			if(path[i]->costF > path[j]->costF){
				temp = path[i];
				path[i] = path[j];
				path[j] = temp;
			}

//If Children have equal f values
			else if(path[i]->costF == path[j]->costF){
//Then get the Node that was added to the Queue first 
//Will have a smaller node number
				if(path[i]->nodeNumber > path[j]->nodeNumber){
					temp = path[i];
					path[i] = path[j];
					path[j] = temp;
				}
			}
		}
	}
}

vector<vector<string>> AStarSearch(vector<vector<string>> maze, Node* node, vector<Node*> frontier){
//Recursive call w/new current cell changes that cell to true for explored
	node->explored = true;
// Get neighbors! and Initialize variables
	int currentROW = 0;
	int currentCOLUMN = 0;
	iterateMaze(currentROW, currentCOLUMN, node->nodeNumber, maze);
//Garbage Collection
	frontier.erase(frontier.begin());
	
// IF REACHED GOAL DONE!! PRINT TO SCREEN 
	if(currentROW == goalROW && currentCOLUMN == goalCOLUMN){
		printToScreen(maze);
		return maze;
	}

// Get Neighbors if Goal is not yet reached! (Adding Leaves into the Frontier)
//Done using Labels instead of as a integer maze to allow any size of maze to be passed As long as it contains assignment specific labels
//Add Children in ORDER: WEST, NORTH, EAST, SOUTH per Assignment
//Checking WEST Child, Have to count for if Child is the Goal!
	if((currentCOLUMN > 0) && (maze[currentROW][currentCOLUMN - 1] == "G") || (currentCOLUMN > 0) && (maze[currentROW][currentCOLUMN - 1] == "[]")){
//Move to Maze Location: 1 Left
		maze[currentROW][currentCOLUMN - 1] = current;
//First Add WEST Child (1 COLUMN to LEFT)
		node->addChild(current, 0, 0, 0, false, maze);
		current = exploredNodes(current);
// Push F Value for Heuristic Search Comparison
		frontier.push_back(node->child[node->child.size() - 1]);
	}
//Checking NORTH Child
	if((currentROW > 0) && (maze[currentROW - 1][currentCOLUMN] == "[]") || (currentROW > 0) && (maze[currentROW - 1][currentCOLUMN] == "G")){
//Move to Maze Location: 1 UP
		maze[currentROW - 1][currentCOLUMN] = current;
//Add NORTH Child (1 ROW ABOVE)
		node->addChild(current, 0, 0, 0, false, maze);
		current = exploredNodes(current);
		frontier.push_back(node->child[node->child.size() - 1]);
	}
//Checking EAST CHild
	if((currentCOLUMN < maze[0].size() - 1) && (maze[currentROW][currentCOLUMN + 1] == "G") || (currentCOLUMN < maze[0].size() - 1) && (maze[currentROW][currentCOLUMN + 1] == "[]")){
//Move to Maze Location: 1 Right
		maze[currentROW][currentCOLUMN + 1] = current;
//Add EAST Child (1 COLUMN to RIGHT)
		node->addChild(current, 0, 0, 0, false, maze);
		current = exploredNodes(current);
		frontier.push_back(node->child[node->child.size() - 1]);
	}
//Checking SOUTH Child
	if((currentROW < maze.size() - 1) && (maze[currentROW + 1][currentCOLUMN] == "G") || (currentROW < maze.size() - 1) && (maze[currentROW + 1][currentCOLUMN] == "[]")){
//Move to Maze Location: 1 DOWN
		maze[currentROW + 1][currentCOLUMN] = current;
//Add SOUTH Child (1 ROW BELOW)
		node->addChild(current, 0, 0, 0, false, maze);
// Mark the current node 
		current = exploredNodes(current);
		frontier.push_back(node->child[node->child.size() - 1]);
	}

//Function Call to Get Lowest F Value Costs in Order for choosing
	lowestCost(frontier);
//Recursive Call to Self to Expand Node
	AStarSearch(maze, frontier[0], frontier);
	return maze;
}

int main(){
	cout << "A Star Search Through a Maze!" << endl;
//Variable Declarations
//11x12 Matrix of string values
	vector<vector<string>> myMaze = {
		{"##", "S", "##", "##", "##", "##", "##", "##", "##", "##", "##", "##" },
		{"##", "[]", "[]", "[]", "[]", "##", "[]", "[]", "[]", "[]", "[]", "##" },
		{"##", "[]", "##", "##", "[]", "##", "[]", "##", "##", "##", "[]", "##" },
		{"##", "[]", "##", "[]", "[]", "##", "[]", "[]", "[]", "##", "[]", "G" },
		{"##", "[]", "##", "##", "##", "##", "##", "##", "[]", "##", "##", "##" },
		{"##", "[]", "[]", "##", "[]","[]", "[]", "##", "[]", "[]", "[]", "##" },
		{"##", "##", "[]", "##", "[]", "##", "##", "##", "[]", "##", "[]", "##" },
		{"##", "[]", "[]", "[]", "[]", "##", "[]", "[]", "[]", "##", "[]", "##" },
		{"##", "[]", "##", "##", "##", "##", "[]", "##", "[]", "##", "##", "##" },
		{"##", "[]", "[]", "[]", "[]","[]", "[]", "##", "[]", "[]", "[]", "##" },
		{"##", "##", "##", "##", "##", "##", "##", "##", "##", "##", "##", "##" } };
	vector<Node*> pQ;
	int startROW = 0;
	int startCOLUMN = 0;
// Walk through Maze until find Start Location Marked with "S"
	iterateMaze(startROW, startCOLUMN, "S", myMaze);
	myMaze[startROW][startCOLUMN] = current;
// Walk through Maze until find Goal Location Marked with "G"
	iterateMaze(goalROW, goalCOLUMN, "G", myMaze);
// Initialize Starting Node with Start Location
	Node* root = new Node(current);
// Mark Starting Node as Explroed
	current = exploredNodes(current);
// Initilize maze
	root->calculateF(myMaze);
	pQ.push_back(root);
	lowestCost(pQ);

// Do A Star Search
	AStarSearch(myMaze, root, pQ);

	return 0;
}

//Mark Explored Nodes in Order they are explored, Had Issues when tried to use an integer grid then change to 
//string, had to implement this way to mark when explored
string exploredNodes(string node){
	stringstream ss;
	int i;
	ss << node;
	ss >> i;
	i++;
	ss.clear();
	ss << i;
	ss >> node;
// Have to add another 0 in front of numbers 0-9 output formatting purposes
	if(node.length() == 1){ node = "0" + node; }
	return node;
}

//Walk through maze to get Cell Information
void iterateMaze(int& i, int& j, string node, vector<vector<string>> maze){
	for(i = 0; i < maze.size(); i++){
		for(j = 0; j < maze[i].size(); j++){
			if(maze[i][j] == node){ return; }
			else continue;
		}
	}
}

//Print to Screen Order if Goal Found
void printToScreen(vector<vector<string>> maze){
	for(int i = 0; i < maze.size(); i++){
		for(int j = 0; j < maze[i].size(); j++){ cout << maze[i][j] << " "; }
		cout << endl;
	}
	cout << endl;
}

