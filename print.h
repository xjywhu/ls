#ifndef PRINT_H_INCLUDED
#define PRINT_H_INCLUDED
#include<sys/stat.h>
#include"ls.h"
void printlongformat(struct filename f[],int count,char* paramater);
void printNormalformat(struct filename f[],int count,char* paramater);
void printWithMode(struct filename f[],int count,char* paramater);
void mode_to_letters(int mode,char str[]);
char *gid_to_name(gid_t gid);
char *uid_to_name( uid_t uid );


#endif // PRINT_H_INCLUDED
