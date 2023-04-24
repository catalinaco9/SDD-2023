#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

union Reference
{
	unsigned char intRef;
	unsigned short extRef;
};

typedef union Reference RefInfo;

struct Student
{
	char* name;
	double income;
	RefInfo reference;
};

typedef struct Student StudentInfo;

struct Node
{
	struct Node* prev;
	StudentInfo* info;
	struct Node* next;
};

typedef struct Node Stack;
typedef struct Node QueueNode;

struct DummyQueue
{
	struct Node* head;
	struct Node* tail;
};
typedef struct DummyQueue Queue;

//useful info memory management
StudentInfo* createStudentInfo(const char*, double, unsigned short);
void* deleteStudentInfo(StudentInfo*);
void displayStudents(StudentInfo**, int);
void displayStudent(StudentInfo*);

QueueNode* getNode(Queue*);
void putNode(Queue*, QueueNode*);
void printQueue(Queue);


#define LINE_SIZE 128

void main()
{
	FILE* pFile = fopen("Data.txt", "r");
	Stack* circularDoubleLinkedList = NULL;
	Queue queue = { .head = NULL, .tail = NULL };
	//queue.head = NULL;
	//queue.tail = NULL;
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
			//Stack* node = createNode(stud);
			//circularDoubleLinkedList = pushNode(circularDoubleLinkedList, node);
			QueueNode* node = createNode(stud);
			putNode(&queue, node);
			agenda[index++] = stud;
		}
		//StudentInfo* stud = peekNode(circularDoubleLinkedList);
		//displayStudent(stud);
		//Stack* node = popNode(&circularDoubleLinkedList);
		//displayStudent(node->info);

		//QueueNode* node = getNode(&queue);
		//displayStudent(node->info);

		printQueue(queue);
		printf("\n-------------------------\n");
		printQueue(queue);

		//clearStack(&circularDoubleLinkedList);
		//displayStudents(agenda, sizeof(agenda) / sizeof(StudentInfo*));
		fclose(pFile);
	}
}

void printQueue(Queue queue)
{
	if (queue.head != NULL)
	{
		QueueNode* ref = queue.head;
		do
		{
			QueueNode* node = getNode(&queue);
			displayStudent(node->info);
			putNode(&queue, node);
		} while (queue.head != ref);
	}
}
QueueNode* getNode(Queue* queue)
{
	QueueNode* result = NULL;
	if (queue->head != NULL)
	{
		result = queue->head;
		if (queue->head == queue->tail)
			queue->head = queue->tail = NULL;
		else
		{
			//the next node after the head becomes the new head 
			queue->head->next->prev = NULL;
			queue->head = queue->head->next;
		}
		result->next = NULL;
	}
	return result;
}

void putNode(Queue* queue, QueueNode* node)
{
	if (queue->head == NULL || queue->tail == NULL)
	{
		queue->head = queue->tail = node;
	}
	else
	{
		node->prev = queue->tail;
		queue->tail->next = node;
		queue->tail = node;
	}
}


void displayStudent(StudentInfo* stud)
{
	if (stud)
	{
		printf("Name: %s, income: %f\n", stud->name, stud->income);
		if (stud->reference.extRef >> 15 == 1)
		{
			short uid = stud->reference.extRef >> 8 & 127;
			printf("University ref: %d\n", uid);
			printf("External ref: %d\n", stud->reference.extRef & 255);
		}
		else
		{
			printf("Internal ref: %d\n", stud->reference.intRef);
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
	result->reference.extRef = ref;
	return result;
