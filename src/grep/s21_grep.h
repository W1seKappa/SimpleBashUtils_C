#ifndef SRC_S21_GREP_H
#define SRC_S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flags_lib.h"

void parser(int argc, char *argv[], flag *exempFlags, int *file_index,
            int *template_index, int *optind_index, char **templates,
            int *flag_break_file);
void search_template(char **template, FILE *inputFile, flag exempFlags,
                     char *fileName, int flag_many_f, int template_index);
void printFile(FILE *file);
void doBreakLineInLastLine(flag exempFlags, char *buffer,
                           int *doBreakLineInLastLineFlag);

#endif  // SRC_S21_GREP_H
