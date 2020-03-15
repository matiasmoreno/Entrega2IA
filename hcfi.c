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
	int adjSize;
};

struct Constraint
{
	int index;
	int type;
	int c1;
	int c2;
};

// Functions

void initialEntityData(struct Room *rooms, struct Entity *entities, int nRooms, int nEntities)
{
	for (int i = 0; i < nEntities; i++)
	{
		entities[i].availableRooms = malloc(sizeof(int) * nRooms);
		for (int j = 0; j < nRooms; j++)
		{
			entities[i].availableRooms[j] = j;
		}
		entities[i].lenghtAvailableRooms = nRooms;
	}
}

void constructInitialSolution(int *allocation, struct Constraint *softConstraints, struct Constraint *hardConstraints, struct Room *rooms, struct Entity *entities, int nSoftConstraints, int nHardConstraints, int nRooms, int nEntities)
{
	int i, entity1, entity2, room, floor;

	// Satisfy hard restriction 0: ALLOCATION_CONSTRAINT

	for (i = 0; i < nHardConstraints; i++)
	{
		if (hardConstraints[i].type == 0)
		{
			// Entity
			entity1 = hardConstraints[i].c1;
			// Room
			room = hardConstraints[i].c2;
			if (allocation[entity1] == 0)
			{
				allocation[entity1] = room;
			}
		}
	}

	// Satisfy hard restriction 4: SAMEROOM_CONSTRAINT

	// Satisfy hard restriction 7: ADJACENCY_CONSTRAINT

	// Satisfy hard restriction 8: NEARBY_CONSTRAINT

	// Satisfy hard restriction 6: NOTSHARING_CONSTRAINT

	// Satisfy hard restriction 9: AWAYFROM_CONSTRAINT

	// Satisfy hard restriction 5: NOTSAMEROOM_CONSTRAINT

	// Satisfy hard restriction 1: NONALLOCATION_CONSTRAINT

	// Satisfy soft restriction 0: ALLOCATION_CONSTRAINT

	for (i = 0; i < nSoftConstraints; i++)
	{
		if (softConstraints[i].type == 0)
		{
			// Entity
			entity1 = softConstraints[i].c1;
			// Room
			room = softConstraints[i].c2;
			if (allocation[entity1] == 0)
			{
				allocation[entity1] = room;
			}
		}
	}

	// Satisfy soft restriction 4: SAMEROOM_CONSTRAINT

	// Satisfy soft restriction 7: ADJACENCY_CONSTRAINT

	// Satisfy soft restriction 8: NEARBY_CONSTRAINT

	// Satisfy soft restriction 6: NOTSHARING_CONSTRAINT

	// Satisfy soft restriction 9: AWAYFROM_CONSTRAINT

	// Satisfy soft restriction 5: NOTSAMEROOM_CONSTRAINT

	// Satisfy soft restriction 1: NONALLOCATION_CONSTRAINT
}

void checkHard(int *allocation, struct Constraint *hardConstraints, struct Room *rooms, struct Entity *entities)
{
}

int evaluationFunction(int *allocation, struct Constraint *softConstraints, struct Room *rooms, struct Entity *entities)
{
}

void checkNeighborhood(int *allocation, struct Entity *entities, int *availableEntities)
{
}

void resetAvailableRooms(struct Entity *entities)
{
}

void resetAvailableEntities(int *availableEntities)
{
}

// Main

