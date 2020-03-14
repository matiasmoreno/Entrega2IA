// Imports

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Structs

struct Entity{
	int index,
	int group,
	float space,
}

struct Room{
	int index,
	int floor,
	float space,
	int* adjacentRooms
}

struct Constraints{
	int index,
	int type,
	int sorh,
	int c1,
	int c2
}

// Functions

// Main