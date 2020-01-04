// GeneticAlgorithmSimulation.cpp - This is a program that demonstrates genetic algorithms using simulated robots
// Andrew Canafe - 12/11/2017
// CISP 400

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <list>
#include <vector>
//#pragma warning(disable:4996)

using namespace std;

// Object class - General class for map objects
class Object {
protected:
	char name;	// 0 = Nothing, 1 = Battery, 2 = Robot, 9 = Wall
public:
	char getName() { return name; }  // Returns ID of object
};

// Robot class - Implements genetic learning algorithms
class Robot : public Object {
private:
	Object*** world;	// 2D array of object pointers
	char genes[24][9];	// 24 genes with 9 chromosomes each
	char direction = 'N';		// Direction robot is facing
	int row = 0;			// Row location
	int col = 0;			// Column location
	int fitness = 0;	// Determined by number of batteries collected
	int energy = 10;	// Changes based on movement and battery collection
public:
	Robot();	// Default class constructor for robot
	Robot(Robot*, Robot*);	// Class constructor used for breeding a new robot
	~Robot() { world[row][col] = 0; }	// Automatically removes pointer in 2D array
	char* getGene(int g) { return genes[g]; }	// Returns specified gene in robot
	int getFitness() { return fitness; }	// Returns fitness of robot
	void takeAction();		// Make the robot do something
	bool checkMove(int, int);	// Check if move is legal, change fitness and energy accordingly
	void setWorld(Object***, int, int);		// Set 2D array and position of robot
	void printGenes();	// Used for debugging
};

// Default class constructor for robot
Robot::Robot() {
	
	// Robot ID
	name = '2';

	// Generate initial genes of robot
	for (int i = 0; i < 24; i++) {

		// First 4 chromosomes
		for (int j = 0; j < 4; j++) {
			int k = rand() % 4;
			switch (k) {
			case 0:
				genes[i][j] = '0';
				break;

			case 1:
				genes[i][j] = '1';
				break;

			case 2:
				genes[i][j] = '2';
				break;

			default:
				genes[i][j] = '9';
				break;
			}
		}

		// Middle 1 chromosome
		int k = rand() % 4;
		switch (k) {
		case 0:
			genes[i][4] = 'N';
			break;

		case 1:
			genes[i][4] = 'E';
			break;

		case 2:
			genes[i][4] = 'S';
			break;

		default:
			genes[i][4] = 'W';
			break;
		}

		// Last 4 chromosomes
		for (int j = 5; j < 9; j++) {
			int k = rand() % 4;
			switch (k) {
			case 0:
				genes[i][j] = 'M';
				break;

			case 1:
				genes[i][j] = 'L';
				break;

			case 2:
				genes[i][j] = 'R';
				break;

			default:
				genes[i][j] = 'X';
				break;
			}
		}
	}
}

// Class constructor used for breeding a new robot
Robot::Robot(Robot* r1, Robot* r2) {

	// Robot ID
	name = '2';

	// Breed new genes using first half of r1's genes
	for (int i = 0; i < 12; i++) {
		strncpy(genes[i], r1->getGene(i), 9);

		// 5% chance for mutation
		if (rand() % 100 < 5) {

			// Chromosome to overwrite
			int j = rand() % 9;

			// Value used for overwriting
			int k = rand() % 4;

			// First 4 chromosomes
			if (j < 4) {
				switch (k) {
				case 0:
					genes[i][j] = '0';
					break;

				case 1:
					genes[i][j] = '1';
					break;

				case 2:
					genes[i][j] = '2';
					break;

				default:
					genes[i][j] = '9';
					break;
				}
			}


			// Middle 1 chromosome
			else if (j == 4) {
				switch (k) {
				case 0:
					genes[i][4] = 'N';
					break;

				case 1:
					genes[i][4] = 'E';
					break;

				case 2:
					genes[i][4] = 'S';
					break;

				default:
					genes[i][4] = 'W';
					break;
				}
			}

			// Last 4 chromosomes
			else {
				switch (k) {
				case 0:
					genes[i][j] = 'M';
					break;

				case 1:
					genes[i][j] = 'L';
					break;

				case 2:
					genes[i][j] = 'R';
					break;

				default:
					genes[i][j] = 'X';
					break;
				}
			}
		}
	}


	// Breed new genes using second half of r2's genes
	for (int i = 12; i < 24; i++) {
		strncpy(genes[i], r2->getGene(i), 9);

		// 5% chance for mutation
		if (rand() % 100 < 5) {

			// Chromosome to overwrite
			int j = rand() % 9;
			
			// Value used for overwriting
			int k = rand() % 4;

			// First 4 chromosomes
			if (j < 4) {
				switch (k) {
				case 0:
					genes[i][j] = '0';
					break;

				case 1:
					genes[i][j] = '1';
					break;

				case 2:
					genes[i][j] = '2';
					break;

				default:
					genes[i][j] = '9';
					break;
				}
			}

			// Middle 1 chromosome
			else if (j == 4) {
				switch (k) {
				case 0:
					genes[i][4] = 'N';
					break;

				case 1:
					genes[i][4] = 'E';
					break;

				case 2:
					genes[i][4] = 'S';
					break;

				default:
					genes[i][4] = 'W';
					break;
				}
			}

			// Last 4 chromosomes
			else {
				switch (k) {
				case 0:
					genes[i][j] = 'M';
					break;

				case 1:
					genes[i][j] = 'L';
					break;

				case 2:
					genes[i][j] = 'R';
					break;

				default:
					genes[i][j] = 'X';
					break;
				}
			}
		}
	}
}

