#ifndef SRC_FLAGS_LIB_H
#define SRC_FLAGS_LIB_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define flagsString "e:ivclnhsf:o"
#define bufferSize 512

typedef struct flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} flag;

void write_flags(int option, flag *exempFlags, char **templates,
                 int *flag_break_file);
void write_templates(char option, char *optarg, char **templateName,
                     int *flag_break_file);
void flagE(char *optarg, char **templateName);
void flagO(char *buffer, char *templates, flag exempFlags);
void flagF(char *optarg, char **templateName, int *flag_break_file);
int flags_with_V(flag exempFlags, int *counterL, int *counterC, char *fileName,
                 int *lineCounter, char *buffer, int template_index,
                 int *flag_many_f, char **template, int *doBreakLineInLastLine);
int flags_without_V(flag exempFlags, int *counterL, int *counterC,
                    char *fileName, int *lineCounter, char *buffer,
                    int template_index, int *flag_many_f, char **template,
                    int *doBreakLineInLastLine);
void check_more_flags(flag exempFlags, int *flag_many_f, int *flagLC,
                      int *counterC, int *counterL, char *fileName);

int get_numbr_templates(char *templates);
void getRegcomp(flag exempFlags, regex_t *re, char *temp, int *t);

#endif
