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
	int *availableRooms;
	int lenghtAvailableRooms;
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

int evaluationFunction(int *allocation, struct Constraints *softConstraints, struct Room *rooms, struct Entity *entities)
{
}

void checkNeighborhood(int *allocation, struct Entity *entities)
{
}

void resetAvailableRooms(struct Entity *entities)
{
}

// Main

int main(int argc, char **argv)
{
	printf("Hello World!\n");
	return 0;
}