// Make the robot do something
void Robot::takeAction() {
	int bestIndex = 0;	// Best matching gene
	int bestMatch = 0;	// Number of matches for best gene

	// Initialize sensor data, use '0' if no objects detected around robot
	char sensors[5] = {
		world[row + 1][col] ? world[row + 1][col]->getName() : '0',
		world[row][col + 1] ? world[row][col + 1]->getName() : '0',
		world[row - 1][col] ? world[row - 1][col]->getName() : '0',
		world[row][col - 1] ? world[row][col - 1]->getName() : '0',
		direction
	};

	// Find best matching gene
	for (int i = 0; i < 24; i++) {
		int currMatch = 0;

		for (int j = 0; j < 5; j++) {
			if (sensors[j] == genes[i][j]) {
				currMatch++;
			}
		}

		if (currMatch > bestMatch) {
			bestIndex = i;
			bestMatch = currMatch;
		}
	}

	// Apply action using best matching gene
	for (int j = 5; j < 9; j++) {
		int rAdd = 0;	// Used for row movement
		int cAdd = 0;	// Used for col movement

		// Check behaviors
		switch (genes[bestIndex][j]) {

		// Move
		case 'M':
			switch (direction) {
			case 'N':
				rAdd++;
				break;
			case 'E':
				cAdd++;
				break;
			case 'S':
				rAdd--;
				break;
			case 'W':
				cAdd--;
				break;
			}

			// If valid move, move robot and adjust pointers inside 2D array
			if (checkMove(rAdd, cAdd)) {
				world[row][col] = 0;
				row += rAdd;
				col += cAdd;
				world[row][col] = this;
			}
			break;

		// Rotate left
		case 'L':
			switch (direction) {
			case 'N':
				direction = 'W';
				break;
			case 'E':
				direction = 'N';
				break;
			case 'S':
				direction = 'E';
				break;
			case 'W':
				direction = 'S';
				break;
			}
			break;

		// Rotate right
		case 'R':
			switch (direction) {
			case 'N':
				direction = 'E';
				break;
			case 'E':
				direction = 'S';
				break;
			case 'S':
				direction = 'W';
				break;
			case 'W':
				direction = 'N';
				break;
			}
			break;

		// Do nothing
		case 'X':
			break;
		}
	}
}

// Check if move is legal, change fitness and energy accordingly
bool Robot::checkMove(int rAdd, int cAdd) {
	
	// Return false immediately if energy is zero
	if (!energy) {
		return false;
	}

	// Decrement energy, check row and col for valid move
	energy--;
	rAdd += row;
	cAdd += col;

	// If object exists
	if (world[rAdd][cAdd]) {

		// If valid battery object ID, return true
		if (world[rAdd][cAdd]->getName() == '1') {

			// Automatically delete and clear pointer
			delete world[rAdd][cAdd];
			world[rAdd][cAdd] = 0;

			energy += 5;	// Add 5 energy
			fitness++;		// Add 1 fitness
			return true;
		}

		// Else return false
		else {
			return false;
		}
	}

	// Else return true
	else {
		return true;
	}
}

// Set 2D array and position of robot
void Robot::setWorld(Object*** w, int r, int c) {

	// If 2D array is set, remove current pointer inside 2D array
	if (world) {
		world[row][col] = 0;
	}

	world = w;	// Set 2D array
	row = r;	// Set row
	col = c;	// Set col

	// Set random direction
	int k = rand() % 4;
	switch (k) {
	case 0:
		direction = 'N';
		break;

	case 1:
		direction = 'E';
		break;

	case 2:
		direction = 'S';
		break;

	default:
		direction = 'W';
		break;
	}

	energy = 10;	// Set energy to 10
	fitness = 0;	// Set fitness to 0

	// Adjust pointer inside 2D array
	world[row][col] = this;
}

// Used for debugging
void Robot::printGenes() {
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 9; j++) {
			printf("%c", genes[i][j]);
		}
		printf("\t");
		for (int j = 0; j < 9; j++) {
			printf("%c", genes[i + 12][j]);
		}
		printf("\n");
	}
	printf("\n");
}

// Battery class - Collected by robot
class Battery : public Object {
public:
	Battery() { name = '1'; }	// Battery ID
};

