#include <stdio.h>
#include <ctype.h>
#include<string.h>
#include <stdlib.h>
#include "ls.h"



int dicCmp(char*s1,char*s2){
char*temp1;
char*temp2;
int count=0;
temp1=(char*)malloc(sizeof(char)*50);
temp2=(char*)malloc(sizeof(char)*50);

for(int i=0;i<strlen(s1);i++){
if(isalnum(s1[i])){
temp1[count]=s1[i];
count++;
}

}

count=0;
for(int j=0;j<strlen(s2);j++){
if(isalnum(s2[j])){

temp2[count]=s2[j];
count++;
}

}

if(strcasecmp(temp1,temp2)>0)
return 0;
else {
if(strcasecmp(temp1,temp2)==0)
return 1;
else
return 2;
}
}



void dicSort(struct filename f[],int count){
char* temp;
int type;
for(int i=0;i<count-1;i++){
  for(int k=i+1;k<count;k++){
if(dicCmp(f[i].name,f[k].name)==0||(dicCmp(f[i].name,f[k].name)==1&&f[i].type==1)){
temp=(char*)malloc(sizeof(char)*50);
strcpy(temp,f[i].name);
strcpy(f[i].name,f[k].name);
strcpy(f[k].name,temp);
type=f[i].type;
f[i].type=f[k].type;
f[k].type=type;
       }
    }
  }
for(int k=0;k<count;k++){
printf("%s\n", f[k].name);
}
}