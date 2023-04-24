//priority queue structure, it is based on a heap tree structure with a binary key, min heap or max heap 
//they will be processed in an ascending order - from min to max, according to the reference of the student
//the optimal implementation of a heap structure is not of a tree

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

union Reference
{
	unsigned char intref;
	unsigned short extref;
};

typedef union Reference RefInfo;

struct Student
{
	char* name;
	double income;
	RefInfo reference;
};

typedef struct Student StudentInfo;


//the priority queue
struct PrioQ { //a dynamic array(*) of pointers(*) to StudentInfo (that's why we have StudentInfo**)
	//when you work with a dynamic array, you always need to store the dimension of the array somewhere - in this case queue size

	StudentInfo** items; //items will change only when we add the first element to the queue (it is initialized to NULL, and after the first element we allocate the address of the first element) 
	int currentIndex;
	int queueSize; //it is the maximum capacity of the queue, not the actual dynamic size of the queue
};

typedef struct PrioQ PrioQueue;

//useful info memory management
StudentInfo* createStudentInfo(const char*, double, unsigned short);
void* deleteStudentInfo(StudentInfo*);
void displayStudents(StudentInfo**, int);
void displayStudent(StudentInfo*);
void enqueue(PrioQueue*, StudentInfo*);
void reHeapUp(PrioQueue*, int);
void changePriority(PrioQueue, int, int);

#define LINE_SIZE 128
#define INITIAL_PQUEUE_SIZE 7

void main()
{
	FILE* pFile = fopen("Data.txt", "r");
	//this is sort of a dummy structure because it holds down the useful data in a way that it will be easier later to access it
	PrioQueue prioQueue = { .currentIndex = 0, .queueSize = 0, .items = NULL };


	if (pFile)
	{
		StudentInfo* agenda[10];
		memset(agenda, 0, sizeof(agenda));
		char* token; char delimiter[] = ",\n";
		double income; unsigned short ref;
		char lineBuffer[LINE_SIZE], name[LINE_SIZE];
		int index = 0;
		while (fgets(lineBuffer, sizeof(lineBuffer), pFile))
		{
			token = strtok(lineBuffer, delimiter);
			strcpy(name, token);
			token = strtok(NULL, delimiter);
			income = atof(token);
			token = strtok(NULL, delimiter);
			ref = atoi(token);
			StudentInfo* stud = createStudentInfo(name, income, ref);
			enqueue(&prioQueue, stud); //we need to pass the address because we have items initiliazed to NULL
		}

		for (int i = 0; i < prioQueue.queueSize; i++)
		{
			if (prioQueue.items[i] != NULL)
			{
				displayStudent(prioQueue.items[i]);
			}
		}

		//HOMEWOrK
		//Problem during the seminar - look for changePriority function
		//change the reference of the student and recalibrate the tree
		//take the student with ref 20 and give it a higher rank - 5, 
		//so it must become the top of the map tree of the queue
		//we only consider the case when the priority changes upwards, so we can use reHeapUp
			//we may discuss reHeapDown
		short index = 3;
		short newPrio = 5;
		changePriority(prioQueue, index, newPrio);

		fclose(pFile);
	}
}

void changePriority(PrioQueue pQueue, int index, int newPrio) {

}

void reHeapUp(PrioQueue* pQueue, int currentIndex) {
	//we adopt a recursive method, not a while loop

	//here we use the formula for priority queues 
	if (currentIndex > 0) {
		int parentIndex = (currentIndex - 1) / 2; //the floor (partea intreaga) of the division

		//we decode the parrent ref
		short parentref = 0; short currentref = 0;
		if (pQueue->items[parentIndex]->reference.extref >> 15 == 1)
			parentref = pQueue->items[parentIndex]->reference.extref >> 15 & 255;
		else
			parentref = pQueue->items[parentIndex]->reference.intref;

		//we decode the current ref
		if (pQueue->items[currentIndex]->reference.extref >> 15 == 1)
			parentref = pQueue->items[currentIndex]->reference.extref >> 15 & 255;
		else
			parentref = pQueue->items[currentIndex]->reference.intref;

		if (parentref > currentref)
		{
			StudentInfo* aux = pQueue->items[currentIndex];
			pQueue->items[currentIndex] = pQueue->items[parentIndex];
			pQueue->items[parentIndex] = aux;
			reHeapUp(pQueue, parentIndex);

		}
	}
}

void enqueue(PrioQueue* pQueue, StudentInfo* stud)
{
	//first initilization - we also define the queueSize - the maximum capacity of the Pqueue	
	if (pQueue->items == NULL) {
		//the array of pointers intilization: MALLOC + MEMSET!!!!!!!!!!
		pQueue->items = (StudentInfo**)
			malloc(INITIAL_PQUEUE_SIZE * sizeof(StudentInfo**));
		memset(pQueue->items, NULL, INITIAL_PQUEUE_SIZE * sizeof(StudentInfo*));
		pQueue->queueSize = INITIAL_PQUEUE_SIZE;
	}

	//now we need to add the element, after we made sure the queue is initialized
	pQueue->items[pQueue->currentIndex] = stud; //items is an array of pointers and stud is a pointer, passed as a parameter (StudentInfo*)
	reHeapUp(pQueue, pQueue->currentIndex);
	pQueue->currentIndex++;
}



void displayStudent(StudentInfo* stud)
{
	if (stud)
	{
		printf("Name: %s, income: %f\n", stud->name, stud->income);
		if (stud->reference.extref >> 15 == 1)
		{
			short uid = stud->reference.extref >> 8 & 127;
			printf("University ref: %d\n", uid);
			printf("External ref: %d\n", stud->reference.extref & 255);
		}
		else
		{
			printf("Internal ref: %d\n", stud->reference.intref);
		}
	}
}
void displayStudents(StudentInfo** agenda, int noEl)
{
	for (int i = 0; i < noEl; i++)
	{
		displayStudent(agenda[i]);
	}
}
void* deleteStudentInfo(StudentInfo* stud)
{
	if (stud != NULL)
	{
		free(stud->name);
		free(stud);
	}
	return NULL;
}

StudentInfo* createStudentInfo(const char* name, double income, unsigned short ref)
{
	StudentInfo* result = NULL;

	result = (StudentInfo*)malloc(sizeof(StudentInfo));
	result->name = (char*)malloc(strlen(name) + 1);
	strcpy(result->name, name);
	result->income = income;
	result->reference.extref = ref;
	return result;

}