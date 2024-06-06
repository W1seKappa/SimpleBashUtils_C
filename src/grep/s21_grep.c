#include "s21_grep.h"

int main(int argc, char *argv[]) {
  if (argc >= 3) {
    flag flags = {0};
    int file_index = 0, template_index = 0, optind_index = 0, flag_many_f = 0,
        flag_break_file = 0;
    char *templates = (char *)calloc(1, sizeof(char));
    parser(argc, argv, &flags, &file_index, &template_index, &optind_index,
           &templates, &flag_break_file);
    FILE *file = NULL;

    if (flags.f == 0 && flags.e == 0) {
      write_templates('a', argv[template_index], &templates, &flag_break_file);
      flag_many_f = (argc - optind_index - 1 == 1) ? 0 : 1;
    } else {
      templates[strlen(templates) - 1] = '\0';
      flag_many_f = (argc - optind_index == 1) ? 0 : 1;
    }

    while (file_index != argc) {
      if ((file = fopen(argv[file_index], "r")) != NULL) {
        if (flag_break_file == 0) {
          search_template(&templates, file, flags, argv[file_index],
                          flag_many_f, template_index);
        } else {
          printFile(file);
        }
        fclose(file);
      } else {
        if (flags.s == 0)
          fprintf(stderr, "grep: %s: No such file or directory\n",
                  argv[file_index]);
      }
      file_index += 1;
    }
    free(templates);
    return 0;
  }
}

void parser(int argc, char *argv[], flag *exempFlags, int *file_index,
            int *template_index, int *optind_index, char **templates,
            int *flag_break_file) {
  int option = 0, long_index = 0;
  static struct option longflags[] = {{0, 0, 0, 0}};
  while ((option = getopt_long(argc, argv, flagsString, longflags,
                               &long_index)) != -1) {
    write_flags(option, exempFlags, templates, flag_break_file);
  }
  *optind_index = optind;
  if (exempFlags->f || exempFlags->e)
    *file_index = optind, *template_index = 2;
  else
    *file_index = optind + 1, *template_index = optind;
}

void search_template(char **template, FILE *inputFile, flag exempFlags,
                     char *fileName, int flag_many_f, int template_index) {
  int t = 0, counterC = 0, counterL = 0, lineCounter = 0, lineCounter2 = 0,
      flagLC = 0, doBreakLineInLastLineFlag = 0;
  regex_t re = {0};
  char buffer[bufferSize] = "0";
  if (exempFlags.i || exempFlags.f || exempFlags.e) {
    if ((t = regcomp(&re, (*template), REG_ICASE | REG_EXTENDED)) != 0) {
      fprintf(stderr, "grep: %s (%s)\n", buffer, (*template));
      return;
    }
  } else if (exempFlags.i) {
    if ((t = regcomp(&re, (*template), REG_ICASE)) != 0) {
      fprintf(stderr, "grep: %s (%s)\n", buffer, (*template));
      return;
    }
  } else {
    if ((t = regcomp(&re, (*template), 0)) != 0) {
      fprintf(stderr, "grep: %s (%s)\n", buffer, (*template));
      return;
    }
  }

  while (fgets(buffer, bufferSize, inputFile) != NULL) {
    lineCounter += 1;
    if (regexec(&re, buffer, 0, NULL, 0) != 0 && (exempFlags.v == 1)) {
      lineCounter2 = lineCounter;
      if (flags_with_V(exempFlags, &counterL, &counterC, fileName, &lineCounter,
                       buffer, template_index, &flag_many_f, template,
                       &doBreakLineInLastLineFlag))

        continue;
    } else if (regexec(&re, buffer, 0, NULL, 0) == 0 && (exempFlags.v != 1)) {
      lineCounter2 = lineCounter;
      if (flags_without_V(exempFlags, &counterL, &counterC, fileName,
                          &lineCounter, buffer, template_index, &flag_many_f,
                          template, &doBreakLineInLastLineFlag))
        continue;
    }
  }
  regfree(&re);
  if (lineCounter == lineCounter2) {
    doBreakLineInLastLine(exempFlags, buffer, &doBreakLineInLastLineFlag);
  }
  check_more_flags(exempFlags, &flag_many_f, &flagLC, &counterC, &counterL,
                   fileName);
}

void printFile(FILE *file) {
  char s[512] = "0";
  while (fgets(s, 512, file) != NULL) {
    fprintf(stdout, "%s", s);
  }
}

void doBreakLineInLastLine(flag exempFlags, char *buffer,
                           int *doBreakLineInLastLineFlag) {
  int i = 0;
  if (exempFlags.v == 1) {
    if (exempFlags.c == 0 && exempFlags.l == 0 &&
        *doBreakLineInLastLineFlag == 0) {
      while (buffer[i] != '\0') {
        if (buffer[i + 1] == '\0' && buffer[0] != '\n' && buffer[i] != '\n') {
          printf("\n");
        }
        i += 1;
      }
    }
  } else if (exempFlags.v == 0) {
    if (exempFlags.c == 0 && exempFlags.l == 0 && exempFlags.o == 0 &&
        *doBreakLineInLastLineFlag == 0) {
      while (buffer[i] != '\0') {
        if (buffer[i + 1] == '\0' && buffer[0] != '\n' && buffer[i] != '\n') {
          printf("\n");
        }
        i += 1;
      }
    }
  }
}
