#include "s21_grep.h"

void GrepFile(FILE *f, Flags flags, regex_t *preg, char *filename, int count) {
  char *line = 0;
  size_t len = 0;
  regmatch_t match;
  while (getline(&line, &len, f) > 0) {
    line[strcspn(line, "\n")] = '\0';
    if (flags.v) {
      if (regexec(preg, line, 1, &match, 0)) {
        if (flags.h)
          printf("%s\n", line);
        else {
          if (count > 1)
            printf("%s:%s\n", filename, line);
          else
            printf("%s\n", line);
        }
      }
    } else {
      if (!regexec(preg, line, 1, &match, 0)) {
        if (flags.o) {
          if (flags.h)
            printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
                   line + match.rm_so);
          else {
            if (count > 1)
              printf("%s:%.*s\n", filename, (int)(match.rm_eo - match.rm_so),
                     line + match.rm_so);
            else
              printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
                     line + match.rm_so);
          }
          char *remain = line + match.rm_eo;

          while (!regexec(preg, remain, 1, &match, 0)) {
            if (flags.h)
              printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
                     remain + match.rm_so);
            else {
              if (count > 1)
                printf("%s:%.*s\n", filename, (int)(match.rm_eo - match.rm_so),
                       remain + match.rm_so);
              else
                printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
                       remain + match.rm_so);
            }
            remain = remain + match.rm_eo;
          }
        } else if (flags.h) {
          printf("%s\n", line);
        } else {
          if (count > 1) {
            printf("%s:%s\n", filename, line);
          } else
            printf("%s\n", line);
        }
      }
    }
  }
  free(line);
}

void GrepCount(FILE *f, regex_t *preg, int cou, char *filename, Flags flags) {
  char *line = 0;
  size_t len = 0;
  regmatch_t match;
  int count = 0;
  int k = 0;
  while (getline(&line, &len, f) > 0) {
    k++;
    if (!regexec(preg, line, 1, &match, 0)) count++;
  }
  if (flags.h) {
    if (flags.v)
      printf("%d\n", k - count);
    else
      printf("%d\n", count);
  } else {
    if (cou > 1) {
      if (flags.v)
        printf("%s:%d\n", filename, k - count);
      else
        printf("%s:%d\n", filename, count);

    }

    else {
      if (flags.v)
        printf("%d\n", k - count);
      else
        printf("%d\n", count);
    }
  }
  free(line);
}

void GrepPrintFile(FILE *f, regex_t *preg, char *filename) {
  char *line = 0;
  size_t len = 0;
  regmatch_t match;
  while (getline(&line, &len, f) > 0) {
    if (!regexec(preg, line, 1, &match, 0)) {
      printf("%s\n", filename);
      break;
    }
  }

  free(line);
}

void GrepNumper(FILE *f, regex_t *preg, int cou, char *filename, Flags flags,
                char **pattern) {
  char *line = 0;
  size_t len = 0;
  regmatch_t match;

  int index = 1;
  while (getline(&line, &len, f) > 0) {
    line[strcspn(line, "\n")] = '\0';
    if (!regexec(preg, line, 1, &match, 0)) {
      if (flags.h) {
        if (flags.o)
          printf("%d:%s\n", index, *pattern);
        else
          printf("%d:%s\n", index, line);

      } else {
        if (flags.o) {
          if (cou > 1)
            printf("%s:%d:%s\n", filename, index, *pattern);
          else
            printf("%d:%s\n", index, *pattern);
        } else {
          if (cou > 1)
            printf("%s:%d:%s\n", filename, index, line);
          else
            printf("%d:%s\n", index, line);
        }
      }
    }
    index++;
  }
  free(line);
}
Flags read_flags(int argc, char **argv) {
  opterr = 0;
  int curr_flag = getopt(argc, argv, "eivhfsclon");
  Flags fl = {false, false, false, false, false, false, false, false, false};
  for (; curr_flag != -1; curr_flag = getopt(argc, argv, "eivfsohcln")) {
    switch (curr_flag) {
      case 'i':
        fl.regeg_flag |= REG_ICASE;
        break;
      case 'e':
        fl.regeg_flag |= REG_EXTENDED;
        break;
      case 'n':
        fl.n = true;
        break;
      case 'v':
        fl.v = true;
        break;
      case 'c':
        fl.c = true;
        break;
      case 'l':
        fl.l = true;
        break;
      case 'o':
        fl.o = true;
        break;
      case 'h':
        fl.h = true;
        break;
      case 's':
        fl.s = true;
        break;
      case 'f':
        fl.f = true;
        fl.regeg_flag |= REG_EXTENDED;
        break;
      case '?':
        if (!fl.s) printf("unknown flag");
        exit(1);
    }
  }
  return fl;
}
char *read_patterns_from_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Error opening pattern file");
    return NULL;
  }
  char *patterns = NULL;
  size_t total_len = 0;
  char *line = 0;
  size_t len = 0;

  while (getline(&line, &len, file) > 0) {
    line[strcspn(line, "\n")] = '\0';
    total_len += strlen(line) + 2;
    patterns = realloc(patterns, total_len);
    if (!patterns) {
      free(patterns);
      free(line);
      fclose(file);
      exit(1);
    }
    if (total_len == strlen(line) + 2) {
      strcpy(patterns, line);
    } else {
      strcat(patterns, "|");
      strcat(patterns, line);
    }
  }
  free(line);
  fclose(file);
  return patterns;
}

void Grep(int argc, char **argv, Flags flags) {
  char **pattern = &argv[1];
  char **end = &argv[argc];
  int count = 0;
  regex_t preg_storange;
  regex_t *preg = &preg_storange;
  for (; pattern != end && pattern[0][0] == '-'; pattern++) {
  }

  if (pattern == end) {
    fprintf(stderr, "No pattrn\n");
    exit(1);
  }

  for (char **filename = pattern + 1; filename != end; filename++) {
    if (**filename != '-') count++;
  }

  if (flags.f) {
    *pattern = read_patterns_from_file(*pattern);
    if (!*pattern) exit(1);
  }

  for (char **filename = pattern + 1; filename != end; filename++) {
    if (**filename != '-') {
      FILE *f = fopen(*filename, "rb");
      if (!f) {
        if (!flags.s) perror(*filename);
        continue;
      }
      if (regcomp(preg, *pattern, flags.regeg_flag)) {
        fprintf(stderr, "Regex compilation failed\n");
        fclose(f);
        exit(1);
      }

      if (flags.c) GrepCount(f, preg, count, *filename, flags);
      if (flags.n) GrepNumper(f, preg, count, *filename, flags, pattern);
      if (flags.l)
        GrepPrintFile(f, preg, *filename);
      else
        GrepFile(f, flags, preg, *filename, count);

      fclose(f);
      regfree(preg);
    }
  }
  if (flags.f) free(*pattern);
}

int main(int argc, char **argv) {
  Flags flags = read_flags(argc, argv);
  Grep(argc, argv, flags);
}
