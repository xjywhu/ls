#include <stdio.h>
#include <ctype.h>
#include<string.h>
#include <stdlib.h>
#include<sys/stat.h>
#include "cmp.h"


void reverseSort(struct filename f[],int count){
char* tempFilename;
int tempType;
for(int i=0;i<count/2;i++){
tempFilename=(char*)malloc(sizeof(char)*50);
strcpy(tempFilename,f[i].name);
tempType=f[i].type;
strcpy(f[i].name,f[count-i-1].name);
f[i].type=f[count-i-1].type;
strcpy(f[count-i-1].name,tempFilename);
f[count-i-1].type=tempType;
}


}
int dicCmp(char*s1,char*s2){
char*temp1;
char*temp2;
int count=0;
temp1=(char*)malloc(sizeof(char)*50);
temp2=(char*)malloc(sizeof(char)*50);

if(strcmp(s1,".")==0||(strcmp(s1,"..")==0&&strcmp(s2,".")!=0))
return secondBig;

if(strcmp(s2,".")==0||(strcmp(s2,"..")==0&&strcmp(s1,".")!=0))
return firstBig;


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
return firstBig;
else {
if(strcasecmp(temp1,temp2)==0)
return equals;
else
return secondBig;
}
}



void dicSort(struct filename f[],int count){
char* temp;
int type;
for(int i=0;i<count-1;i++){
  for(int k=i+1;k<count;k++){
if(dicCmp(f[i].name,f[k].name)==firstBig||(dicCmp(f[i].name,f[k].name)==equals&&f[i].type==isDirectory&&f[i+1].type==isFile)){
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

}

int timespecCmp(struct timespec times1,struct timespec times2){
if(times1.tv_sec>times2.tv_sec)
return firstBig;
else if(times1.tv_sec==times2.tv_sec){

if(times1.tv_nsec>times2.tv_nsec)
return firstBig;
else if(times1.tv_nsec==times2.tv_nsec)
return equals;
else
return secondBig;

}
else
return secondBig;



}

int timeCmp(char*s1,char*s2,char flag){
struct stat st1;
struct stat st2;
struct timespec times1[3];
struct timespec times2[3];
if(lstat(s1, &st1) < 0) {

  perror(s1);
  exit(1);
 }
if(lstat(s2, &st2) < 0) {

  perror(s2);
  exit(1);
 }

 times1[0]=st1.st_atim;
 times1[1]=st1.st_mtim;
 times1[2]=st1.st_ctim;

 times2[0]=st2.st_atim;
 times2[1]=st2.st_mtim;
 times2[2]=st2.st_ctim;

 if(flag=='c')
return timespecCmp(times1[2],times2[2]);
if(flag=='u')
/*数据访问时间*/
return timespecCmp(times1[0],times2[0]);
if(flag=='t')
/*数据修改时间*/
return timespecCmp(times1[1],times2[1]);

/*标识符号flag不正确*/
return 1;

}


void timeSort(struct filename f[],int count,char flag){
dicSort(f,count);
char* temp;
int type;
for(int i=0;i<count-1;i++){
  for(int k=i+1;k<count;k++){
if(timeCmp(f[i].name,f[k].name,flag)==secondBig/*||(timeCmp(f[i].name,f[k].name,flag)==3&&f[i].type==isDirectory)*/){
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

}


int sizeCmp(char*s1,char*s2){
struct stat st1;
struct stat st2;
if(lstat(s1, &st1) < 0) {
  perror(s1);
  exit(1);
 }
if(lstat(s2, &st2) < 0) {
  perror(s2);
  exit(1);
 }
if(st1.st_size>st2.st_size)
return firstBig;
else{
if(st1.st_size==st2.st_size)
return equals;
else
return secondBig;
}
}

void sizeSort(struct filename f[],int count){
dicSort(f,count);
char* temp;
int type;
for(int i=0;i<count-1;i++){
  for(int k=i+1;k<count;k++){
if(sizeCmp(f[i].name,f[k].name)==secondBig/*||(sizeCmp(f[i].name,f[k].name)==equals&&f[i].type==isDirectory)*/){
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

}

int sortWithMode(struct filename f[],int count,char*paramater){


if(strchr(paramater,'f')!=NULL)
return 0;
if(strlen(paramater)==0){
dicSort(f,count);
return 0;
}
if(strlen(paramater)==1){
switch(paramater[0]){
case 'S':
sizeSort(f,count);
break;
case 'c':
//timeSort(f,count,'c');
break;
case 'u':
//timeSort(f,count,'u');
break;
case 't':
timeSort(f,count,'t');
break;
case 'r':
dicSort(f,count);
reverseSort(f,count);
break;
default:
printf("The paramater about sort doesn't exit\n");
exit(1);
}
return 0;
}
/*delete r*/
int hasr=0;
int i=0,j=0;
char tempParamater[strlen(paramater)];
//debug
for(i=0;i<strlen(paramater);i++){
tempParamater[i]=paramater[i];
}
for(i=0;i<strlen(tempParamater);i++){
if(tempParamater[i]!='r'){
tempParamater[j++]=tempParamater[i];
}
else{
hasr=1;
}
}


switch(tempParamater[strlen(tempParamater)-2]){
case 'S':
sizeSort(f,count);
break;
case 'c':
if(strchr(paramater,'t')!=NULL)
timeSort(f,count,'c');
break;
case 'u':
if(strchr(paramater,'t')!=NULL)
timeSort(f,count,'u');
break;
case 't':
if(strrchr(paramater,'u')==NULL&&strrchr(paramater,'c')==NULL)
timeSort(f,count,'t');
else if(strrchr(paramater,'u')!=NULL&&strrchr(paramater,'c')==NULL)
timeSort(f,count,'u');
else if(strrchr(paramater,'c')!=NULL&&strrchr(paramater,'u')==NULL)
timeSort(f,count,'c');
else if(strrchr(paramater,'c')>strrchr(paramater,'u'))
timeSort(f,count,'c');
else
timeSort(f,count,'u');
break;
default:
printf("The paramater about sort doesn't exit\n");
exit(1);
break;

}
if(hasr==1)
reverseSort(f,count);
return 0;

}
