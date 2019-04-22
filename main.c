#include <stdio.h>
#include <stdlib.h>
#include<sys/stat.h>
#include<dirent.h>
#include <unistd.h>
#include<string.h>
#include "cmp.h"
#include "print.h"
#include "ls.h"

/*参数优先级d,a,A default*/
int getFileNameArrayWithMode(char*file_path,struct filename f[],char* paramater,int* count){
 DIR *dp;
 struct dirent *dirp;
 struct stat st;
 struct filename fn;
 int tempcount=0;
 	if(lstat(file_path, &st) < 0) {
    perror(file_path);
    exit(1);
    }
	if(S_ISREG(st.st_mode)){
    fn.name=(char*)malloc(sizeof(char)*50);
	strcpy(fn.name,file_path);
	fn.type=isFile;
	f[tempcount]=fn;
	tempcount++;
	*count=tempcount;
	return 0;
	}
	if(S_ISDIR(st.st_mode)){
    if ((dp = opendir(file_path)) == NULL){
	printf("can't open %s",file_path);
	exit(1);
	}
	}


    if(strrchr(paramater,'d')!=NULL){
    fn.name=(char*)malloc(sizeof(char)*1);
	strcpy(fn.name,".");
	fn.type=isDirectory;
	f[tempcount]=fn;
	tempcount++;
	*count=tempcount;
	return 0;
    }
while ((dirp = readdir(dp)) != NULL){


    if(strrchr(paramater,'A')!=NULL&&strrchr(paramater,'a')==NULL&&strrchr(paramater,'f')){
        if(strcmp(dirp->d_name,".")==0||strcmp(dirp->d_name,"..")==0)
	    continue;
    }
    if(strlen(paramater)==0){
      if(dirp->d_name[0]=='.')
	    continue;
    }

	fn.name=(char*)malloc(sizeof(char)*50);
	strcpy(fn.name,dirp->d_name);
	if(lstat(dirp->d_name, &st) < 0) {
    perror(dirp->d_name);
    exit(1);
    }
	if(S_ISREG(st.st_mode)){
	fn.type=isFile;
	}
	if(S_ISDIR(st.st_mode)){
    fn.type=isDirectory;
	}
	f[tempcount]=fn;
	tempcount++;
	}
    closedir(dp);
    *count=tempcount;
	return 0;
}


/*int do_ls(char*file_path){

 int count=0;
 struct filename f[30];
 if(getFileNameArrayWithMode(file_path,f,"a",&count)==1)
 exit(1);

	//dicSort(f,count);
    sortWithMode(f,count,"Sfc",'1');
	printNormalformat(f,count,"");
	//printWithMode(f,count,"l");
    //reverseSort(f,count);xjy
	//printNormalformat(f,count,"Fsih");


	return 0;
}*/

int do_ls(char*file_path,char*fileNumber,char*sort,char*print,char format){

 int count=0;
 struct filename f[30];
 if(getFileNameArrayWithMode(file_path,f,fileNumber,&count)==1)
 exit(1);

	//dicSort(f,count);
sortWithMode(f,count,sort,format);
printWithMode(f,count,print,format);
//printNormalformat(f,count,"");
	//printWithMode(f,count,"l");
    //reverseSort(f,count);xjy
	//printNormalformat(f,count,"Fsih");
	return 0;
}



int doCurrent(char*fileNumber,char*sort,char*print,char format){
 char buf[80];
 char*currentpath;
 if((currentpath=getcwd(buf,sizeof(buf)))==NULL){
 exit(1);
 }
 else{
  if(do_ls(currentpath,fileNumber,sort,print,format)==1)
  exit(1);
 }
 return 0;
}


/*int doTarget(char*filepath){
struct stat st;

if(lstat(filepath, &st) < 0) {
  perror(filepath);
  exit(1);
 }

 if(S_ISREG(st.st_mode)){
   printf("%s\n",filepath);
   return 0;
 }

 if(S_ISDIR(st.st_mode)){
    if(do_ls(filepath)==1)
    exit(1);
 }
return 0;
}*/


int main(int argc,char*argv[])
{

char*arguments="AacdFfhiklnrSstu";
int args;
int fileNumberParamaterCount=0;
int sortParamaterCount=0;
int printParamaterCount=0;
int formatParamaterCount=0;
char*fileNumber=(char*)malloc(sizeof(char)*10);
char*sort=(char*)malloc(sizeof(char)*10);
char*print=(char*)malloc(sizeof(char)*10);
char*formatNumber=(char*)malloc(sizeof(char)*10);
/*while((args=getopt(argc,argv,arguments))!=-1){
if(args=='A'||args=='a'||args=='d'||args=='f')
fileNumberParamaterCount++;
if(args=='c'||args=='f'||args=='r'||args=='s'||args=='t'||args=='u')
sortParamaterCount++;
if(args=='k'||args=='F'||args=='s'||args=='i'||args=='h')
printParamaterCount++;
if(args=='n'||args=='l')
formatParamaterCount++;
}*/
while((args=getopt(argc,argv,arguments))!=-1){
if(args=='A'||args=='a'||args=='d'||args=='f')
fileNumber[fileNumberParamaterCount++]=args;
if(args=='c'||args=='f'||args=='r'||args=='S'||args=='t'||args=='u')
sort[sortParamaterCount++]=args;
if(args=='k'||args=='F'||args=='s'||args=='i'||args=='h')
print[printParamaterCount++]=args;
if(args=='n'||args=='l')
formatNumber[formatParamaterCount++]=args;
}
/*char*fileNumber=(char*)malloc(sizeof(char)*fileNumberParamaterCount);
char*sort=(char*)malloc(sizeof(char)*sortParamaterCount);
char*print=(char*)malloc(sizeof(char)*printParamaterCount);
char*formatNumber=(char*)malloc(sizeof(char)*formatParamaterCount);
fileNumberParamaterCount=0;
sortParamaterCount=0;
printParamaterCount=0;
formatParamaterCount=0;
args=0;
while((args=getopt(argc,argv,arguments))!=-1){
if(args=='A'||args=='a'||args=='d'||args=='f')
fileNumber[fileNumberParamaterCount++]=args;
if(args=='c'||args=='f'||args=='r'||args=='s'||args=='t'||args=='u')
sort[sortParamaterCount++]=args;
if(args=='k'||args=='F'||args=='s'||args=='i'||args=='h')
print[printParamaterCount++]=args;
if(args=='n'||args=='l')
formatNumber[formatParamaterCount++]=args;
}*/
char format=formatNumber[formatParamaterCount-1];
doCurrent(fileNumber,sort,print,format);
    /*switch(argc){
    case 1:
    if(doCurrent()==1)
    exit(1);
    break;
    case 2:
    if(doTarget(argv[1])==1)
    exit(1);
    break;
    case 3:
    break;
    default:
    printf("The format of the arguement is not correct\n");
    exit(1);
    }*/

}
