// Day one of Advent of Code 2024
//
#include "read_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void simple_sort(int *list, size_t n) {
  int i = 1;
  int j;
  while (i < n) {
    int x = list[i];
    j = i;
    while ((j > 0) && (list[j - 1] > x)) {
      list[j] = list[j - 1];
      j--;
    }
    list[j] = x;
    i++;
  }
}

int parse_input(char *data, size_t size, int **list_1, int **list_2) {
  // Count lines
  int num_lines = 0;
  for (int i = 0; i < size; i++) {
    if (data[i] == '\n') {
      num_lines++;
    }
  }

  int *l1 = (int *)malloc(sizeof(int) * num_lines);
  int *l2 = (int *)malloc(sizeof(int) * num_lines);

  char *line;

  char *outer_saveptr = NULL;
  char *inner_saveptr = NULL;

  line = strtok_r(data, "\n", &outer_saveptr);

  for (int i = 0; i < num_lines; i++) {
    char *number;
    number = strtok_r(line, " ", &inner_saveptr);
    l1[i] = atoi(number);

    number = strtok_r(NULL, " ", &inner_saveptr);
    l2[i] = atoi(number);

    line = strtok_r(NULL, "\n", &outer_saveptr);
  }

  *list_1 = l1;
  *list_2 = l2;

  return num_lines;
}

int main() {

  // load puzzle file
  char *data = malloc(sizeof(char) * 20);
  size_t size = 0;

  int res = read_input(&data, &size);
  if (res != 0)
    return -1;

  // Parse input
  int *list_1 = NULL;
  int *list_2 = NULL;
  int len = parse_input(data, size, &list_1, &list_2);

  // Order lists
  simple_sort(list_1, len);
  simple_sort(list_2, len);

  // Compare and sum lists

  int diff_sum = 0;
  for (int i = 0; i < len; i++) {
    diff_sum += abs(list_1[i] - list_2[i]);
  }

  // PART 2: similarity score

  int similarity = 0;
  for (int i = 0; i < len; i++) {
    int a = list_1[i];
    for (int j = 0; j < len; j++) {
      if (list_2[j] == a)
        similarity += a;
    }
  }

  printf("%d\n", diff_sum);
  printf("%d\n", similarity);

  free(data);
  free(list_1);
  free(list_2);

  return 0;
}
