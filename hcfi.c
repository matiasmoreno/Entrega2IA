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
	bool used;
};

// Functions

void resetHardConstraints(struct Constraint *hardConstraints, int nHardConstraints)
{
	for (int i = 0; i < nHardConstraints; i++)
	{
		if (hardConstraints[i].type == 3)
		{
			hardConstraints[i].used = true;
		}
		else
		{
			hardConstraints[i].used = false;
		}
	}
}

void initialEntityData(struct Room *rooms, struct Entity *entities, int nRooms, int nEntities)
{
	for (int i = 0; i < nEntities; i++)
	{
		entities[i].availableRooms = malloc(sizeof(int) * nRooms);
		for (int j = 0; j < nRooms; j++)
		{
			entities[i].availableRooms[j] = j;
		}
	}
}

bool violatesHard(int *allocation, struct Constraint *hardConstraints, struct Room *rooms, struct Entity *entities, int nHardConstraints, int nRooms, int nEntities)
{
	int i, j, entity1, entity2, room, room1, room2;
	float space, totalUsed;
	bool adjacent;
	for (i = 0; i < nHardConstraints; i++)
	{
		if (hardConstraints[i].used)
		{
			// Check hard restriction 0: ALLOCATION_CONSTRAINT
			if (hardConstraints[i].type == 0)
			{
				entity1 = hardConstraints[i].c1;
				room = hardConstraints[i].c2;
				if (allocation[entity1] != room)
				{
					return true;
				}
			}
			// Check hard restriction 1: NONALLOCATION_CONSTRAINT
			if (hardConstraints[i].type == 1)
			{
				entity1 = hardConstraints[i].c1;
				room = hardConstraints[i].c2;
				if (allocation[entity1] == room)
				{
					return true;
				}
			}
			// Check hard restriction 3: CAPACITY_CONSTRAINT
			if (hardConstraints[i].type == 3)
			{
				totalUsed = 0;
				room = hardConstraints[i].c1;
				space = rooms[room].space;
				for (j = 0; j < nEntities; j++)
				{
					if (allocation[j] == room)
					{
						totalUsed = totalUsed + entities[j].space;
					}
				}
				if (totalUsed > space)
				{
					return true;
				}
			}
			// Check hard restriction 4: SAMEROOM_CONSTRAINT
			if (hardConstraints[i].type == 4)
			{
				entity1 = hardConstraints[i].c1;
				entity2 = hardConstraints[i].c2;
				if (allocation[entity1] != allocation[entity2])
				{
					return true;
				}
			}
			// Check hard restriction 5: NOTSAMEROOM_CONSTRAINT
			if (hardConstraints[i].type == 5)
			{
				entity1 = hardConstraints[i].c1;
				entity2 = hardConstraints[i].c2;
				if (allocation[entity1] == allocation[entity2])
				{
					printf("Se viola restriccion NOTSAMEROOM_CONSTRAINT\n");
					return true;
				}
			}
			// Check hard restriction 6: NOTSHARING_CONSTRAINT
			if (hardConstraints[i].type == 6)
			{
				entity1 = hardConstraints[i].c1;
				room = allocation[entity1];
				for (j = 0; j < nEntities; j++)
				{
					if (j != entity1)
					{
						if (allocation[j] == room)
						{
							printf("Se viola restriccion NOTSHARING_CONSTRAINT\n");
							return true;
						}
					}
				}
			}

			// Check hard restriction 7: ADJACENCY_CONSTRAINT
			if (hardConstraints[i].type == 7)
			{
				entity1 = hardConstraints[i].c1;
				entity2 = hardConstraints[i].c2;
				room1 = allocation[entity1];
				room2 = allocation[entity2];
				adjacent = false;
				if (room1 == room2)
				{
					adjacent = true;
				}
				for (j = 0; j < rooms[room2].adjSize; j++)
				{
					if (room1 == rooms[room2].adjacentRooms[j])
					{
						adjacent = true;
					}
				}
				if (adjacent == false)
				{
					printf("Se viola restriccion ADJACENCY_CONSTRAINT\n");
					return true;
				}
			}
			// Check hard restriction 8: NEARBY_CONSTRAINT
			// Check hard restriction 9: AWAYFROM_CONSTRAINT
		}
	}
	return false;
}

