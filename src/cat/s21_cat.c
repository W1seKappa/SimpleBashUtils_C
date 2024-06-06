#include "s21_cat.h"

struct flag {
  int b;
  int e;
  int v;
  int n;
  int s;
  int t;
};

int main(int argc, char* argv[]) {
  struct flag options = {0};
  int count_flag = parser(argc, argv, &options);
  if (count_flag != -1) {
    parsing_file(argc, argv, options);
  }
  return 0;
}

void open_file(char* file_name, struct flag options) {
  FILE* file = fopen(file_name, "r");
  if (file == NULL) {
    printf("cat: %s: No such file or directory\n", file_name);
  } else {
    char c;
    int new_line = 1;
    int count = 1;
    int second_line = 1;
    int third_line = 0;
    int non_J = 0;
    c = fgetc(file);
    while (!feof(file)) {
      if (options.s == 1 && new_line && c == '\n') {
        if (third_line) {
          third_line = -1;
        }
      }
      if (options.n == 1 && new_line && options.b != 1) {
        if (third_line != -1) {
          printf("%6d\t", count++);
        }
      }
      if (options.b == 1 && new_line && c != '\n') {
        printf("%6d\t", count++);
      }
      if (options.e == 1 && c == '\n') {
        if (third_line != -1) {
          printf("$");
        }
      }
      if (options.v == 1) {
        int ch = (int)c;
        if (c < 0) {
          c &= 127;
          ch = (int)c;
          ch += 128;
        }
        if (ch != 9 && ch != 10 && ch < 32) {
          printf("^");
          c += 64;
        } else if (ch == 127) {
          printf("^");
          c = '?';
        } else if (ch > 127 && ch < 160) {
          printf("M-^");
          ch = ch - 64;
          if (c == 'J' && (options.b || options.n)) {
            printf("%c", c);
            printf("%6d\t", count);
            count += 1;
            non_J = 1;
          }
        } else if (ch > 160 && ch <= 255) {
          ch -= 128;
        }
      }
      if (options.t == 1 && c == '\t') {
        printf("^");
        c = 'I';
      }
      if (third_line != -1 && non_J != 1) {
        printf("%c", c);
      }
      new_line = (c == '\n') ? 1 : 0;
      third_line = (second_line && c == '\n') ? 1 : 0;
      second_line = (new_line && c == '\n') ? 1 : 0;
      non_J = 0;
      c = fgetc(file);
    }
    fclose(file);
  }
}

int parser(int argc, char* argv[], struct flag* options) {
  int count_flags = 0;
  char simbol_error;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == '-') {
      if (strcmp("--number-nonblank", argv[i]) == 0) {
        options->b = 1;
        count_flags++;
      } else if (strcmp("--number", argv[i]) == 0) {
        options->n = 1;
        count_flags++;
      } else if (strcmp("--squeeze-blank", argv[i]) == 0) {
        options->s = 1;
        count_flags++;
      } else {
        simbol_error = argv[i][1];
        printf("cat: illegal option -- %c\nusage: cat [-benstuv] [file ...]\n",
               simbol_error);
        count_flags = -1;
      }
    }
    if (argv[i][0] == '-' && argv[i][1] != '-' && count_flags != -1) {
      for (size_t j = 1; j < strlen(argv[i]); j++) {
        if (argv[i][j] == 'b' && options->b != 1) {
          options->b = 1;
          count_flags++;
        } else if (argv[i][j] == 'e') {
          options->e = 1;
          options->v = 1;
          count_flags++;
        } else if (argv[i][j] == 'v') {
          options->v = 1;
          count_flags++;
        } else if (argv[i][j] == 'n' && options->n != 1) {
          options->n = 1;
          count_flags++;
        } else if (argv[i][j] == 's' && options->s != 1) {
          options->s = 1;
          count_flags++;
        } else if (argv[i][j] == 't') {
          options->t = 1;
          options->v = 1;
          count_flags++;
        } else if (argv[i][j] == 'T') {
          options->t = 1;
          count_flags++;
        } else if (argv[i][j] == 'E') {
          options->e = 1;
          count_flags++;
        } else if (count_flags != -1) {
          simbol_error = argv[i][1];
          printf("cat illegal option -- %c\nusage: cat [-benstuv] [file ...]\n",
                 simbol_error);
          count_flags = -1;
        }
      }
    }
  }
  return count_flags;
}

void parsing_file(int argc, char* argv[], struct flag options) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      open_file(argv[i], options);
    }
  }
}