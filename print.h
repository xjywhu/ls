#ifndef PRINT_H_INCLUDED
#define PRINT_H_INCLUDED
#include<sys/stat.h>
#include"ls.h"
void printlongformat(char*filepath,struct filename f[],int count,char* paramater);
void printNormalformat(char*filepath,struct filename f[],int count,char* paramater,char format);
void printWithMode(char*filepath,struct filename f[],int count,char* paramater,char format);
void mode_to_letters(int mode,char str[]);
char *gid_to_name(gid_t gid);
char *uid_to_name( uid_t uid );
#endif // PRINT_H_INCLUDED