int evaluationFunction(int *allocation, struct Constraint *softConstraints, struct Room *rooms, struct Entity *entities)
{
	return 99999;
}

void checkNeighborhood(int *allocation, struct Entity *entities, int *availableEntities)
{
}

void resetAvailableRooms(struct Entity *entities, int nEntities, int nRooms)
{
	for (int i = 0; i < nEntities; i++)
	{
		for (int j = 0; j < nRooms; j++)
		{
			entities[i].availableRooms[j] = j;
		}
	}
}

void resetAvailableEntities(int *availableEntities)
{
}

int getRoom(struct Entity *entities, int nRooms, int index)
{
	int i, j, random, room;

	j = 0;
	for (i = 0; i < nRooms; i++)
	{
		if (entities[index].availableRooms[i] != -1)
		{
			j++;
		}
	}

	int roomPicker[j];

	j = 0;
	for (i = 0; i < nRooms; i++)
	{
		if (entities[index].availableRooms[i] != -1)
		{
			roomPicker[j] = entities[index].availableRooms[i];
			j++;
		}
	}

	if (j == 0)
	{
		return -1;
	}

	random = rand() % j;
	room = roomPicker[random];
	entities[index].availableRooms[random] = -1;
	printf("Room picker out, return %d\n", room);
	if (room > 91)
	{
		printf("Error\n");
		for (int k = 0; k < nRooms; k++)
		{
			printf("Habitacion en available rooms: %d\n", entities[index].availableRooms[k]);
		}

		for (int k = 0; k < j; k++)
		{
			printf("Habitacion en room picker: %d\n", roomPicker[k]);
		}
	}

	return room;
}

