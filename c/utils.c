#include "utils.h"
#include <stdio.h>
#include <string.h>

void readLine(char *buffer, size_t size) {
  if (!fgets(buffer, size, stdin)) {
    buffer[0] = '\0';
  }
  size_t len = strlen(buffer);
  if (len > 0 && buffer[len - 1] == '\n') {
    buffer[len - 1] = '\0';
  }
}

int readInt(void) {
  int value;
  while (1) {
    char buffer[128];
    readLine(buffer, sizeof(buffer));
    if (sscanf(buffer, "%d", &value) == 1) {
      return value;
    }
    printf("Invalid input. Please enter an integer: ");
  }
}

inline float readFloat(void) {
  float value;
  while (1) {
    char buffer[128];
    readLine(buffer, sizeof(buffer));
    if (sscanf(buffer, "%f", &value) == 1) {
      return value;
    }
    printf("Invalid input. Please enter a float: ");
  }
}