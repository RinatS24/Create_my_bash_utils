
#include "s21_cat.h"

Flags read_flags(int argc, char **argv) {
  struct option lg_option[] = {{"number-nonblank", 0, NULL, 'b'},
                               {"number", 0, NULL, 'n'},
                               {"squeeze-blank", 0, NULL, 's'},
                               {NULL, 0, NULL, 0}};
  opterr = 0;
  int curr_flag = getopt_long(argc, argv, "benvstET", lg_option, NULL);
  Flags fl = {false, false, false, false, false, false};
  for (; curr_flag != -1;
       curr_flag = getopt_long(argc, argv, "benvstET", lg_option, NULL)) {
    switch (curr_flag) {
      case 'b':
        fl.NumberNonBlank = true;
        break;
      case 'e':
        fl.MarkEndl = true;
        fl.PrintNon = true;
        break;
      case 'n':
        fl.Number = true;
        break;
      case 's':
        fl.SqueezeBlank = true;
        break;
      case 't':
        fl.Tab = true;
        fl.PrintNon = true;
        break;
      case 'E':
        fl.MarkEndl = true;
        break;
      case 'T':
        fl.Tab = true;
        break;
      case 'v':
        fl.PrintNon = true;
        break;
      case '?':
        printf("unknown flag");
        exit(1);
    }
  }
  return fl;
}

int end_symvol(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    if (!strchr(argv[i], '-') && strchr(argv[i], '.')) {
      return i;
    }
  }
  return -1;
}

void print_symb(int c, int *prev, Flags flags, int *index,
                bool *eline_printed) {
  if (!(flags.SqueezeBlank && *prev == '\n' && c == '\n' && *eline_printed)) {
    if (*prev == '\n' && c == '\n') {
      *eline_printed = 1;

    } else {
      *eline_printed = 0;
    }

    if (((flags.Number && !flags.NumberNonBlank) ||
         (flags.NumberNonBlank && c != '\n')) &&
        *prev == '\n') {
      *index += 1;
      printf("%6d\t", *index);
    }

    if (flags.MarkEndl && c == '\n') printf("$");

    if (flags.Tab && c == '\t') {
      printf("^");
      c = '\t' + 64;
    }
    if (flags.PrintNon && c >= 0 && c <= 31 && c != '\n' && c != '\t') {
      printf("^");
      c = c + 64;
    }
    fputc(c, stdout);
  }
  *prev = c;
}

int print_file(char *name, Flags *flags) {
  int err_code = 0;
  FILE *f;
  if (strcmp("-", name) == 0)
    f = stdin;
  else
    f = fopen(name, "rt");

  if (f != NULL) {
    int index = 0;
    bool eline_printed = 0;
    int c = fgetc(f), prev = '\n';
    while (c != EOF) {
      print_symb(c, &prev, *flags, &index, &eline_printed);
      c = fgetc(f);
    }
    if (f != stdin) fclose(f);
  } else {
    err_code = 1;
  }
  return err_code;
}

int main(int argc, char **argv) {
  int index_end_flags = end_symvol(argc, argv);
  Flags fl = read_flags(argc, argv);

  if (argc == 1) print_file("-", &fl);

  for (int i = index_end_flags; i < argc; i++) {
    if (print_file(argv[i], &fl)) exit(1);
    if (fl.MarkEndl && i + 1 != argc) printf("$");

    if (i + 1 != argc) printf("\n");
  }

  return 0;
}