bool constructInitialSolution(int *allocation, struct Constraint *softConstraints, struct Constraint *hardConstraints, struct Room *rooms, struct Entity *entities, int nSoftConstraints, int nHardConstraints, int nRooms, int nEntities)
{
	resetHardConstraints(hardConstraints, nHardConstraints);

	bool allocated = false;
	int i, j, entity1, entity2, room, room2;

	// Satisfy hard restriction 0: ALLOCATION_CONSTRAINT

	for (i = 0; i < nHardConstraints; i++)
	{
		if (hardConstraints[i].type == 0)
		{
			// Entity
			entity1 = hardConstraints[i].c1;
			// Room
			room = hardConstraints[i].c2;
			if (allocation[entity1] == -1)
			{
				allocation[entity1] = room;
				if (violatesHard(allocation, hardConstraints, rooms, entities, nHardConstraints, nRooms, nEntities))
				{
					return false;
				}
			}
			resetAvailableRooms(entities, nEntities, nRooms);
			hardConstraints[i].used = true;
		}
	}

	// Satisfy hard restriction 6: NOTSHARING_CONSTRAINT

	for (i = 0; i < nHardConstraints; i++)
	{
		if (hardConstraints[i].type == 6)
		{
			entity1 = hardConstraints[i].c1;

			allocated = false;

			if (allocation[entity1] != -1)
			{
				allocated = true;
			}

			while (!allocated)
			{
				room = getRoom(entities, nRooms, entity1);
				if (room == -1)
				{
					printf("Entidad %d no pudo ser asignada\n", i);
					return false;
				}
				allocation[entity1] = room;
				if (!violatesHard(allocation, hardConstraints, rooms, entities, nHardConstraints, nRooms, nEntities))
				{
					allocated = true;
					resetAvailableRooms(entities, nEntities, nRooms);
				}
			}
			hardConstraints[i].used = true;
		}
	}

	// Satisfy hard restriction 4: SAMEROOM_CONSTRAINT

	for (i = 0; i < nHardConstraints; i++)
	{
		if (hardConstraints[i].type == 4)
		{
			entity1 = hardConstraints[i].c1;
			entity2 = hardConstraints[i].c2;

			allocated = false;

			if (allocation[entity1] != -1)
			{
				allocation[entity2] = allocation[entity1];
				if (!violatesHard(allocation, hardConstraints, rooms, entities, nHardConstraints, nRooms, nEntities))
				{
					allocated = true;
				}
			}
			else if (allocation[entity2] != -1)
			{
				if (!violatesHard(allocation, hardConstraints, rooms, entities, nHardConstraints, nRooms, nEntities))
				{
					allocated = true;
				}
			}

			while (!allocated)
			{
				room = getRoom(entities, nRooms, entity1);
				if (room == -1)
				{
					printf("Entidad %d no pudo ser asignada\n", i);
					return false;
				}
				allocation[entity1] = room;
				allocation[entity2] = room;
				if (!violatesHard(allocation, hardConstraints, rooms, entities, nHardConstraints, nRooms, nEntities))
				{
					allocated = true;
					resetAvailableRooms(entities, nEntities, nRooms);
				}
			}
			hardConstraints[i].used = true;
		}
	}

	// Satisfy hard restriction 7: ADJACENCY_CONSTRAINT

	for (i = 0; i < nHardConstraints; i++)
	{
		if (hardConstraints[i].type == 7)
		{
			entity1 = hardConstraints[i].c1;
			entity2 = hardConstraints[i].c2;

			allocated = false;
			while (!allocated)
			{
				room = getRoom(entities, nRooms, entity1);
				if (room == -1)
				{
					printf("Entidad %d no pudo ser asignada\n", i);
					return false;
				}
				allocation[entity1] = room;
				for (j = 0; j < rooms[room].adjSize; j++)
				{
					if (!allocated)
					{
						room2 = rooms[room].adjacentRooms[j];
						allocation[entity2] = room2;
						if (!violatesHard(allocation, hardConstraints, rooms, entities, nHardConstraints, nRooms, nEntities))
						{
							allocated = true;
							resetAvailableRooms(entities, nEntities, nRooms);
						}
					}
				}
			}
			hardConstraints[i].used = true;
		}
	}

	// Satisfy hard restriction 8: NEARBY_CONSTRAINT

	for (i = 0; i < nHardConstraints; i++)
	{
		if (hardConstraints[i].type == 8)
		{
			entity1 = hardConstraints[i].c1;
			entity2 = hardConstraints[i].c2;

			allocated = false;
			while (!allocated)
			{
				room = getRoom(entities, nRooms, entity1);
				if (room == -1)
				{
					printf("Entidad %d no pudo ser asignada\n", i);
					return false;
				}
				allocation[entity1] = room;
				for (j = 0; j < nRooms; j++)
				{
					if (!allocated)
					{
						if (rooms[room].floor == rooms[j].floor)
						{
							room2 = j;
							allocation[entity2] = room2;
							if (!violatesHard(allocation, hardConstraints, rooms, entities, nHardConstraints, nRooms, nEntities))
							{
								allocated = true;
								resetAvailableRooms(entities, nEntities, nRooms);
							}
						}
					}
				}
			}
			hardConstraints[i].used = true;
		}
	}

	// Satisfy hard restriction 9: AWAYFROM_CONSTRAINT

	for (i = 0; i < nHardConstraints; i++)
	{
		if (hardConstraints[i].type == 9)
		{
			entity1 = hardConstraints[i].c1;
			entity2 = hardConstraints[i].c2;

			allocated = false;

			if (allocation[entity1] != -1)
			{
				if (allocation[entity2] != -1)
				{
					if (rooms[allocation[entity1]].floor != rooms[allocation[entity2]].floor)
					{
						allocated = true;
					}
				}
			}

			while (!allocated)
			{
				room = getRoom(entities, nRooms, entity1);
				if (room == -1)
				{
					printf("Entidad %d no pudo ser asignada\n", i);
					return false;
				}
				allocation[entity1] = room;
				for (j = 0; j < nRooms; j++)
				{
					if (!allocated)
					{
						if (rooms[room].floor != rooms[j].floor)
						{
							room2 = j;
							allocation[entity2] = room2;
							if (!violatesHard(allocation, hardConstraints, rooms, entities, nHardConstraints, nRooms, nEntities))
							{
								allocated = true;
								resetAvailableRooms(entities, nEntities, nRooms);
							}
						}
					}
				}
			}
			hardConstraints[i].used = true;
		}
	}

	// Satisfy hard restriction 5: NOTSAMEROOM_CONSTRAINT

	for (i = 0; i < nHardConstraints; i++)
	{
		if (hardConstraints[i].type == 5)
		{
			entity1 = hardConstraints[i].c1;
			entity2 = hardConstraints[i].c2;

			allocated = false;

			if (allocation[entity1] != -1)
			{
				if (allocation[entity2] != -1)
				{
					if (allocation[entity1] != allocation[entity2])
					{
						allocated = true;
					}
				}
			}

			while (!allocated)
			{
				room = getRoom(entities, nRooms, entity1);
				if (room == -1)
				{
					printf("Entidad %d no pudo ser asignada\n", i);
					return false;
				}
				allocation[entity1] = room;
				for (j = 0; j < nRooms; j++)
				{
					if (!allocated)
					{
						if (allocation[entity1] != j)
						{
							room2 = j;
							allocation[entity2] = room2;
							if (!violatesHard(allocation, hardConstraints, rooms, entities, nHardConstraints, nRooms, nEntities))
							{
								allocated = true;
								resetAvailableRooms(entities, nEntities, nRooms);
							}
						}
					}
				}
			}
			hardConstraints[i].used = true;
		}
	}

	// Satisfy hard restriction 1: NONALLOCATION_CONSTRAINT

	for (i = 0; i < nHardConstraints; i++)
	{
		if (hardConstraints[i].type == 1)
		{
			entity1 = hardConstraints[i].c1;
			room2 = hardConstraints[i].c2;
			allocation = false;
			if (allocation[entity1] == -1)
			{
				if (allocation[entity1] != room2)
				{
					allocated = true;
				}
			}
			while (!allocation)
			{
				room = getRoom(entities, nRooms, entity1);
				if (room == -1)
				{
					printf("Entidad %d no pudo ser asignada\n", i);
					return false;
				}
				if (room != room2)
				{
					if (!violatesHard(allocation, hardConstraints, rooms, entities, nHardConstraints, nRooms, nEntities))
					{
						allocated = true;
						resetAvailableRooms(entities, nEntities, nRooms);
					}
				}
			}
			hardConstraints[i].used = true;
		}
	}

	// Soft restrictions

	// Satisfy soft restriction 6: NOTSHARING_CONSTRAINT

	for (i = 0; i < nSoftConstraints; i++)
	{
		if (softConstraints[i].type == 6)
		{
			entity1 = softConstraints[i].c1;

			allocated = false;
			while (!allocated)
			{
				room = getRoom(entities, nRooms, entity1);
				if (room == -1)
				{
					printf("Entidad %d no pudo ser asignada\n", i);
					return false;
				}
				allocation[entity1] = room;
				if (!violatesHard(allocation, hardConstraints, rooms, entities, nHardConstraints, nRooms, nEntities))
				{
					allocated = true;
					resetAvailableRooms(entities, nEntities, nRooms);
				}
			}
		}
	}

	// Satisfy soft restriction 0: ALLOCATION_CONSTRAINT

	for (i = 0; i < nSoftConstraints; i++)
	{
		if (softConstraints[i].type == 0)
		{
			entity1 = softConstraints[i].c1;
			room = softConstraints[i].c2;

			if (allocation[entity1] == -1)
			{
				allocation[entity1] = room;
				// If allocation violates a hard constraint
				if (violatesHard(allocation, hardConstraints, rooms, entities, nHardConstraints, nRooms, nEntities))
				{
					allocation[entity1] = -1;
				}
			}
			resetAvailableRooms(entities, nEntities, nRooms);
		}
	}

	// Satisfy soft restriction 4: SAMEROOM_CONSTRAINT

	for (i = 0; i < nSoftConstraints; i++)
	{
		if (softConstraints[i].type == 4)
		{
			entity1 = softConstraints[i].c1;
			entity2 = softConstraints[i].c2;

			allocated = false;
			while (!allocated)
			{
				room = getRoom(entities, nRooms, entity1);
				if (room == -1)
				{
					printf("Entidad %d no pudo ser asignada\n", i);
					return false;
				}
				allocation[entity1] = room;
				allocation[entity2] = room;
				if (!violatesHard(allocation, hardConstraints, rooms, entities, nHardConstraints, nRooms, nEntities))
				{
					allocated = true;
					resetAvailableRooms(entities, nEntities, nRooms);
				}
			}
		}
	}

	// Satisfy soft restriction 7: ADJACENCY_CONSTRAINT

	for (i = 0; i < nSoftConstraints; i++)
	{
		if (softConstraints[i].type == 7)
		{
			entity1 = softConstraints[i].c1;
			entity2 = softConstraints[i].c2;

			allocated = false;
			while (!allocated)
			{
				room = getRoom(entities, nRooms, entity1);
				if (room == -1)
				{
					printf("Entidad %d no pudo ser asignada\n", i);
					return false;
				}
				allocation[entity1] = room;
				for (j = 0; j < rooms[room].adjSize; j++)
				{
					if (!allocated)
					{
						room2 = rooms[room].adjacentRooms[j];
						allocation[entity2] = room2;
						if (!violatesHard(allocation, hardConstraints, rooms, entities, nHardConstraints, nRooms, nEntities))
						{
							allocated = true;
							resetAvailableRooms(entities, nEntities, nRooms);
						}
					}
				}
			}
		}
	}

	// Satisfy soft restriction 8: NEARBY_CONSTRAINT

	for (i = 0; i < nSoftConstraints; i++)
	{
		if (softConstraints[i].type == 8)
		{
			entity1 = softConstraints[i].c1;
			entity2 = softConstraints[i].c2;

			allocated = false;
			while (!allocated)
			{
				room = getRoom(entities, nRooms, entity1);
				if (room == -1)
				{
					printf("Entidad %d no pudo ser asignada\n", i);
					return false;
				}
				allocation[entity1] = room;
				for (j = 0; j < nRooms; j++)
				{
					if (!allocated)
					{
						if (rooms[room].floor == rooms[j].floor)
						{
							room2 = j;
							allocation[entity2] = room2;
							if (!violatesHard(allocation, hardConstraints, rooms, entities, nHardConstraints, nRooms, nEntities))
							{
								allocated = true;
								resetAvailableRooms(entities, nEntities, nRooms);
							}
						}
					}
				}
			}
		}
	}

	// Satisfy soft restriction 9: AWAYFROM_CONSTRAINT

	// Satisfy soft restriction 5: NOTSAMEROOM_CONSTRAINT

	// Satisfy soft restriction 1: NONALLOCATION_CONSTRAINT

	// Entities must be allocated

	j = 0;
	for (i = 0; i < nEntities; i++)
	{
		if (allocation[i] == -1)
		{
			printf("Entidad %d no estaba asignada\n", i);
			allocated = false;
			j++;
			while (!allocated)
			{
				room = getRoom(entities, nRooms, i);
				if (room == -1)
				{
					printf("Entidad %d no pudo ser asignada\n", i);
					return false;
				}
				allocation[i] = room;
				if (!violatesHard(allocation, hardConstraints, rooms, entities, nHardConstraints, nRooms, nEntities))
				{
					allocated = true;
					resetAvailableRooms(entities, nEntities, nRooms);
					printf("***Entidad %d asignada a habitación %d\n", i, room);
				}
			}
		}
	}
	printf("%d entidades no estaban asignadas antes de usar la ultima restricción\n", j);
	return true;
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
	int nEntities, nRooms, nConstraints, nHardConstraints, nSoftConstraints;
	int i, j, k, n;
	// int penalties[] = {20, 10, 10, 10, 10, 10, 50, 10, 10, 10};
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

				// case 2:
				// 	nFloors = n;
				// 	break;

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

	j = 0;
	for (i = 0; i < nHardConstraints; i++)
	{
		if (hardConstraints[i].type == 6)
		{
			j++;
		}
	}
	printf("Existen %d restricciones duras del tipo 6\n", j);

	// Initial available rooms for every entity

	initialEntityData(rooms, entities, nRooms, nEntities);

	// Hill Climbing + First Improvement Algorithm

	// int bestSolQuality = 32000;
	int *bestSol = malloc(sizeof(int) * nEntities);

	int *allocation = malloc(sizeof(int) * nEntities);

	for (i = 0; i < nEntities; i++)
	{
		allocation[i] = -1;
	}

	if (constructInitialSolution(allocation, softConstraints, hardConstraints, rooms, entities, nSoftConstraints, nHardConstraints, nRooms, nEntities))
	{
		printf("Solución inicial creada correctamente\n");
	}
	else
	{
		printf("Fallo en solución inicial\n");
	}

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
