#include "flags_lib.h"

void write_flags(int option, flag *exempFlags, char **templates,
                 int *flag_break_file) {
  switch (option) {
    case 'e':
      exempFlags->e = 1;
      write_templates('e', optarg, templates, flag_break_file);
      break;
    case 'i':
      exempFlags->i = 1;
      break;
    case 'v':
      exempFlags->v = 1;
      break;
    case 'c':
      exempFlags->c = 1;
      break;
    case 'l':
      exempFlags->l = 1;
      break;
    case 'n':
      exempFlags->n = 1;
      break;
    case 'h':
      exempFlags->h = 1;
      break;
    case 's':
      exempFlags->s = 1;
      break;
    case 'f':
      exempFlags->f = 1;
      write_templates('f', optarg, templates, flag_break_file);
      break;
    case 'o':
      exempFlags->o = 1;
      break;
    default:
      fprintf(stderr, "use correct flags\n");
      exit(1);
  }
}

void write_templates(char option, char *optarg, char **templateName,
                     int *flag_break_file) {
  if (option == 'f') {
    flagF(optarg, templateName, flag_break_file);
  } else if (option == 'e') {
    flagE(optarg, templateName);
  } else {
    int strLenTemplate = strlen(optarg);
    (*templateName) =
        (char *)realloc((*templateName), (strLenTemplate + 1) * sizeof(char));
    for (int i = 0; i < strLenTemplate; i++) {
      (*templateName)[i] = optarg[i];
    }
    (*templateName)[strLenTemplate] = '\0';
  }
}

void flagE(char *optarg, char **templateName) {
  int strLenTemplate = strlen(optarg), strLenArray = strlen(*templateName),
      index = 0;
  (*templateName) = (char *)realloc(
      (*templateName), (strLenTemplate + strLenArray + 2) * sizeof(char));
  for (int i = strLenArray; i < strLenArray + strLenTemplate + 1; i++) {
    (*templateName)[i] = optarg[index];
    index += 1;
  }
  (*templateName)[strLenArray + strLenTemplate] = '|';
  (*templateName)[strLenArray + strLenTemplate + 1] = '\0';
}

void flagO(char *buffer, char *templates, flag exempFlags) {
  regex_t re = {0};
  regmatch_t pmatch[1] = {0};
  int t = 1, ct = 0, i = 0, j = 0, countTemp = 0;
  char *str = buffer;
  countTemp = get_numbr_templates(templates);

  while (ct < countTemp) {
    char *temp = (char *)calloc(1, sizeof(char));
    while (templates[i] != '|' && templates[i] != '\0') {
      temp = (char *)realloc(temp, (j + 1) * sizeof(char));
      temp[j] = templates[i];
      i += 1;
      j += 1;
    }
    temp = (char *)realloc(temp, (j + 1) * sizeof(char));
    temp[j] = '\0';
    getRegcomp(exempFlags, &re, temp, &t);
    while (!regexec(&re, str, 1, pmatch, 0)) {
      for (int k = pmatch[0].rm_so; k < pmatch[0].rm_eo; ++k)
        printf("%c", str[k]);
      printf("\n");
      str += pmatch[0].rm_eo;
    }
    ct += 1;
    j = 0;
    regfree(&re);
    i += 1;
    free(temp);
  }
}

void flagF(char *optarg, char **templateName, int *flag_break_file) {
  FILE *file = NULL;
  if ((file = fopen(optarg, "r")) != NULL) {
    char currentChar = '0', previousChar = '\n';
    int strLenArray = strlen(*templateName);
    while ((currentChar = fgetc(file)) != EOF) {
      if (currentChar == '\n' && previousChar != '\n') {
        (*templateName) =
            (char *)realloc((*templateName), (strLenArray + 1) * sizeof(char));
        (*templateName)[strLenArray] = '|';
        strLenArray += 1;
        previousChar = currentChar;
      } else if (currentChar == '\n' && previousChar == '\n') {
        *flag_break_file = 1;
      } else {
        (*templateName) =
            (char *)realloc((*templateName), (strLenArray + 1) * sizeof(char));
        (*templateName)[strLenArray] = currentChar;
        strLenArray += 1;
        previousChar = currentChar;
      }
    }
    (*templateName) =
        (char *)realloc((*templateName), (strLenArray + 2) * sizeof(char));
    (*templateName)[strLenArray] = '|';
    (*templateName)[strLenArray + 1] = '\0';
    fclose(file);
  } else {
    printf("не удалось открыть файл \n");
  }
}

