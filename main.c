#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  const char *data;
  size_t len;
} string_t;

typedef struct {
  struct {
    size_t row, col, idx;
  } *data;

  size_t used;
  size_t size;
} array_t;

typedef size_t (*f_hash)(const char *text);

#ifndef START_SIZE
#define START_SIZE 4086
#endif

array_t robin(FILE *text, string_t pattern, f_hash hash) {
  array_t array = {
      .data = malloc(sizeof(*array.data) * START_SIZE),
      .used = 0,
      .size = START_SIZE,
  };

  char buff[1024];
  size_t idGlobal = 0;
  size_t row = 0;
  size_t col = 0;
  size_t i = 0;
  fread(buff, sizeof(*buff), sizeof(buff) - 1, text);
  while (!feof(text)) {
    int reset = 1;
    if (buff[i] == '\n') {
      row++;
      col = 0;
    }
    if (buff[i + pattern.len] == '\0') {
	  printf("[READ]: CALLED");
      memmove(buff, buff + i, pattern.len);
      fread(buff + pattern.len, sizeof(*buff), sizeof(buff) - 1 - pattern.len,
            text);
      i = 0;
      reset = 0;
	  idGlobal--;
    }

	printf("[%zu]: \"%.*s\"\n", idGlobal, (int)pattern.len, buff + i);
    int equal = 1;
    for (size_t j = 0; j < pattern.len && equal; j++) {
      if (pattern.data[j] != buff[i + j]) {
        equal = 0;
      }
    }

    if (equal) {
      if (array.used == array.size) {
        array.size *= 2;
        array.data = realloc(array.data, array.size * sizeof(*array.data));
      }
      // printf("Match at %zu:%zu (%zu) -> %.*s\n", row, col, idGlobal,
      //        (int)pattern.len, buff + i);
      array.data[array.used].row = row;
      array.data[array.used].col = col;
      array.data[array.used++].idx = idGlobal;
	  printf("[MATCH][%zu]: \"%.*s\"\n", idGlobal, (int)pattern.len + 20, buff + i);
    }
    i += reset;
    idGlobal++;
    col++;
  }

  return array;
}

void print_matches(FILE *file, array_t array) {
  char buff[20];
  puts("Matches: ");
  rewind(file);
  for (size_t i = 0; i < array.used; i++) {
    fseek(file, array.data[i].idx, SEEK_SET);
    size_t read = fread(buff, sizeof(*buff), sizeof(buff) - 1, file);
	buff[read] = '\0';
    printf("\t[%zu] (%zu:%zu) -> %zu: %s\n", i, array.data[i].row,
           array.data[i].col, array.data[i].idx, buff);
  }
}

int main(int argc, const char **argv) {
  if (argc < 2) {
    printf("Usage %s <file> <patter>\n", argv[0]);
  }

  FILE *textFile = fopen(argv[1], "r");
  string_t pattern = {
      .data = argv[2],
      .len = strlen(argv[2]),
  };

  struct timespec start, end;
  double elapsed_time_ms;

  if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
    perror("clock_gettime");
    return 1;
  }
  array_t array = robin(textFile, pattern, NULL);
  if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
    perror("clock_gettime");
    return 1;
  }

  elapsed_time_ms = (end.tv_sec - start.tv_sec) * 1000.0;
  elapsed_time_ms += (end.tv_nsec - start.tv_nsec) / 1000000.0;

  printf("Function 'robin' took %.3f milliseconds to execute.\n",
         elapsed_time_ms);

  print_matches(textFile, array);
}
