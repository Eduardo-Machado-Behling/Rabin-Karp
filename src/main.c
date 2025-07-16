#include <assert.h>
#include <math.h>
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
  } * data;

  size_t used;
  size_t size;
} array_t;

typedef size_t (*f_hash)(const char *text);

#ifndef START_SIZE
#define START_SIZE 4086
#endif

array_t naive(FILE *file, string_t pattern, f_hash hash) {
  array_t array = {
      .data = malloc(sizeof(*array.data) * START_SIZE),
      .used = 0,
      .size = START_SIZE,
  };

  char buff[1024];
  buff[fread(buff, sizeof(*buff), sizeof(buff) - 1, file)] = '\0';

  size_t i = 0;
  for (size_t s = 0; !feof(file); s++) {
    int delta = 1;
    if (strncmp(buff + i, pattern.data, pattern.len) == 0) {
      if (array.used == array.size) {
        array.size *= 2;
        array.data = realloc(array.data, array.size * sizeof(*array.data));
      }
      array.data[array.used++].idx = s;
    }
    if (buff[i + pattern.len + 1] == '\0') {
      memmove(buff, buff + i, pattern.len + 1);
      buff[pattern.len + 1 +
           fread(buff + pattern.len + 1, sizeof(*buff),
                 sizeof(buff) - 1 - pattern.len, file)] = '\0';
      i = 0;
      delta = 0;
      s--;
    }

    i += delta;
  }

  return array;
}

array_t RabinKarpMatcher(FILE *file, string_t pattern, size_t alphabetSize,
                         size_t prime, size_t *colissions) {
  array_t array = {
      .data = malloc(sizeof(*array.data) * START_SIZE),
      .used = 0,
      .size = START_SIZE,
  };
  *colissions = 0;

  size_t h =
      (size_t)powl((long double)alphabetSize, (long double)pattern.len - 1.0l) %
      prime;

  size_t p = 0;
  size_t t0 = 0;

  char buff[1024];

  buff[fread(buff, sizeof(buff[0]), sizeof(buff) - 1, file)] = '\0';

  for (size_t i = 0; i < pattern.len; i++) {
    p = (alphabetSize * p + pattern.data[i]) % prime;
    t0 = (alphabetSize * t0 + buff[i]) % prime;
  }

  size_t i = 0;
  for (size_t s = 0; !feof(file); s++) {
    int delta = 1;
    if (t0 == p) {
      if (strncmp(buff + i, pattern.data, pattern.len) == 0) {
        if (array.used == array.size) {
          array.size *= 2;
          array.data = realloc(array.data, array.size * sizeof(*array.data));
        }
        array.data[array.used++].idx = s;
      } else {
        (*colissions)++;
      }
    }
    if (buff[i + pattern.len + 1] != '\0') {
      size_t term_to_remove = (buff[i] * h) % prime;
      t0 = (alphabetSize * (t0 - term_to_remove + prime) +
            buff[i + pattern.len]) %
           prime;
    } else {
      memmove(buff, buff + i, pattern.len + 1);
      buff[pattern.len + 1 +
           fread(buff + pattern.len + 1, sizeof(*buff),
                 sizeof(buff) - 1 - pattern.len, file)] = '\0';
      i = 0;
      delta = 0;
      s--;
    }

    i += delta;
  }

  return array;
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
  array_t res[2];
  double times[2];
  size_t colissions[2] = {0};

  struct timespec start, end;
  double elapsed_time_ms;

  if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
    perror("clock_gettime");
    return 1;
  }
  res[0] = naive(textFile, pattern, NULL);
  if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
    perror("clock_gettime");
    return 1;
  }

  elapsed_time_ms = (end.tv_sec - start.tv_sec) * 1000.0;
  elapsed_time_ms += (end.tv_nsec - start.tv_nsec) / 1000000.0;
  times[0] = elapsed_time_ms;

  printf("Function 'naive' took %.3f milliseconds to execute.\n",
         elapsed_time_ms);

  fseek(textFile, 0, SEEK_SET);
  if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
    perror("clock_gettime");
    return 1;
  }
  res[1] = RabinKarpMatcher(textFile, pattern, 256, 764053, colissions + 1);
  if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
    perror("clock_gettime");
    return 1;
  }

  elapsed_time_ms = (end.tv_sec - start.tv_sec) * 1000.0;
  elapsed_time_ms += (end.tv_nsec - start.tv_nsec) / 1000000.0;

  printf("Function 'RabinKarpMatcher' took %.3f milliseconds to execute.\n",
         elapsed_time_ms);
  times[1] = elapsed_time_ms;

  fclose(textFile);

  FILE *out = fopen("./results.csv", "w");

  fputs("Naive,Rabin-Karp\n", out);

  size_t max = res[0].used;
  for (size_t j = 1; j < sizeof(res) / sizeof(*res); j++) {
    if (res[j].used > max) {
      max = res[j].used;
    }
  }

  for (size_t i = 0; i < max; i++) {
    for (size_t j = 0; j < sizeof(res) / sizeof(*res); j++) {
      if (i < res[j].used) {
        fprintf(out, "%zu", res[j].data[i].idx);
      }
      fputc(',', out);
    }
    fputc('\n', out);
  }

  fclose(out);

  out = fopen("./metrics.csv", "w");
  fputs("Algorithm,Matches,Time,Colission\n", out);
  const char *algos[] = {"Naive", "Rabin-Karp"};
  for (size_t i = 0; i < sizeof(res) / sizeof(*res); i++) {
    fprintf(out, "%s,%zu,%lf,%zu\n", algos[i], res[i].used, times[i],
            colissions[i]);
  }

  fclose(out);

  // print_matches(textFile, array);
}
