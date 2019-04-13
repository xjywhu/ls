#ifndef CMP_H_INCLUDED
#define CMP_H_INCLUDED
#include"ls.h"
int sortWithMode(struct filename f[],int count,char*paramater);
void dicSort(struct filename f[],int count);
void timeSort(struct filename f[],int count,char flag);
void sizeSort(struct filename f[],int count);
void reverseSort(struct filename f[],int count);
int dicCmp(char*s1,char*s2);
int timeCmp(char*s1,char*s2,char flag);
int sizeCmp(char*s1,char*s2);

#endif // CMP_H_INCLUDED
