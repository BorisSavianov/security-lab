#include "student.h"
#include "utils.h"
#include <stdlib.h>

int option;

void addStudentMenu(void) {
  Student student;
  printf("Press q to cancel\n");
  printf("Enter student name: ");
  readLine(student.name, sizeof(student.name));

  if (student.name[0] == 'q') {
    return;
  }

  printf("Enter student grade: ");
  student.grade = readFloat();
  addStudent(&student);
}

void removeStudentMenu(void) {
  printf("Enter student id: ");
  int id = readInt();
  removeStudent(id);
}

void findStudentMenu(void) {
  printf("Enter student name: ");
  char query[50];
  readLine(query, sizeof(query));
  findStudent(query);
}

int main(void) {

  initArray();

  do {
    printf("Menu:\n");
    printf("1. Add student\n");
    printf("2. Remove student\n");
    printf("3. List students\n");
    printf("4. Find student\n");
    printf("5. Save to file\n");
    printf("6. Load from file\n");
    printf("7. Exit\n");
    option = readInt();

    // printf("\x1B[1;1H\x1B[2J");
    system("clear");

    switch (option) {
    case 1: {
      addStudentMenu();
      break;
    }
    case 2: {
      removeStudentMenu();
      break;
    }
    case 3: {
      listStudents();
      break;
    }
    case 4: {
      findStudentMenu();
      break;
    }
    case 5: {
      saveToFile("students.dat");
      break;
    }
    case 6: {
      loadFromFile("students.dat");
      break;
    }
    case 7: {
      freeArray();
      return 0;
    }
    default: {
      printf("Invalid option. Please try again.\n");
      break;
    }
    }

    // printf("\x1B[1;1H\x1B[2J");
  } while (option != 7);
}
