#define _CTR_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

union Reference {
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

//useful info memory management
StudentInfo* createStudentInfo(const char*, double, unsigned short);
void* deleteStudentInfo(StudentInfo*);
void displayStudents(StudentInfo**, int);

#define LINE_SIZE 128

void main()
{
	StudentInfo student;
	student.name = "Popescu Maria";
	student.income = 1400.3;

	FILE* pFile = fopen("Data.txt", "r");
	if (pFile)
	{
		StudentInfo* agenda[10];
		memset(agenda, NULL, sizeof(agenda));
		char* token;
		char delimiter[] = ",\n";
		double income;
		unsigned short ref;
		char lineBuffer[LINE_SIZE], name[LINE_SIZE]; //statically declared array ; the size of the two arrays is known when you build
		int index = 0;

		while (fgets(lineBuffer, size(lineBuffer), pFile)) {
			//token holds down only the current value
			token = strtok(lineBuffer, delimiter); //popescu maria
			strcpy(name, token);

			token = strtok(NULL, delimiter);
			income = atof(token);

			token = strtok(NULL, delimiter);
			ref = atoi(token);

			StudentInfo* stud = createStudentInfo(name, income, ref);
			agenda[index++] = stud;
		}
		displayStudents(agenda, sizeof(agenda) / sizeof(StudentInfo*));
		fclose(pFile);
	}
}

void displayStudents(StudentInfo** agenda, int noEl) {
	for (int i = 0; i < noEl; i++) {
		printf("Name: %s, income: %f/n", agenda[i]->name, agenda[i]->income);
		if (agenda[i]->reference.extRef >> 15 == 1) {
			short uid = agenda[i]->reference.extRef >> 8 & 127;
			printf("University ref: %d\n");
			printf("External ref: %d\n", agenda[i]->reference.extRef & 255); //0xFF
		}
		else {
			printf("Internal ref: %d\n", agenda[i]->reference.intRef);
		}
		
	}
}
//off switcher-> AND
//on switcher-> OR
//switcher -> XOR
//0 of switcher

//ALWAYS INITIALIZE POINTERS

void* deleteStudentInfo(StudentInfo* stud) {
	if (stud != NULL) {
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

//matrix of studentinfo type of 100 rows and undefined no of columns
//the income is 8 bites cdcdcd cdcdcd