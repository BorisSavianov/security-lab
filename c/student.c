#include "student.h"
#include <stdlib.h>
#include <string.h>

Student *array = NULL;
int arraySize = 0;
int arrayCapacity = 10;

Student *initArray(void) {
  array = (Student *)malloc(sizeof(Student) * arrayCapacity);
  return array;
}

void freeArray(void) {
  free(array);
  array = NULL;
  arraySize = 0;
  arrayCapacity = 10;
}

void addStudent(Student *student) {
  if (arraySize + 1 > arrayCapacity) {
    arrayCapacity *= 2;
    array = (Student *)realloc(array, sizeof(Student) * arrayCapacity);
    if (!array) {
      printf("Error reallocating memory\n");
      exit(1);
    }
  }

  student->id = arraySize + 1;

  array[arraySize] = *student;
  arraySize++;
}

void removeStudent(int id) {
  for (int i = 0; i < arraySize; i++) {
    if (array[i].id == id) {
      for (int j = i; j < arraySize - 1; j++) {
        array[j] = array[j + 1];
      }
      arraySize--;
      return;
    }
  }
}

void listStudents(void) {
  for (int i = 0; i < arraySize; i++) {
    printf("%d %s %.2f\n", array[i].id, array[i].name, array[i].grade);
  }
}

void findStudent(const char *query) {
  for (int i = 0; i < arraySize; i++) {
    if (strcmp(array[i].name, query) == 0) {
      printf("%d %s %f\n", array[i].id, array[i].name, array[i].grade);
      return;
    }
  }
  printf("Student not found\n");
}

void saveToFile(const char *filename) {
  FILE *file = fopen(filename, "w");
  if (!file) {
    printf("Error opening file\n");
    return;
  }

  if (file) {
    printf("There is an existing save. Overwrite? (y/n): ");
    char option;
    scanf("%s", &option);
    if (option != 'y') {
      return;
    }
  }

  printf("Operation successfull\n");

  for (int i = 0; i < arraySize; i++) {
    fprintf(file, "%d %s %f\n", array[i].id, array[i].name, array[i].grade);
  }
  fclose(file);
}

void loadFromFile(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("Error opening file\n");
    return;
  }

  initArray();
  arraySize = 0;

  Student temp;
  while (fscanf(file, "%d %49s %f", &temp.id, temp.name, &temp.grade) == 3) {
    addStudent(&temp);
  }
  fclose(file);
}