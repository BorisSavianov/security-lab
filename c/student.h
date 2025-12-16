#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>

typedef struct {
  int id;
  char name[50];
  float grade;
} Student;

extern Student *array;
extern int arraySize;
extern int arrayCapacity;

Student *initArray(void);
void freeArray(void);
void addStudent(Student *student);
void removeStudent(int id);
void listStudents(void);
void findStudent(const char *query);
void saveToFile(const char *filename);
void loadFromFile(const char *filename);

#endif
