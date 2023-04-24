//Lists

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define LINE_SIZE 128

union Reference {
	//one-byte(8-bit)
	unsigned char intRef;
	//two-bytes (16-bit)
	unsigned short extRef;
};

//an alias for the union above
typedef union Reference RefInfo;

struct Student {
	char* name;
	double income;
	RefInfo reference;
};

typedef struct Student StudentInfo;

struct Node {
	StudentInfo* info;
	struct Node* next;
};

typedef struct Node ListNode;

//useful info memory
StudentInfo* createStudentInfo(const char*, double, unsigned short);
void displayStudent(StudentInfo*);
void displayStudents(StudentInfo**, int);
void* deleteStudentInfo(StudentInfo*);

ListNode* createNode(StudentInfo*);
void insertNode(ListNode**, ListNode*);
ListNode* insertOnPosition(ListNode*, ListNode*, int);
void printList(ListNode*);

void main() {
	FILE* pFile = fopen("Data.txt", "r");
	ListNode* simpleLinkedList = NULL;

	if (pFile) {
		//each element from the array 'agenda' is a pointer to a struct StudentInfo
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
			insertNode(&simpleLinkedList, node);
			agenda[index++] = stud;
		}

		StudentInfo* info = createStudentInfo("Popescu Eugen", 1300.34, 38767);
		ListNode* node = createNode(info);
		int position = 78;
		simpleLinkedList = insertOnPosition(simpleLinkedList, node, position);
		printList(simpleLinkedList);

		//displayStudents(agenda, sizeof(agenda) / sizeof(StudentInfo*));
		fclose(pFile);
	}
}

ListNode* createNode(StudentInfo* stud) {
	ListNode* result = NULL;
	result = (ListNode*)malloc(sizeof(ListNode));
	result->info = stud;
	result->next = NULL;
	return result;
}

//there is a double * because the fucntion needs to modify the head of the list
//to change the address where the head pointer is pointing to
void insertNode(ListNode** list, ListNode* node) {
	if (*list == NULL) {
		*list = node;
	}
	else {
		ListNode* aux = *list;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = node;
	}
}

//list->ppointer to the head of the list
ListNode* insertOnPosition(ListNode* list, ListNode* node, int index)
{
	if (index == 1)
	{
		node->next = list;
		return node;
	}

	int i = 1;
	ListNode* aux = list;
	while (aux->next && i < index - 1)
	{
		i++;
		aux = aux->next;
	}
	if (aux->next == NULL)
	{
		aux->next = node;
	}
	else
	{
		//insert the node between two existing nodes
		/*If we do find the desired position, we insert the new node between the current nodeand the next node.
			To do this, we set the next field of the new node to point to the current next node,
			and set the next field of the current node to point to the new node.*/
		node->next = aux->next;
		aux->next = node;
	}
	return list;
}

void printList(ListNode* list) {
	while (list) {
		displayStudent(list->info);
		list = list->next;
	}
}

void displayStudent(StudentInfo* stud) {
	if (stud) {
		printf("Name: %s, income: %f\n", stud->name, stud->income);
		if (stud->reference.extRef >> 15 == 1) {
			//extracts the 7 least significant bits of the shifted value 
			short uid = stud->reference.extRef >> 8 & 127;
			printf("University ref: %d\n", uid);
			//set all bits outside the least significant one to 0
			printf("External ref: %d\n", stud->reference.extRef & 255);
		}
		else {
			printf("Internal ref: %d\n", stud->reference.intRef);
		}
	}
}

void displayStudents(StudentInfo** agenda, int noEl) {
	for (int i = 0; i < noEl; i++) {
		displayStudent(agenda[i]);
	}
}

void* deleteStudentInfo(StudentInfo* stud) {
	if (stud != NULL) {
		free(stud->name);
		free(stud);
	}
	return NULL;
}

//for dinamically allocation of memory
StudentInfo* createStudentInfo(const char* name, double income, unsigned short ref) {
	StudentInfo* result = NULL;

	result = (StudentInfo*)malloc(sizeof(StudentInfo));
	result->name = (char*)malloc(strlen(name) + 1);
	strcpy(result->name, name);
	result->income = income;
	result->reference.extRef = ref;
	return result;
}