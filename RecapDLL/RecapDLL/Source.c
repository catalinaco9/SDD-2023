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

struct Node {
	struct Node* prev;
	StudentInfo* info;
	struct Node* next;
};

typedef struct Node ListNode;

//useful info memory management
StudentInfo* createStudentInfo(const char*, double, unsigned short);
void* deleteStudentInfo(StudentInfo*);
void displayStudents(StudentInfo**, int);
void displayStudent(StudentInfo*);

void printList(ListNode*);
ListNode* createNode(StudentInfo*);
ListNode* insertInFront(ListNode*, ListNode*);
ListNode* insertAtTail(ListNode*, ListNode*);
void insertAfterKey(ListNode*, ListNode*, char*);
ListNode* insertOnPosition(ListNode* , ListNode* , int );

#define LINE_SIZE 128

void main()
{
	FILE* pFile = fopen("Data.txt", "r");
	ListNode* doubleLinkedList = NULL;
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
			ListNode* node = createNode(stud);
			//doubleLinkedList = insertInFront(doubleLinkedList, node);
			//doubleLinkedList = insertAtTail(doubleLinkedList, node);
			doubleLinkedList = insertOnPosition(doubleLinkedList, node, index);
			agenda[index++] = stud;
		}
		//displayStudents(agenda, sizeof(agenda) / sizeof(StudentInfo*));
		StudentInfo* stud = createStudentInfo("Popescu Emil", 1700.43, 56);
		ListNode* node = createNode(stud);
		//insertAfterKey(doubleLinkedList, node, "Popescu Maria");

		insertOnPosition(doubleLinkedList, node, 3);
		printList(doubleLinkedList);
		fclose(pFile);
	}
}

ListNode* createNode(StudentInfo* stud) {
	ListNode* result = NULL;
	result = (ListNode*)malloc(sizeof(ListNode));
	result->info = stud;
	result->next = result->prev = NULL;
	return result;
}

ListNode* insertInFront(ListNode* list, ListNode* node) {
	if (list != NULL) {
		//points to the current head of the list
		node->next = list;
		//prev of the current head to the new head
		list->prev = node;
	}
	return node;
}

ListNode* insertOnPosition(ListNode* list, ListNode* node, int position)
{
	if (position <= 0)
	{
		if (list != NULL)
		{
			node->next = list;
			list->prev = node;
		}
		list = node;
	}
	else
	{
		ListNode* aux = list;
		int index = 0;
		while (aux->next != NULL && index < position - 1)
		{
			aux = aux->next;
			index++;
		}
		if (aux->next == NULL)
		{
			//the tail
			node->prev = aux;
			aux->next = node;
		}
		else
		{
			//index==position-1
			//sets the new node to the node at the current position
			node->next = aux->next;
			node->prev = aux;
			//the prev of the node followong the new node to the new node
			aux->next->prev = node;
			aux->next = node;
		}
	}
	return list;
}


void insertAfterKey(ListNode* list, ListNode* node, char* keyName) {
	while (list && strcmp(list->info->name, keyName) != 0) {
		list = list->next;
	}
	if (list != NULL) {
		//set to point to the matching node
		node->prev = list;
		//the new node points to the node immediatly following the matching point
		node->next = list->next;
		//the matching node setto point to the new node
		list->next = node;

		if (node->next) {
			//the prev of the next node
			node->next->prev = node;
		}
	}
}

ListNode* insertAtTail(ListNode* list, ListNode* node) {
	if (list == NULL) {
		list = node;
	}
	else {
		ListNode* aux = list;
		while (aux->next) {
			aux = aux->next;
		}
		node->prev = aux;
		aux->next = node;
	}
	return list;
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

void printList(ListNode* list) {
	while (list) {
		displayStudent(list->info);
		list = list->next;
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

}