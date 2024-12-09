

#ifndef S21_CAT
#define S21_CAT

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  bool NumberNonBlank;
  bool MarkEndl;
  bool Number;
  bool SqueezeBlank;
  bool Tab;
  bool PrintNon;
} Flags;

Flags read_flags(int argc, char **argv);
int end_symvol(int argc, char **argv);
void print_symb(int c, int *prev, Flags flags, int *index, bool *eline_printed);
int print_file(char *name, Flags *flags);

#endif
