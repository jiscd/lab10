#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MARKSCOUNT 5
#define STUDENTSCOUNT 10

typedef struct SStudentInfo sStudentInfo;
struct SStudentInfo
{
	char name[50];
	char surname[50];
	int day;
	int month;
	int year;
	int marks[MARKSCOUNT];
	sStudentInfo* next_student;
};


void insertStudentSorted(sStudentInfo** head, sStudentInfo* newStudent);
void FileRead(sStudentInfo** first_student);
void StructPrint(sStudentInfo* first_student);
void RemoveStudent(sStudentInfo** first_student, sStudentInfo* student);
double AvarageMark(sStudentInfo* student);
void RemoveBottomThree(sStudentInfo** first_student);
void FreeList(sStudentInfo** first_student);


int main(void)
{
	sStudentInfo* first_student = NULL;
	FileRead(&first_student);
	StructPrint(first_student);
	printf("\n");
	RemoveBottomThree(&first_student);
	StructPrint(first_student);
	FreeList(&first_student);
}

void FileRead(sStudentInfo** first_student)
{
	FILE* fp;
	sStudentInfo* student;
	if (!(fopen_s(&fp, "d:/data/students.txt", "r")))
	{
		for (int i = 0; i < STUDENTSCOUNT; i++)
		{
			student = (sStudentInfo*)malloc(sizeof(sStudentInfo));
			if (!student)
			{
				printf("error"); return;
			}
			fscanf_s(fp, "%s", student->surname, 50);
			fscanf_s(fp, "%s", student->name, 50);
			fscanf_s(fp, "%d", &student->day);
			fscanf_s(fp, ".%d", &student->month);
			fscanf_s(fp, ".%d", &student->year);
			fscanf_s(fp, "%d", &student->marks[0]);
			for (int j = 1; j < MARKSCOUNT; j++)
			{
				fscanf_s(fp, ",%i", &student->marks[j]);
			}
			student->next_student = NULL;
			insertStudentSorted(first_student, student);
		}
		fclose(fp);
	}
}


void insertStudentSorted(sStudentInfo** first_student, sStudentInfo* newStudent)
{
	if (!*first_student || strcmp((*first_student)->surname, newStudent->surname) > 0)
	{
		newStudent->next_student = *first_student;
		*first_student = newStudent;
		return;
	}

	sStudentInfo* current = *first_student;
	while (current->next_student && strcmp(current->next_student->surname, newStudent->surname) < 0)
	{
		current = current->next_student;
	}
	newStudent->next_student = current->next_student;
	current->next_student = newStudent;
}


void StructPrint(sStudentInfo* first_student)
{
	sStudentInfo* current = first_student;
	while (current)
	{
		printf("%s %s\t%.2i.%.2i.%i\t ", current->surname, current->name, current->day, current->month, current->year);
		for (int j = 0; j < (MARKSCOUNT - 1); j++)
		{
			printf("%d,", current->marks[j]);
		}
		printf("%d\n", current->marks[MARKSCOUNT - 1]);
		current = current->next_student;
	}
}

void RemoveStudent(sStudentInfo** first_student,sStudentInfo*student)
{
	if (!*first_student || !student)
		return;

	sStudentInfo* temp = NULL;

	if (*first_student == student)
	{
		temp = *first_student;
		*first_student = (*first_student)->next_student;
		free(temp);
		return;
	}

	sStudentInfo* current = *first_student;
	while (current)
	{
		if (current->next_student == student)
		{
			temp = current->next_student;
			current->next_student = current->next_student->next_student;
			free(temp);
			return;
		}
		current = current->next_student;
	}
}

void RemoveBottomThree(sStudentInfo** first_student)
{
	sStudentInfo* current;
	for (int i = 0; i < 3; i++)
	{
		current = *first_student;
		double min = AvarageMark(*first_student);
		while (current)
		{
			if (min > AvarageMark(current))
				min = AvarageMark(current);
			current = current->next_student;
		}
		current = *first_student;
		while (current)
		{
			if (AvarageMark(current) == min)
			{
				RemoveStudent(first_student, current);
				break;
			}
			current = current->next_student;
		}
	}
}

double AvarageMark(sStudentInfo* student)
{
	int avarage = 0;
	for (int i = 0; i < MARKSCOUNT; i++)
	{
		avarage += student->marks[i];
	}
	return (double)avarage / MARKSCOUNT;
}

void FreeList(sStudentInfo** first_student)
{
	sStudentInfo* current = *first_student;
	while (current)
	{
		sStudentInfo* temp = current;
		current = current->next_student;
		free(temp);
	}
}