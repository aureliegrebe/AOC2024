#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "input"

/* Size of each input chunk to be
   read and allocate for. */
#ifndef READALL_CHUNK
#define READALL_CHUNK 262144
#endif

#define READALL_OK 0       /* Success */
#define READALL_INVALID -1 /* Invalid parameters */
#define READALL_ERROR -2   /* Stream error */
#define READALL_TOOMUCH -3 /* Too much input */
#define READALL_NOMEM -4   /* Out of memory */

int readall(FILE *in, char **dataptr, size_t *sizeptr) {
  char *data = NULL, *temp;
  size_t size = 0;
  size_t used = 0;
  size_t n;

  /* None of the parameters can be NULL. */
  if (in == NULL || dataptr == NULL || sizeptr == NULL)
    return READALL_INVALID;

  /* A read error already occurred? */
  if (ferror(in))
    return READALL_ERROR;

  while (1) {

    if (used + READALL_CHUNK + 1 > size) {
      size = used + READALL_CHUNK + 1;

      /* Overflow check. Some ANSI C compilers
         may optimize this away, though. */
      if (size <= used) {
        free(data);
        return READALL_TOOMUCH;
      }

      temp = realloc(data, size);
      if (temp == NULL) {
        free(data);
        return READALL_NOMEM;
      }
      data = temp;
    }

    n = fread(data + used, 1, READALL_CHUNK, in);
    if (n == 0)
      break;

    used += n;
  }

  if (ferror(in)) {
    free(data);
    return READALL_ERROR;
  }

  temp = realloc(data, used + 1);
  if (temp == NULL) {
    free(data);
    return READALL_NOMEM;
  }
  data = temp;
  data[used] = '\0';

  *dataptr = data;
  *sizeptr = used;

  return READALL_OK;
}

int read_input(char **data, size_t *size) {
  FILE *f = fopen(INPUT_FILE, "r");
  if (f == NULL) {
    perror("Failed to open file");
  }
  int res = readall(f, data, size);
  fclose(f);

  switch (res) {
  case READALL_INVALID:
    perror("Invalid file read parameters");
    return -1;
  case READALL_ERROR:
    perror("Stream Error");
    return -1;
  case READALL_TOOMUCH:
    perror("Too much input");
    return -1;
  case READALL_NOMEM:
    perror("No more memory :(");
    return -1;
  }

  return 0;
}