int main(int argc, char **argv)
{
	// Getting the data

	FILE *fp = fopen(argv[1], "r");

	if (fp == NULL)
	{
		printf("Could not open file %s", argv[1]);
		return -1;
	}

	char buffer[255], numberOf[50];
	int penalties[] = {20, 10, 10, 10, 10, 10, 50, 10, 10, 10}, nEntities, nRooms, nFloors, nConstraints, nHardConstraints, nSoftConstraints;
	int i, j, k, n;
	struct Entity *entities;
	struct Room *rooms;
	struct Constraint *hardConstraints;
	struct Constraint *softConstraints;

	// Get entities

	i = 0;
	while (i < 6)
	{
		if (fgets(buffer, 255, fp) != NULL)
		{
			sscanf(buffer, "%s %d", numberOf, &n);
			switch (i)
			{
			case 0:
				nEntities = n;
				entities = malloc(sizeof(struct Entity) * n);
				break;

			case 1:
				nRooms = n;
				rooms = malloc(sizeof(struct Room) * n);
				break;

			case 2:
				nFloors = n;
				break;

			case 3:
				nConstraints = n;
				break;

			case 4:
				nHardConstraints = n;
				hardConstraints = malloc(sizeof(struct Constraint) * n);
				break;

			case 5:
				nSoftConstraints = n;
				softConstraints = malloc(sizeof(struct Constraint) * n);
				break;
			}
		}
		i++;
	}

	fgets(buffer, 255, fp);
	fgets(buffer, 255, fp);

	// Get Rooms

	i = 0;
	while (i < nEntities)
	{
		fgets(buffer, 255, fp);
		sscanf(buffer, "%d %d %f", &entities[i].index, &entities[i].group, &entities[i].space);
		i++;
	}

	fgets(buffer, 255, fp);
	fgets(buffer, 255, fp);

	i = 0;
	char *token;
	while (i < nRooms)
	{
		fgets(buffer, 255, fp);
		sscanf(buffer, "%d %d %f %d", &rooms[i].index, &rooms[i].floor, &rooms[i].space, &rooms[i].adjSize);
		rooms[i].adjacentRooms = malloc(sizeof(int) * rooms[i].adjSize);

		// Get adjacent rooms

		token = strtok(buffer, " ");
		j = 0;
		while (token != NULL)
		{
			if (j > 3)
			{
				rooms[i].adjacentRooms[j - 4] = atoi(token);
			}
			j++;
			token = strtok(NULL, " ");
		}

		i++;
	}

	fgets(buffer, 255, fp);
	fgets(buffer, 255, fp);

	// Get constraints

	int CID, CTYPE, SorH, C1, C2;
	i = 0;
	j = 0;
	k = 0;
	while (i < nConstraints)
	{
		fgets(buffer, 255, fp);
		sscanf(buffer, "%d %d %d %d %d", &CID, &CTYPE, &SorH, &C1, &C2);
		if (SorH == 0)
		{
			softConstraints[j].index = CID;
			softConstraints[j].type = CTYPE;
			softConstraints[j].c1 = C1;
			softConstraints[j].c2 = C2;
			j++;
		}
		else
		{
			hardConstraints[k].index = CID;
			hardConstraints[k].type = CTYPE;
			hardConstraints[k].c1 = C1;
			hardConstraints[k].c2 = C2;
			k++;
		}
		i++;
	}

	// Initial available rooms for every entity

	initialEntityData(rooms, entities, nRooms, nEntities);

	// Hill Climbing + First Improvement Algorithm

	int bestSolQuality = 32000;
	int *bestSol = malloc(sizeof(int) * nEntities);

	int *allocation = malloc(sizeof(int) * nEntities);

	for (i = 0; i < nEntities; i++)
	{
		allocation[i] = 0;
	}

	for (i = 0; i < nEntities; i++)
	{
		printf("%d ", allocation[i]);
	}

	printf("\n ***** \n");

	constructInitialSolution(allocation, softConstraints, hardConstraints, rooms, entities, nSoftConstraints, nHardConstraints, nRooms, nEntities);

	for (i = 0; i < nEntities; i++)
	{
		printf("%d ", allocation[i]);
	}
	printf("\n");

	// Free memory

	fclose(fp);
	free(bestSol);
	for (i = 0; i < nEntities; i++)
	{
		free(entities[i].availableRooms);
	}

	for (i = 0; i < nRooms; i++)
	{
		free(rooms[i].adjacentRooms);
	}
	free(entities);
	free(rooms);
	free(hardConstraints);
	free(softConstraints);

	return 0;
}
