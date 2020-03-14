// Imports

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structs

struct Entity
{
	int index;
	int group;
	float space;
};

struct Room
{
	int index;
	int floor;
	float space;
	int *adjacentRooms;
};

struct Constraints
{
	int index;
	int type;
	int c1;
	int c2;
};

// Functions

void constructInitialSolution(int *allocation, struct Constraints *softConstraints, struct Constraints *hardConstraints, struct Room *rooms, struct Entity *entities)
{
}

void checkHard(int *allocation, struct Constraints *hardConstraints, struct Room *rooms, struct Entity *entities)
{
}

void checkSoft(int *allocation, struct Constraints *softConstraints, struct Room *rooms, struct Entity *entities)
{
}

int solutionValue(int *allocation, struct Constraints *softConstraints, struct Room *rooms, struct Entity *entities)
{
}

void checkNeighborhood(int *allocation, int *visited)
{
}

// Main

int main(int argc, char **argv)
{
	printf("Hello World!\n");
	return 0;
}