int flags_with_V(flag exempFlags, int *counterL, int *counterC, char *fileName,
                 int *lineCounter, char *buffer, int template_index,
                 int *flag_many_f, char **template,
                 int *doBreakLineInLastLine) {
  int flagEnd = 0;
  *counterL += 1;
  if (exempFlags.n || exempFlags.h || template_index) {
    if (exempFlags.c) {
      *counterC += 1;
      flagEnd = 1;
    }
    if (!flagEnd) {
      if (exempFlags.h == 0 && *flag_many_f && !exempFlags.l)
        printf("%s:", fileName);
      if (exempFlags.n && !exempFlags.l) printf("%d:", *lineCounter);
      if (exempFlags.l != 1) {
        if (exempFlags.o && exempFlags.v != 1) {
          flagO(buffer, *template, exempFlags);
        } else {
          printf("%s", buffer);
        }
        if (buffer[strlen(buffer)] == '\0' &&
            buffer[strlen(buffer) - 1] != '\n' && exempFlags.o == 0) {
          printf("\n");
          *doBreakLineInLastLine = 1;
        }
      }
    }
  }
  return flagEnd;
}

int flags_without_V(flag exempFlags, int *counterL, int *counterC,
                    char *fileName, int *lineCounter, char *buffer,
                    int template_index, int *flag_many_f, char **template,
                    int *doBreakLineInLastLine) {
  int flagEnd = 0;
  *counterL += 1;
  if (exempFlags.n || exempFlags.h || template_index) {
    if (exempFlags.c) {
      if (buffer[0] != '\n') {
        *counterC += 1;
      }
      flagEnd = 1;
    }
    if (!flagEnd) {
      if (exempFlags.h == 0 && *flag_many_f && exempFlags.l == 0)
        printf("%s:", fileName);
      if (exempFlags.n && !exempFlags.l) printf("%d:", *lineCounter);
      if (exempFlags.l != 1) {
        if (exempFlags.o) {
          flagO(buffer, *template, exempFlags);
        } else {
          printf("%s", buffer);
        }
        if (buffer[strlen(buffer)] == '\0' &&
            buffer[strlen(buffer) - 1] != '\n' && exempFlags.o == 0) {
          printf("\n");
          *doBreakLineInLastLine = 1;
        }
      }
    }
  }
  return flagEnd;
}

void check_more_flags(flag exempFlags, int *flag_many_f, int *flagLC,
                      int *counterC, int *counterL, char *fileName) {
  if (exempFlags.c == 1 && exempFlags.l == 1 && *counterC != 0) {
    if (*flag_many_f && exempFlags.h == 0) {
      printf("%s:", fileName);
    }
    printf("%d\n", 1);
    *flagLC = 1;
  } else {
    if (*flag_many_f && exempFlags.c && exempFlags.h == 0 && *flagLC == 0)
      printf("%s:%d\n", fileName, *counterC);
    if ((*flag_many_f == 0 || exempFlags.h == 1) && exempFlags.c &&
        *flagLC == 0)
      printf("%d\n", *counterC);
  }
  if (exempFlags.l && *counterL > 0) {
    printf("%s\n", fileName);
  }
}

int get_numbr_templates(char *templates) {
  int countTemp = 1, strLenArray = strlen(templates);

  for (int i = 0; i < strLenArray; i++) {
    if (templates[i] == '|') countTemp += 1;
  }
  return countTemp;
}

void getRegcomp(flag exempFlags, regex_t *re, char *temp, int *t) {
  if (exempFlags.i && (exempFlags.f || exempFlags.e)) {
    if ((*t = regcomp(re, temp, REG_ICASE | REG_EXTENDED)) != 0) {
      printf("failed %d", *t);
      regfree(re);
    }
  } else if (exempFlags.i) {
    if ((*t = regcomp(re, temp, REG_ICASE)) != 0) {
      printf("failed %d", *t);
      regfree(re);
    }
  } else {
    if ((*t = regcomp(re, temp, 0)) != 0) {
      printf("failed %d", *t);
      regfree(re);
    }
  }
}