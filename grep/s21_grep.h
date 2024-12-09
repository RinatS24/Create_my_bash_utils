#ifndef S21_CAT
#define S21_CAT
#define _GNU_SOURCE
#define BUFFER_SIZE 1024
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int regeg_flag;
  bool v;
  bool c;
  bool l;
  bool n;
  bool o;
  bool h;
  bool s;
  bool f;
} Flags;

void Grep(int argc, char **argv, Flags flags);
void GrepFile(FILE *f, Flags flags, regex_t *preg, char *filename, int count);
Flags read_flags(int argc, char **argv);
void GrepCount(FILE *f, regex_t *preg, int cou, char *filename, Flags flags);
void GrepNumper(FILE *f, regex_t *preg, int cou, char *filename, Flags flags,
                char **pattern);

#endif
