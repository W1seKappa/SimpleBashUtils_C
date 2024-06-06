#ifndef S21_CAT_H
#define S21_CAT_H
#include <stdio.h>
#include <string.h>
struct flag;
void open_file(char* file_name, struct flag options);
int parser(int argc, char* argv[], struct flag* options);
void parsing_file(int argc, char* argv[], struct flag options);
#endif