// Wall class - Obstacle for robot
class Wall : public Object {
public:
	Wall() { name = '9'; }	// Wall ID
};

// Main
int main() {

	// Vector containing surviving/newly bred robots
	vector<Robot*> myRobots;
	vector<float> firstLastFitness;

	// Manually construct 2D array of pointers, initialized to zero
	Object*** world = new Object**[14];
	for (int i = 0; i < 14; i++) {
		world[i] = new Object*[14]();
	}

	// Total number of robots created
	int count = 0;

	// Program Greeting
	printf("This is a program that demonstrates genetic algorithms using simulated robots.\nThe end-of-round robot positions are displayed. Total population: 20000\n");
	printf("  0 - Nothing (not shown)\n");
	printf("  1 - Battery\n");
	printf("  2 - Robot\n");
	printf("  9 - Wall\n\n");

	printf("Press any key to continue...\n");
	getchar();
	//system("pause");

	printf("\n");

	// Build the walls (Can be optimized)
	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 14; j++) {
			if (i == 0 || i == 13 || j == 0 || j == 13) {
				world[i][j] = new Wall;
			}
		}
	}

	// Initial population size 10
	while (count < 10) {

		// Within bounds of walls
		int row = rand() % 12 + 1;
		int col = rand() % 12 + 1;

		// Check for valid random location
		if (!world[row][col]) {
			Robot* temp;
			temp = new Robot;

			// Push robot pointer to vector
			myRobots.push_back(temp);

			// Set 2D array of robot
			temp->setWorld(world, row, col);

			// Increment robot count
			count++;
		}
	}


	// Repeat until 20000 robots have been created
	while (count <= 20000) {

		// 58 batteries = 40% of squares within bounds of walls
		int battCount = 0;
		while (battCount < 58) {

			// Within bounds of walls
			int row = rand() % 12 + 1;
			int col = rand() % 12 + 1;

			// Check for valid random location
			if (!world[row][col]) {

				// Set pointer inside 2D array
				world[row][col] = new Battery;

				// Increment battery count
				battCount++;
			}
		}
	
		// 25 moves per robot
		for (int i = 0; i < 25; i++) {

			// 10 robots
			for (int j = 0; j < 10; j++) {
				myRobots[j]->takeAction();
			}
		}

		// Print all squares using object IDs
		for (int i = 0; i < 14; i++) {
			for (int j = 0; j < 14; j++) {
				if (world[i][j]) {
					printf("%c ", world[i][j]->getName());
				}

				else {
					printf("  ");
				}

			}
			printf("\n");
		}

		// Sort robots in vector by fitness and print average fitness
		float avgFitness = 0;
		sort(myRobots.begin(), myRobots.end(), [](Robot* a, Robot* b) { return a->getFitness() > b->getFitness(); });
		for (unsigned int i = 0; i < myRobots.size(); i++) {
			avgFitness += myRobots[i]->getFitness();
		}
		avgFitness /= 10;
		printf("Average Fitness: %.1f\n\n", avgFitness);
		
		if (firstLastFitness.size() == 2) {
			firstLastFitness.pop_back();
			firstLastFitness.push_back(avgFitness);
		}

		else {
			firstLastFitness.push_back(avgFitness);
		}

		// Delete all batteries and clear battery pointers in 2D array
		for (int i = 0; i < 14; i++) {
			for (int j = 0; j < 14; j++) {
				if (world[i][j] && world[i][j]->getName() == '1') {
					delete world[i][j];
					world[i][j] = 0;
				}
			}
		}

		// Kill off 5 weakest robots
		// 1 breeds with 2, 2 breeds with 3, 3 breeds with 4
		// 4 breeds with 5, 5 breeds with 1, totals 5 newly created robots
		for (int i = 0; i < 5; i++) {
			delete myRobots[i + 5];
			myRobots[i + 5] = new Robot(myRobots[i], myRobots[(i + 1) % 5]);
		}

		// Repeat until 10 robot pointers have been set inside 2D array
		int turnCount = 0;
		while (turnCount < 10) {

			// Within bounds of walls
			int row = rand() % 12 + 1;
			int col = rand() % 12 + 1;

			// Check for valid random location
			if (!world[row][col]) {

				// Set 2D array of robot
				myRobots[turnCount]->setWorld(world, row, col);

				// Increment count for newly created robots
				turnCount++;
			}
		}

		// Add 5 to total number of robots created
		count += 5;
	}

	// Bonus - Print final genes of robot
	printf("Final genes:\n");
	myRobots[0]->printGenes();

	// Bonus - Compare first and last generation average fitness
	float lastFitness = firstLastFitness.back();
	firstLastFitness.pop_back();
	printf("First Generation Average Fitness: %.1f\nLast  Generation Average Fitness: %.1f\n\n", firstLastFitness.back(), lastFitness);

	printf("Press any key to continue...\n");
	getchar();
	//system("pause");

	return 0;
}
